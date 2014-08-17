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

#include <cassert>
#include <cmath>
#include <stdexcept>
#include "kkr-viewconfig.hpp"

namespace Kkr
{

    /**
     * Default bindings of events
     */
    static const int default_event_binding[][3] =
    {
        {GESTURE_LEFT_BUTTON_DRAG , ORIENTATION_VERTICAL, 0},
        {GESTURE_LEFT_BUTTON_DRAG , ORIENTATION_HORIZONTAL, 0},
        {GESTURE_RIGHT_BUTTON_DRAG, ORIENTATION_HORIZONTAL, 0},
        {GESTURE_RIGHT_BUTTON_DRAG, ORIENTATION_VERTICAL, 0},
        {GESTURE_SCROLL           , ORIENTATION_VERTICAL, 0},
        {GESTURE_ARROW_KEY        , ORIENTATION_VERTICAL, 0},
        {GESTURE_ARROW_KEY        , ORIENTATION_HORIZONTAL, 0}
    };

    ViewConfig::ViewConfig (int dimention,
                            int n_rotation)
        : m_dimention(dimention),
          m_n_rotation(n_rotation),
          m_scale(1.0),
          m_focus(0.0),
          m_view_z(0.0),
          m_rotation(0),
          m_matrix(0)
    {
        assert(dimention >= 2);
        assert(n_rotation > 0);

        m_rotation = new Rotation [n_rotation];
        m_matrix   = new double [dimention * dimention];

        for (int i = 0; i < m_n_rotation; ++i)
        {
            m_rotation[i].axis1 = 0;
            if (i + 1 < dimention)
                m_rotation[i].axis2 = i + 1;
            else
                m_rotation[i].axis2 = 0;

            if (i < (int) G_N_ELEMENTS(default_event_binding))
            {
                m_rotation[i].event_type  = (GestureType) default_event_binding[i][0];
                m_rotation[i].event_orien = (OrientationType) default_event_binding[i][1];
                m_rotation[i].event_key   = (KeyMask) default_event_binding[i][2];
            }
            else
            {
                m_rotation[i].event_type = GESTURE_NOTHING;
            }
        }

        reset_matrix();
        set_view_z(3.0);
    }

    ViewConfig::~ViewConfig ()
    {
        delete[] m_rotation;
        delete[] m_matrix;
    }

    void
    ViewConfig::get_axis (int index,
                          int & axis1,
                          int & axis2) const
    {
        if (index < 0 || index >= m_n_rotation) throw std::out_of_range("index is out of range");

        axis1 = m_rotation[index].axis1;
        axis2 = m_rotation[index].axis2;

        return;
    }

    void
    ViewConfig::get_event (int               index,
                           GestureType     & event_type,
                           OrientationType & event_orien,
                           int             & event_key) const
    {
        if (index < 0 || index >= m_n_rotation) throw std::out_of_range("index is out of range");

        event_type  = m_rotation[index].event_type;
        event_orien = m_rotation[index].event_orien;
        event_key   = m_rotation[index].event_key;

        return;
    }

    void
    ViewConfig::set_scale (double scale)
    {
        m_scale = scale;

        return;
    }

    void
    ViewConfig::set_view_z (double view_z)
    {
        m_view_z = view_z;
        m_focus = std::sqrt(view_z * view_z / m_dimention - 1.0);

        return;
    }

    void
    ViewConfig::set_bg_color (const Color & color)
    {
        m_bg_color = color;

        return;
    }

    void
    ViewConfig::set_edge_color (const Color & color)
    {
        m_edge_color = color;

        return;
    }

    void
    ViewConfig::set_face_color (const Color & color)
    {
        m_face_color = color;

        return;
    }

    void
    ViewConfig::set_axis (int index,
                          int axis1,
                          int axis2)
    {
        if (index < 0 || index >= m_n_rotation) throw std::out_of_range("index is out of range");
        if (axis1 < 0 || axis1 >= m_dimention) throw std::out_of_range("axis1 is out of range");
        if (axis2 < 0 || axis2 >= m_dimention) throw std::out_of_range("axis2 is out of range");

        m_rotation[index].axis1 = axis1;
        m_rotation[index].axis2 = axis2;

        return;
    }

    void
    ViewConfig::set_event (int             index,
                           GestureType     event_type,
                           OrientationType event_orien,
                           int             event_key)
    {
        if (index < 0 || index >= m_n_rotation) throw std::out_of_range("index is out of range");

        m_rotation[index].event_type  = event_type;
        m_rotation[index].event_orien = event_orien;
        m_rotation[index].event_key   = event_key;

        return;
    }

    /**
     * Set an identity matrix to the current transformation matrix.
     */
    void
    ViewConfig::reset_matrix ()
    {
        int i, j;

        for (i = 0; i < m_dimention; ++i)
        {
            for (j = 0; j < m_dimention; ++j)
            {
                m_matrix[i * m_dimention + j] = (i == j ? 1.0 : 0.0);
            }
        }

        return;
    }

    void
    ViewConfig::rotate (int    index,
                        double angle)
    {
        double *row_p, *row_q, sin_v, cos_v, x, y;
        int p, q;

        if (index < 0 || index >= m_n_rotation) throw std::out_of_range("index is out of range");

        p = m_rotation[index].axis1;
        q = m_rotation[index].axis2;

        if (p != q)
        {
            sin_v = std::sin(angle);
            cos_v = std::cos(angle);

            row_p = &m_matrix[p * m_dimention];
            row_q = &m_matrix[q * m_dimention];

            for (int i = 0; i < m_dimention; ++i)
            {
                x = row_p[i];
                y = row_q[i];

                row_p[i] = cos_v * x - sin_v * y;
                row_q[i] = sin_v * x + cos_v * y;
            }
        }

        return;
    }

    /**
     * Execute a registered event and update the transformation matrix.
     * Returns true if one or more events (including the given event) are executed.
     */
    bool
    ViewConfig::exec_events (GestureType event_type,
                             KeyMask     event_key,
                             double      angle_x,
                             double      angle_y)
    {
        bool is_accepted = false;
        int i;

        event_key = static_cast<KeyMask>(event_key & KEY_MASK);

        for (i = 0; i < m_n_rotation; ++i)
        {
            if (m_rotation[i].event_type == event_type && m_rotation[i].event_key == event_key)
            {
                if (m_rotation[i].event_orien == ORIENTATION_HORIZONTAL)
                {
                    rotate(i, angle_x);
                }
                else
                {
                    rotate(i, angle_y);
                }

                is_accepted = true;
            }
        }

        return is_accepted;
    }

}
