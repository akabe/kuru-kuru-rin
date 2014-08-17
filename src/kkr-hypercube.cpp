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
#include <iostream>
#include <stdexcept>
#include <gdkmm.h>
#include "kkr-hypercube.hpp"

namespace Kkr
{

    Hypercube::Hypercube (int dimention)
        : m_dimention(dimention),
          m_n_vertexes((1 << dimention)), /* (2 ** dimention) */
          m_n_edges((1 << (dimention - 1)) * dimention), /* (2 ** (dimention-1)) * dimention */
          m_n_faces((1 << (dimention - 3)) * dimention * (dimention - 1)),
          m_org_vertex(0),
          m_rot_vertex(0),
          m_prj_vertex(0),
          m_edge(0),
          m_rendering_id(0)
    {
        assert(dimention >= 3);

        m_org_vertex = new double [dimention * m_n_vertexes];
        m_rot_vertex = new double [3 * m_n_vertexes];
        m_prj_vertex = new double [3 * m_n_vertexes];
        m_edge       = new Edge [m_n_edges];
        m_face       = new Face [m_n_faces];

        init_vertex();
        init_edge();
        init_face();
    }

    void
    Hypercube::init_vertex ()
    {
        double *v = m_org_vertex;
        int n, i, j;

        for (i = 0; i < m_n_vertexes; ++i)
        {
            n = i;
            for (j = 0; j < m_dimention; ++j)
            {
                v[j] = (n & 1 ? -1.0 : 1.0);
                n >>= 1;
            }

            v += m_dimention;
        }

        return;
    }

    void
    Hypercube::init_edge ()
    {
        int n = 0, k, i, j;

        /* Joint each pair of two points whose hamming distance is 1. */
        for (i = 0; i < m_n_vertexes; ++i)
        {
            for (j = 0; j < m_dimention; ++j)
            {
                k = (i ^ (1 << j));
                if (i < k)
                {
                    m_edge[n].vertex[0] = &m_prj_vertex[i * 3];
                    m_edge[n].vertex[1] = &m_prj_vertex[k * 3];
                    if (++n == m_n_edges) break;
                }
            }
        }

        return;
    }

    void
    Hypercube::init_face ()
    {
        int i, j, k, m, n, r, p = 0;

        for (i = 0; i < m_n_vertexes; ++i)
        {
            for (j = 0; j < m_dimention; ++j)
            {
                m = (i ^ (1 << j));
                if (i < m)
                {
                    for (k = j + 1; k < m_dimention; ++k)
                    {
                        n = (i ^ (1 << k));
                        if (i < n)
                        {
                            r = (n ^ (1 << j));

                            m_face[p].vertex[0] = &m_prj_vertex[i * 3];
                            m_face[p].vertex[1] = &m_prj_vertex[m * 3];
                            m_face[p].vertex[2] = &m_prj_vertex[r * 3];
                            m_face[p].vertex[3] = &m_prj_vertex[n * 3];

                            if (++p == m_n_faces) return;
                        }
                    }
                }
            }
        }

        return;
    }

    Hypercube::~Hypercube ()
    {
        delete[] m_org_vertex;
        delete[] m_rot_vertex;
        delete[] m_prj_vertex;
        delete[] m_edge;
    }

    void
    Hypercube::transform (const double * matrix)
    {
        const double *src = m_org_vertex;
        double *dest = m_rot_vertex, val;
        int i, j, k;

        assert(matrix != NULL);

        for (i = 0; i < m_n_vertexes; ++i)
        {
            for (j = 0; j < 3; ++j)
            {
                val = 0.0;
                for (k = 0; k < m_dimention; ++k)
                {
                    val += matrix[j * m_dimention + k] * src[k];
                }
                dest[j] = val;
            }

            src += m_dimention;
            dest += 3;
        }

        return;
    }

    void
    Hypercube::project (double focus,
                        double view_z)
    {
        const double *src = m_rot_vertex;
        double *dest = m_prj_vertex, z, a;
        int i;

        for (i = 0; i < m_n_vertexes; ++i)
        {
            z = src[2] + view_z;
            a = focus / z;

            dest[0] = src[0] * a;
            dest[1] = src[1] * a;
            dest[2] = src[0] * src[0] + src[1] * src[1] + z * z;

            src += 3;
            dest += 3;
        }

        return;
    }

    void
    Hypercube::draw (Cairo::RefPtr<Cairo::Context> & cr,
                     const Color                   & edge_color,
                     const Color                   & face_color)
    {
        double distance;
        int i, j, n;

        /* Measure the distance between a face and the origin. */
        for (i = 0; i < m_n_faces; ++i)
        {
            m_face[i].distance = m_face[i].vertex[0][2] + m_face[i].vertex[1][2] + m_face[i].vertex[2][2] + m_face[i].vertex[3][2];
            m_face[i].is_drawn = false;
        }

        cr->set_source_rgba(face_color.red, face_color.green, face_color.blue, face_color.alpha);
        for (i = 0; i < m_n_faces; ++i)
        {
            /* Find the face whose distance from the origin is maximum. */
            distance = -1.0;
            n = 0;
            for (j = 0; j < m_n_faces; ++j)
            {
                if (!m_face[j].is_drawn)
                {
                    if (distance < m_face[j].distance)
                    {
                        distance = m_face[j].distance;
                        n = j;
                    }
                }
            }

            /* Draw the face. */
            cr->move_to(m_face[n].vertex[0][0], m_face[n].vertex[0][1]);
            cr->line_to(m_face[n].vertex[1][0], m_face[n].vertex[1][1]);
            cr->line_to(m_face[n].vertex[2][0], m_face[n].vertex[2][1]);
            cr->line_to(m_face[n].vertex[3][0], m_face[n].vertex[3][1]);
            cr->close_path();
            cr->fill();

            m_face[n].is_drawn = true;
        }

        cr->set_source_rgba(edge_color.red, edge_color.green, edge_color.blue, edge_color.alpha);
        for (i = 0; i < m_n_edges; ++i)
        {
            cr->move_to(m_edge[i].vertex[0][0], m_edge[i].vertex[0][1]);
            cr->line_to(m_edge[i].vertex[1][0], m_edge[i].vertex[1][1]);
            cr->stroke();
        }

        return;
    }

}
