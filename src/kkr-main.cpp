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

#include <iostream>
#include <gtkmm.h>
#include "kkr-mainwindow.hpp"

int
main (int argc, char ** argv)
{
    Gtk::Main kit(&argc, &argv);
    Kkr::MainWindow window;

    window.show_all_children();
    Gtk::Main::run(window);

    return 0;
}