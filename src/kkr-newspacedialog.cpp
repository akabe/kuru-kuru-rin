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

#include <gtkmm.h>
#include "kkr-newspacedialog.hpp"

namespace Kkr
{

    NewSpaceDialog::NewSpaceDialog (Gtk::Window & parent)
        : Gtk::Dialog("New hypercube", parent, true, true),
          m_table(3, 2, false),
          m_lb_dimention("The dimension of a space"),
          m_lb_n_rotation("The number of rotation")
    {
        Gtk::VBox *vbox = get_vbox();
        vbox->pack_start(m_table, true, true, 0);

        set_default_size(240, 80);

        m_sb_dimention.set_range(3.0, 10.0);
        m_sb_dimention.set_increments(1.0, 1.0);
        m_sb_dimention.set_value(4.0);
        m_sb_n_rotation.set_range(1.0, 30.0);
        m_sb_n_rotation.set_increments(1.0, 1.0);
        m_sb_n_rotation.set_value(3.0);

        m_sb_dimention.signal_value_changed().connect(sigc::mem_fun(*this, &NewSpaceDialog::sb_dimention_on_value_changed));

        m_table.attach(m_lb_dimention, 0, 1, 0, 1, Gtk::FILL | Gtk::EXPAND, Gtk::FILL | Gtk::EXPAND, 10, 10);
        m_table.attach(m_lb_n_rotation, 0, 1, 1, 2, Gtk::FILL | Gtk::EXPAND, Gtk::FILL | Gtk::EXPAND, 10, 10);
        m_table.attach(m_sb_dimention, 1, 2, 0, 1, Gtk::FILL | Gtk::EXPAND, Gtk::FILL | Gtk::EXPAND, 10, 10);
        m_table.attach(m_sb_n_rotation, 1, 2, 1, 2, Gtk::FILL | Gtk::EXPAND, Gtk::FILL | Gtk::EXPAND, 10, 10);

        add_button(Gtk::Stock::OK, Gtk::RESPONSE_ACCEPT);
        add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_REJECT);
    }

    NewSpaceDialog::~NewSpaceDialog ()
    {
    }

    void
    NewSpaceDialog::sb_dimention_on_value_changed ()
    {
        m_sb_n_rotation.set_value(m_sb_dimention.get_value() - 1.0);

        return;
    }

    ViewConfig *
    NewSpaceDialog::run ()
    {
        int response = Gtk::Dialog::run();

        if (response == Gtk::RESPONSE_ACCEPT)
        {
            return new ViewConfig(m_sb_dimention.get_value(), m_sb_n_rotation.get_value());
        }
        else
        {
            return 0;
        }
    }

}
