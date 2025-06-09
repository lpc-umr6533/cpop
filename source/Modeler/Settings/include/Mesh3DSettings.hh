#ifndef MESH_3D_SETTINGS_HH
#define MESH_3D_SETTINGS_HH

#include "GeometrySettings.hh"
#include "AgentSettings.hh"
#include "Mesh.hh"

#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Regular_triangulation_3.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>
#include <CGAL/Triangulation_cell_base_3.h>
#include <CGAL/Triangulation_3.h>

namespace Settings::Geometry::Mesh3D {

using t_Mesh_3 = Mesh<double, Point_3, Vector_3>;			///< \brief CGAL 3D Mesh

using Vb0 = CGAL::Regular_triangulation_vertex_base_3<K>;
using Vb_3 = CGAL::Triangulation_vertex_base_with_info_3<const Settings::nAgent::t_SpatialableAgent_3*, K, Vb0>;	///< \brief CGAL Vertex base with info ( info is CPOP cell )

/// \brief CGAL 3D cell base
/// \details no hidden point
using Cb_3 = CGAL::Regular_triangulation_cell_base_3<K>;        // keep hidden point and can return some "empty vertex". (but quicker)

using Tds_3 = CGAL::Triangulation_data_structure_3<Vb_3, Cb_3>; /// < \brief CGAL 3D triangulation data structure
using RT_3 = CGAL::Regular_triangulation_3<K, Tds_3>;           /// < \brief CGAL 3D Regular triangulation

using DT_3 = CGAL::Delaunay_triangulation_3<K>;                 /// < \brief CGAL 3D delaunay triangulation
using Base_Tri_3 = CGAL::Triangulation_3<K>;                    /// < \brief CGAL Triangulation 3

using Weighted_point_3 = RT_3::Weighted_point;                  /// < \brief CGAL 3D weighted point
using Vertex_3_handle = RT_3::Vertex_handle;                    /// < \brief CGAL 3D vertex handle
using Segment_3 = RT_3::Segment;                                /// < \brief CGAL 3D segment
using Ray_3 = RT_3::Ray;                                        /// < \brief CGAL 3D Ray
using Plane_3 = K::Plane_3;                                     /// < \brief CGAL 3D plane
using Polyhedron_3 = CGAL::Polyhedron_3<K>;                     /// < \brief CGAL 3D polyhedron
using Tetrahedron_3 = K::Tetrahedron_3;                         /// < \brief CGAL 3D Tetrahedron
using Sphere_3 = K::Sphere_3;                                   /// < \brief CGAL 3D Sphere
using Circle_3 = K::Circle_3;                                   /// < \brief CGAL 3D Circle
using Triangle_3 = K::Triangle_3;                               /// < \brief CGAL 3D Triangle

}

#endif
