/*----------------------
Copyright (C): Henri Payno, Axel Delsol,
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "CellFactory.hh"
#include "ParametersGetter.hh"
#include "RandomEngineManager.hh"
#include "SimpleDiscoidalCell.hh"
#include "SimpleSpheroidalCell.hh"
#include <QString>

#if ( defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) )
	#define _USE_MATH_DEFINES
	#include <math.h>
#endif

static CellFactory* factory = 0;	/// the factory defined as a singleton

///////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////
CellFactory::CellFactory()
{

}

///////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////
CellFactory::~CellFactory()
{

}

///////////////////////////////////////////////////////
/// \return { The cell factory singleton}
///////////////////////////////////////////////////////
CellFactory* CellFactory::getInstance()
{
	if(!factory)
	{
		factory = new CellFactory();
	}
	return factory;
}

////////////////////////////////////// TEMPLATE SPECIALIZATION /////////////////////

#include "RandomEngineManager.hh"
#include "Box_Shape.hh"
#include "Round_Shape.hh"

using namespace Settings::Geometry;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief 3D specialization of produce
/// \param pCellProp The propertie to give to the cell
/// \param pLifeCycle The life cycle to give to this cell
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
Cell<double, Point_3, Vector_3>* CellFactory::produce(const CellProperties* pCellProp, const LifeCycles::LifeCycle pLifeCycle)
{
	LifeCycles::LifeCycle lLifeCycle = pLifeCycle;
	// if life state is unknow :  pick one randomly
	if(lLifeCycle == LifeCycles::NA)
	{
		lLifeCycle = static_cast<LifeCycles::LifeCycle> ( RandomEngineManager::getInstance()->randi( 0, (int)LifeCycles::NA -1 ));
	}

	switch(pCellProp->getCellType())
	{
		// --- 3D shapes defined
		case SIMPLE_ROUND :
		{
			// get cell radius
			double radius;
			const RoundCellProperties* lDSProp = dynamic_cast<const RoundCellProperties*>(pCellProp);
			if(lDSProp)
			{
				radius = RandomEngineManager::getInstance()->randd(lDSProp->getMembraneRadius(pLifeCycle).var_min(), lDSProp->getMembraneRadius(pLifeCycle).var_max());
			}
			else
			{
				QString mess = "!!! Unable to find radius for the requested cell from cell properties";
				InformationSystemManager::getInstance()->Message(InformationSystemManager::FATAL_ERROR_MES, mess.toStdString(), "CellFactory::produce");
				exit(0);
			}

			// get nucleus radius
			double nucleusRadius = RandomEngineManager::getInstance()->randd(pCellProp->getNucleusRadius(lLifeCycle).var_min(), pCellProp->getNucleusRadius(lLifeCycle).var_max());
			// get masse
			double masse = RandomEngineManager::getInstance()->randd(pCellProp->getMasses(lLifeCycle).var_min(), pCellProp->getMasses(lLifeCycle).var_max());
			// create cell
			SimpleSpheroidalCell* cell = new SimpleSpheroidalCell(pCellProp, Point_3(0., 0., 0.), radius, nucleusRadius, pCellProp->getNucleusPosType(), masse);

			assert(cell);
			return cell;
		}

		default:
		{
			QString mess = "!!! Unable to create the wanted Cell - Undefined cell for this kind of type";
			InformationSystemManager::getInstance()->Message(InformationSystemManager::FATAL_ERROR_MES, mess.toStdString(), "CellFactory::produce");
			exit(0);
		}
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pCellProp The propertie to give to the cell
/// \param pLifeCycle The life cycle to give to this cell
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
Cell<double, Point_2, Vector_2>* CellFactory::produce(const CellProperties* pCellProp, const LifeCycles::LifeCycle pLifeCycle)
{
	LifeCycles::LifeCycle lLifeCycle = pLifeCycle;
	/// if life state is unknow :  pick one randomly
	if(lLifeCycle == LifeCycles::NA)
	{
		lLifeCycle = static_cast<LifeCycles::LifeCycle> ( RandomEngineManager::getInstance()->randi( 0, (int)LifeCycles::NA -1));
	}

	/// FOR now we only generate cell weight and volume from a random distribution
	switch(pCellProp->getCellType())
	{
		/// 2D shapes defined
		case SIMPLE_ROUND :
		{
			// cell radius
			double radius;
			const RoundCellProperties* lDSProp = dynamic_cast<const RoundCellProperties*>(pCellProp);
			if(lDSProp)
			{
				radius = RandomEngineManager::getInstance()->randd(lDSProp->getMembraneRadius(pLifeCycle).var_min(), lDSProp->getMembraneRadius(pLifeCycle).var_max());
			}
			else
			{
				QString mess = "!!! Unable to find radius for the requested cell from cell properties";
				InformationSystemManager::getInstance()->Message(InformationSystemManager::FATAL_ERROR_MES, mess.toStdString(), "CellFactory");
				exit(0);
			}

			G4cout<< "\n\n\n CellFactory \n\n\n" << G4endl;

			// nucleus radius
			double nucleusRadius = RandomEngineManager::getInstance()->randd(pCellProp->getNucleusRadius(lLifeCycle).var_min(), pCellProp->getNucleusRadius(lLifeCycle).var_max());
			// get weight
			double masse = RandomEngineManager::getInstance()->randd(pCellProp->getMasses(lLifeCycle).var_min(), pCellProp->getMasses(lLifeCycle).var_max());

			// create cell
			SimpleDiscoidalCell* cell = new SimpleDiscoidalCell(pCellProp, Point_2(), radius, nucleusRadius, pCellProp->getNucleusPosType(), masse);

			assert(cell);
			return cell;
		}
		default:
		{
			QString mess = "!!! Unable to create the wanted Cell - Undefined cell for this kind of type";
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "CellFactory");
		}
	}
	return NULL;
}
