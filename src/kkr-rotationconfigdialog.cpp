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
#include "kkr-rotationconfigdialog.hpp"

namespace Kkr
{

    template <typename enumT>
    struct NamedEnumValue
    {
        enumT value;
        const char * name;
    };

    static const NamedEnumValue<GestureType> named_gesture_type[] =
    {
        {GESTURE_NOTHING, "(No event)"},
        {GESTURE_LEFT_BUTTON_DRAG, "Left drag"},
        {GESTURE_MIDDLE_BUTTON_DRAG, "Middle drag"},
        {GESTURE_RIGHT_BUTTON_DRAG, "Right drag"},
        {GESTURE_SCROLL, "Scroll"},
        {GESTURE_ARROW_KEY, "Arrow keys"}
    };

    static const NamedEnumValue<OrientationType> named_orientation_type[] =
    {
        {ORIENTATION_HORIZONTAL, "Horizontal"},
        {ORIENTATION_VERTICAL, "Vertical"}
    };

    static const NamedEnumValue<int> named_key_mask[] =
    {
        {0, ""},
        {KEY_SHIFT_MASK, "Shift"},
        {KEY_CTRL_MASK, "Ctrl"},
        {KEY_ALT_MASK, "Alt"},
        {KEY_SHIFT_MASK | KEY_CTRL_MASK, "Shift+Ctrl"},
        {KEY_SHIFT_MASK | KEY_ALT_MASK, "Shift+Alt"},
        {KEY_CTRL_MASK | KEY_ALT_MASK, "Ctrl+Alt"},
        {KEY_SHIFT_MASK | KEY_CTRL_MASK | KEY_ALT_MASK, "Shift+Ctrl+Alt"}
    };

    template <typename enumT>
    static void
    combo_box_text_append_entries (Gtk::ComboBoxText           & combo_box,
                                   const NamedEnumValue<enumT> * entries,
                                   std::size_t                   n_entries,
                                   enumT                         default_value)
    {
        std::size_t i;

        for (i = 0; i < n_entries; ++i)
        {
            combo_box.append_text(entries[i].name);

            if (default_value == entries[i].value)
            {
                combo_box.set_active(i);
            }
        }

        return;
    }

    RotationConfigDialog::RotationConfigDialog (Gtk::Window & parent,
                                                ViewConfig  & view_config)
        : Gtk::Dialog("Configuration of rotation", parent, true, true),
          m_view_config(view_config),
          m_table(1, 6)
    {
        int i, axis1, axis2, event_key;
        GestureType event_type;
        OrientationType event_orien;

        Gtk::VBox *vbox = get_vbox();
        vbox->pack_start(m_table, true, true, 0);

        m_sb_axis1        = new Gtk::SpinButton [m_view_config.get_n_rotation()];
        m_sb_axis2        = new Gtk::SpinButton [m_view_config.get_n_rotation()];
        m_cbt_event_type  = new Gtk::ComboBoxText [m_view_config.get_n_rotation()];
        m_cbt_event_orien = new Gtk::ComboBoxText [m_view_config.get_n_rotation()];
        m_cbt_event_key   = new Gtk::ComboBoxText [m_view_config.get_n_rotation()];

        m_table.attach(*Gtk::manage(new Gtk::Label("Axis 1")), 1, 2, 0, 1);
        m_table.attach(*Gtk::manage(new Gtk::Label("Axis 2")), 2, 3, 0, 1);
        m_table.attach(*Gtk::manage(new Gtk::Label("Event")), 3, 4, 0, 1);
        m_table.attach(*Gtk::manage(new Gtk::Label("Direction")), 4, 5, 0, 1);
        m_table.attach(*Gtk::manage(new Gtk::Label("Key")), 5, 6, 0, 1);

        for (i = 0; i < m_view_config.get_n_rotation(); ++i)
        {
            Gtk::Label *label = Gtk::manage(new Gtk::Label(Glib::ustring::compose("Rot #%1", i + 1)));

            m_view_config.get_axis(i, axis1, axis2);
            m_view_config.get_event(i, event_type, event_orien, event_key);

            m_sb_axis1[i].set_range(1.0, m_view_config.get_dimention());
            m_sb_axis2[i].set_range(1.0, m_view_config.get_dimention());
            m_sb_axis1[i].set_increments(1.0, 1.0);
            m_sb_axis2[i].set_increments(1.0, 1.0);
            m_sb_axis1[i].set_value(axis1 + 1);
            m_sb_axis2[i].set_value(axis2 + 1);

            combo_box_text_append_entries(m_cbt_event_type[i],
                                          named_gesture_type,
                                          G_N_ELEMENTS(named_gesture_type),
                                          event_type);
            combo_box_text_append_entries(m_cbt_event_orien[i],
                                          named_orientation_type,
                                          G_N_ELEMENTS(named_orientation_type),
                                          event_orien);
            combo_box_text_append_entries(m_cbt_event_key[i],
                                          named_key_mask,
                                          G_N_ELEMENTS(named_key_mask),
                                          event_key);

            m_table.attach(*label              , 0, 1, i + 1, i + 2, Gtk::FILL | Gtk::EXPAND, Gtk::FILL | Gtk::EXPAND, 10, 5);
            m_table.attach(m_sb_axis1[i]       , 1, 2, i + 1, i + 2, Gtk::FILL | Gtk::EXPAND, Gtk::FILL | Gtk::EXPAND, 10, 5);
            m_table.attach(m_sb_axis2[i]       , 2, 3, i + 1, i + 2, Gtk::FILL | Gtk::EXPAND, Gtk::FILL | Gtk::EXPAND, 10, 5);
            m_table.attach(m_cbt_event_type[i] , 3, 4, i + 1, i + 2, Gtk::FILL | Gtk::EXPAND, Gtk::FILL | Gtk::EXPAND, 10, 5);
            m_table.attach(m_cbt_event_orien[i], 4, 5, i + 1, i + 2, Gtk::FILL | Gtk::EXPAND, Gtk::FILL | Gtk::EXPAND, 10, 5);
            m_table.attach(m_cbt_event_key[i]  , 5, 6, i + 1, i + 2, Gtk::FILL | Gtk::EXPAND, Gtk::FILL | Gtk::EXPAND, 10, 5);
        }

        add_button(Gtk::Stock::OK, Gtk::RESPONSE_ACCEPT);
        add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_REJECT);
    }

    RotationConfigDialog::~RotationConfigDialog ()
    {
        delete[] m_sb_axis1;
        delete[] m_sb_axis2;
        delete[] m_cbt_event_type;
        delete[] m_cbt_event_orien;
        delete[] m_cbt_event_key;
    }

    void
    RotationConfigDialog::on_response (int response_id)
    {
        int i, axis1, axis2, event_key;
        GestureType event_type;
        OrientationType event_orien;

        if (response_id == Gtk::RESPONSE_ACCEPT)
        {
            for (i = 0; i < m_view_config.get_n_rotation(); ++i)
            {
                axis1 = m_sb_axis1[i].get_value() - 1;
                axis2 = m_sb_axis2[i].get_value() - 1;
                m_view_config.set_axis(i, axis1, axis2);

                event_type  = named_gesture_type[ m_cbt_event_type[i].get_active_row_number() ].value;
                event_orien = named_orientation_type[ m_cbt_event_orien[i].get_active_row_number() ].value;
                event_key   = named_key_mask[ m_cbt_event_key[i].get_active_row_number() ].value;
                m_view_config.set_event(i, event_type, event_orien, event_key);
            }
        }

        return;
    }

}
