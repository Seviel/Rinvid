/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <algorithm>
#include <chrono>
#include <exception>
#include <stdexcept>
#include <utility>

#include <rinvid/util/image_cache.h>

namespace rinvid
{

ImageCache::Job::Job(std::filesystem::path file_path)
    : path{std::move(file_path)}, result{promise.get_future().share()}
{
}

ImageCache::ImageCache()
    : ImageCache{[](const std::filesystem::path& path) { return load_image(path); }}
{
}

ImageCache::ImageCache(Loader loader)
    : base_path_{std::filesystem::current_path()}, loader_{std::move(loader)},
      worker_{&ImageCache::run, this}
{
}

ImageCache::~ImageCache()
{
    {
        std::lock_guard<std::mutex> lock{mutex_};
        stopping_ = true;
        pending_.clear();
    }
    work_available_.notify_one();
    worker_.join();
}

std::filesystem::path ImageCache::key(const std::filesystem::path& path) const
{
    return (base_path_ / path).lexically_normal();
}

void ImageCache::retain(const std::vector<std::filesystem::path>& paths)
{
    {
        std::lock_guard<std::mutex> lock{mutex_};
        // Build replacements first so allocation/path errors leave the existing queue intact.
        std::map<std::filesystem::path, std::shared_ptr<Job>> retained;
        std::deque<std::shared_ptr<Job>>                      pending;

        for (const auto& path : paths)
        {
            auto normalized = key(path);
            if (retained.find(normalized) != retained.end())
            {
                continue;
            }
            const auto existing = jobs_.find(normalized);
            auto       job =
                existing == jobs_.end() ? std::make_shared<Job>(normalized) : existing->second;
            retained.emplace(std::move(normalized), job);
            if (!job->started)
            {
                pending.push_back(job);
            }
        }
        jobs_.swap(retained);
        pending_.swap(pending);
    }

    work_available_.notify_one();
}

void ImageCache::preload(const std::vector<std::filesystem::path>& paths)
{
    if (paths.empty())
    {
        return;
    }

    {
        std::lock_guard<std::mutex> lock{mutex_};
        // Commit both containers together so a failed allocation cannot leave an unqueued job.
        auto jobs    = jobs_;
        auto pending = pending_;
        for (const auto& path : paths)
        {
            auto normalized = key(path);
            if (jobs.find(normalized) != jobs.end())
            {
                continue;
            }
            auto job = std::make_shared<Job>(normalized);
            jobs.emplace(std::move(normalized), job);
            pending.push_back(std::move(job));
        }
        jobs_.swap(jobs);
        pending_.swap(pending);
    }

    work_available_.notify_one();
}

void ImageCache::evict(const std::filesystem::path& path)
{
    evict(std::vector<std::filesystem::path>{path});
}

void ImageCache::evict(std::initializer_list<std::filesystem::path> paths)
{
    evict(std::vector<std::filesystem::path>{paths});
}

void ImageCache::evict(const std::vector<std::filesystem::path>& paths)
{
    if (paths.empty())
    {
        return;
    }
    // Normalize the whole batch before changing entries, so path/allocation errors leave it intact.
    std::vector<std::filesystem::path> normalized;
    normalized.reserve(paths.size());

    for (const auto& path : paths)
    {
        normalized.push_back(key(path));
    }

    std::lock_guard<std::mutex> lock{mutex_};
    for (const auto& path : normalized)
    {
        jobs_.erase(path);
    }

    pending_.erase(std::remove_if(pending_.begin(), pending_.end(), [this](const auto& job)
                                  { return jobs_.find(job->path) == jobs_.end(); }),
                   pending_.end());
}

void ImageCache::clear()
{
    std::lock_guard<std::mutex> lock{mutex_};
    pending_.clear();
    jobs_.clear();
}

std::shared_future<ImageCache::Image> ImageCache::result(const std::filesystem::path& path) const
{
    const auto                  normalized = key(path);
    std::lock_guard<std::mutex> lock{mutex_};
    const auto                  it = jobs_.find(normalized);

    if (it == jobs_.end())
    {
        throw std::logic_error{"Image was not retained: " + path.string()};
    }

    return it->second->result;
}

ImageCache::Image ImageCache::try_get(const std::filesystem::path& path) const
{
    const auto future = result(path);

    if (future.wait_for(std::chrono::seconds{0}) != std::future_status::ready)
    {
        return nullptr;
    }

    return future.get();
}

ImageCache::Image ImageCache::get(const std::filesystem::path& path) const
{
    return result(path).get();
}

void ImageCache::run()
{
    for (;;)
    {
        std::shared_ptr<Job> job;
        {
            std::unique_lock<std::mutex> lock{mutex_};
            work_available_.wait(lock, [this] { return stopping_ || !pending_.empty(); });
            if (stopping_)
            {
                return;
            }
            job = pending_.front();
            pending_.pop_front();
            job->started = true;
        }
        try
        {
            job->promise.set_value(std::make_shared<const ImageData>(loader_(job->path)));
        }
        catch (...)
        {
            job->promise.set_exception(std::current_exception());
        }
    }
}

} // namespace rinvid
