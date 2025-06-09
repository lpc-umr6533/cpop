#ifndef MESH_2D_SETTINGS_HH
#define MESH_2D_SETTINGS_HH

#include "GeometrySettings.hh"
#include "AgentSettings.hh"
#include "Mesh.hh"

#include <CGAL/Regular_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

/// \brief 2D mesh settings
namespace Settings::Geometry::Mesh2D {

using t_Mesh_2 = Mesh<double, Point_2, Vector_2>; ///< \brief 2D Mesh

using Vb_2 = CGAL::Regular_triangulation_vertex_base_2<K>;  ///< \brief CGAL 2D vertex base
using Vb_2_info = CGAL::Triangulation_vertex_base_with_info_2<const Settings::nAgent::t_SpatialableAgent_2*, K, Vb_2>; ///< \brief CGAL 2D vertex base info ( info is CPOP cell into)

/// \brief CGAL 2D face base
/// \details no hidden point
using Fb_2 = CGAL::Regular_triangulation_face_base_2<K>;

using Tds_2 = CGAL::Triangulation_data_structure_2<Vb_2_info, Fb_2>; /// < \brief CGAL 2D triangulation data structure
using RT_2 = CGAL::Regular_triangulation_2<K, Tds_2>;                /// < \brief CGAL 2D regualar triangulation definition

using Weighted_point_2 = RT_2::Weighted_point;                       /// < \brief CGAL 2D weighted point
using Vertex_2_handle = RT_2::Vertex_handle;                         /// < \brief CGAL 2D vertex handle
using Circle_2 = K::Circle_2;                                        /// < \brief CGAL 2D circle
using Segment_2 = K::Segment_2;                                      /// < \brief CGAL 2D Segment
using Line_2 = K::Line_2;                                            /// < \brief CGAL 2D Line
using Triangle_2 = K::Triangle_2;                                    /// < \brief CGAL 2D Triangle

using Iso_rectangle_2 = K::Iso_rectangle_2;                          /// < \brief CGAL 2D rectangle

}

#endif
