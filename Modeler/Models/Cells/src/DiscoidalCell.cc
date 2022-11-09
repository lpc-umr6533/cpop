/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "RandomEngineManager.hh"
#include "CellMeshSettings.hh"
#include "DiscoidalCell.hh"
#include "File_Utils_TXT.hh"
#include "Geometry_Utils_Circle.hh"

#include <CGAL/centroid.h>
#include <CGAL/ch_graham_andrew.h>

///////////////////////////////////////////////////////////////////////////////////////////
/// \param pCellProperties The cell properties to set to the cell
/// \param pOrigin The origin of the cell ( center ).
/// \param pDiscoidRadius The radius of the cell.
/// \param pMass The mass of the cell
/// \param pMembraneShape The inital  membrane delimitation.
///////////////////////////////////////////////////////////////////////////////////////////
DiscoidalCell::DiscoidalCell(
	const CellProperties* pCellProperties, 
	Point_2 pOrigin, 
	double pDiscoidRadius, 
	double pMass, 
	std::vector<Mesh2D::Segment_2> pMembraneShape):
	RoundCell<double, Point_2, Vector_2>(pCellProperties, pOrigin, pDiscoidRadius, pMass)
{
	shape = new std::vector<Mesh2D::Segment_2>(pMembraneShape);	
}

///////////////////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////////////////
DiscoidalCell::~DiscoidalCell()
{
	delete shape;
	shape = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////
/// \return The area of the membrane
/// \todo;
///////////////////////////////////////////////////////////////////////////////////////////
double DiscoidalCell::getMembraneSurface() const
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////////////////
double DiscoidalCell::getCellArea() const
{
	double area = 0;
	std::vector<Segment_2>::const_iterator itSeg;
	for(itSeg = shape->begin(); itSeg != shape->end(); ++itSeg )
	{
		area += Triangle_2(getPosition(), itSeg->source(), itSeg->target()).area();
	}
	return area;
}

///////////////////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////////////////
void DiscoidalCell::resetMesh()
{
	delete shape;
	shape = new std::vector<Mesh2D::Segment_2>();
}

///////////////////////////////////////////////////////////////////////////////////////////
/// \return The random spot requested on the cytoplasm 
///////////////////////////////////////////////////////////////////////////////////////////
Point_2 DiscoidalCell::getSpotOnCellMembrane() const
{
	if(shape->size() < 1)
	{
		QString mess = "Unvalid shape, unable to compute a spot on the membrane";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "DiscoidalCell");
		return Point_2();
	}else
	{
		int indexShape = RandomEngineManager::getInstance()->randi(0, shape->size() -1);
		double x = RandomEngineManager::getInstance()->randd(0., 1.);
		return (shape->at(indexShape).source() + shape->at(indexShape).to_vector() * x );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
/// \return The random spot requested on the cytoplasm 
///////////////////////////////////////////////////////////////////////////////////////////
Point_2 DiscoidalCell::getSpotOnCytoplasm() const
{
	Vector_2 v(getSpotOnCellMembrane() - getPosition());
	double x = RandomEngineManager::getInstance()->randd(0., 1.);
	Point_2 res = getPosition() + x*(v);

	std::vector<Nucleus<double, Point_2, Vector_2> *>::const_iterator itNuc;
	for(itNuc = nuclei.begin(); itNuc != nuclei.end(); ++itNuc)
	{
		// if point is on a nucleus, pick an other one
		if((*itNuc)->hasIn(res))
		{
			return getSpotOnCytoplasm();
		}
	}
	return res;
}

///////////////////////////////////////////////////////////////////////////////////////////
/// \return The random spot requested on a nuclei
///////////////////////////////////////////////////////////////////////////////////////////
Point_2 DiscoidalCell::getSpotOnNuclei() const
{
	if(nuclei.size() < 1)
	{
		QString mess = "No nuclei, unable to compute a spot on a  nuclei";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "DiscoidalCell");
		return Point_2();
	}else
	{
		int indexShape = RandomEngineManager::getInstance()->randi(0, nuclei.size() -1);
		assert(nuclei[indexShape]);
		return nuclei[indexShape]->getSpotOn();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
/// \return The random spot requested in a nuclei 
///////////////////////////////////////////////////////////////////////////////////////////
Point_2 DiscoidalCell::getSpotInNuclei() const
{
	if(nuclei.size() < 1)
	{
		QString mess = "No nuclei, unable to compute a spot on a  nuclei";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "DiscoidalCell");
		return Point_2();
	}else
	{
		int indexShape = RandomEngineManager::getInstance()->randi(0, nuclei.size() -1);
		assert(nuclei[indexShape]);
		return nuclei[indexShape]->getSpotIn();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param ptToCheck the point we want to know if is in the cell
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool DiscoidalCell::hasIn(Point_2 ptToCheck) const
{
	// check if ordered
	if(!hasMesh())
	{
		return false;
	}

	// if isn't on the theorical cell
	if( CGAL::squared_distance(ptToCheck, getPosition()) >= getSquareRadius() )
	{
		return false;
	}

	// else check if is in the right side of all ORIENTED segment
	vector<Mesh2D::Segment_2>::iterator itSeg;
	for(itSeg = shape->begin(); itSeg != shape->end(); ++itSeg)
	{
		if(itSeg->supporting_line().oriented_side(ptToCheck) == CGAL::ON_NEGATIVE_SIDE)
		{
			return false;
		}
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \return true if the cell has a mesh
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool DiscoidalCell::hasMesh() const
{
	if(shape && (shape->size() != 0 ) )
	{
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool DiscoidalCell::cross(const BoundingBox<Point_2>* pBB) const
{
	Point_2 pt = this->getPosition();
	double radius = getRadius();

	return 	( 
		(pt.x() <= (pBB->getTopRight().x() 		+ radius)) && 
		(pt.x() >= (pBB->getBottomLeft().x()	- radius)) &&
		(pt.y() <= (pBB->getTopRight().y()		+ radius)) && 
		(pt.y() >= (pBB->getBottomLeft().y() 	- radius)) );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool DiscoidalCell::isIn(const BoundingBox<Point_2>* pBB) const
{
	Point_2 pt = this->getPosition();
	double radius = getRadius();

	return 	( 
		((pt.x() - radius) < pBB->getTopRight().x()		) && 
		((pt.x() + radius) > pBB->getBottomLeft().x() 	) &&
		((pt.y() - radius) < pBB->getTopRight().y()		) && 
		((pt.y() + radius) > pBB->getBottomLeft().y() 	) );

}
