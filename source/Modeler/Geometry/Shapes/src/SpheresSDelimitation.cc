/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "RandomEngineManager.hh"
#include "CGAL_Utils.hh"
#include "Geometry_Utils_Sphere.hh"
#include "InformationSystemManager.hh"
#include "Round_Shape.hh"
#include "SpheresSDelimitation.hh"
#include "XMLSettings.hh"

#include <math.h>

#ifndef NDEBUG
	#define SPHERES_DELIM_DEBUG 0
#else
	#define SPHERES_DELIM_DEBUG 0	// must always stay at 0
#endif

using namespace XML::CPOP_Flag;

///////////////////////////////////////////////////////////////////////////////////////
/// \param pInternalRadius The radius of the internal sphere
/// \param pExternalRadius The radius of the external sphere
/// \param pCenter the center of the spheroid delimitation 
/// \warning depending on cell position, center != barycenter. First is the origin of sphere used
/// to define spatial delimitation. Second is the barycenter of cell centers
///////////////////////////////////////////////////////////////////////////////////////
SpheresSDelimitation::SpheresSDelimitation(double pInternalRadius, double pExternalRadius, Point_3 pCenter):
	SpatialDelimitation<double, Point_3, Vector_3>(pCenter), 
	internalRadius(pInternalRadius), 
	externalRadius(pExternalRadius)
{
	assert(pInternalRadius < pExternalRadius);
	Round_Shape<double, Point_3, Vector_3>* externalSphere = new Round_Shape<double, Point_3, Vector_3>(pExternalRadius);
	Round_Shape<double, Point_3, Vector_3>* internalSphere = new Round_Shape<double, Point_3, Vector_3>(pInternalRadius);

	// TODO : set crossable, deformable to false !!!
	ActiveDelimitation<double, Point_3, Vector_3>* delimitationInt = new ActiveDelimitation<double, Point_3, Vector_3>(static_cast<Body*>(externalSphere));
	ActiveDelimitation<double, Point_3, Vector_3>* delimitationExt = new ActiveDelimitation<double, Point_3, Vector_3>(static_cast<Body*>(internalSphere));

	this->internalDelimitation.insert(delimitationInt);
	this->externalDelimitation.insert(delimitationExt);

	boundingBox = createBoundingBox();
}

///////////////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////////////
SpheresSDelimitation::~SpheresSDelimitation()
{

}

///////////////////////////////////////////////////////////////////////////////////////
/// \param pDistType The distribution type to apply
/// \return A random spot.
///////////////////////////////////////////////////////////////////////////////////////
Point_3 SpheresSDelimitation::getSpot(Distribution::DistributionType pDistType) const
{	
	Point_3 p;
	switch(pDistType)
	{
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

	if(SPHERES_DELIM_DEBUG)
	{
		QString mess = "find a spoat at " + QString::number(p.x()) + ", " + QString::number(p.y()) + ", " + QString::number(p.z());
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "SphereSDelimitation");
	}
	return p;
}

///////////////////////////////////////////////////////////////////////////////////////
/// \param p The point to check if isIn the sphere
/// \return True is the point is in the Sphere
///////////////////////////////////////////////////////////////////////////////////////
bool SpheresSDelimitation::isIn(Point_3 p) const
{
	double squaredDistance = CGAL::squared_distance(p, this->getOrigin());
	return (
		(squaredDistance >= (internalRadius*internalRadius) ) &&
		(squaredDistance <= (externalRadius*externalRadius) )
		);
}

///////////////////////////////////////////////////////////////////////////////////////
/// \param writer the output of the spheredelimitation description
///////////////////////////////////////////////////////////////////////////////////////
void SpheresSDelimitation::write(QXmlStreamWriter& writer) const
{
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

///////////////////////////////////////////////////////////////////////////////////////
/// \return An almost-shorter bounding box including the delimitation.
///////////////////////////////////////////////////////////////////////////////////////
BoundingBox<Point_3> SpheresSDelimitation::createBoundingBox()
{
	return BoundingBox<Point_3> ( 
				Point_3( origin.x() - externalRadius, origin.y() - externalRadius, origin.z() - externalRadius),
				Point_3( origin.x() + externalRadius, origin.y() + externalRadius, origin.z() + externalRadius)
				 );
}