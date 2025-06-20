#include "RandomEngineManager.hh"
#include "Geometry_Utils_Sphere.hh"
#include "InformationSystemManager.hh"
#include "Round_Shape.hh"
#include "SpheresSDelimitation.hh"
#include "XMLSettings.hh"

#include <cmath>

#ifndef NDEBUG
	#define SPHERES_DELIM_DEBUG 0
#else
	#define SPHERES_DELIM_DEBUG 0	// must always stay at 0
#endif

using namespace XML::CPOP_Flag;

/// \param pInternalRadius The radius of the internal sphere
/// \param pExternalRadius The radius of the external sphere
/// \param pCenter the center of the spheroid delimitation
/// \warning depending on cell position, center != barycenter. First is the origin of sphere used
/// to define spatial delimitation. Second is the barycenter of cell centers
SpheresSDelimitation::SpheresSDelimitation(double pInternalRadius, double pExternalRadius, Point_3 pCenter):
	SpatialDelimitation<double, Point_3, Vector_3>(pCenter),
	internalRadius(pInternalRadius),
	externalRadius(pExternalRadius)
{
	assert(pInternalRadius < pExternalRadius);
	auto* externalSphere = new Round_Shape<double, Point_3, Vector_3>(pExternalRadius);
	auto* internalSphere = new Round_Shape<double, Point_3, Vector_3>(pInternalRadius);

	// TODO : set crossable, deformable to false !!!
	auto* delimitationInt = new ActiveDelimitation<double, Point_3, Vector_3>(static_cast<Body*>(externalSphere));
	auto* delimitationExt = new ActiveDelimitation<double, Point_3, Vector_3>(static_cast<Body*>(internalSphere));

	_internalDelimitation.insert(delimitationInt);
	_externalDelimitation.insert(delimitationExt);

	_boundingBox = createBoundingBox();
}

/// \param pDistType The distribution type to apply
/// \return A random spot.
Point_3 SpheresSDelimitation::getSpot(Distribution::DistributionType pDistType) const {
	Point_3 p;
	switch(pDistType) {
		case Distribution::RANDOM :
		{
			p = Utils::Geometry::Sphere::getSpotOnSphere(externalRadius, this->getOrigin(), internalRadius);
			break;
		}
		case Distribution::NORMALE :
		{
			/// \todo : define normale distribution
			// std::default_random_engine generator;
			// std::normal_distribution<double> distribution((externalRadius - internalRadius)/2., );
			break;
		}
		default :
		{
		}
	}

	if(SPHERES_DELIM_DEBUG) {
		std::string mess = "find a spoat at " + std::to_string(p.x()) + ", " + std::to_string(p.y()) + ", " + std::to_string(p.z());
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess, "SphereSDelimitation");
	}

	return p;
}

/// \param p The point to check if isIn the sphere
/// \return True is the point is in the Sphere
bool SpheresSDelimitation::isIn(Point_3 p) const {
	double squaredDistance = CGAL::squared_distance(p, this->getOrigin());
	return (
		(squaredDistance >= (internalRadius*internalRadius)) &&
		(squaredDistance <= (externalRadius*externalRadius))
	);
}

/// \param writer the output of the spheredelimitation description
void SpheresSDelimitation::write(QXmlStreamWriter& writer) const {
	writer.writeStartElement(spheresSDelimitation_flag);
	{
		/// write internal radius
		writer.writeStartElement(internalDelimitation_flag);
		writer.writeAttribute(radius_flag, QString::number(internalRadius) );
		writer.writeEndElement();

		/// write external radius
		writer.writeStartElement(externalDelimitation_flag);
		writer.writeAttribute(radius_flag, QString::number(externalRadius) );
		writer.writeEndElement();

		/// write center
		IO::CPOP::writePoint(position_flag, writer, this->getOrigin());
	}
	writer.writeEndElement();	// spheresSDelimitationflag
}

/// \return An almost-shorter bounding box including the delimitation.
BoundingBox<Point_3> SpheresSDelimitation::createBoundingBox()
{
	return {
		Point_3( _origin.x() - externalRadius, _origin.y() - externalRadius, _origin.z() - externalRadius),
		Point_3( _origin.x() + externalRadius, _origin.y() + externalRadius, _origin.z() + externalRadius)
	};
}
