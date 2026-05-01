/**********************************************************************
 * Copyright (c) 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <memory>

#include <rinvid/system/application.h>

#include "perf_stress_screen.h"

int main()
{
    rinvid::Application perf_stress_app{1280U, 720U, "Rinvid performance stress", false};
    perf_stress_app.set_screen(std::make_unique<PerfStressScreen>("resources/aquifer.ttf"));
    perf_stress_app.set_fps(0U);
    perf_stress_app.run();

    return 0;
}
