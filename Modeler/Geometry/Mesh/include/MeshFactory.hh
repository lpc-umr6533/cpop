/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef MESH_FACTORY_HH
#define MESH_FACTORY_HH

#include "Cell.hh"
#include "MeshTypes.hh"
#include "Voronoi_2D_Mesh.hh"
#include "Voronoi_3D_Mesh.hh"
#include "SpatialDataStructure.hh"
#include "EnvironmentSettings.hh"

#include "AgentSettings.hh"
#include "CellSettings.hh"
#include "Mesh2DSettings.hh"
#include "Mesh3DSettings.hh"

#include <map>

#define MEAN_DEFAULT_NB_FACET 100
#define MEAN_DEFAULT_NB_POINT 100
#define DELTA_REFINNEMENT 0.		// stop the reffinement when we reach this "smallest" gain.

using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh2D;
using namespace Settings::Geometry::Mesh3D;
using namespace Settings::nAgent;
using namespace Settings::nCell;
using namespace Settings::nEnvironment;

//////////////////////////////////////////////////////////////////////////////
/// \brief The mesh factory produce requested mesh/mesher from user need ( type of Mesh)
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class MeshFactory
{
	
public:
	/// \brief constructor
	MeshFactory();
	/// \brief destructor
	~MeshFactory();

	/// \brief return the singleton of the mesh factory
	static MeshFactory* getInstance();

	/// \brief create a 2D mesh from a dimensioned layer
	t_Mesh_2* create_2DMesh(int* pError, const t_Sub_Env_2*, MeshTypes::MeshType, unsigned int pMeanNbPointPerCell = MEAN_DEFAULT_NB_POINT, double smallestGain = DELTA_REFINNEMENT);
	/// \brief create a 3D mesh from a dimensioned layer
	t_Mesh_3* create_3DMesh(int* pError, const t_Sub_Env_3*, MeshTypes::MeshType, unsigned int pMeanNbPointPerCell = MEAN_DEFAULT_NB_POINT, double smallestGain = DELTA_REFINNEMENT);

	/// \brief create a 2D mesh from a set fo staptialable agent
	t_Mesh_2* create_2DMesh(int* pError, std::set<t_Cell_2*>, MeshTypes::MeshType, unsigned int pMeanNbFacetPerCell = MEAN_DEFAULT_NB_POINT, double smallestGain = DELTA_REFINNEMENT);
	/// \brief create a 3D mesh from a set fo staptialable agent
	t_Mesh_3* create_3DMesh(int* pError, std::set<t_Cell_3*>, MeshTypes::MeshType, unsigned int pMeanNbFacetPerCell = MEAN_DEFAULT_NB_POINT, double smallestGain = DELTA_REFINNEMENT);

protected:
	/// \brief return the spatialable and there weights necessary to create a 2D Delaunay or a Voronoi 
	void get2DWeightedSpatialables(std::set<t_Cell_2*>*, std::set<std::pair<const t_Spatialable_2*, double> >*, bool );
	/// \brief return the spatialable and there weights necessary to create a 3D Delaunay or a Voronoi 
	void get3DWeightedSpatialables(std::set<t_Cell_3*>*, std::set<std::pair<const t_Spatialable_3*, double> >*, bool );

};

//////////////////////////// FUNCTION IMPLEMENTATION //////////////////////////////////////////////////

#endif