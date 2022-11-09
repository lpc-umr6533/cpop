/*----------------------
Copyright (C): Henri Payno, Axel Delsol,
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "RandomEngineManager.hh"
#include "CellMeshSettings.hh"
#include "Geometry_Utils_Sphere.hh"
#include "SimpleSpheroidalCell.hh"

#include <CGAL/centroid.h>
//////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pCellProperties the cell properties
/// \param pOrigin the origin to the cell
/// \param pSpheroidRadius the radius of the cell membrane at rest
/// \param pNucleusRadius the radius of the nuclear membrane.
/// \param pPosType the type of position to set to the nucleus
/// \param pWeight the weight of the cell
/// \param pMembraneShape the inital shape of the cell
//////////////////////////////////////////////////////////////////////////////////////////////////////
SimpleSpheroidalCell::SimpleSpheroidalCell(
	const CellProperties* pCellProperties,
	Point_3 pOrigin,
	double pSpheroidRadius,
	double pNucleusRadius,
	eNucleusPosType pPosType,
	double pWeight,
	Mesh3D::Polyhedron_3 pMembraneShape):
	SpheroidalCell(pCellProperties, pOrigin, pSpheroidRadius, pWeight, pMembraneShape)
{
	nucleus = new RoundNucleus<double, Point_3, Vector_3>(pNucleusRadius, pOrigin, pPosType );
	nuclei.push_back(nucleus);
	//SpheroidalCell::convertToG4Structure()
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////////////////
SimpleSpheroidalCell::~SimpleSpheroidalCell()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param nucleusPositionType The type of position of the cell
/// \return the center of the nucleus
//////////////////////////////////////////////////////////////////////////////////////////////////////
Point_3 SimpleSpheroidalCell::getNucleusCenter(eNucleusPosType nucleusPositionType) const
{
	switch(nucleusPositionType)
	{
		case ORIGIN:
		{
			return getPosition();
		}
		case BARYCENTER:
		{
			// The centroid is the barycenter with equal weight for each vertices from the polyhedron
			return CGAL::centroid(shape->points_begin(), shape->points_end(), CGAL::Dimension_tag<0>());
		}
		case NO_STANDARD:
		{
			return getNucleusCenter( static_cast<eNucleusPosType>(RandomEngineManager::getInstance()->randi(0, (int)END_CHOICE - 2)));
		}
		default:
		{
			QString mess = "unknow nucleusPositionType" + QString::number(nucleusPositionType) + ", Set tposition to the barycenter ";
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "SpheroidalCellMesh");
			return getNucleusCenter(BARYCENTER);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleSpheroidalCell::setNucleusCenter()
{
	nucleus->setOrigin( getNucleusCenter( nucleus->getPositionType()));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param intersections The plane defining the boundary
//////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleSpheroidalCell::generateNuclei(std::vector<Plane_3*> intersections)
{
	assert(getRadius() > 0.);
	assert(nucleus->getRadius() > 0.);
	assert(getRadius() > nucleus->getRadius());
	assert(maxRatioToCellRadius <= 1.);
	assert(maxRatioToCellRadius > 0.);

	// check intersection plane.
	double maxRadius = maxRatioToCellRadius * this->getRadius();
	double optimalRadius = nucleus->getRadius();	// the one requested

	// if intersections
	for(std::vector<Plane_3*>::const_iterator itPlane = intersections.begin(); itPlane != intersections.end(); ++itPlane)
	{
		assert(*itPlane);
		double localRadius = sqrt( squared_distance((*itPlane)->projection(nucleus->getOrigin()), nucleus->getOrigin()) );
		if(localRadius < optimalRadius)
		{
			optimalRadius = localRadius;
		}
	}

	// if too larger radius
	if(optimalRadius > maxRadius)
	{
		optimalRadius = maxRadius;
	}
	nucleus->setRadius( optimalRadius );
	//cout << "changing nucleus radius : " << optimalRadius*CLHEP::um << endl;
	//SpheroidalCell::
	//cout << "mass  :  " << pWeight << endl;

	assert(nucleus->getRadius() > 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////////////
void SimpleSpheroidalCell::resetMesh()
{
	SpheroidalCell::resetMesh();
	/// nothing to do for the nucleus mesh
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \return The random spot requested on the cytoplasm
/// \details here we optimize the function because we have only one nucleus and we can speed up this process.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
Point_3 SimpleSpheroidalCell::getSpotOnCytoplasm() const
{
	//G4cout<< "\n\n\n getSpotOnCytoplasm" << G4endl;

	// // pick a point on the membrane, then pick a point on the segment [MembranePoint/cellOrigin] Until we found one not on the nucleus
	// // because shape is convex we are sure the picked point is on the cytoplsam (work until no other organ are defined)
	// Point_3 cellMembraneSpot = getSpotOnCellMembrane();
	// // as we are on a simple cell ( one nucleus, one cytoplasm ) we are sure that the resting space is empty.
	// // in this case nucleus is a perfect sphere
	// Point_3 projectedPtOnNucleus = Utils::Geometry::Sphere::getProjectionOnSphere(cellMembraneSpot, getNucleus()->getRadius(), getNucleus()->getOrigin());
	//
	// Vector_3 v( cellMembraneSpot - projectedPtOnNucleus);
	// double x = RandomEngineManager::getInstance()->randd(0., 1.);
	// Point_3 res = getPosition() + x*(v);
	//
	// return res;


	Vector_3 v( getSpotOnCellMembrane() - getPosition());
	double x = RandomEngineManager::getInstance()->randd(0., 1.);
	Point_3 res = getPosition() + x*(v);

	std::vector<Nucleus<double, Point_3, Vector_3> *>::const_iterator itNuc;
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
