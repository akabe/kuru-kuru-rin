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

#ifndef INCLUDE_GUARD_4A657014_AA56_11E2_ABFB_8C736E05BBBD
#define INCLUDE_GUARD_4A657014_AA56_11E2_ABFB_8C736E05BBBD

#include <gtkmm/window.h>
#include <gtkmm/paned.h>
#include "kkr-grapharea.hpp"
#include "kkr-viewconfig.hpp"

namespace Kkr
{

    class MainWindow : public Gtk::Window
    {
        public:
            MainWindow ();
            virtual ~MainWindow ();

        private:
            void bt_newspace_on_clicked     ();
            void bt_event_on_clicked        ();
            void bt_reset_on_clicked        ();
            void sb_scale_on_value_changed  ();
            void sb_view_z_on_value_changed ();
            void cb_bg_color_on_color_set   ();
            void cb_edge_color_on_color_set ();
            void cb_face_color_on_color_set ();

            ViewConfig     * m_view_config;

            Gtk::HPaned      m_hpaned;
            GraphArea        m_grapharea;
            Gtk::Table       m_table;

            Gtk::Button      m_bt_newspace;
            Gtk::Button      m_bt_event;
            Gtk::Button      m_bt_reset;
            Gtk::Label       m_lb_scale;
            Gtk::SpinButton  m_sb_scale;
            Gtk::Label       m_lb_view_z;
            Gtk::SpinButton  m_sb_view_z;
            Gtk::Label       m_lb_bg_color;
            Gtk::ColorButton m_cb_bg_color;
            Gtk::Label       m_lb_edge_color;
            Gtk::ColorButton m_cb_edge_color;
            Gtk::Label       m_lb_face_color;
            Gtk::ColorButton m_cb_face_color;
    };

}

#endif /* !INCLUDE_GUARD_4A657014_AA56_11E2_ABFB_8C736E05BBBD */
