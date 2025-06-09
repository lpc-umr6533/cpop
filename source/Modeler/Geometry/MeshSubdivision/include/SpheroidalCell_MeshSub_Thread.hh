#ifndef SPHEROIDAL_CELL_MESH_SUBDIVION_THREAD_HH
#define SPHEROIDAL_CELL_MESH_SUBDIVION_THREAD_HH

#include "Voronoi3DCellMeshSubThread.hh"

#include <map>
#include <vector>

#include <QThread>

using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh3D;

/// \brief The thread to refine spheroid cell from voronoi cell.
/// @author Henri Payno
class SpheroidalCellMeshSubThread : public Voronoi3DCellMeshSubThread {
	Q_OBJECT

public:
	SpheroidalCellMeshSubThread(
		unsigned int, 
		unsigned int, 
		double, 
		const std::map<SpheroidalCell*, std::set<const SpheroidalCell*> >*,
		double pMaxRatioToCellRadius,
		std::vector<SpheroidalCell*> cellsToReffine = std::vector<SpheroidalCell*>(),
		double pSpaceBetweenCells = 0
	);
	~SpheroidalCellMeshSubThread() override;
	
	/// \brief main function called to reffine a specific cell
	bool reffineCell(SpheroidalCell* cell) override;
	
protected:
	/// \brief generate the nucleus radius
	bool generateNucleusRadius(SpheroidalCell* cell);
};

#endif
