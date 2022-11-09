/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef DISC_SPATIAL_DELIMIATION
#define DISC_SPATIAL_DELIMIATION

#include "SpatialDelimitation.hh"
#include "Round_Shape.hh"

///////////////////////////////////////////////////////////////////////////
/// \brief define a spatial delimitation which goes from an internal circle to an external circle.
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
class DiscsSDelimitation : public SpatialDelimitation<Kernel, Point, Vector>
{
public:
	/// \brief constructor
	DiscsSDelimitation(Kernel, Kernel, Point pCenter = Point());
	/// \brief destructor
	virtual ~DiscsSDelimitation();

	/// \brief return the position according to the distribution.
	virtual Point getSpot(Distribution::DistributionType) const;
	/// \brief return true if the given point is inside the spatial delimitation
	virtual bool isIn(Point) const;

	/// \brief print cell information (used also to save the cell on a .txt file)
	virtual void write(QXmlStreamWriter&) const;

protected:
	/// \brief function defining the bounding box of the world
	virtual BoundingBox<Point> createBoundingBox();

private:
	Kernel internalRadius;		///< \brief the internal radius of the spehere
	Kernel externalRadius;		///< \brief the external radius of the spehere

};

/////////////////////////// FUNCTION DEFINITIONS ///////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// 		constructor
/// \param pInternalRadius 	The internal radius
/// \param pExternalRadius 	The external radius
/// \param pCenter			The center of the discs
/////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
DiscsSDelimitation<Kernel, Point, Vector>::DiscsSDelimitation(Kernel pInternalRadius, Kernel pExternalRadius, Point pCenter):
	SpatialDelimitation<Kernel, Point, Vector>(pCenter),
	internalRadius(pInternalRadius),
	externalRadius(pExternalRadius)
{
	assert(pInternalRadius < pExternalRadius);
	
	Round_Shape<Kernel, Point, Vector>* externalDisc = new Round_Shape<Kernel, Point, Vector>(pExternalRadius);
	Round_Shape<Kernel, Point, Vector>* internalDisc = new Round_Shape<Kernel, Point, Vector>(pInternalRadius);

	// TODO : set crossable, deformable to false !!!
	ActiveDelimitation<Kernel, Point, Vector>* delimitationInt = new ActiveDelimitation<Kernel, Point, Vector>(static_cast<Body*>(externalDisc));
	ActiveDelimitation<Kernel, Point, Vector>* delimitationExt = new ActiveDelimitation<Kernel, Point, Vector>(static_cast<Body*>(internalDisc));

	SpatialDelimitation<Kernel, Point, Vector>::internalDelimitation.insert(delimitationInt);
	SpatialDelimitation<Kernel, Point, Vector>::externalDelimitation.insert(delimitationExt);
	
	SpatialDelimitation<Kernel, Point, Vector>::boundingBox = createBoundingBox();

}

/////////////////////////////////////////////////////////////////////////////////////////////
// 		destructor
/////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
DiscsSDelimitation<Kernel, Point, Vector>::~DiscsSDelimitation()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////
// 		destructor
/////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
void DiscsSDelimitation<Kernel, Point, Vector>::write(QXmlStreamWriter& writer) const
{
	writer.writeStartElement(discsSDelimitation_flag);
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
		IO::CPOP::writePoint(position_flag, writer, SpatialDelimitation<Kernel, Point, Vector>::getOrigin());
	}	
	writer.writeEndElement();	// spheresSDelimitationflag
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 			get spot fonction. 
/// \param pDistributionType	The type of distribution we base on the spot
/// \return 	 				The spot according to a type of distribution
/////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
Point DiscsSDelimitation<Kernel, Point, Vector>::getSpot(Distribution::DistributionType pDistributionType) const
{
	QString mess = "getSpot not defined for this type of template parameter";
	InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "DiscSDelimitation");	
	return Point();
}

/// \cond
// 2D
template<>
Point_2 DiscsSDelimitation<double, Point_2, Vector_2>::getSpot(Distribution::DistributionType) const;

// 3D
template<>
Point_3 DiscsSDelimitation<double, Point_3, Vector_3>::getSpot(Distribution::DistributionType) const;
/// \endcond


/////////////////////////////////////////////////////////////////////////////////////////////
// 			hasIn fonction. 
template<typename Kernel, typename Point, typename Vector>
bool DiscsSDelimitation<Kernel, Point, Vector>::isIn(Point) const
{
	QString mess = "hasIn undefined for this type of template parameter";
	InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "DiscSDelimitation");	
	return false;
}

/// \cond

template<>
bool DiscsSDelimitation<double, Point_2, Vector_2>::isIn(Point_2) const;

template<>
bool DiscsSDelimitation<double, Point_3, Vector_3>::isIn(Point_3) const;
/// \endcond

/////////////////////////////////////////////////////////////////////////////////////////////
// 			hasIn fonction. 
template<typename Kernel, typename Point, typename Vector>
BoundingBox<Point> DiscsSDelimitation<Kernel, Point, Vector>::createBoundingBox()
{
	QString mess = "Unable to generate bounding box for this configuration";
	InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "DiscSDelimitation");	
	return BoundingBox<Point>(Point(), Point());
}

template<>
BoundingBox<Point_2> DiscsSDelimitation<double, Point_2, Vector_2>::createBoundingBox();

template<>
BoundingBox<Point_3> DiscsSDelimitation<double, Point_3, Vector_3>::createBoundingBox();

#endif //DISC_SPATIAL_DELIMIATION_2D