/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef INCLUDE_RINVID_UTIL_IMAGE_CACHE_H
#define INCLUDE_RINVID_UTIL_IMAGE_CACHE_H

#include <deque>
#include <filesystem>
#include <functional>
#include <future>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <condition_variable>
#include <initializer_list>

#include <rinvid/util/image_loader.h>

namespace rinvid
{

/**************************************************************************************************
 * @brief Caches immutable RGBA images decoded by one background worker.
 * Call public methods and destroy the cache on the owning game thread. No OpenGL context is
 * needed. Texture creation and destruction remain the caller's responsibility on the render thread.
 * Relative paths resolve against the working directory captured at construction. Keys are absolute
 * and lexically normalized; symlink and case aliases are not deduplicated.
 *
 *************************************************************************************************/
class ImageCache
{
  public:
    /**************************************************************************************************
     * @brief Shared pixels that remain valid after cache eviction or destruction.
     *
     *************************************************************************************************/
    using Image = std::shared_ptr<const ImageData>;

    /**************************************************************************************************
     * @brief Captures the working directory and starts the decoding worker.
     *
     *************************************************************************************************/
    ImageCache();

    /**************************************************************************************************
     * @brief Discards queued work and joins the worker, waiting for any active decode.
     *
     *************************************************************************************************/
    ~ImageCache();

    /**************************************************************************************************
     * @brief Cache ownership cannot be copied or moved.
     *
     *************************************************************************************************/
    ImageCache(const ImageCache&)            = delete;
    ImageCache& operator=(const ImageCache&) = delete;
    ImageCache(ImageCache&&)                 = delete;
    ImageCache& operator=(ImageCache&&)      = delete;

    /**************************************************************************************************
     * @brief Replaces the retained set without waiting for decoding.
     * Reuses retained jobs and results, drops obsolete queued jobs, and orders queued work by
     * paths. Active obsolete work may finish but cannot repopulate the cache. An empty list clears
     * it. Failed entries are reused too; remove and request them again to retry. Memory is bounded
     * by the retained images, an active decode, and images held by callers.
     *
     * @param paths Images to retain in loading priority order; duplicates are ignored.
     *
     *************************************************************************************************/
    void retain(const std::vector<std::filesystem::path>& paths);

    /**************************************************************************************************
     * @brief Adds images without evicting existing entries or waiting for decoding.
     * Missing images are appended to the queue in list order. Existing queued, active, completed,
     * and failed entries are reused without changing their priority. An empty list does nothing.
     *
     * @param paths Images to preload; normalized duplicates are ignored.
     *
     *************************************************************************************************/
    void preload(const std::vector<std::filesystem::path>& paths);

    /**************************************************************************************************
     * @brief Removes one image and cancels its queued work without waiting for decoding.
     * An active decode may finish but cannot repopulate the cache. Images held by callers remain
     * valid. A subsequent preload starts a fresh request, including after a failed load.
     *
     * @param path Image to evict; a path that is not cached is ignored.
     *
     *************************************************************************************************/
    void evict(const std::filesystem::path& path);

    /**************************************************************************************************
     * @brief Removes multiple images and their queued work without waiting for decoding.
     * Other entries keep their results and queue order. Active evicted work may finish but cannot
     * repopulate the cache. Images held by callers remain valid.
     *
     * @param paths Images to evict; duplicates and missing entries are ignored. Empty is a no-op.
     *
     *************************************************************************************************/
    void evict(const std::vector<std::filesystem::path>& paths);

    /**************************************************************************************************
     * @brief Evicts a braced list of images with the same behavior as the vector overload.
     *
     * @param paths Images to evict; an empty list does nothing.
     *
     *************************************************************************************************/
    void evict(std::initializer_list<std::filesystem::path> paths);

    /**************************************************************************************************
     * @brief Releases all cached entries and queued work without waiting for an active decode.
     * Equivalent to retain({}). Images held by callers remain valid. The cache can be reused;
     * an obsolete active decode cannot repopulate it.
     *
     *************************************************************************************************/
    void clear();

    /**************************************************************************************************
     * @brief Looks up an image without waiting for decoding.
     *
     * @param path Previously retained image path.
     *
     * @return Loaded image, or nullptr while queued or decoding.
     *
     * @throws std::logic_error if the path is not retained.
     *
     * @throws ResourceLoadError if decoding failed. Allocation failures also propagate.
     *
     *************************************************************************************************/
    Image try_get(const std::filesystem::path& path) const;

    /**************************************************************************************************
     * @brief Waits for a retained image. Use at startup or during an explicit loading screen.
     *
     * @param path Previously retained image path.
     *
     * @return Loaded image.
     *
     * @throws std::logic_error if the path is not retained.
     *
     * @throws ResourceLoadError if decoding failed. Allocation failures also propagate.
     *
     *************************************************************************************************/
    Image get(const std::filesystem::path& path) const;

  private:
    friend struct ImageCacheTestAccess;
    using Loader = std::function<ImageData(const std::filesystem::path&)>;
    explicit ImageCache(Loader loader);

    struct Job
    {
        explicit Job(std::filesystem::path file_path);
        std::filesystem::path     path;
        std::promise<Image>       promise;
        std::shared_future<Image> result;
        bool                      started{false};
    };

    std::filesystem::path     key(const std::filesystem::path& path) const;
    std::shared_future<Image> result(const std::filesystem::path& path) const;
    void                      run();

    const std::filesystem::path                           base_path_;
    Loader                                                loader_;
    mutable std::mutex                                    mutex_;
    std::condition_variable                               work_available_;
    std::map<std::filesystem::path, std::shared_ptr<Job>> jobs_;
    std::deque<std::shared_ptr<Job>>                      pending_;
    bool                                                  stopping_{false};
    std::thread                                           worker_;
};

} // namespace rinvid

#endif // INCLUDE_RINVID_UTIL_IMAGE_CACHE_H
