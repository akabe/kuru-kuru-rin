/* kuru-kuru-rin  An interactive viewer of hypercubes
   Copyright (C) 2013 Akinori ABE

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDE_GUARD_00865A0A_AA86_11E2_A4CD_8C736E05BBBD
#define INCLUDE_GUARD_00865A0A_AA86_11E2_A4CD_8C736E05BBBD

#include <gdkmm/color.h>

namespace Kkr
{

    struct Color
    {
        double red;
        double green;
        double blue;
        double alpha;

        Color ()
            : red(0.0), green(0.0), blue(0.0), alpha(0.0)
        {
        }

        Color (double r, double g, double b, double a)
            : red(r), green(g), blue(b), alpha(a)
        {
        }

        Color (const Gdk::Color & color)
            : red(color.get_red_p()),
              green(color.get_green_p()),
              blue(color.get_blue_p()),
              alpha(1.0)
        {
        }
    };

}

#endif /* !INCLUDE_GUARD_00865A0A_AA86_11E2_A4CD_8C736E05BBBD */
