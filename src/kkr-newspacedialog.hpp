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

#ifndef INCLUDE_GUARD_A4D11D52_AA59_11E2_9C04_8C736E05BBBD
#define INCLUDE_GUARD_A4D11D52_AA59_11E2_9C04_8C736E05BBBD

#include <gtkmm/dialog.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include "kkr-viewconfig.hpp"

namespace Kkr
{

    class NewSpaceDialog : public Gtk::Dialog
    {
        public:
            NewSpaceDialog (Gtk::Window & parent);
            virtual ~NewSpaceDialog ();

            ViewConfig * run ();

        private:
            void sb_dimention_on_value_changed ();

            Gtk::Table      m_table;
            Gtk::Label      m_lb_dimention;
            Gtk::Label      m_lb_n_rotation;
            Gtk::SpinButton m_sb_dimention;
            Gtk::SpinButton m_sb_n_rotation;
    };

}

#endif /* !INCLUDE_GUARD_A4D11D52_AA59_11E2_9C04_8C736E05BBBD */
