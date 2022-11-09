/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef SHPERE_SPATIAL_DELIMITATION
#define SHPERE_SPATIAL_DELIMITATION

#include "SpatialDelimitation.hh"
#include "GeometrySettings.hh"

using namespace Settings::Geometry;

///////////////////////////////////////////////////////////////////////////
/// \brief define a spatial delimitation which goes from an internal sphere to an external sphere.
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////
class SpheresSDelimitation : public SpatialDelimitation<double, Point_3, Vector_3>
{
public:
	/// \brief constructor
	SpheresSDelimitation(double, double, Point_3 pCenter = Point_3(0., 0., 0.));
	/// \brief destructor
	virtual ~SpheresSDelimitation();

	/// \brief return the position according to the distribution.
	Point_3 getSpot(Distribution::DistributionType) const;
	/// \brief return true if the given point is inside the spatial delimitation
	virtual bool isIn(Point_3) const;
	/// \brief print cell information (used also to save the cell on a .txt file)
	virtual void write(QXmlStreamWriter&) const;

protected:
	/// \brief function defining the bounding box of the world
	virtual BoundingBox<Point_3> createBoundingBox();	

private:
	double internalRadius;		///< \brief the internal radius of the spehere
	double externalRadius;		///< \brief the external radius of the spehere
};

#endif // SHPERE_SPATIAL_DELIMITATION