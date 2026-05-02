/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef EXAMPLES_PERF_STRESS_PERF_STRESS_SCREEN_H
#define EXAMPLES_PERF_STRESS_PERF_STRESS_SCREEN_H

#include <cstdint>
#include <memory>
#include <string>

#include <rinvid/system/screen.h>

class StressSceneContent;

namespace rinvid
{

class Text;

} // namespace rinvid

class PerfStressScreen : public rinvid::Screen
{
  public:
    explicit PerfStressScreen(std::string font_path);

    ~PerfStressScreen();

    virtual void create() override;

    virtual void destroy() override;

  private:
    void update(double delta_time) override;

    void apply_uniform_preset(std::uint32_t count);

    void update_status_text();

    std::string                         font_path_;
    std::unique_ptr<StressSceneContent> stress_scene_;
    std::unique_ptr<rinvid::Text>       status_text_;
    std::uint32_t                       active_preset_count_;
};

#endif // EXAMPLES_PERF_STRESS_PERF_STRESS_SCREEN_H
