#ifndef CELL_FACTORY_HH
#define CELL_FACTORY_HH

#include "Cell.hh"
#include "CellProperties.hh"
#include "RoundCellProperties.hh"

/// \brief define the cell factory
/// @author Henri Payno
class CellFactory {
public:
	/// \brief return the cell factory.
	static CellFactory* getInstance();
	/// \brief produce a cell accoridng to the cell properties.
	template<typename Kernel, typename Point, typename Vector>
	Cell<Kernel, Point, Vector>* produce(const CellProperties* pCellProp, const LifeCycles::LifeCycle pLifeCycle = LifeCycles::NA);
};

///////////////////////// TEMPLATE DEFINITION /////////////////////////
#include <QString>
#include <InformationSystemManager.hh>

/// \param pCellProp The cell properties
/// \return Cell The generated cell from her properties
template<typename Kernel, typename Point, typename Vector>
Cell<Kernel, Point, Vector>* CellFactory::produce(const CellProperties* pCellProp, const LifeCycles::LifeCycle pLifeCycle) {
	QString mess = "Unable to create cell for this template parameter " + 
		QString(typeid(Kernel).name()) + ", " + QString(typeid(Point).name()) + ", " + QString(typeid(Vector).name()) + ", ";
	InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "CellFactory");
}

#include "GeometrySettings.hh"
// 2D specialization
template<>
Cell<double, Point_2, Vector_2>* CellFactory::produce(const CellProperties* pCellProp, const LifeCycles::LifeCycle pLifeCycle);

// 3D specialization
template<>
Cell<double, Point_3, Vector_3>* CellFactory::produce(const CellProperties* pCellProp, const LifeCycles::LifeCycle pLifeCycle);

#endif
