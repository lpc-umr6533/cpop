/*----------------------
Copyright (C): Henri Payno, Axel Delsol,
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "MeshFactory.hh"

#include "Agent.hh"
#include "Round_Shape.hh"
#include "DiscoidalCellMesh.hh"
#include "InformationSystemManager.hh"
#include "Round_Shape.hh"
#include "SpheroidalCellMesh.hh"

#ifndef NDEBUG
	#define DEBUG_MESH_FACTORY 0
#else
	#define DEBUG_MESH_FACTORY 0	// must always stay at 0
#endif

static MeshFactory* factory = 0;

using namespace Mesh2D;
using namespace Mesh3D;

//////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////
MeshFactory::MeshFactory()
{

}

//////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////
MeshFactory::~MeshFactory()
{

}

//////////////////////////////////////////////////////////////////////////////
/// \return The instance of factory manager.
//////////////////////////////////////////////////////////////////////////////
MeshFactory* MeshFactory::getInstance()
{
	if(!factory)
	{
		factory = new MeshFactory();
	}
	return factory;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pCells The cell for which we want the weighted spatialable
/// \param lWeightedSpatialables where to store weighted spatialables
/// \param pWeighted True if we want to get weight of the spatialables
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \todo : combine both function 2D and 3D
void MeshFactory::get2DWeightedSpatialables(std::set<t_Cell_2*>* pCells, std::set<std::pair<const t_Spatialable_2*, double> >* lWeightedSpatialables, bool pWeighted)
{
	std::set<t_Cell_2*>::const_iterator itCell;
	double lWeight;
	for(itCell = pCells->begin(); itCell != pCells->end(); ++itCell)
	{
		lWeight = 1.;	// default weight
		if(pWeighted)
		{
			t_Cell_2* pCell = (t_Cell_2*)*itCell;
			Body* lBody = (Body*) pCell->getBody();
			Round_Shape<double, Point_2, Vector_2>* lDisc = dynamic_cast<Round_Shape<double, Point_2, Vector_2>*>(lBody);
			if(lDisc)
			{
				lWeight = lDisc->getRadius();	// on voronoi mesh, the weigth is the the diameter of the cell.
			}
		}
		if(DEBUG_MESH_FACTORY)
		{
			QString mess = "add a point to the vornoi with a weight of :  " + QString::number(lWeight);
			InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString() , "MeshFactory");
		}

		lWeightedSpatialables->insert(std::pair<const t_Spatialable_2*, double>(static_cast<const t_Spatialable_2*>(*itCell), lWeight));
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pCells The cell for which we want the weighted spatialable
/// \param lWeightedSpatialables where to store weighted spatialables
/// \param pWeighted True if we want to get weight of the spatialables
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MeshFactory::get3DWeightedSpatialables(std::set<t_Cell_3*>* pCells, std::set<std::pair<const t_Spatialable_3*, double> >* lWeightedSpatialables, bool pWeighted)
{
	std::set<t_Cell_3*>::const_iterator itCell;
	double lWeight;
	for(itCell = pCells->begin(); itCell != pCells->end(); ++itCell)
	{
		lWeight = 1.;
		if(pWeighted)
		{
			t_Cell_3* pCell = (t_Cell_3*)*itCell;
			Body* lBody = (Body*) pCell->getBody();
			Round_Shape<double, Point_3, Vector_3>* lSphere = dynamic_cast<Round_Shape<double, Point_3, Vector_3>*>(lBody);
			if(lSphere)
			{
				lWeight = lSphere->getRadius();	// On CGAL voronoi the weight is the radius and not the diameter
			}
		}
		if(DEBUG_MESH_FACTORY)
		{
			QString mess = "add a point to the vornoi with a weight of :  " + QString::number(lWeight);
			InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString() , "MeshFactory");
		}
		lWeightedSpatialables->insert(std::pair<const t_Spatialable_3*, double>(static_cast<const t_Spatialable_3*>(*itCell), lWeight));
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pError 				The result of the mesh creation
/// \param pLayer 				The layer including the agents we want to create a mesh for
/// \param pMeshType 			The type of mesh we want to create
/// \param pMeanNbPointPerCell 	The number of point threshold to stop membrane mesh subdivision
/// \param pSmallestGain		The gain of the better subdivision threashold to stop membrane mesh subdivision
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
t_Mesh_2* MeshFactory::create_2DMesh(int* pError, const t_Sub_Env_2* pLayer, MeshTypes::MeshType pMeshType, unsigned int pMeanNbPointPerCell, double pSmallestGain )
{
	std::set<t_Cell_2*> lCells;
	std::set<Agent*> lAgents = pLayer->getAgents(WITHOUT_DELIMITATION);
	std::set<Agent*>::const_iterator itA;
	for(itA = lAgents.begin(); itA != lAgents.end(); ++itA)
	{
		t_Cell_2* lSpa = dynamic_cast<t_Cell_2*>(*itA);
		if(lSpa)
		{
			lCells.insert(lSpa);
		}
	}
	return create_2DMesh(pError, lCells, pMeshType, pMeanNbPointPerCell, pSmallestGain);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pError 				The result of the mesh creation
/// \param pLayer 				The layer including the agents we want to create a mesh for
/// \param pMeshType 			The type of mesh we want to create
/// \param pMeanNbSegPerCell 	The number of segment threshold to stop membrane mesh subdivision
/// \param pSmallestGain		The gain of the better subdivision threashold to stop membrane mesh subdivision
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
t_Mesh_3* MeshFactory::create_3DMesh(int* pError, const t_Sub_Env_3* pLayer, MeshTypes::MeshType pMeshType, unsigned int pMeanNbSegPerCell, double pSmallestGain )
{
	std::set<t_Cell_3*> lCells;
	std::set<Agent*> lAgents = pLayer->getAgents(WITHOUT_DELIMITATION);
        //std::cout << " nb agents : " << lAgents.size() << std::endl;
	assert(lAgents.size() > 0);
	std::set<Agent*>::const_iterator itA;
	// G4int int_test = 0 ;
	for(itA = lAgents.begin(); itA != lAgents.end(); ++itA)
	{
		t_Cell_3* lSpa = dynamic_cast<t_Cell_3*>(*itA);
		if(lSpa)
		{
			// int_test += 1;
			lCells.insert(lSpa);
			// G4cout << "\n lSpa.getID()" << lSpa->getID() << G4endl;
		}
	}
	// G4cout << "\n int_test" << int_test << G4endl;
	return create_3DMesh(pError, lCells, pMeshType, pMeanNbSegPerCell, pSmallestGain);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pError 				The result of the mesh creation
/// \param pCells 				The cells for which we want to create a mesh
/// \param pMeshType 			The type of mesh we want to create
/// \param pMeanNbPointPerCell 	The number of point threshold to stop membrane mesh subdivision
/// \param pSmallestGain		The gain of the better subdivision threashold to stop membrane mesh subdivision
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
t_Mesh_2* MeshFactory::create_2DMesh(int* pError, std::set<t_Cell_2*> pCells, MeshTypes::MeshType pMeshType, unsigned int pMeanNbPointPerCell, double pSmallestGain )
{
	if(pCells.size() == 0)
	{
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "not enought cell to generate a mesh", "MeshFactory");
		(*pError) = 101;
		return NULL;
	}

	switch(pMeshType)
	{
		case MeshTypes::Weighted_Voronoi_Tesselation:
		{
			*pError = 0;
			return new Voronoi_2D_Mesh(pMeanNbPointPerCell, pSmallestGain, pCells);
		}
		case MeshTypes::Round_Cell_Tesselation:
		{
			*pError = 0;
			return new DiscoidalCellMesh(pMeanNbPointPerCell, pSmallestGain, pCells);
		}
		default:
		{
			QString mess = "Failed to create the mesh, unknow mesh type with ID " + QString::number(pMeshType);
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString() , "MeshFactory");
			return NULL;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pError 				The result of the mesh creation
/// \param pCells 				The cells for which we want to create a mesh
/// \param pMeshType 			The type of mesh we want to create
/// \param pMeanNbSegPerCell 	The number of segment threshold to stop membrane mesh subdivision
/// \param pSmallestGain		The gain of the better subdivision threashold to stop membrane mesh subdivision
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
t_Mesh_3* MeshFactory::create_3DMesh(int* pError, std::set<t_Cell_3*> pCells, MeshTypes::MeshType pMeshType, unsigned int pMeanNbSegPerCell, double pSmallestGain )
{
	if(pCells.size() == 0)
	{
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "not enought cell to generate a mesh", "MeshFactory");
		(*pError) = 101;
		return NULL;
	}

	switch(pMeshType)
	{
		case MeshTypes::Weighted_Voronoi_Tesselation:
		{
			*pError = 0;
			return new Voronoi_3D_Mesh(pMeanNbSegPerCell, pSmallestGain, pCells);
		}
		case MeshTypes::Round_Cell_Tesselation:
		{
			*pError = 0;
			return new SpheroidalCellMesh(pMeanNbSegPerCell, pSmallestGain, pCells);
		}
		default:
		{
			QString mess = "Failed to create the mesh, unknow mesh type with ID " + QString::number(pMeshType);
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString() , "MeshFactory");
			return NULL;
		}
	}
}
