#include "Voronoi_3D_Mesh.hh"

#include "CGAL_Utils.hh"
#include "CellMeshSettings.hh"
#include "EngineSettings.hh"
#include "File_Utils_OFF.hh"
#include "Voronoi3DCellMeshSubThread.hh"

#include <CGAL/Polyhedron_3.h>
#include <CGAL/convex_hull_3.h>

#include <cstdint>
#include <string>
#include <fstream>

#ifndef NDEBUG
	#define VORONOI_3D_MESH_DEBUG 0
#else
	#define VORONOI_3D_MESH_DEBUG 0	// must always stay at 0
#endif

#undef max

#include <iostream>
#include <string>
#include <cstdlib>
#include <string>

/// \param pInitSpatialables The set of spatialable to include on the mesh.
/// \param pMaxNbFacet The maximal number of facet a cell must contained
/// \param delta The minimal value for which we continu to reffine
Voronoi_3D_Mesh::Voronoi_3D_Mesh(
	unsigned int pMaxNbFacet,
	double delta,
	std::set<t_Cell_3* > pInitSpatialables ):
	Mesh<double, Point_3, Vector_3>(MeshTypes::Weighted_Voronoi_Tesselation),
	Delaunay_3D_SDS("Voronoi_based_Mesh_3D"),
	_maxNumberOfFacetPerCell(pMaxNbFacet),
	_deltaGain(delta)
{
	_minWeight = std::numeric_limits<double>::max();
	_maxWeight = -1;
	for(auto const& pInitSpatialable : pInitSpatialables)
		add(pInitSpatialable);
}

Voronoi_3D_Mesh::~Voronoi_3D_Mesh() {
	clean();
}

K::Iso_cuboid_3 Voronoi_3D_Mesh::getBoundingBox() const {
	// create the Bounding box.
	std::vector<Point_3> pts;
	for(auto itVertex = _delaunay.finite_vertices_begin(); itVertex != _delaunay.finite_vertices_end(); ++itVertex)
		pts.push_back(itVertex->point().point());

	K::Iso_cuboid_3 bb = CGAL::bounding_box( pts.begin(), pts.end());
	// add the potential next point from the voronoi cell and sphere shape
	return {
		Point_3(bb.xmin()-2*_maxWeight, bb.ymin()-2*_maxWeight, bb.zmin()-2*_maxWeight),
		Point_3(bb.xmax()+2*_maxWeight, bb.ymax()+2*_maxWeight, bb.zmax()+2*_maxWeight)
	};	// for now weight correspond to the cell radius.
			// * 2 because is own radius and the radius of bounding point
}

void Voronoi_3D_Mesh::clean() {
	Delaunay_3D_SDS::clean();
	_neighboursCell.clear();
}

/// \brief the spatialable to add on the mesh
/// \warning :  The weight corresponf to the cell diameter because of the CGAL implementation.
bool Voronoi_3D_Mesh::add(t_Cell_3* pToAdd) {
	assert(pToAdd);
	assert(CGAL::is_finite(pToAdd->getPosition().x()));
	assert(CGAL::is_finite(pToAdd->getPosition().y()));
	assert(CGAL::is_finite(pToAdd->getPosition().z()));

	auto* cell = dynamic_cast<SpheroidalCell*> (pToAdd);

	if(!cell) {
		QString mess = "Unable to add the cell " + QString::number(cell->getID()) + ", none spheroidal cell";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "Voronoi_3D_Mesh");
		return false;
	}

	if(VORONOI_3D_MESH_DEBUG) {
		QString mess = "adding the point at " + QString::number(pToAdd->getPosition().x()) + ", " + QString::number(pToAdd->getPosition().y()) +
			" to the voronoi with weight : " + QString::number(cell->getRadius());
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "Weighted Voronoi 3D");
	}

	if(! Delaunay_3D_SDS::add(pToAdd))
		return false;

	_constCellToSpheroidal.insert(
		std::pair<const t_Cell_3*, SpheroidalCell*>(
			const_cast<const t_Cell_3*>(pToAdd),
			cell
		)
	);

	// deal with weights
	_weights[pToAdd->getPosition()] = cell->getRadius();
	if( cell->getRadius() < _minWeight )
		_minWeight = cell->getRadius();

	if( cell->getRadius() > _maxWeight )
		_maxWeight = cell->getRadius();

	/// init cell mesh to empty
	pToAdd->resetMesh();
	return true;
}

/// \param pSpatialable The spatialable to remove from the mesh
void Voronoi_3D_Mesh::remove(t_Cell_3* pSpatialable) {
	assert(pSpatialable);
	auto* cell = dynamic_cast<SpheroidalCell*> (pSpatialable);
	if(!cell)
		return;

	// TODO const_cast!
	Delaunay_3D_SDS::remove(pSpatialable);
	_constCellToSpheroidal.erase(
		const_cast<const t_SpatialableAgent_3*>(
			static_cast<t_SpatialableAgent_3*>(pSpatialable)
		)
	);
}

/// \param pPath The output path file
/// \param cells The list of cell to export
/// \param pDivided True if we want to create a new file for each polyhedron
/// \return return values
///					- 0 : success
///					- 1 : not implemented yet
///					- 2 : failed during export
int Voronoi_3D_Mesh::exportToFileOff(QString pPath, std::vector<SpheroidalCell*> cells, bool pDivided) {
	if(pDivided) {
		return exportToFileOff_divided(pPath, &cells);
	} else {
		return exportToFileOff_undivided(pPath, &cells);
	}
}

/// \param pPath The output path file
/// \param cells The list of cell to export
/// \param pDivided True if we want to create a new file for each polyhedron
/// \return return values
///					- 0 : success
///					- 1 : not implemented yet
///					- 2 : failed during export
int Voronoi_3D_Mesh::exportToFileSTL(QString pPath, std::vector<SpheroidalCell*> cells, bool pDivided) {
	if(pDivided)
		return exportToFileSTL_divided(pPath, &cells);
	else
		return exportToFileSTL_undivided(pPath, &cells);
}

/// \param pPath The output path file
/// \param pFormat The requested file format
/// \param pDivided True if we want to create a file for each cell
/// \return return values :
///					- 0 : success
///					- 1 : not implemented yet
///					- 2 : failed during export
int Voronoi_3D_Mesh::exportToFile(QString pPath, MeshOutFormats::outputFormat pFormat, bool pDivided) {
	assert(_delaunay.is_valid());

	switch(pFormat) {
		case MeshOutFormats::OFF :
		{
			std::vector<SpheroidalCell*> cells = generateMesh();
			std::vector<const SpheroidalCell*> constCells;
			int error = exportToFileOff(pPath, cells, pDivided);

			return error;
		}
		default :
		{
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "Exporter do not deal with this kind of format yet", "Weighted Voronoi 3D");
			return 1;
		}
	}
}

/// \param pPath The output path file
/// \param cells The list of cells to export
/// \return values :
///					- 0 : success
///					- 1 : not implemented yet
///					- 2 : failed during export
///
int Voronoi_3D_Mesh::exportToFileOff_undivided(QString pPath, std::vector<SpheroidalCell*>* cells) {
	unsigned long int nbFacets = 0;

	std::set<Point_3, comparePoint_3> points;
	std::map<Point_3, unsigned long int, comparePoint_3> indexes;

	std::vector<SpheroidalCell*>::iterator itCell;
	pPath += ".off";
	std::ofstream* voronoiOut = IO::OFF::createOffFileWithHeader(pPath.toStdString());
	Polyhedron_3::Point_iterator itPolyPts;
	for(auto const& cell : *cells) {
		// add shape points
		for(itPolyPts = cell->shape_points_begin(); itPolyPts != cell->shape_points_end(); ++itPolyPts)
			points.insert(*itPolyPts);
		nbFacets += Utils::myCGAL::getNumberOfFacets(cell->getShape());
	}

	*voronoiOut << points.size() << " " << nbFacets << " 0" << std::endl;
	IO::OFF::exportVerticesToOff(points, indexes, voronoiOut);

	// export each cell to OFF
	for(itCell = (*cells).begin(); itCell != (*cells).end(); ++itCell)
	    IO::OFF::exportPolyhedronToOff((*itCell)->getShape(), voronoiOut, indexes);

	voronoiOut->close();
	delete voronoiOut;
	return 0;
}

/// \param pPath The output path file
/// \param cells The list of cells to export
/// \return values :
///					- 0 : success
///					- 1 : not implemented yet
///					- 2 : failed during export
///
int Voronoi_3D_Mesh::exportToFileSTL_undivided(QString pPath, std::vector<SpheroidalCell*>* cells) {
	std::uint32_t nbFacets = 0;

	std::set<Point_3, comparePoint_3> points;
	std::map<Point_3, unsigned long int, comparePoint_3> indexes;

	std::vector<SpheroidalCell*>::iterator itCell;
	for(auto const& cell : *cells) {
		// add shape points
		for(auto itPolyPts = cell->shape_points_begin(); itPolyPts != cell->shape_points_end(); ++itPolyPts)
			points.insert(*itPolyPts);
		nbFacets += Utils::myCGAL::getNumberOfFacets(cell->getShape());
	}

	// write STL file
	pPath += ".stl";
	std::ofstream of(pPath.toStdString(), std::ios::trunc | std::ios::binary);

	// header, ignored but must not contain "solid"
	for(int i = 0; i < 80; ++i) of.write("*", 1);
	of.write(reinterpret_cast<char*>(&nbFacets), sizeof nbFacets);

	for(auto* cell: *cells) {
		auto const& shape = cell->getShape();
		for(auto it = shape->faces_begin(); it != shape->faces_end(); ++it) {
			auto const& points = {
				(*it).halfedge()->vertex()->point(),
				(*it).halfedge()->next()->vertex()->point(),
				(*it).halfedge()->next()->next()->vertex()->point()
			};

			float nx = 0, ny = 0, nz = 0;
			of.write(reinterpret_cast<char*>(&nx), sizeof nx);
			of.write(reinterpret_cast<char*>(&ny), sizeof ny);
			of.write(reinterpret_cast<char*>(&nz), sizeof nz);

			for(auto& point : points) {
				float x = point.x(), y = point.y(), z = point.z();

				of.write(reinterpret_cast<char*>(&x), sizeof x);
				of.write(reinterpret_cast<char*>(&y), sizeof y);
				of.write(reinterpret_cast<char*>(&z), sizeof z);
			}

			std::uint16_t zero = 0;
			of.write(reinterpret_cast<char*>(&zero), sizeof zero);
		}
	}

	return 0;
}

/// \param pPath The oputput path file
/// \param cells The list of cells to export
/// \return return values :
///					- 0 : success
///					- 1 : not implemented yet
///					- 2 : failed during export
int Voronoi_3D_Mesh::exportToFileOff_divided(QString pPath, std::vector<SpheroidalCell*>* cells) {
	std::map<Point_3, unsigned long int> indexes;
	unsigned long int iFile = 0;
	for(auto const& curCell : *cells) {
		QString fileName = pPath + "_" + QString::number(iFile);
		std::vector<SpheroidalCell*> cell;
		cell.push_back(curCell);
		if(exportToFileOff_undivided(fileName, &cell) != 0 )
			return 2;
		iFile++;
	}

	return 0;
}

/// \param pPath The oputput path file
/// \param cells The list of cells to export
/// \return return values :
///					- 0 : success
///					- 1 : not implemented yet
///					- 2 : failed during export
int Voronoi_3D_Mesh::exportToFileSTL_divided(QString pPath, std::vector<SpheroidalCell*>* cells) {
	std::map<Point_3, unsigned long int> indexes;
	unsigned long int iFile = 0;
	for(auto const& curCell : *cells) {
		QString fileName = pPath + "_" + QString::number(iFile);
		std::vector<SpheroidalCell*> cell;
		cell.push_back(curCell);
		if(exportToFileSTL_undivided(fileName, &cell) != 0 )
			return 2;
		iFile++;
	}

	return 0;
}

/// for example if a cell is at (0, 0, 0) with a radius of 10 and an other at (1, 0, 0,) with a radius of 1.
/// The second cell will be removed.
void Voronoi_3D_Mesh::removeConflicts() {
 	/// \todo : remove conflict without weights : must be done, check it

	assert(_delaunay.is_valid());
	// create shape
	std::set<Vertex_3_handle> verticesToRemove; // we will remove all vertices which aren'Gabriel

	std::ofstream id_cell_file("OutputTxt/IDCell.txt", std::ios::app);

	id_cell_file << " ";

	for(auto itEdge = _delaunay.finite_edges_begin(); itEdge != _delaunay.finite_edges_end(); ++itEdge) {
		Vertex_3_handle v1 = itEdge->first->vertex(itEdge->second);
		Vertex_3_handle v2 = itEdge->first->vertex(itEdge->third);
		double maxLength = std::max(_weights[v1->point().point()], _weights[v2->point().point()]);
		RT_3::Vertex_handle vToRemove;

		// TODO optimisation
		// if the two points are in conflict
		if(sqrt(CGAL::squared_distance(v1->point().point(), v2->point().point())) < maxLength) {
			// pick the vertex to remove
			if(_weights[v1->point().point()] < _weights[v2->point().point()]) {
				vToRemove = (REMOVE_SMALLEST_WEIGHT ? v1 : v2 );
			} else if(_weights[v1->point().point()] == _weights[v2->point().point()]) {
				// if same weights : remove the one with the bigest ID ( to insure repetability, need to have a rule )
				vToRemove = (v1->info()->getID() < v2->info()->getID()) ? v2 : v1;
			} else {
				vToRemove = (REMOVE_SMALLEST_WEIGHT ? v2 : v1 );
			}

			id_cell_file << vToRemove->info()->getID() << " ";
			verticesToRemove.insert(vToRemove);
		}
	}

	// remove conflicting vertex ( cell )
	int nbConflict = verticesToRemove.size();
	auto itVertexToRmv = verticesToRemove.begin();
	while(itVertexToRmv != verticesToRemove.end()) {
		remove(_constCellToSpheroidal[(*itVertexToRmv)->info()]);
		verticesToRemove.erase(itVertexToRmv++);
	}

	assert(_delaunay.is_valid());
	// check if conflict remain, if it is the case, do removeConflict againt
	if(nbConflict > 0)
		removeConflicts();
}

#include <CGAL/Direction_3.h>
/// \details reference all the neighbouring cell for each cell
void Voronoi_3D_Mesh::generateNeighbourhood() {
	assert(_delaunay.is_valid());
	/// by default no hidden point, CGAL triangulation by default set the weight of this kind of point to 0
	for(auto itVertex = _delaunay.finite_vertices_begin(); itVertex != _delaunay.finite_vertices_end(); ++itVertex) {
		const t_SpatialableAgent_3* currentSpa = itVertex->info();
		assert(currentSpa);
		SpheroidalCell* currentCell = _constCellToSpheroidal[currentSpa];
		assert(currentCell);
		_neighboursCell.insert(std::make_pair(currentCell, std::set<const SpheroidalCell*>()));

		// add each neighbours
		{
			std::set<const t_SpatialableAgent_3*> neighbours = getNeighbours(currentCell);
			for(auto const& neighbour : neighbours)
			{
				assert(neighbour);
				if(_constCellToSpheroidal.find(neighbour) != _constCellToSpheroidal.end()) {
					assert(_constCellToSpheroidal[neighbour]);
					_neighboursCell[currentCell].insert(const_cast<const SpheroidalCell*>(_constCellToSpheroidal[neighbour]));
				} else {
					// std::cout << " failed to find the pos : " << endl;
				}
			}
		}
	}
}

/// \return The vector corresponding to the cell structure.
std::vector<SpheroidalCell*> Voronoi_3D_Mesh::getCellsStructure()
{
	generateNeighbourhood();

	/// we will only export unhidden vertices ()
	std::vector<SpheroidalCell*> results;
	std::map<SpheroidalCell*, std::set<const SpheroidalCell*> >::iterator itCell;
	for(auto itVertex = _delaunay.finite_vertices_begin(); itVertex != _delaunay.finite_vertices_end(); ++itVertex) {
		assert(itVertex->info());
		results.push_back(_constCellToSpheroidal[itVertex->info()]);
	}

	return results;
}

std::vector<SpheroidalCell*> Voronoi_3D_Mesh::generateMesh() {
	assert(_delaunay.is_valid());
	// remove conflicting cells ( if one is included into an other one )
	// G4cout << "\n\n\n generateMesh :: Voronoi3DMesh" << G4endl;
	removeConflicts();

	QString mess = "start exporting for "  + QString::number(_delaunay.number_of_vertices() ) + " Cell(s) ";
	InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "Voronoi_3DMesh");

	std::vector<SpheroidalCell*> cells = getCellsStructure();

	// TODO ==?
	auto neighbours = static_cast<const std::map<SpheroidalCell*, std::set<const SpheroidalCell*>>>(_neighboursCell);

	// if not using thread
	if(!USE_THREAD_FOR_MESH_SUBDVN) {
		//cout << "\n\n\n not using THREAD_FOR_MESH_SUBDVN : " << endl;
		Voronoi3DCellMeshSubThread reffinement(
			0,
			getMaxNbFacetPerCell(),
			getDeltaWin(),
			&neighbours
		);
		for(auto const& cell : cells)
			reffinement.reffineCell(cell);
	} else {
		std::vector<Voronoi3DCellMeshSubThread*> reffinementThreads;
		// create the reffinement threads
		unsigned int nbThreadToCreate = (cells.size() / MIN_NB_CELL_PER_THREAD) + 1;
		{
			// create as much thread as needed
			if(nbThreadToCreate > INITIAL_MAX_THREAD)
				nbThreadToCreate = INITIAL_MAX_THREAD;

			unsigned int threadID = 0;
			while(reffinementThreads.size() < nbThreadToCreate) {
				if(VORONOI_3D_MESH_DEBUG) {
					mess = "create a new thread of ID "  + QString::number(threadID);
					InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "Voronoi_3DMesh");
				}

				reffinementThreads.push_back(
					new Voronoi3DCellMeshSubThread(
						threadID,
						getMaxNbFacetPerCell(),
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
			for(auto const& cell : cells)
			{
				//IDManager::getInstance()->setID(this);
				// itCell->forceID(iCell)
				reffinementThreads[iCell%nbThreadToCreate]->addCell(cell);
				iCell++;
			}
		}

		// run threads.
		for(auto* reffinementThread : reffinementThreads)
			reffinementThread->start(MESHING_THREAD_PRIORITY);

		/// wait until all threads process
		for(auto* reffinementThread : reffinementThreads)
			reffinementThread->wait();

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

bool Voronoi_3D_Mesh::isValid() {
	/// \todo
	return true;
}

std::set<t_Cell_3*> Voronoi_3D_Mesh::getCells() const {
	std::set<t_Cell_3*> cells;
	for(auto const& cell : _constCellToSpheroidal) {
		assert(cell.second);
		cells.insert(cell.second);
	}
	return cells;
}

std::set<t_Cell_3*> Voronoi_3D_Mesh::getCellsWithShape() const {
	std::set<t_Cell_3*> cells;
	std::map<const t_SpatialableAgent_3*, SpheroidalCell*>::const_iterator itCell;
	for(auto const& cell : _constCellToSpheroidal) {
		assert(cell.second);
		if(cell.second->hasMesh())
			cells.insert(cell.second);
	}
	return cells;
}
