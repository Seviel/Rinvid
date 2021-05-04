/**********************************************************************
 * Copyright (c) 2021, Filip Vasiljevic
 * All rights reserved.
 *
 * This file is subject to the terms and conditions of the BSD 2-Clause
 * License.  See the file LICENSE in the root directory of the Rinvid
 * repository for more details.
 **********************************************************************/

#ifndef CORE_INCLUDE_SCREEN_H
#define CORE_INCLUDE_SCREEN_H

namespace rinvid
{

class Screen
{
  public:
    virtual void create()                  = 0;
    virtual void update(double delta_time) = 0;
    virtual void destroy()                 = 0;
};

} // namespace rinvid

#endif // CORE_INCLUDE_SCREEN_H
