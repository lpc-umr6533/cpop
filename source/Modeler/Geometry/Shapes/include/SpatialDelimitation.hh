#ifndef SPATIAL_DELIMITATION_HH
#define SPATIAL_DELIMITATION_HH

#include "ActiveDelimitation.hh"
#include "BoundingBox.hh"
#include "DistributionType.hh"
#include "Writable.hh"

#include <set>

template <typename Kernel, typename Point, typename Vector>
class ADistribution;

/// \brief define a spatial delimitation.
/// The space definied is the space between those two parts :
///		- the internal boundary/delimitation
///		- the external boundary/delimitation
/// @author Henri Payno
template <typename Kernel, typename Point, typename Vector>
class SpatialDelimitation : public Writable {
public:
	SpatialDelimitation(
		Point pOrigin = Point(),
		std::set<ActiveDelimitation<Kernel, Point, Vector> *> pExternal= std::set<ActiveDelimitation<Kernel, Point, Vector> *>(),
		std::set<ActiveDelimitation<Kernel, Point, Vector> *> pInternal = std::set<ActiveDelimitation<Kernel, Point, Vector> *>()
	);

	~SpatialDelimitation() override;

	/// \brief internal delimitation setter
	void setSpatialInternalDelimitation(std::set<ActiveDelimitation<Kernel, Point, Vector> *> pDelimitation) { _internalDelimitation = pDelimitation; }
	/// \brief internal delimitation getter
	std::set<ActiveDelimitation<Kernel, Point, Vector> *> getSpatialInternalDelimitation() { return _internalDelimitation; }

	/// \brief internal delimitation setter
	void setSpatialExternalDelimitation(std::set<ActiveDelimitation<Kernel, Point, Vector> *> pDelimitation) { _externalDelimitation = pDelimitation; }
	/// \brief internal delimitation getter
	std::set<ActiveDelimitation<Kernel, Point, Vector> *> getSpatialExternalDelimitation() { return _externalDelimitation; }

	/// \brief return a position inside this spatial delimitaion according to a distribution type.
	virtual Point getSpot(Distribution::DistributionType type) const = 0;

	/// \brief return true if the given point is inside the spatial delimitation
	virtual bool isIn(Point) const = 0;
	/// \brief print cell information (used also to save the cell on a .txt file)
	void write(QXmlStreamWriter&) const override = 0;

	/// \brief bounding box getter
	BoundingBox<Point> getBoundingBox() const { return _boundingBox; }
	/// \brief spatialDelimitation origin getter
	void setOrigin( Point pOrigin) { _origin = pOrigin; }
	/// \brief spatialDelimitation getter
	Point getOrigin() const { return _origin; }

protected:
	/// \brief function defining the bounding box of the world
	virtual BoundingBox<Point> createBoundingBox() = 0;

protected:
	Point _origin;																///< \brief the center of the spatial delimitation
	std::set<ActiveDelimitation<Kernel, Point, Vector> *> _internalDelimitation;	///< \brief the set of ActiveDelimitation composing the internal delimitation
	std::set<ActiveDelimitation<Kernel, Point, Vector> *> _externalDelimitation; ///< \brief the set of ActiveDelimitation composing the external delimitation

	BoundingBox<Point> _boundingBox;												///< \brief the minimal bounding box including the spatial delimitation
};


////////////////////// FUNCTION DEFINITIONS  ////////////////////////////

/// \param pOrigin the center point (spatial anchor) of the spatial delimitation
/// \param pExternal the set of ActiveDelimitation composing the external delimitation
/// \param pInternal the set of ActiveDelimitation composing the internal delimitation
template <typename Kernel, typename Point, typename Vector>
SpatialDelimitation<Kernel, Point, Vector>::SpatialDelimitation(
	Point pOrigin,
	std::set<ActiveDelimitation<Kernel, Point, Vector> *> pExternal,
	std::set<ActiveDelimitation<Kernel, Point, Vector> *> pInternal
):
	_origin(pOrigin),
	_internalDelimitation(pInternal),
	_externalDelimitation(pExternal)
{
}

template <typename Kernel, typename Point, typename Vector>
SpatialDelimitation<Kernel, Point, Vector>::~SpatialDelimitation() {
	for(auto it = _internalDelimitation.begin(); it != _internalDelimitation.end(); ++it)
		delete (*it);

	for(auto it = _externalDelimitation.begin(); it != _externalDelimitation.end(); ++it)
		delete (*it);

	_internalDelimitation.clear();
	_externalDelimitation.clear();
}

#endif
