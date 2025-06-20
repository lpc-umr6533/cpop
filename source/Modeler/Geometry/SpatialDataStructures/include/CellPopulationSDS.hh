#ifndef CELL_POPULATION_SDS_HH
#define CELL_POPULATION_SDS_HH

#include <Cell.hh>
#include <ShapeTypes.hh>
#include <SpatialDataStructure.hh>

#include <set>
#include <QString>
#include <utility>

/// \brief Define a spatial data structure specific for cell population ( adding getNeighbors function )
/// @author Henri Payno
template<typename Kernel, typename Point, typename Vector>
class CellPopulationSDS : public SpatialDataStructure<Kernel, Point, Vector> {
	using t_SpatialableAgent = SpatialableAgent<Kernel, Point, Vector>;
	/// \todo : le faire avec les aptialable agent ???

public:
	CellPopulationSDS(QString, std::set<ShapeTypes::ShapeType> = std::set<ShapeTypes::ShapeType>());
	virtual ~CellPopulationSDS() = default;

	/// \brief function to call to add a spatialable entity
	virtual bool add(const t_SpatialableAgent*);
	/// \brief function to call to remove a spatialable entity
	virtual void remove(const t_SpatialableAgent*);
	/// \brief update the all structure
	virtual int update() = 0;

	/// \brief export the structure to a file. For checking for example.
	[[nodiscard]] virtual int exportToFile(std::string const&) const;
	/// \brief return the neighbors we want to interact with
	virtual std::set<const t_SpatialableAgent*> getNeighbours(const t_SpatialableAgent*) const = 0;
	/// \brief return the compatible shape types of the spatial data structure
	[[nodiscard]] std::set<ShapeTypes::ShapeType> compatibleShapes() const { return compShapes; }

	/// \brief return the nearest agent of a given point
	const t_SpatialableAgent* getNearestAgent(const Point) const;

protected:
	std::set<ShapeTypes::ShapeType> compShapes;	///< \brief the ompatible shapes with this SDS
};

////////////////////////////// FUNCTION DEFINITIONS ////////////////////////////////

/// \param <pName> {The anme of the spatial data structure}
/// \param <pCompatibleShapes> {The set of compatible shapes}
template<typename Kernel, typename Point, typename Vector>
CellPopulationSDS<Kernel, Point, Vector>::CellPopulationSDS(QString pName, std::set<ShapeTypes::ShapeType> pCompatibleShapes):
	SpatialDataStructure<Kernel, Point, Vector>(pName),
	compShapes(std::move(pCompatibleShapes))
{
}

/// \param <pOutFile> {The out put file}
/// \return { return :
///					- 0 : sucess.
///					- 1 : not implemented yet
///	}
template<typename Kernel, typename Point, typename Vector>
int CellPopulationSDS<Kernel, Point, Vector>::exportToFile(std::string const&) const {
	InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES,
		"export not implemented yet",
		SpatialDataStructure<Kernel, Point, Vector>::getName().toStdString()
	);
	return 1;
}

/// Adding a spatialable from the MAS SpatialDataStructure
/// This will add the spatialable to the list of contained spatialable
template<typename Kernel, typename Point, typename Vector>
bool CellPopulationSDS<Kernel, Point, Vector>::add(const t_SpatialableAgent* toAdd) {
	return SpatialDataStructure<Kernel, Point, Vector>::add(toAdd);
}

/// Removing a spatialable from the MAS SpatialDataStructure
/// This will remove the spatialable to the list of contained spatialable
template<typename Kernel, typename Point, typename Vector>
void CellPopulationSDS<Kernel, Point, Vector>::remove(const t_SpatialableAgent* toRemove) {
	return SpatialDataStructure<Kernel, Point, Vector>::remove(toRemove);
}

#endif
