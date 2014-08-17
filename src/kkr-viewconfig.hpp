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

#ifndef INCLUDE_GUARD_92F3646A_A9BD_11E2_9888_8C736E05BBBD
#define INCLUDE_GUARD_92F3646A_A9BD_11E2_9888_8C736E05BBBD

#include <gdkmm.h>
#include "kkr-color.hpp"

namespace Kkr
{

    enum GestureType
    {
        GESTURE_NOTHING = 0,
        GESTURE_LEFT_BUTTON_DRAG,
        GESTURE_MIDDLE_BUTTON_DRAG,
        GESTURE_RIGHT_BUTTON_DRAG,
        GESTURE_SCROLL,
        GESTURE_ARROW_KEY
    };

    enum OrientationType
    {
        ORIENTATION_HORIZONTAL,
        ORIENTATION_VERTICAL
    };

    typedef Gdk::ModifierType KeyMask;
    static const KeyMask KEY_SHIFT_MASK = Gdk::SHIFT_MASK;
    static const KeyMask KEY_CTRL_MASK  = Gdk::CONTROL_MASK;
    static const KeyMask KEY_ALT_MASK   = Gdk::MOD1_MASK;
    static const KeyMask KEY_MASK       = KEY_SHIFT_MASK | KEY_CTRL_MASK | KEY_ALT_MASK;

    class ViewConfig
    {
        public:
            ViewConfig (int dimention,
                        int n_rotation);
            ~ViewConfig ();

            inline int            get_dimention  () const;
            inline int            get_n_rotation () const;
            inline double         get_scale      () const;
            inline double         get_focus      () const;
            inline double         get_view_z     () const;
            inline Color        & get_bg_color   ();
            inline const Color  & get_bg_color   () const;
            inline Color        & get_edge_color ();
            inline const Color  & get_edge_color () const;
            inline Color        & get_face_color ();
            inline const Color  & get_face_color () const;
            inline const double * get_matrix     () const;

            void get_axis (int index,
                           int & axis1,
                           int & axis2) const;
            void get_event (int               index,
                            GestureType     & event_type,
                            OrientationType & event_orien,
                            int             & event_key) const;

            void set_scale      (double scale);
            void set_view_z     (double view_z);
            void set_bg_color   (const Color & color);
            void set_edge_color (const Color & color);
            void set_face_color (const Color & color);
            void set_axis       (int index,
                                 int axis1,
                                 int axis2);
            void set_event      (int             index,
                                 GestureType     event_type,
                                 OrientationType event_orien,
                                 int             event_key);

            void reset_matrix ();
            void rotate       (int    index,
                               double angle);
            bool exec_events  (GestureType event_type,
                               KeyMask     event_key,
                               double      angle_x,
                               double      angle_y);

        private:
            struct Rotation
            {
                int             axis1;
                int             axis2;
                GestureType     event_type;
                OrientationType event_orien;
                int             event_key;
            };

            ViewConfig (const ViewConfig & other); // Do not call!
            ViewConfig & operator = (const ViewConfig & other); // Do not call!

            const int  m_dimention;
            const int  m_n_rotation;
            double     m_scale;
            double     m_focus;
            Color      m_bg_color;
            Color      m_edge_color;
            Color      m_face_color;
            double     m_view_z;
            Rotation * m_rotation;
            double   * m_matrix;
    };

    /**
     * Get the dimension of the current space.
     */
    inline int
    ViewConfig::get_dimention () const
    {
        return m_dimention;
    }

    /**
     * Get the number of kinds of rotation.
     */
    inline int
    ViewConfig::get_n_rotation () const
    {
        return m_n_rotation;
    }

    inline double
    ViewConfig::get_scale () const
    {
        return m_scale;
    }

    inline double
    ViewConfig::get_focus () const
    {
        return m_scale * m_focus;
    }

    inline double
    ViewConfig::get_view_z () const
    {
        return m_view_z;
    }

    inline Color &
    ViewConfig::get_bg_color ()
    {
        return m_bg_color;
    }

    inline const Color &
    ViewConfig::get_bg_color () const
    {
        return m_bg_color;
    }

    inline Color &
    ViewConfig::get_edge_color ()
    {
        return m_edge_color;
    }

    inline const Color &
    ViewConfig::get_edge_color () const
    {
        return m_edge_color;
    }

    inline Color &
    ViewConfig::get_face_color ()
    {
        return m_face_color;
    }

    inline const Color &
    ViewConfig::get_face_color () const
    {
        return m_face_color;
    }

    /**
     * Get a transformation matrix.
     */
    inline const double *
    ViewConfig::get_matrix () const
    {
        return const_cast<const double *>(m_matrix);
    }

}

#endif /* !INCLUDE_GUARD_92F3646A_A9BD_11E2_9888_8C736E05BBBD */
