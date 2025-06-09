#ifndef CUTTER_HH
#define CUTTER_HH

#include "CellSettings.hh"
#include "Direction.hh"

using namespace Settings::Geometry;
using namespace Settings::nCell;

/// \brief Cutter_3 class enable user to select some 3D spatialables by removing 3D spatialables
/// not included on defined region.
/// @author Henri Payno
class Cutter_3 {
public:
	Cutter_3(std::vector<t_SpatialableAgent_3*>);
	Cutter_3(std::set<t_SpatialableAgent_3*>);

	/// \brief cut all 3D spatialables on the given direction from the given val
	void cut(Direction, double val, double bufferWidth=0.);
	/// \brief uncuted 3D spatialables getter
	[[nodiscard]] std::vector<t_SpatialableAgent_3*> getUncutCells() const 	{ return _uncutCells;}
	/// \brief uncuted 3D spatialables getter
	[[nodiscard]] std::vector<t_SpatialableAgent_3*> getCutCells() const 	{ return _cutCells;}
	/// \brief uncuted 3D spatialables getter
	[[nodiscard]] std::vector<t_SpatialableAgent_3*> getCellsOnBuffer() const { return _cellsOnBuffer;}
	/// \brief will set all cell membrane of the cell included on the buffer to the requested color
	void setColorToCellOnBuffer(CGAL::Color );
	/// \brief will set the given color to all the cell uncuted
	void setColorToUncuttedCell(CGAL::Color);
	/// \brief will set the given color to the cutted cell
	void setColorToCuttedCell(CGAL::Color);

private:
	/// \brief will cut 3D spatialables on to cut trhought the given direction at the given value.
	void processCut(
		Direction,
		double,
		double,
		std::vector<t_SpatialableAgent_3*>* toCut,
		std::vector<t_SpatialableAgent_3*>* buffer = nullptr,
		std::vector<t_SpatialableAgent_3*>* cutted = nullptr,
		bool cutBuffer = false
	);

private:
	std::vector<t_SpatialableAgent_3*> _uncutCells;		///< \brief the cell uncuted : the one targetted by the user
	std::vector<t_SpatialableAgent_3*> _cutCells;			///< \brief the removed 3D spatialables
	std::vector<t_SpatialableAgent_3*> _cellsOnBuffer;	///< \brief the cell stored on the buffer accross the different cut
};

#endif // CUTTER_HH
