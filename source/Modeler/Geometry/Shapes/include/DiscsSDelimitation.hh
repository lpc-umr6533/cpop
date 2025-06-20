#ifndef DISC_SPATIAL_DELIMIATION
#define DISC_SPATIAL_DELIMIATION

#include "File_CPOP_Utils.hh"
#include "Round_Shape.hh"
#include "SpatialDelimitation.hh"
#include "XMLSettings.hh"

/// \brief define a spatial delimitation which goes from an internal circle to an external circle.
/// @author Henri Payno
template<typename Kernel, typename Point, typename Vector>
class DiscsSDelimitation : public SpatialDelimitation<Kernel, Point, Vector> {
public:
	DiscsSDelimitation(Kernel, Kernel, Point pCenter = Point());

	/// \brief return the position according to the distribution.
	[[nodiscard]] Point getSpot(Distribution::DistributionType) const override;
	/// \brief return true if the given point is inside the spatial delimitation
	[[nodiscard]] bool isIn(Point) const override;

	/// \brief print cell information (used also to save the cell on a .txt file)
	void write(QXmlStreamWriter&) const override;

protected:
	/// \brief function defining the bounding box of the world
	BoundingBox<Point> createBoundingBox() override;

private:
	Kernel _internalRadius; ///< \brief the internal radius of the spehere
	Kernel _externalRadius; ///< \brief the external radius of the spehere

};

/////////////////////////// FUNCTION DEFINITIONS ///////////////////////////

/// \param pInternalRadius 	The internal radius
/// \param pExternalRadius 	The external radius
/// \param pCenter			The center of the discs
template<typename Kernel, typename Point, typename Vector>
DiscsSDelimitation<Kernel, Point, Vector>::DiscsSDelimitation(Kernel pInternalRadius, Kernel pExternalRadius, Point pCenter):
	SpatialDelimitation<Kernel, Point, Vector>(pCenter),
	_internalRadius(pInternalRadius),
	_externalRadius(pExternalRadius)
{
	assert(pInternalRadius < pExternalRadius);

	auto* externalDisc = new Round_Shape<Kernel, Point, Vector>(pExternalRadius);
	auto* internalDisc = new Round_Shape<Kernel, Point, Vector>(pInternalRadius);

	// TODO : set crossable, deformable to false !!!
	auto* delimitationInt = new ActiveDelimitation<Kernel, Point, Vector>(static_cast<Body*>(externalDisc));
	auto* delimitationExt = new ActiveDelimitation<Kernel, Point, Vector>(static_cast<Body*>(internalDisc));

	SpatialDelimitation<Kernel, Point, Vector>::_internalDelimitation.insert(delimitationInt);
	SpatialDelimitation<Kernel, Point, Vector>::_externalDelimitation.insert(delimitationExt);

	SpatialDelimitation<Kernel, Point, Vector>::_boundingBox = createBoundingBox();
}

template<typename Kernel, typename Point, typename Vector>
void DiscsSDelimitation<Kernel, Point, Vector>::write(QXmlStreamWriter& writer) const {
	using namespace XML::CPOP_Flag;

	writer.writeStartElement(discsSDelimitation_flag);
	{
		/// write internal radius
		writer.writeStartElement(internalDelimitation_flag);
		writer.writeAttribute(radius_flag, QString::fromStdString(std::to_string(_internalRadius)));
		writer.writeEndElement();

		/// write external radius
		writer.writeStartElement(externalDelimitation_flag);
		writer.writeAttribute(radius_flag, QString::fromStdString(std::to_string(_externalRadius)));
		writer.writeEndElement();

		/// write center
		IO::CPOP::writePoint(position_flag, writer, SpatialDelimitation<Kernel, Point, Vector>::getOrigin());
	}
	writer.writeEndElement();	// spheresSDelimitationflag
}

/// \param pDistributionType	The type of distribution we base on the spot
/// \return 	 				The spot according to a type of distribution
template<typename Kernel, typename Point, typename Vector>
Point DiscsSDelimitation<Kernel, Point, Vector>::getSpot(Distribution::DistributionType pDistributionType) const {
	std::string mess = "getSpot not defined for this type of template parameter";
	InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess, "DiscSDelimitation");
	return {};
}

/// \cond
// 2D
template<>
Point_2 DiscsSDelimitation<double, Point_2, Vector_2>::getSpot(Distribution::DistributionType) const;

// 3D
template<>
Point_3 DiscsSDelimitation<double, Point_3, Vector_3>::getSpot(Distribution::DistributionType) const;
/// \endcond

template<typename Kernel, typename Point, typename Vector>
bool DiscsSDelimitation<Kernel, Point, Vector>::isIn(Point) const {
	std::string mess = "hasIn undefined for this type of template parameter";
	InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess, "DiscSDelimitation");
	return false;
}

/// \cond

template<>
bool DiscsSDelimitation<double, Point_2, Vector_2>::isIn(Point_2) const;

template<>
bool DiscsSDelimitation<double, Point_3, Vector_3>::isIn(Point_3) const;
/// \endcond

template<typename Kernel, typename Point, typename Vector>
BoundingBox<Point> DiscsSDelimitation<Kernel, Point, Vector>::createBoundingBox() {
	std::string mess = "Unable to generate bounding box for this configuration";
	InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess, "DiscSDelimitation");
	return BoundingBox<Point>(Point(), Point());
}

template<>
BoundingBox<Point_2> DiscsSDelimitation<double, Point_2, Vector_2>::createBoundingBox();

template<>
BoundingBox<Point_3> DiscsSDelimitation<double, Point_3, Vector_3>::createBoundingBox();

#endif
