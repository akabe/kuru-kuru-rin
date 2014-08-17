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
#include <cmath>
#include <algorithm>

#include "kkr-grapharea.hpp"

#define KKR_KEY_ARROW_LEFT       (65361)
#define KKR_KEY_ARROW_UP         (65362)
#define KKR_KEY_ARROW_RIGHT      (65363)
#define KKR_KEY_ARROW_DOWN       (65364)
#define KKR_RADIAN_PER_PIXEL     (M_PI / 1000.0)
#define KKR_RADIAN_PER_SCROLL    (M_PI / 100.0)
#define KKR_RADIAN_PER_KEY_PRESS (M_PI / 100.0)

namespace Kkr
{

    GraphArea::GraphArea ()
        : m_view_config(0),
          m_hypercube(0),
          m_motion_button(GESTURE_NOTHING),
          m_motion_x(0.0),
          m_motion_y(0.0)
    {
        set_can_focus(true); // To accept "key-press" and "key-release" events
        set_events(Gdk::EXPOSURE_MASK |
                   Gdk::BUTTON_MOTION_MASK |
                   Gdk::BUTTON_PRESS_MASK |
                   Gdk::BUTTON_RELEASE_MASK |
                   Gdk::SCROLL_MASK |
                   Gdk::KEY_PRESS_MASK);
    }

    GraphArea::~GraphArea ()
    {
        if (m_hypercube != 0) delete m_hypercube;
    }

    void
    GraphArea::set_view_config (ViewConfig & view_config)
    {
        if (m_hypercube != 0) delete m_hypercube;

        m_view_config = &view_config;
        m_hypercube = new Hypercube(view_config.get_dimention());

        return;
    }

    bool
    GraphArea::on_expose_event (GdkEventExpose * event G_GNUC_UNUSED)
    {
        if (m_view_config != 0)
        {
            Cairo::RefPtr<Cairo::Context> cr = get_window()->create_cairo_context();

            double width  = get_width() / 2.0;
            double height = get_height() / 2.0;

            double focus = m_view_config->get_focus() * std::min(width, height);

            m_hypercube->transform(m_view_config->get_matrix());
            m_hypercube->project(focus, m_view_config->get_view_z());

            cr->translate(width, height);

            const Color & color = m_view_config->get_bg_color();
            cr->set_source_rgba(color.red, color.green, color.blue, color.alpha);
            cr->paint();

            m_hypercube->draw(cr, m_view_config->get_edge_color(), m_view_config->get_face_color());

            return true;
        }

        return false;
    }

    bool
    GraphArea::on_button_press_event (GdkEventButton * event)
    {
        /* Start dragging */
        m_motion_x = event->x;
        m_motion_y = event->y;

        switch (event->button)
        {
            case 1:
                m_motion_button = GESTURE_LEFT_BUTTON_DRAG;
                break;
            case 2:
                m_motion_button = GESTURE_MIDDLE_BUTTON_DRAG;
                break;
            case 3:
                m_motion_button = GESTURE_RIGHT_BUTTON_DRAG;
                break;
        }

        return true;
    }

    bool
    GraphArea::on_button_release_event (GdkEventButton * event G_GNUC_UNUSED)
    {
        m_motion_button = GESTURE_NOTHING;

        return true;
    }

    bool
    GraphArea::on_motion_notify_event  (GdkEventMotion * event)
    {
        bool is_accepted = false;

        if (m_view_config != NULL && m_motion_button != GESTURE_NOTHING)
        {
            const double angle_x = (event->x - m_motion_x) * KKR_RADIAN_PER_PIXEL;
            const double angle_y = (event->y - m_motion_y) * KKR_RADIAN_PER_PIXEL;

            m_motion_x = event->x;
            m_motion_y = event->y;

            is_accepted = m_view_config->exec_events(m_motion_button,
                                                     static_cast<KeyMask>(event->state),
                                                     angle_x, angle_y);

            if (is_accepted) queue_draw();
        }

        return is_accepted;
    }

    bool
    GraphArea::on_scroll_event (GdkEventScroll * event)
    {
        bool is_accepted = false;

        if (m_view_config != 0)
        {
            switch (event->direction)
            {
                case GDK_SCROLL_UP:
                    is_accepted = m_view_config->exec_events(GESTURE_SCROLL,
                                                            static_cast<KeyMask>(event->state),
                                                            0.0, KKR_RADIAN_PER_SCROLL);
                    break;
                case GDK_SCROLL_DOWN:
                    is_accepted = m_view_config->exec_events(GESTURE_SCROLL,
                                                            static_cast<KeyMask>(event->state),
                                                            0.0, -KKR_RADIAN_PER_SCROLL);
                    break;
                case GDK_SCROLL_LEFT:
                    is_accepted = m_view_config->exec_events(GESTURE_SCROLL,
                                                            static_cast<KeyMask>(event->state),
                                                            KKR_RADIAN_PER_SCROLL, 0.0);
                    break;
                case GDK_SCROLL_RIGHT:
                    is_accepted = m_view_config->exec_events(GESTURE_SCROLL,
                                                            static_cast<KeyMask>(event->state),
                                                            -KKR_RADIAN_PER_SCROLL, 0.0);
                    break;
            }

            if (is_accepted) queue_draw();
        }

        return is_accepted;
    }

    bool
    GraphArea::on_key_press_event (GdkEventKey * event)
    {
        bool is_accepted = false;

        if (m_view_config != 0)
        {
            switch (event->keyval)
            {
                case KKR_KEY_ARROW_UP:
                    is_accepted = m_view_config->exec_events(GESTURE_ARROW_KEY,
                                                             static_cast<KeyMask>(event->state),
                                                             0.0, KKR_RADIAN_PER_KEY_PRESS);
                    break;
                case KKR_KEY_ARROW_DOWN:
                    is_accepted = m_view_config->exec_events(GESTURE_ARROW_KEY,
                                                             static_cast<KeyMask>(event->state),
                                                             0.0, -KKR_RADIAN_PER_KEY_PRESS);
                    break;
                case KKR_KEY_ARROW_LEFT:
                    is_accepted = m_view_config->exec_events(GESTURE_ARROW_KEY,
                                                             static_cast<KeyMask>(event->state),
                                                             KKR_RADIAN_PER_KEY_PRESS, 0.0);
                    break;
                case KKR_KEY_ARROW_RIGHT:
                    is_accepted = m_view_config->exec_events(GESTURE_ARROW_KEY,
                                                             static_cast<KeyMask>(event->state),
                                                             -KKR_RADIAN_PER_KEY_PRESS, 0.0);
                break;
            }

            if (is_accepted) queue_draw();
        }

        return is_accepted;
    }

}
