/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef MESH_TYPES_HH
#define MESH_TYPES_HH

#include <QString>

///////////////////////////////////////////////////////
/// \brief define all the mesh types
///////////////////////////////////////////////////////
namespace MeshTypes
{
	/// \todo coupler Discoidal_cell tesselation and spheroidal cell tesselation : the same exept for dimension.
	enum MeshType
	{
		Weighted_Voronoi_Tesselation,		///< a weighted vornoi tesselation
		Round_Cell_Tesselation,				///< a biological cell representation ( including nuclei) based on a 3D weighted voronoi tesselation. Weight is the diameter
		Unknow 
	};

	/// \brief return the corresponding name tp the mesh type
	QString getName(MeshType pType);

}

#endif // MESH_TYPES_HH