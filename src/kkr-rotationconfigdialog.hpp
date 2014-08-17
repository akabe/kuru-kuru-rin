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

#ifndef INCLUDE_GUARD_02CAE6B4_AA63_11E2_B660_8C736E05BBBD
#define INCLUDE_GUARD_02CAE6B4_AA63_11E2_B660_8C736E05BBBD

#include <gtkmm/dialog.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/combobox.h>
#include "kkr-viewconfig.hpp"

namespace Kkr
{

    class RotationConfigDialog : public Gtk::Dialog
    {
        public:
            RotationConfigDialog (Gtk::Window & parent,
                                  ViewConfig  & view_config);
            virtual ~RotationConfigDialog ();

        protected:
            virtual void on_response (int response_id);

        private:
            ViewConfig        & m_view_config;
            Gtk::Table          m_table;
            Gtk::SpinButton   * m_sb_axis1;
            Gtk::SpinButton   * m_sb_axis2;
            Gtk::ComboBoxText * m_cbt_event_type;
            Gtk::ComboBoxText * m_cbt_event_orien;
            Gtk::ComboBoxText * m_cbt_event_key;
    };

}

#endif /* !INCLUDE_GUARD_02CAE6B4_AA63_11E2_B660_8C736E05BBBD */
