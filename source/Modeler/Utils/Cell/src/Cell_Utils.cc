/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "Cell_Utils.hh"

namespace Utils
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \param pAgents The set of agent we want the properties for
	/// \return set<const CellProperties*> The set of Cell properties present in giving cells.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	set<const CellProperties*> getAllCellsProperties(vector<const ICell*> pAgents)
	{
		set<const CellProperties*> lCellProperties;

		vector<const ICell*>::const_iterator itCell;
		for(itCell = pAgents.begin(); itCell != pAgents.end(); ++itCell)
		{
			lCellProperties.insert((*itCell)->getCellProperties());
		}
		return lCellProperties;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \return LifeCyclesRates all existing life cycle with uniform rates
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	map<LifeCycles::LifeCycle, double> generateUniformLifeCycle()
	{
		map<LifeCycles::LifeCycle, double> rates;
		for(int iState = 0; iState != LifeCycles::NA; iState++)
		{
			rates.insert(pair<LifeCycles::LifeCycle, double> (static_cast<LifeCycles::LifeCycle>(iState), 0.2));
		}

		return rates;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \param pCells the cells for which we want to reset center
	/// \param pNewBary The new location of the barycenter
	/// \warning this will only affect cells position, and never their meshes. So if you need cell meshes with the
	/// new cell positions please run again meshing process.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	void setBarycenter(vector<t_Cell_3*>& pCells, Point_3 pNewBary)
	{
		vector<const t_Cell_3*> lCstCells(pCells.begin(), pCells.end());
		Point_3 lCurrentBary = getCentroid(lCstCells.begin(), lCstCells.end());
		Vector_3 lTransformation = pNewBary - lCurrentBary;
		vector<t_Cell_3*>:: iterator itCell;
		for(itCell = pCells.begin(); itCell != pCells.end(); ++itCell)
		{
			Point_3 lNewPos =  (*itCell)->getPosition() + lTransformation;
			(*itCell)->setPosition( lNewPos );
		}
	}
}

