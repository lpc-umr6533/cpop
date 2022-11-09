/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
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
//#include <CGAL/Default.h>
#include <CGAL/Triangulation_3.h>

/// @namespace Settings
namespace Settings
{
	/// 
	namespace Geometry
	{
		namespace Mesh3D
		{
			typedef Mesh<double, Point_3, Vector_3> 					t_Mesh_3;			///< \brief CGAL 3D Mesh

			typedef CGAL::Regular_triangulation_vertex_base_3<K>        Vb0;
			typedef CGAL::Triangulation_vertex_base_with_info_3<const Settings::nAgent::t_SpatialableAgent_3*, K, Vb0> Vb_3;	///< \brief CGAL Vertex base with info ( info is CPOP cell )
			
			/// \brief CGAL 3D cell base
			/// \details no hidden point
			//typedef CGAL::Triangulation_cell_base_3<K>      			Cb_3;	
			typedef CGAL::Regular_triangulation_cell_base_3<K>	Cb_3; // keep hidden point and can return some "empty vertex". (but quicker)

			typedef CGAL::Triangulation_data_structure_3<Vb_3, Cb_3> 	Tds_3; 				///< \brief CGAL 3D triangulation data structure
			typedef CGAL::Regular_triangulation_3<K, Tds_3>     		RT_3;				///< \brief CGAL 3D Regular triangulation
	
			typedef CGAL::Delaunay_triangulation_3<K>	                DT_3;				///< \brief CGAL 3D delaunay triangulation
			typedef CGAL::Triangulation_3<K>                            Base_Tri_3;			///< \brief CGAL Triangulation 3

			typedef RT_3::Weighted_point        						Weighted_point_3;	///< \brief CGAL 3D weighted point
			typedef RT_3::Vertex_handle 								Vertex_3_handle;	///< \brief CGAL 3D vertex handle
			typedef RT_3::Segment										Segment_3;			///< \brief CGAL 3D segment
			typedef RT_3::Ray											Ray_3;				///< \brief CGAL 3D Ray
			typedef K::Plane_3											Plane_3;			///< \brief CGAL 3D plane
			typedef CGAL::Polyhedron_3<K> 								Polyhedron_3;		///< \brief CGAL 3D polyhedron
			typedef K::Tetrahedron_3    								Tetrahedron_3;		///< \brief CGAL 3D Tetrahedron
			typedef K::Sphere_3     									Sphere_3;			///< \brief CGAL 3D Sphere
			typedef K::Circle_3         								Circle_3;			///< \brief CGAL 3D Circle
			typedef K::Triangle_3   									Triangle_3;			///< \brief CGAL 3D Triangle
			//~ typedef CGAL::Polyhedron_3<K> 								Polyhedron_3;		///< \brief CGAL 3D polyhedron
			//~ typedef CGAL::Tetrahedron_3<K> 								Tetrahedron_3;		///< \brief CGAL 3D Tetrahedron
			//~ typedef CGAL::Sphere_3<K>									Sphere_3;			///< \brief CGAL 3D Sphere
			//~ typedef CGAL::Circle_3<K>									Circle_3;			///< \brief CGAL 3D Circle
			//~ typedef CGAL::Triangle_3<K>									Triangle_3;			///< \brief CGAL 3D Triangle
		}
	}
}

#endif // MESH_3D_SETTINGS_HH
