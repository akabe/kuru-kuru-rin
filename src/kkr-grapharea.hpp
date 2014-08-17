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

#ifndef INCLUDE_GUARD_41385A54_A9BD_11E2_9BEE_8C736E05BBBD
#define INCLUDE_GUARD_41385A54_A9BD_11E2_9BEE_8C736E05BBBD

#include <gtkmm/drawingarea.h>
#include "kkr-hypercube.hpp"
#include "kkr-viewconfig.hpp"

namespace Kkr
{

    class GraphArea : public Gtk::DrawingArea
    {
        public:
            GraphArea ();
            virtual ~GraphArea ();

            void set_view_config (ViewConfig & view_config);

        protected:
            virtual bool on_expose_event         (GdkEventExpose * event);
            virtual bool on_button_press_event   (GdkEventButton * event);
            virtual bool on_button_release_event (GdkEventButton * event);
            virtual bool on_motion_notify_event  (GdkEventMotion * event);
            virtual bool on_scroll_event         (GdkEventScroll * event);
            virtual bool on_key_press_event      (GdkEventKey    * event);

        private:
            ViewConfig * m_view_config;
            Hypercube  * m_hypercube;

            GestureType  m_motion_button;
            double       m_motion_x;
            double       m_motion_y;
    };

}

#endif /* !INCLUDE_GUARD_41385A54_A9BD_11E2_9BEE_8C736E05BBBD */
