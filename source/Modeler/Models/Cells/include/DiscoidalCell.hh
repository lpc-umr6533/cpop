/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef DISCOIDAL_CELL_STRUCTURE_HH
#define DISCOIDAL_CELL_STRUCTURE_HH

#include "RoundCell.hh"
#include "RoundNucleus.hh"
#include "Mesh2DSettings.hh"

using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh2D;
////////////////////////////////////////////////////////////////////////////
/// \brief A discoidal cell is defined by her cell membrane represented by a deformable circle/disc.
/// \details discoidal cells contained n organelles.
////////////////////////////////////////////////////////////////////////////
class DiscoidalCell : public RoundCell<double, Point_2, Vector_2>
{	
	typedef std::vector<Mesh2D::Segment_2>::iterator shapeIterator;	///< \brief iterator on Discoidal cell membrane mesh

public:
	/// \brief constructor
	DiscoidalCell(const CellProperties*, Point_2 pOrigin, double pSpheroidRadius, double pMass, std::vector<Mesh2D::Segment_2> pMembraneShape);
	/// \brief destructor
	virtual ~DiscoidalCell();

	/// \brief membrane area getter
	virtual double getCellArea() const;
	/// \brief membrane area getter
	virtual double getMembraneSurface() const;
	/// \brief return the iterator on the begining of the shape
	shapeIterator shape_begin()								{ return shape->begin(); };
	/// \brief return the iterator on the end of the shape
	shapeIterator shape_end()								{ return shape->end(); };
	/// \brief return the number of element on the shaê
	unsigned int shape_size()								{ return shape->size(); };
	/// \brief add a segment to the shae
	void add_to_shape(Segment_2 s)							{ shape->push_back(s); };
	/// \brief shape getter
	std::vector<Mesh2D::Segment_2>* getShape() const		{ return shape; };
	/// \brief return a random point on/inside the membrane
	virtual Point_2 getSpotOnCellMembrane() const;
	/// \brief return a random point inside the cytoplasm, avoiding membrane and nucleus
	virtual Point_2 getSpotOnCytoplasm() const;
	/// \brief return a random point on/inside the nucleus
	virtual Point_2 getSpotOnNuclei() const;
	/// \brief return a random point on/inside the nucleus
	virtual Point_2 getSpotInNuclei() const;
	/// \brief return true if the point is inside the cell
	virtual bool hasIn(Point_2) const;
	/// \brief will generate the nucleus shape
	virtual void generateNuclei(std::vector<Line_2*> intersections) = 0;

	/// \brief return the position of the nucleus according to his type of nucleus position
	virtual Point_2 getNucleusCenter(eNucleusPosType nucleusPositionType) const = 0;
	/// \brief set the position of the nucleus according to his type of nucleus position
	virtual void setNucleusCenter() = 0;
	/// \brief return true if nuclei radius are coherent
	virtual bool checkNucleiRadius() const = 0;

	/// \brief reset the mesh
	virtual void resetMesh();
	/// \brief return the cell description
	virtual QString getDescription() const = 0;

	/// \brief return the volume defined by the mesh
	double getMeshVolume(MeshOutFormats::outputFormat ) const 	{return -1.;};

	/// \brief return true if the cell own a mesh
	virtual bool hasMesh() const ;

	/// \brief return true if the spatialable cross the bounding box
	virtual bool cross(const BoundingBox<Point_2>*) const;
	/// \brief return true if the point is in the bounding box
	virtual bool isIn(const BoundingBox<Point_2>*) const;

protected:
	/// \brief The polyhedron representing the cell boundaries.
	std::vector<Mesh2D::Segment_2>* shape;	

};

#endif // DISCOIDAL_CELL_STRUCTURE_HH