#include "DiscsSDelimitation.hh"
#include "Geometry_Utils_Circle.hh"

#ifndef NDEBUG
	#define DISCS_DELIM_DEBUG 0
#else
	#define DISCS_DELIM_DEBUG 0	// must always stay at 0
#endif

template<>
Point_2 DiscsSDelimitation<double, Point_2, Vector_2>::getSpot(Distribution::DistributionType pDistType) const {
	Point_2 p;
	switch(pDistType) {
		case Distribution::RANDOM :
		{
			p = Utils::Geometry::Circle::getSpotOnDisc(_externalRadius, _origin, _internalRadius);
			break;
		}
		case Distribution::NORMALE :
		{
			/// \todo
			break;
		}
		default:
		{}
	}

	if(DISCS_DELIM_DEBUG) {
		QString mess = "find a spot at " + QString::number(p.x()) + ", " + QString::number(p.y());
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "DiscsSDelimitation");
	}
	return p;
}

template<>
Point_3 DiscsSDelimitation<double, Point_3, Vector_3>::getSpot(Distribution::DistributionType pDistType) const {
	Point_3 p;
	switch(pDistType) {
		case Distribution::RANDOM :
		{
			p = Utils::Geometry::Circle::getSpotOnDisc(_externalRadius, _origin, _internalRadius);
			break;
		}
		case Distribution::NORMALE :
		{
			/// \todo
			break;
		}
		default:
		{}
	}

	if(DISCS_DELIM_DEBUG) {
		QString mess = "find a spot at " + QString::number(p.x()) + ", " + QString::number(p.y()) + ", 0.";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "DiscsSDelimitation");
	}
	return p;
}

template<>
bool DiscsSDelimitation<double, Point_2, Vector_2>::isIn(Point_2 p) const {
	double squaredDistance = CGAL::squared_distance(p, _origin);
	return (
		(squaredDistance >= (_internalRadius*_internalRadius)) &&
		(squaredDistance <= (_externalRadius*_externalRadius))
	);
}

template<>
bool DiscsSDelimitation<double, Point_3, Vector_3>::isIn(Point_3 p) const {
	double squaredDistance = CGAL::squared_distance(p, _origin);
	return (
		(squaredDistance >= (_internalRadius*_internalRadius)) &&
		(squaredDistance <= (_externalRadius*_externalRadius)) &&
		(p.z() == 0.)
	);
}

template<>
BoundingBox<Point_2> DiscsSDelimitation<double, Point_2, Vector_2>::createBoundingBox() {
	return {
		Point_2(_origin.x() - _externalRadius, _origin.y() - _externalRadius),
		Point_2(_origin.x() - _externalRadius, _origin.y() - _externalRadius)
	};
}

template<>
BoundingBox<Point_3> DiscsSDelimitation<double, Point_3, Vector_3>::createBoundingBox() {
	return {
		Point_3(_origin.x() - _externalRadius, _origin.y() - _externalRadius, _origin.z() - _externalRadius),
		Point_3(_origin.x() + _externalRadius, _origin.y() + _externalRadius, _origin.z() + _externalRadius)
	};
}
