#ifndef SHPERE_SPATIAL_DELIMITATION
#define SHPERE_SPATIAL_DELIMITATION

#include "SpatialDelimitation.hh"
#include "GeometrySettings.hh"

using namespace Settings::Geometry;

/// \brief define a spatial delimitation which goes from an internal sphere to an external sphere.
/// @author Henri Payno
class SpheresSDelimitation : public SpatialDelimitation<double, Point_3, Vector_3> {
public:
	SpheresSDelimitation(double, double, Point_3 pCenter = Point_3(0., 0., 0.));

	/// \brief return the position according to the distribution.
	[[nodiscard]] Point_3 getSpot(Distribution::DistributionType) const override;
	/// \brief return true if the given point is inside the spatial delimitation
	[[nodiscard]] bool isIn(Point_3) const override;
	/// \brief print cell information (used also to save the cell on a .txt file)
	void write(QXmlStreamWriter&) const override;

protected:
	/// \brief function defining the bounding box of the world
	BoundingBox<Point_3> createBoundingBox() override;

private:
	double internalRadius;  ///< \brief the internal radius of the spehere
	double externalRadius;  ///< \brief the external radius of the spehere
};

#endif
