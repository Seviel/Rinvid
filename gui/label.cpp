/**********************************************************************
 * Copyright (c) 2025 - 2026, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#include <utility>

#include "include/label.h"

namespace rinvid
{

namespace gui
{

Label::Label(std::string text, const std::string& font_path, Vector2f position, Color color,
             std::uint32_t size)
    : Text(std::move(text), font_path, position, color, size)
{
}

} // namespace gui

} // namespace rinvid
