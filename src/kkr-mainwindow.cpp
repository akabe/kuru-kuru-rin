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
#include <iostream>
#include "kkr-rotationconfigdialog.hpp"
#include "kkr-newspacedialog.hpp"
#include "kkr-mainwindow.hpp"

#define KKR_DEFAULT_SCALE  (100.0)
#define KKR_DEFAULT_VIEW_Z (5.0)

namespace Kkr
{
    /**
     * Convert a value input from Gtk::ColorButton to Kkr::Color.
     */
    static Color
    color_button_get_kkr_color (const Gtk::ColorButton & color_button)
    {
        const Gdk::Color src = color_button.get_color();
        return Color(src.get_red_p(),
                     src.get_green_p(),
                     src.get_blue_p(),
                     color_button.get_alpha() * (1.0 / 65535.0));
    }

    MainWindow::MainWindow ()
        : m_view_config(0),
          m_table(8, 2, false),
          m_bt_newspace("New"),
          m_bt_event("Configure rotation"),
          m_bt_reset("Reset"),
          m_lb_scale("Scale"),
          m_lb_view_z("Focus"),
          m_lb_bg_color("Background Color"),
          m_cb_bg_color(Gdk::Color("#ffffff")),
          m_lb_edge_color("Line Color"),
          m_cb_edge_color(Gdk::Color("#000000")),
          m_lb_face_color("Face Color"),
          m_cb_face_color(Gdk::Color("#0000ff"))
    {
        m_bt_newspace.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::bt_newspace_on_clicked));
        m_bt_event.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::bt_event_on_clicked));
        m_bt_reset.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::bt_reset_on_clicked));
        m_sb_scale.signal_value_changed().connect(sigc::mem_fun(*this, &MainWindow::sb_scale_on_value_changed));
        m_sb_view_z.signal_value_changed().connect(sigc::mem_fun(*this, &MainWindow::sb_view_z_on_value_changed));
        m_cb_bg_color.signal_color_set().connect(sigc::mem_fun(*this, &MainWindow::cb_bg_color_on_color_set));
        m_cb_edge_color.signal_color_set().connect(sigc::mem_fun(*this, &MainWindow::cb_edge_color_on_color_set));
        m_cb_face_color.signal_color_set().connect(sigc::mem_fun(*this, &MainWindow::cb_face_color_on_color_set));

        m_sb_scale.set_range(0.0, 999.0);
        m_sb_scale.set_increments(2.0, 10.0);
        m_sb_scale.set_digits(1);
        m_sb_scale.set_value(KKR_DEFAULT_SCALE);
        m_sb_view_z.set_range(2.0, 9999.0);
        m_sb_view_z.set_increments(0.5, 10.0);
        m_sb_view_z.set_digits(3);
        m_sb_view_z.set_value(KKR_DEFAULT_VIEW_Z);

        m_cb_edge_color.set_use_alpha(true);
        m_cb_face_color.set_use_alpha(true);
        m_cb_face_color.set_alpha(0x3333);

        m_table.attach(m_bt_newspace  , 0, 2, 0, 1, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 0, 5);
        m_table.attach(m_bt_event     , 0, 2, 1, 2, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 0, 5);
        m_table.attach(m_bt_reset     , 0, 2, 2, 3, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 0, 5);
        m_table.attach(m_lb_scale     , 0, 1, 3, 4, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 0, 5);
        m_table.attach(m_lb_view_z    , 0, 1, 4, 5, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 0, 5);
        m_table.attach(m_lb_bg_color  , 0, 1, 5, 6, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 0, 5);
        m_table.attach(m_lb_edge_color, 0, 1, 6, 7, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 0, 5);
        m_table.attach(m_lb_face_color, 0, 1, 7, 8, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 0, 5);
        m_table.attach(m_sb_scale     , 1, 2, 3, 4, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 0, 5);
        m_table.attach(m_sb_view_z    , 1, 2, 4, 5, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 0, 5);
        m_table.attach(m_cb_bg_color  , 1, 2, 5, 6, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 0, 5);
        m_table.attach(m_cb_edge_color, 1, 2, 6, 7, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 0, 5);
        m_table.attach(m_cb_face_color, 1, 2, 7, 8, Gtk::FILL | Gtk::EXPAND, Gtk::SHRINK, 0, 5);

        m_hpaned.add1(m_grapharea);
        m_hpaned.add2(m_table);
        m_hpaned.set_position(640);

        set_title("kuru-kuru-rin");
        set_default_size(900, 600);
        set_position(Gtk::WIN_POS_CENTER);
        add(m_hpaned);
    }

    MainWindow::~MainWindow ()
    {
        if (m_view_config != 0) delete m_view_config;
    }

    void
    MainWindow::bt_newspace_on_clicked ()
    {
        NewSpaceDialog nsdialog(*this);

        nsdialog.show_all_children();
        ViewConfig *view_config = nsdialog.run();

        if (view_config != 0)
        {
            if (m_view_config != 0) delete m_view_config;

            m_view_config = view_config;

            /* Set the current parameters to ViewConfig */
            m_view_config->set_scale(m_sb_scale.get_value() * 1e-2);
            m_view_config->set_view_z(m_sb_view_z.get_value());
            m_view_config->set_bg_color(color_button_get_kkr_color(m_cb_bg_color));
            m_view_config->set_edge_color(color_button_get_kkr_color(m_cb_edge_color));
            m_view_config->set_face_color(color_button_get_kkr_color(m_cb_face_color));

            /* Set the ViewConfig to GraphArea and redraw the screen. */
            m_grapharea.set_view_config(*view_config);
            m_grapharea.queue_draw();
        }

        return;
    }

    void
    MainWindow::bt_event_on_clicked ()
    {
        if (m_view_config != 0)
        {
            RotationConfigDialog rcdialog(*this, *m_view_config);

            rcdialog.show_all_children();
            rcdialog.run();
        }

        return;
    }

    void
    MainWindow::bt_reset_on_clicked ()
    {
        if (m_view_config != 0)
        {
            m_sb_scale.set_value(KKR_DEFAULT_SCALE);
            m_sb_view_z.set_value(KKR_DEFAULT_VIEW_Z);
            m_view_config->reset_matrix();
            m_grapharea.queue_draw();
        }

        return;
    }

    void
    MainWindow::sb_scale_on_value_changed ()
    {
        if (m_view_config != 0)
        {
            m_view_config->set_scale(m_sb_scale.get_value() * 1e-2);
            m_grapharea.queue_draw();
        }

        return;
    }

    void
    MainWindow::sb_view_z_on_value_changed ()
    {
        if (m_view_config != 0)
        {
            m_view_config->set_view_z(m_sb_view_z.get_value());
            m_grapharea.queue_draw();
        }

        return;
    }

    void
    MainWindow::cb_bg_color_on_color_set ()
    {
        if (m_view_config != 0)
        {
            m_view_config->set_bg_color(color_button_get_kkr_color(m_cb_bg_color));
            m_grapharea.queue_draw();
        }

        return;
    }

    void
    MainWindow::cb_edge_color_on_color_set ()
    {
        if (m_view_config != 0)
        {
            m_view_config->set_edge_color(color_button_get_kkr_color(m_cb_edge_color));
            m_grapharea.queue_draw();
        }

        return;
    }

    void
    MainWindow::cb_face_color_on_color_set ()
    {
        if (m_view_config != 0)
        {
            m_view_config->set_face_color(color_button_get_kkr_color(m_cb_face_color));
            m_grapharea.queue_draw();
        }

        return;
    }

}
