/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef SIMPLE_DISCOIDAL_CELL_STRUCTURE_HH
#define SIMPLE_DISCOIDAL_CELL_STRUCTURE_HH

#include "DiscoidalCell.hh"
#include "GeometrySettings.hh"
#include "Mesh2DSettings.hh"
#include "RoundNucleus.hh"

using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh2D;
////////////////////////////////////////////////////////////////////////////
/// \brief define a simple discoidal cell.
/// A simple cell is composed of a deformable round cell membrane and a single round nucleus.
/// @author Henri Payno
////////////////////////////////////////////////////////////////////////////
class SimpleDiscoidalCell : public DiscoidalCell
{	
	typedef std::vector<Mesh2D::Segment_2>::iterator shapeIterator;	///< \brief iterator on membrane cell mesh

public:
	/// \brief constructor
	SimpleDiscoidalCell(const CellProperties*, Point_2 pOrigin, double pSpheroidRadius, double pNucleusRadius, eNucleusPosType pPosType = BARYCENTER, double pMass = 1., std::vector<Mesh2D::Segment_2> pMembraneShape=std::vector<Mesh2D::Segment_2>());
	/// \brief destructor
	virtual ~SimpleDiscoidalCell();

	/// \brief membrane area getter
	//virtual double getCytoplasmArea() const = 0;
	/// \brief return the position of the nucleus according to his type of nucleus position
	/// \warning this is not necessarily the one setted
	virtual Point_2 getNucleusCenter(eNucleusPosType nucleusPositionType) const;
	/// \brief set the position of the nucleus according to his type of nucleus position
	virtual void setNucleusCenter();
	/// \brief return the set of points defining nuclei
	std::vector<std::set<Point_2> > getNucleiPoints() const;
	/// \brief will generate the nucleus shape
	virtual void generateNuclei(std::vector<Line_2*> intersections);
	/// \brief return true if nuclei radius are coherent
	virtual bool checkNucleiRadius() const 			{ return nucleus->getRadius() > 0;};	
	/// \brief reset the mesh
	virtual void resetMesh();
	/// \brief return the cell description
	virtual QString getDescription()  const 		{ return "SimpleDiscoidalCell"; };

private:
	/// \brief the nucleus
	RoundNucleus<double, Point_2, Vector_2>* nucleus;
};

#endif // SIMPLE_DISCOIDAL_CELL_STRUCTURE_HH