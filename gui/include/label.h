/**********************************************************************
 * Copyright (c) 2025, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef GUI_INCLUDE_LABEL_H
#define GUI_INCLUDE_LABEL_H

#include "extern/glm/glm/glm.hpp"

#include "core/include/text.h"

namespace rinvid
{

namespace gui
{

/**************************************************************************************************
 * @brief A textual label.
 *
 *************************************************************************************************/
class Label : public Text
{
  public:
    /**************************************************************************************************
     * @brief Label constructor.
     *
     *************************************************************************************************/
    Label(std::string text, std::string font_path, Vector2f position, Color color,
          std::uint32_t size);
};

} // namespace gui

} // namespace rinvid

#endif // GUI_INCLUDE_LABEL_H
