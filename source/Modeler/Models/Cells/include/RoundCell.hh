#ifndef ROUND_CELL_HH
#define ROUND_CELL_HH

#include "Cell.hh"
#include "Round_Shape.hh"

using namespace XML::CPOP_Flag;

/// \brief define an n d round cell
/// @author Henri Payno
template <typename Kernel, typename Point, typename Vector>
class RoundCell : public Cell<Kernel, Point, Vector> {
public:
	RoundCell(const CellProperties* pCellProperties, Point pOrigin, Kernel pCellRadius, Kernel pMass);
	virtual ~RoundCell() = default;

	/// \brief cell radius getter (maxima // at rest without neighbour)
	inline Kernel getRadius() const;
	/// \brief square radius getter
	inline Kernel getSquareRadius() const { return getRadius()*getRadius(); }
	/// \brief cell radius setter
	void setRadius(Kernel pRadius);

	/// \brief origin point of the cell getter
	inline Point getOrigin() const { return Spatialable<Kernel, Point, Vector>::getPosition(); }

	/// \brief print cell information (used also to save the cell on a .txt file)
	virtual void writeAttributes(QXmlStreamWriter& writer) const;

	/// \brief will return cell mesh specificities for R.
	[[nodiscard]] virtual std::string addStatsData() const;
	/// \brief inform about the statistics exported by the meitter
	[[nodiscard]] virtual std::string writeStatsHeader() const;

private:
	/// \brief a pointer to the body, converted on the round shape he must be
	Round_Shape<Kernel, Point, Vector>* _shape;

};

/////////////////////////////// FUNCTION Definitions ////////////////////////////////////////

/// \param pCellProperties  The properties of the cell
/// \param pOrigin      The origin of the cell
/// \param pCellRadius  The radius of the shape defining the cell
/// \param pMass        The mass of the cell
template <typename Kernel, typename Point, typename Vector>
RoundCell<Kernel, Point, Vector>::RoundCell(const CellProperties* pCellProperties, Point pOrigin, Kernel pCellRadius, Kernel pMass):
	Cell<Kernel, Point, Vector>(pCellProperties, NULL, pMass)
{
	_shape = new Round_Shape<Kernel, Point, Vector>(pCellRadius);
	_shape->setWeight(pMass);
	_shape->setCrossable(false);	// a cell is never crossable
	_shape->setDeformable(true);
	DynamicAgent<Kernel, Point, Vector>::setPosition(pOrigin);
	Agent::_body = static_cast<Body*>(_shape);
}

/// \param writer the exporter used to write statistical attributes
template<typename Kernel, typename Point, typename Vector>
void RoundCell<Kernel, Point, Vector>::writeAttributes(QXmlStreamWriter& writer) const {
	Cell<Kernel, Point, Vector>::writeAttributes(writer);
	writer.writeTextElement(radius_flag, QString::fromStdString(std::to_string(getRadius())));
}

template <typename Kernel, typename Point, typename Vector>
Kernel RoundCell<Kernel, Point, Vector>::getRadius() const {
	assert(_shape);
	return _shape->getRadius();
}

template <typename Kernel, typename Point, typename Vector>
void RoundCell<Kernel, Point, Vector>::setRadius(Kernel pRadius) {
	assert(_shape);
	_shape->setRadius(pRadius);
}

template <typename Kernel, typename Point, typename Vector>
std::string RoundCell<Kernel, Point, Vector>::addStatsData() const {
	return Cell<Kernel, Point, Vector>::addStatsData() + std::string("\t" + std::to_string(getRadius()));
}

template <typename Kernel, typename Point, typename Vector>
std::string RoundCell<Kernel, Point, Vector>::writeStatsHeader() const {
	return Cell<Kernel, Point, Vector>::writeStatsHeader() + "\t" + Settings::Statistics::Cell_Radius_flag;
}

#endif
