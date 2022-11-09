/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef SIMPLE_SPHEROIDAL_CELL_HH
#define SIMPLE_SPHEROIDAL_CELL_HH

#include "SpheroidalCell.hh"
#include "RoundNucleus.hh"

////////////////////////////////////////////////////////////////////////////
/// \brief define a simple spheroidal cell.
/// A simple cell is composed of a deformable round cell membrane and a single round nucleus.
/// @author Henri Payno
////////////////////////////////////////////////////////////////////////////
class SimpleSpheroidalCell : public SpheroidalCell
{
public:
	/// \brief constructor
	SimpleSpheroidalCell(const CellProperties*, Point_3 pOrigin, double pSpheroidRadius, double pNucleusRadius, eNucleusPosType pPosType = BARYCENTER, double pMass=1., Mesh3D::Polyhedron_3 pMembraneShape=Mesh3D::Polyhedron_3());
	/// \brief destructor
	virtual ~SimpleSpheroidalCell();
	/// \brief return the position of the nucleus according to his type of nucleus position
	/// \warning this is not necessarily the one setted
	Point_3 getNucleusCenter(eNucleusPosType nucleusPositionType) const;
	/// \brief set the position of the nucleus according to his type of nucleus position
	void setNucleusCenter();	
	/// \brief will generate the nucleus shape
	virtual void generateNuclei(std::vector<Plane_3*>);
	/// \brief return true if nuclei radius are coherent
	virtual bool checkNucleiRadius() const 			{ return nucleus->getRadius() > 0;};	
	/// \brief return the cell description
	virtual QString getDescription()  const 		{ return "SimpleSpheroidalCell"; };
	/// \brief return a random point inside the cytoplasm, avoiding membrane and nucleus
	// virtual Point_3 getSpotOnCytoplasm() const;
	/// \brief return the round nucleus
	RoundNucleus<double, Point_3, Vector_3>* getNucleus() const { return nucleus;}
	/// \brief return a random point inside the cytoplasm, avoiding membrane and nucleus
	virtual Point_3 getSpotOnCytoplasm() const;
	
protected:
	/// \brief max rattio getter
	double getMaxRatioNucleusToCell() const { return maxRatioToCellRadius;};

	/// \brief reset the mesh
	virtual void resetMesh();
private:
	/// \brief the nucleus
	RoundNucleus<double, Point_3, Vector_3>* nucleus;
};

#endif // SIMPLE_SPHEROIDAL_CELL_HH