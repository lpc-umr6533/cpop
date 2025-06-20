#include "DiscoidalCellMesh.hh"
#include "CellMeshSettings.hh"
#include "CGAL_Utils.hh"
#include "EngineSettings.hh"
#include "File_Utils.hh"
#include "File_Utils_OFF.hh"
#include "DiscoidalCell_MeshSub_Thread.hh"

#ifndef NDEBUG
	#define DISCOIDAL_CELL_MESH_DEBUG 0
#else
	#define DISCOIDAL_CELL_MESH_DEBUG 0	// must always stay at 0
#endif
#if ( defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) )
	#undef max
#endif

/// \param nbFacetPerCell 	The threshold number to stop membrane mesh subdivision
/// \param delta 			The minimal value for which we continu to reffine
/// \param pCells 			The set of cell to include on the mesh.
DiscoidalCellMesh::DiscoidalCellMesh(	unsigned int nbFacetPerCell,
	double delta,
	std::set<t_Cell_2* > pCells
):
	Voronoi_2D_Mesh(nbFacetPerCell, delta, pCells)
{
}

DiscoidalCellMesh::~DiscoidalCellMesh() {
	clean();
}

/// \param pPath 	the output file path
/// \param pFormat 	the output format of the mesh
/// \param pDivided	True if we want to create one file per cell
int DiscoidalCellMesh::exportToFile(std::string const& path, MeshOutFormats::outputFormat format, bool divided) {
	if(format == MeshOutFormats::OFF) {
		return exportToFileOff(path, divided);
	} else {
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "Exporter do not deal with this kind of format yet", "DiscoidalCellMesh");
		return 1;
	}
}

/// \param pPath the output file path
/// \param cells The cells to export
/// \return 0 if success
int DiscoidalCellMesh::exportToFileOff_undivided(std::string const& path, DiscoidalCells const& cells) { /// TODO : passer en const
	std::map<Point_2, unsigned long int> indexes;

	/// create the .off file to set the mesh
	std::string fullPath = path + ".off";
	std::ofstream* voronoiOut = IO::OFF::createOffFileWithHeader(fullPath);

	// deal with cell
	std::set<Point_2> points;
	for(auto const* cell : cells) {
		// add membrane shape points
		for(auto itPolyPts = cell->shape_begin(); itPolyPts != cell->shape_end(); ++itPolyPts)
			points.insert(itPolyPts->source());

		// add nucleus shape points
		auto lNuclei = cell->getNuclei();
		for(auto const* nucleus : lNuclei) {
			auto const& lCellNucleusPoints = nucleus->getShapePoints();
			points.insert(lCellNucleusPoints.begin(), lCellNucleusPoints.end());
		}
	}

	// deal with markup points
	unsigned int nbSegForMarkupPts = 0;
	std::vector<std::pair<CGAL::Color, std::vector<Point_2>>> rectangles;
	{
		// convert all of them to rectangles
		for(auto const& markupPoint : markupPoints) {
			auto const& trianglePoints = Utils::myCGAL::convertPointToBox(markupPoint.first, markupPoint.second.second);
			rectangles.emplace_back(markupPoint.second.first, trianglePoints);
			points.insert(trianglePoints.begin(), trianglePoints.end());
			nbSegForMarkupPts += trianglePoints.size();
		}
	}

	*voronoiOut << points.size() << " " << rectangles.size() + cells.size() * 2 << " 0" << std::endl;	// *2 because we also include cell nucleus
	IO::OFF::exportVerticesToOff(points, indexes, voronoiOut);

	// export cell shapes
	for(auto const* cell : cells) {
		// define membrane shape
		*voronoiOut << cell->shape_size();
		for(auto itPolyPts = cell->shape_begin(); itPolyPts != cell->shape_end(); ++itPolyPts)
			*voronoiOut << " " << indexes[itPolyPts->source()];

		*voronoiOut << " " << IO::exportColor(cell->getColor()).toStdString() << std::endl; /// add a random color to each voronoi cell

		// define cell nucleus
		std::vector<t_Nucleus_2*> lNuclei = cell->getNuclei();
		std::vector<t_Nucleus_2*>::iterator itNucleus;
		for(auto const& nucleus : lNuclei) {
			std::vector<Point_2> lCellNucleusPoints = nucleus->getShapePoints();
			*voronoiOut << std::distance(lCellNucleusPoints.begin(), lCellNucleusPoints.end());
			for(auto & lCellNucleusPoint : lCellNucleusPoints)
				*voronoiOut << " " << indexes[lCellNucleusPoint];
		}
		*voronoiOut << " " << IO::exportColor(cell->getColor()).toStdString() << std::endl; /// add a random color to each voronoi cell
	}

	// export markup point shape
	for(auto const& rectangle : rectangles) {
		*voronoiOut << 4;
		for(auto markPt : rectangle.second)
			*voronoiOut << " " << indexes[markPt];
		*voronoiOut << " " << IO::exportColor(rectangle.first).toStdString() << std::endl;
	}

	voronoiOut->close();
	delete voronoiOut;
 	return 0;
}

void DiscoidalCellMesh::clean() {
	Voronoi_2D_Mesh::clean();
}

/// \return the cells for which we generate a mesh
std::vector<DiscoidalCell*> DiscoidalCellMesh::generateMesh() {
	assert(_delaunayTriangulation.is_valid());

	removeConflicts();
	auto const& cells = getCellsStructure();

	// TODO ==?
	auto const neighbours = static_cast<const std::map<DiscoidalCell*, std::set<const DiscoidalCell*>>>(_neighboursCell);

	if(!USE_THREAD_FOR_MESH_SUBDVN) {
		DiscoidalCellMeshSubThread reffinement(
			0,
			getMaxNbSegPerCell(),
			getDeltaWin(),
			&neighbours,
			MAX_RATIO_NUCLEUS_TO_CELL
		);
		for(auto const& cell : cells)
			reffinement.reffineCell(cell);
	} else {
		std::vector<DiscoidalCellMeshSubThread*> reffinementThreads;

		// create the reffinement threads
		unsigned int nbThreadToCreate = (cells.size() / MIN_NB_CELL_PER_THREAD) + 1;
		{
			// create as much thread as needed
			if(nbThreadToCreate > INITIAL_MAX_THREAD)
				nbThreadToCreate = INITIAL_MAX_THREAD;

			unsigned int threadID = 0;
			while(reffinementThreads.size() < nbThreadToCreate) {
				if(DISCOIDAL_CELL_MESH_DEBUG) {
					std::string mess = "create a new thread of ID "  + std::to_string(threadID);
					InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess, "DiscoidalCellMesh");
				}

				reffinementThreads.push_back(
					new DiscoidalCellMeshSubThread(
						threadID,
						getMaxNbSegPerCell(),
						getDeltaWin(),
						&neighbours,
						MAX_RATIO_NUCLEUS_TO_CELL
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
