#include "Voronoi_2D_Mesh.hh"

#include "CellMeshSettings.hh"
#include "EngineSettings.hh"
#include "File_Utils.hh"
#include "File_Utils_OFF.hh"
#include "Voronoi2DCellMeshSubThread.hh"

#include <fstream>
#include <CGAL/Complex_2_in_triangulation_3.h>
#include <CGAL/IO/Complex_2_in_triangulation_3_file_writer.h>

#include <CGAL/bounding_box.h>

#include <CGAL/range_search_delaunay_2.h>

#ifndef NDEBUG
	#define VORONOI_2D_MESH_DEBUG 0
#else
	#define VORONOI_2D_MESH_DEBUG 0	// must always stay at 0
#endif

#undef max

/// \param pMaxNbSegs The maximl number of segment the mesh of a cell should contain
/// \param pDelta the threshold for mesh subdivision
/// \param pInitSpatialables The set of spatialable to include on the mesh.
Voronoi_2D_Mesh::Voronoi_2D_Mesh(
	unsigned int pMaxNbSegs,
	double pDelta,
	std::set<t_Cell_2*> pInitSpatialables):
	Mesh<double, Point_2, Vector_2>(MeshTypes::Weighted_Voronoi_Tesselation),
	Delaunay_2D_SDS("Voronoi_based_Mesh_2D"),
	_maxNumberOfSegPerCell(pMaxNbSegs),
	_deltaGain(pDelta)
{
	_minWeight = std::numeric_limits<double>::max();
	for(auto pInitSpatialable : pInitSpatialables)
	{
		add(pInitSpatialable);
	}
}

Voronoi_2D_Mesh::~Voronoi_2D_Mesh() {
	clean();
}

void Voronoi_2D_Mesh::clean() {
	// neighboursCell.clear();
}

/// \param pToAdd The spatialable to add to the mesh.
/// \return success or fail
bool Voronoi_2D_Mesh::add(t_Cell_2* pToAdd) {
	assert(pToAdd);
	assert(CGAL::is_finite(pToAdd->getPosition().x()));
	assert(CGAL::is_finite(pToAdd->getPosition().y()));

	/// complete the map
	auto* cell = dynamic_cast<DiscoidalCell*> (pToAdd);
	if(!cell) {
		std::string mess = "Unable to add the cell " + std::to_string(cell->getID()) + ", none discoidal cell";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess, "Voronoi_2D_Mesh");
		return false;
	}

	/// TODO : must be removed ?
	{
		std::vector<RT_2::Face_handle > conflictCells;
		std::vector<RT_2::Vertex_handle > hiddenVertices;
		Weighted_point_2 p(Weighted_point_2(pToAdd->getPosition(), cell->getRadius()));
		if(_delaunayTriangulation.number_of_vertices() > 3)
			_delaunayTriangulation.get_conflicts_and_hidden_vertices(p, std::back_inserter(conflictCells), std::back_inserter(hiddenVertices));

		if(hiddenVertices.size() > 0) {
			// Do not add it besause in is in conflict
			return true;
		}
	}

	if(! Delaunay_2D_SDS::add(pToAdd))
		return false;

	_constCellToDiscoidal.insert(
		std::pair<const t_Cell_2*, DiscoidalCell*>(
			const_cast<const t_Cell_2*>(pToAdd),
			cell
		)
	);

	// deal with weights
	_weights.insert(std::pair<Point_2, double>(pToAdd->getPosition(),  cell->getRadius()));

	if(cell->getRadius() < _minWeight)
		_minWeight = cell->getRadius();

	pToAdd->resetMesh();
	return true;
}

/// \param pSpatialable The spatialable to remove from the mesh
void Voronoi_2D_Mesh::remove(t_Cell_2* pSpatialable) {
	assert(pSpatialable);
	auto* cell = dynamic_cast<DiscoidalCell*>(pSpatialable);
	if(!cell)
		return;
	Delaunay_2D_SDS::remove(pSpatialable);
}

/// \param pPath The output path file
/// \param pFormat The requested file format
/// \param pDivided True if we want to divide the mesh
/// \return return values :
///					- 0 : success
///					- 1 : not implemented yet
///					- 2 : failed during export
int Voronoi_2D_Mesh::exportToFile(std::string const& path, MeshOutFormats::outputFormat format, bool divided) {
	switch(format) {
		case MeshOutFormats::OFF :
			return exportToFileOff(path, divided);
		default:
		{
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "Exporter do not deal with this kind of format yet", "Weighted Voronoi 2D");
			return 1;
		}
	}
}

/// \return the bounding box containing all cellMesh
K::Iso_rectangle_2 Voronoi_2D_Mesh::getBoundingBox() const {
 	// create the Bounding box.
	std::vector<Point_2> pts;
	for(auto itVertex = _delaunayTriangulation.finite_vertices_begin(); itVertex != _delaunayTriangulation.finite_vertices_end(); ++itVertex) {
		Point_2 mypt = (itVertex->point()).point();
		pts.push_back(mypt);
	}

	Iso_rectangle_2 bb = CGAL::bounding_box( pts.begin(), pts.end());
	// add the potential next point from the voronoi cell and disc shape
	return {
		Point_2(bb.xmin()-2.*_minWeight, bb.ymin()-2.*_minWeight),
		Point_2(bb.xmax()+2.*_minWeight, bb.ymax()+2.*_minWeight)
	};
}

void Voronoi_2D_Mesh::removeConflicts() {
	/// TODO : redo it better
	assert(_delaunayTriangulation.is_valid());
	// create shape
	std::set<RT_2::Vertex_handle> verticesToRemove; // we will remove all vertices which aren'Gabriel

	/// check directly vertices
	for(auto itEdge = _delaunayTriangulation.finite_edges_begin(); itEdge != _delaunayTriangulation.finite_edges_end(); ++itEdge) {
		RT_2::Vertex_handle v1 = itEdge->first->vertex(RT_2::ccw(itEdge->second));
		RT_2::Vertex_handle v2 = itEdge->first->vertex(RT_2::cw(itEdge->second));
		double maxLength = std::max(_weights[v1->point().point()], _weights[v2->point().point()]);
		RT_2::Vertex_handle vToRemove;

		if(_weights[v1->point().point()] < _weights[v2->point().point()]) {
			vToRemove = (REMOVE_SMALLEST_WEIGHT ? v1 : v2 );
		} else if(_weights[v1->point().point()] == _weights[v2->point().point()]) {
			// if same weights : remove the one with the bigest ID ( to insure repetability, need to have a rule )
			vToRemove = (v1->info()->getID() < v2->info()->getID()) ? v2 : v1;
		} else {
			vToRemove = (REMOVE_SMALLEST_WEIGHT ? v2 : v1 );
		}

		// TODO optimisation
		if(sqrt(CGAL::squared_distance(v1->point().point(), v2->point().point())) < maxLength) {
			verticesToRemove.insert(vToRemove);
		}
	}

	int nbConflict = verticesToRemove.size();

	auto itVertexToRmv = verticesToRemove.begin();
	while(itVertexToRmv != verticesToRemove.end()) {
		const t_SpatialableAgent_2* tmp = (*itVertexToRmv)->info();
		remove(_constCellToDiscoidal[tmp]);
		verticesToRemove.erase(itVertexToRmv++);
	}

	assert(_delaunayTriangulation.is_valid());
	if(nbConflict > 0)
		removeConflicts();
}

void Voronoi_2D_Mesh::generateNeighbourhood() {
	assert(_delaunayTriangulation.is_valid());
	/// by default no hidden point, CGAL triangulation by default set the weight of this kind of point to 0
	for(auto itVertex = _delaunayTriangulation.finite_vertices_begin(); itVertex != _delaunayTriangulation.finite_vertices_end(); ++itVertex) {
		const t_SpatialableAgent_2* currentSpa = itVertex->info();
		assert(currentSpa);
		DiscoidalCell* currentCell = _constCellToDiscoidal[currentSpa];
		assert(currentCell);
		_neighboursCell.insert(std::make_pair(currentCell, std::set<const DiscoidalCell*>()));

		// add each neighbours
		{
			std::set<const t_SpatialableAgent_2*> neighbours = getNeighbours(currentCell);
			for(auto const& neighbour : neighbours) {
				assert(neighbour);
				if(_constCellToDiscoidal.find(neighbour) != _constCellToDiscoidal.end()) {
					assert(_constCellToDiscoidal[neighbour]);
					_neighboursCell[currentCell].insert( const_cast<const DiscoidalCell*>(_constCellToDiscoidal[neighbour ]) );
				}
			}
		}
	}
}

/// \return  The vector corresponding to the cell structure.
std::vector<DiscoidalCell*> Voronoi_2D_Mesh::getCellsStructure() {
	generateNeighbourhood();
	/// we will only export unhidden vertices
	std::vector<DiscoidalCell*> results;
	RT_2::Finite_vertices_iterator itVertex;
	for(itVertex = _delaunayTriangulation.finite_vertices_begin(); itVertex != _delaunayTriangulation.finite_vertices_end(); ++itVertex) {
		if(!itVertex->is_hidden()) {
			assert(itVertex->info());
			results.push_back(( _constCellToDiscoidal[itVertex->info()]));
		}
	}

	return results;
}

/// \return the cells containg a mesh generate by the process
std::vector<DiscoidalCell*> Voronoi_2D_Mesh::generateMesh() {
	assert(_delaunayTriangulation.is_valid());

	removeConflicts();

	std::string mess = "start exporting for "  + std::to_string(_delaunayTriangulation.number_of_vertices()) + " Cell(s) ";
	InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess, "Vornoi_2DMesh");

	std::vector<DiscoidalCell*> cells = getCellsStructure();

	// TODO ==?
	auto neighbours = static_cast<const std::map<DiscoidalCell*, std::set<const DiscoidalCell*>>>(_neighboursCell);

	if(!USE_THREAD_FOR_MESH_SUBDVN) {
		Voronoi2DCellMeshSubThread reffinement(
			0,
			getMaxNbSegPerCell(),
			getDeltaWin(),
			&neighbours
		);

		for(auto const& cell : cells)
			reffinement.reffineCell(cell);
	} else {
		std::vector<Voronoi2DCellMeshSubThread*> reffinementThreads;
		// create the reffinement threads
		unsigned int nbThreadToCreate = (cells.size() / MIN_NB_CELL_PER_THREAD) + 1;
		{
			// create as much thread as needed
			if(nbThreadToCreate > INITIAL_MAX_THREAD)
				nbThreadToCreate = INITIAL_MAX_THREAD;

			unsigned int threadID = 0;
			while(reffinementThreads.size() < nbThreadToCreate) {
				if(VORONOI_2D_MESH_DEBUG) {
					mess = "create a new thread of ID "  + std::to_string(threadID);
					InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess, "Voronoi_2DMesh");
				}

				reffinementThreads.push_back(
					new Voronoi2DCellMeshSubThread(
						threadID,
						getMaxNbSegPerCell(),
						getDeltaWin(),
						&neighbours
					)
				);
				threadID++;
			}
		}

		// add cells to threads
		{
			int iCell = 0;
			for(auto const& cell : cells) {
				reffinementThreads[iCell%nbThreadToCreate]->addCell(cell);
				iCell++;
			}
		}

		// run threads.
		for(auto* reffinementThread : reffinementThreads)
			reffinementThread->start(MESHING_THREAD_PRIORITY);

		/// wait until all threads process
		for(auto* reffinementThread : reffinementThreads)
		{
			reffinementThread->wait();
		}

		//delete reffinement thread
		for(auto*& reffinementThread : reffinementThreads)
		{
			delete reffinementThread;
			reffinementThread = nullptr;
		}
	}

	_neighboursCell.clear();
	return cells;
}

/// \param pPath Theoputput path file
/// \param pDivided True if we want to create a new file .off for each poly
/// \return return values :
///					- 0 : success
///					- 1 : not implemented yet
///					- 2 : failed during export
#include <CGAL/Arrangement_2.h>
#include <CGAL/Direction_2.h>

int Voronoi_2D_Mesh::exportToFileOff(std::string const& path, bool divided) {
	auto cells = generateMesh();
	int error = 0;
	if(divided) {
		error = exportToFileOff_divided(path, cells);
	} else {
		error = exportToFileOff_undivided(path, cells);
	}

	return error;
}

/// \param pPath Theoputput path file
/// \param cells The list of cell to export
/// \return return values :
///					- 0 : success
///					- 1 : not implemented yet
///					- 2 : failed during export
int Voronoi_2D_Mesh::exportToFileOff_divided(std::string const& path, DiscoidalCells const& cells) {
	std::map<Point_3, unsigned long int> indexes;
	unsigned long int iFile = 0;
	for(auto const& curCell : cells) {
		std::string fileName = path + "_" + std::to_string(iFile);
		if(exportToFileOff_undivided(fileName, {curCell}) != 0 )
			return 2;
		iFile++;
	}
	return 0;
}

/// \param pPath Theoputput path file
/// \param cells The list of cell to export
/// \return return values :
///					- 0 : success
///					- 1 : not implemented yet
///					- 2 : failed during export
int Voronoi_2D_Mesh::exportToFileOff_undivided(std::string const& path, DiscoidalCells const& cells) {
	std::map<Point_2, unsigned long int> indexes;
	std::ofstream* voronoiOut = nullptr;

	std::string fullPath = path + ".off";
	voronoiOut = IO::OFF::createOffFileWithHeader(fullPath);

	/// export voronoi points
	std::set<Point_2> points;
	for(auto const* cell : cells) {
		// add shape points
		for(auto itPolyPts = cell->shape_begin(); itPolyPts != cell->shape_end(); ++itPolyPts)
			points.insert(itPolyPts->source());
	}

	*voronoiOut << points.size() << " " << cells.size() << " 0" << std::endl;
	IO::OFF::exportVerticesToOff(points, indexes, voronoiOut);

	// export polyhedrons
	for(auto const* cell : cells) {
		*voronoiOut << cell->shape_size();
		for(auto itPolyPts = cell->shape_begin(); itPolyPts != cell->shape_end(); ++itPolyPts)
			*voronoiOut << " " << indexes[itPolyPts->source()];
		*voronoiOut << " " << IO::exportColor(cell->getColor()).toStdString() << std::endl; /// add a random color to each voronoi cell
	}

	voronoiOut->close();
	delete voronoiOut;
	return 0;
}

bool Voronoi_2D_Mesh::isValid() {
	/// \todo
	return true;
}

std::set<t_Cell_2*> Voronoi_2D_Mesh::getCells() const {
	std::set<t_Cell_2*> cells;
	for(auto const& cell : _constCellToDiscoidal) {
		assert(cell.second);
		cells.insert(cell.second);
	}
	return cells;
}

std::set<t_Cell_2*> Voronoi_2D_Mesh::getCellsWithShape() const {
	std::set<t_Cell_2*> cells;
	for(auto const& cell : _constCellToDiscoidal) {
		assert(cell.second);
		if(cell.second->hasMesh())
			cells.insert(cell.second);
	}

	return cells;
}
