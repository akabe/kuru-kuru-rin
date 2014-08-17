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

#ifndef INCLUDE_GUARD_755256FA_AA3A_11E2_BF55_8C736E05BBBD
#define INCLUDE_GUARD_755256FA_AA3A_11E2_BF55_8C736E05BBBD

#include <cstddef>
#include <cairomm/cairomm.h>
#include "kkr-color.hpp"

namespace Kkr
{

    class Hypercube
    {
        public:
            Hypercube (int dimention);
            ~Hypercube ();

            void transform (const double * matrix);
            void project   (double focus,
                            double view_z);
            void draw      (Cairo::RefPtr<Cairo::Context> & cr,
                            const Color                   & edge_color,
                            const Color                   & face_color);

        private:
            struct Edge
            {
                const double * vertex [2];
                /** The end point of the edge (on the projected two-dimensional space). */
            };
            struct Face
            {
                const double * vertex [4];
                double distance;
                bool is_drawn;
            };

            void init_vertex ();
            void init_edge   ();
            void init_face   ();

            int       m_dimention;  /** The dimension of the current space. */
            int       m_n_vertexes; /** The number of vertexes of a hypercube. */
            int       m_n_edges;    /** The number of edges of a hypercube. */
            int       m_n_faces;    /** The number of faces of a hypercube. */
            double  * m_org_vertex; /** The original coordinates of a hypercube. */
            double  * m_rot_vertex; /** Rotated (three dimensional) coodinates. */
            double  * m_prj_vertex; /** Projected coordinates:
                                        an array of two dimensional vectors and
                                        (three-dimensional) distance. */
            Edge    * m_edge;       /** An array of edges. */
            Face    * m_face;       /** An array of faces. */
            int       m_rendering_id;
    };

}

#endif /* !INCLUDE_GUARD_755256FA_AA3A_11E2_BF55_8C736E05BBBD */
