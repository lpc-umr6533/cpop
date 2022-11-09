/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef MESH_2D_SETTINGS_HH
#define MESH_2D_SETTINGS_HH

#include "GeometrySettings.hh"
#include "AgentSettings.hh"
#include "Mesh.hh"

//#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Regular_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

/// @namespace Settings
namespace Settings
{
	/// @namespace Geometry
	namespace Geometry
	{
		/// \brief 2D mesh settings
		namespace Mesh2D
		{
			typedef Mesh<double, Point_2, Vector_2> 						t_Mesh_2;			///< \brief 2D Mesh
			
			typedef CGAL::Regular_triangulation_vertex_base_2< K > 	Vb_2;				///< \brief CGAL 2D vertex base
			typedef CGAL::Triangulation_vertex_base_with_info_2<const Settings::nAgent::t_SpatialableAgent_2*, K, Vb_2> Vb_2_info;	///< \brief CGAL 2D vertex base info ( info is CPOP cell into)
			// typedef CGAL::Triangulation_vertex_base<Traits_2> Vb_2;
			
			/// \brief CGAL 2D face base
			/// \details no hidden point
			typedef CGAL::Regular_triangulation_face_base_2<K>	    	      Fb_2;	

			typedef CGAL::Triangulation_data_structure_2<Vb_2_info, Fb_2> 	Tds_2;				///< \brief CGAL 2D triangulation data structure
			typedef CGAL::Regular_triangulation_2<K, Tds_2> 		    	RT_2;				///< \brief CGAL 2D regualar triangulation definition

			typedef RT_2::Weighted_point									Weighted_point_2;	///< \brief CGAL 2D weighted point
			typedef RT_2::Vertex_handle 									Vertex_2_handle;	///< \brief CGAL 2D vertex handle
			typedef K::Circle_2     										Circle_2;			///< \brief CGAL 2D circle
			typedef K::Segment_2        									Segment_2;			///< \brief CGAL 2D Segment
			typedef K::Line_2   											Line_2;				///< \brief CGAL 2D Line
			typedef K::Triangle_2   										Triangle_2;			///< \brief CGAL 2D Triangle

			typedef K::Iso_rectangle_2	Iso_rectangle_2;										///< \brief CGAL 2D rectangle

		}
	}
}

#endif // MESH_2D_SETTINGS_HH
