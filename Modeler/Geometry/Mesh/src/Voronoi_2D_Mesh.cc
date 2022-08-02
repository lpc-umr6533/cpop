/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "Voronoi_2D_Mesh.hh"

#include "CellMeshSettings.hh"
#include "EngineSettings.hh"
#include "File_Utils.hh"
#include "File_Utils_OFF.hh"
#include "Voronoi2DCellMeshSubThread.hh"
// export file
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

using namespace std;
//////////////////////////////////////////////////////////////////////////////
/// \param pMaxNbSegs The maximl number of segment the mesh of a cell should contain
/// \param pDelta the threshold for mesh subdivision
/// \param pInitSpatialables The set of spatialable to include on the mesh.
//////////////////////////////////////////////////////////////////////////////
Voronoi_2D_Mesh::Voronoi_2D_Mesh(
	unsigned int pMaxNbSegs,
	double pDelta,
	std::set<t_Cell_2*> pInitSpatialables):
	Mesh<double, Point_2, Vector_2>(MeshTypes::Weighted_Voronoi_Tesselation),
	Delaunay_2D_SDS("Voronoi_based_Mesh_2D"),
	maxNumberOfSegPerCell(pMaxNbSegs),
	deltaGain(pDelta)
{
	minWeight = std::numeric_limits<double>::max();
	std::set<t_Cell_2*>::const_iterator itS;
	for(itS = pInitSpatialables.begin(); itS != pInitSpatialables.end(); ++itS)
	{
		add(*itS);
	}
}

//////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////
Voronoi_2D_Mesh::~Voronoi_2D_Mesh()
{
	clean();
}

//////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////
void Voronoi_2D_Mesh::clean()
{
	// neighboursCell.clear();
}

//////////////////////////////////////////////////////////////////////////////
/// \param pToAdd The spatialable to add to the mesh.
/// \return success or fail
//////////////////////////////////////////////////////////////////////////////
bool Voronoi_2D_Mesh::add(t_Cell_2* pToAdd)
{
	assert(pToAdd);
	assert(CGAL::is_finite(pToAdd->getPosition().x()));
	assert(CGAL::is_finite(pToAdd->getPosition().y()));

	/// complete the map
	DiscoidalCell* cell = dynamic_cast<DiscoidalCell*> (pToAdd);
	if(!cell)
	{
		QString mess = "Unable to add the cell " + QString::number(cell->getID()) + ", none discoidal cell";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "Voronoi_2D_Mesh");
		return false;
	}

	/// TODO : must be removed ?
	{
		std::vector<RT_2::Face_handle > conflictCells;
		std::vector<RT_2::Vertex_handle > hiddenVertices;
		Weighted_point_2 p(Weighted_point_2(pToAdd->getPosition(), cell->getRadius()));
		if(delaunayTriangulation.number_of_vertices() > 3)
		{
			delaunayTriangulation.get_conflicts_and_hidden_vertices(p, std::back_inserter(conflictCells), std::back_inserter(hiddenVertices));
		}
		
		if(hiddenVertices.size() > 0 )
		{
			// Do not add it besause in is in conflict
			return true;
		}
	}
	
	if(! Delaunay_2D_SDS::add(pToAdd))
	{
		return false;
	}

	mConstCellToDiscoidal.insert(std::pair<const t_Cell_2*, DiscoidalCell*>(
		const_cast<const t_Cell_2*>(pToAdd),
		cell
		));
	
	// deal with weights	
	weights.insert(std::pair<Point_2, double>(pToAdd->getPosition(),  cell->getRadius()));
	
	if( cell->getRadius() < minWeight )
	{
		minWeight = cell->getRadius();
	}
	pToAdd->resetMesh();
	return true;
}

//////////////////////////////////////////////////////////////////////////////
/// \param pSpatialable The spatialable to remove from the mesh
//////////////////////////////////////////////////////////////////////////////
void Voronoi_2D_Mesh::remove(t_Cell_2* pSpatialable)
{
	assert(pSpatialable);
	DiscoidalCell* cell = dynamic_cast<DiscoidalCell*> (pSpatialable);
	if(!cell)
	{
		return;
	}
	Delaunay_2D_SDS::remove(pSpatialable);
}

//////////////////////////////////////////////////////////////////////////////
/// \param pPath The output path file
/// \param pFormat The requested file format
/// \param pDivided True if we want to divide the mesh
/// \return return values : 
///					- 0 : success
///					- 1 : not implemented yet
///					- 2 : failed during export
//////////////////////////////////////////////////////////////////////////////
int Voronoi_2D_Mesh::exportToFile(QString pPath, MeshOutFormats::outputFormat pFormat, bool pDivided) 
{
	switch(pFormat)
	{
		case MeshOutFormats::OFF :
		{
			return exportToFileOff(pPath, pDivided);
		}
		default:
		{
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "Exporter do not deal with this kind of format yet", "Weighted Voronoi 2D");
			return 1;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
/// \return the bounding box containing all cellMesh
//////////////////////////////////////////////////////////////////////////////
K::Iso_rectangle_2 Voronoi_2D_Mesh::getBoundingBox() const
{
 	// create the Bounding box.
	std::vector<Point_2> pts;
	RT_2::Finite_vertices_iterator itVertex;
	for(itVertex = delaunayTriangulation.finite_vertices_begin(); itVertex != delaunayTriangulation.finite_vertices_end(); ++itVertex)
	{
		Point_2 mypt = (itVertex->point()).point();
		pts.push_back(mypt);
	}
	Iso_rectangle_2 bb = CGAL::bounding_box( pts.begin(), pts.end());
	// add the potential next point from the voronoi cell and disc shape
	return Iso_rectangle_2( Point_2(bb.xmin()-2.*minWeight, bb.ymin()-2.*minWeight),
							Point_2(bb.xmax()+2.*minWeight, bb.ymax()+2.*minWeight) );
}

///////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////
void Voronoi_2D_Mesh::removeConflicts()
{
	/// TODO : redo it better
	assert(delaunayTriangulation.is_valid());
	// create shape 
	std::set<RT_2::Vertex_handle> verticesToRemove; // we will remove all vertices which aren'Gabriel

	/// check directly vertices
	RT_2::Finite_edges_iterator itEdge;
	for(itEdge = delaunayTriangulation.finite_edges_begin(); itEdge != delaunayTriangulation.finite_edges_end(); ++itEdge)
	{
		RT_2::Vertex_handle v1 = itEdge->first->vertex(RT_2::ccw(itEdge->second));
		RT_2::Vertex_handle v2 = itEdge->first->vertex(RT_2::cw(itEdge->second));
		double maxLength = max(weights[v1->point().point()], weights[v2->point().point()]);
		RT_2::Vertex_handle vToRemove;

		if(weights[v1->point().point()] < weights[v2->point().point()]) 
		{
			vToRemove = (REMOVE_SMALLEST_WEIGHT ? v1 : v2 );
		}else
		if(weights[v1->point().point()] == weights[v2->point().point()])
		{
			// if same weights : remove the one with the bigest ID ( to insure repetability, need to have a rule )
			vToRemove = (v1->info()->getID() < v2->info()->getID()) ? v2 : v1;
		}else
		{
			vToRemove = (REMOVE_SMALLEST_WEIGHT ? v2 : v1 );
		}

		if(sqrt(CGAL::squared_distance(v1->point().point(), v2->point().point())) < maxLength)
		{
			verticesToRemove.insert(vToRemove);
		}
	}

	int nbConflict = verticesToRemove.size();

	std::set<RT_2::Vertex_handle>::iterator itVertexToRmv = verticesToRemove.begin();
	while(itVertexToRmv != verticesToRemove.end())
	{
		const t_SpatialableAgent_2* tmp = (*itVertexToRmv)->info();
		remove( mConstCellToDiscoidal[tmp]);
		verticesToRemove.erase(itVertexToRmv++);
	}

	assert(delaunayTriangulation.is_valid());
	if(nbConflict > 0)
	{
		removeConflicts();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////////////
void Voronoi_2D_Mesh::generateNeighbourhood()
{

	assert(delaunayTriangulation.is_valid());
	/// by default no hidden point, CGAL triangulation by default set the weight of this kind of point to 0
	RT_2::Finite_vertices_iterator itVertex;
	for(itVertex = delaunayTriangulation.finite_vertices_begin(); itVertex != delaunayTriangulation.finite_vertices_end(); ++itVertex)
	{
		const t_SpatialableAgent_2* currentSpa = itVertex->info();
		assert(currentSpa);
		DiscoidalCell* currentCell = mConstCellToDiscoidal[currentSpa];
		assert(currentCell);
		neighboursCell.insert( make_pair(currentCell, set<const DiscoidalCell*>() ) );

		// add each neighbours
		{
			std::set<const t_SpatialableAgent_2*> neighbours = getNeighbours(currentCell);
			std::set<const t_SpatialableAgent_2*>::iterator itDC;
			for(itDC = neighbours.begin(); itDC != neighbours.end(); ++itDC)
			{
				assert(*itDC);
				if(mConstCellToDiscoidal.find(*itDC) != mConstCellToDiscoidal.end() )
				{
					assert(mConstCellToDiscoidal[*itDC ]);
					neighboursCell[currentCell].insert( const_cast<const DiscoidalCell*> (mConstCellToDiscoidal[*itDC ]) );
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/// \return  The vector corresponding to the cell structure.
//////////////////////////////////////////////////////////////////////////////////////////////////
vector<DiscoidalCell*> Voronoi_2D_Mesh::getCellsStructure()
{
	generateNeighbourhood();
	/// we will only export unhidden vertices
	std::vector<DiscoidalCell*> results;
	RT_2::Finite_vertices_iterator itVertex;
	for(itVertex = delaunayTriangulation.finite_vertices_begin(); itVertex != delaunayTriangulation.finite_vertices_end(); ++itVertex)
	{
		if(!itVertex->is_hidden()) 
		{
			assert(itVertex->info());
			results.push_back(( mConstCellToDiscoidal[itVertex->info()]));
		}
	}
	return results;
}

//////////////////////////////////////////////////////////////////////////////
/// \return the cells containg a mesh generate by the process
//////////////////////////////////////////////////////////////////////////////
std::vector<DiscoidalCell*> Voronoi_2D_Mesh::generateMesh()
{
	assert(delaunayTriangulation.is_valid());

	removeConflicts();

	QString mess = "start exporting for "  + QString::number(delaunayTriangulation.number_of_vertices() ) + " Cell(s) "; 
	InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "Vornoi_2DMesh");

	vector<DiscoidalCell*> cells = getCellsStructure();
	
	std::map<DiscoidalCell*, set<const DiscoidalCell*> > neighbours = static_cast<const map<DiscoidalCell*, set<const DiscoidalCell*> > > (neighboursCell );
	
	if(!USE_THREAD_FOR_MESH_SUBDVN)
	{
		Voronoi2DCellMeshSubThread reffinement(	0, 
												getMaxNbSegPerCell(),
												getDeltaWin(),
												&neighbours);
		for(vector<DiscoidalCell*>::iterator itCell = cells.begin(); itCell != cells.end(); ++itCell)
		{
			reffinement.reffineCell(*itCell);
		}
	}else
	{
		vector<Voronoi2DCellMeshSubThread*> reffinementThreads;
		// create the reffinement threads
		unsigned int nbThreadToCreate = (cells.size() / MIN_NB_CELL_PER_THREAD) + 1 ;
		{
			// create as much thread as needed
			if(nbThreadToCreate > INITIAL_MAX_THREAD)
			{
				nbThreadToCreate = INITIAL_MAX_THREAD;
			}

			unsigned int threadID = 0;
			while(reffinementThreads.size() < nbThreadToCreate)
			{
				if(VORONOI_2D_MESH_DEBUG)
				{
					mess = "create a new thread of ID "  + QString::number(threadID);
					InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "Voronoi_2DMesh");
				}

					reffinementThreads.push_back(	new Voronoi2DCellMeshSubThread(	threadID,
																					getMaxNbSegPerCell(),
																					getDeltaWin(),
																					&neighbours
																					));		
				threadID++;
			}
		}

		// add cells to threads
		{
			vector<DiscoidalCell*>::iterator itCell;
			int iCell = 0;
			for(itCell = cells.begin(); itCell != cells.end(); ++itCell)
			{
				reffinementThreads[iCell%nbThreadToCreate]->addCell(*itCell);
				iCell++;
			}
		}		

		// run threads.
		vector<Voronoi2DCellMeshSubThread*>::iterator itThread;
		for(itThread = reffinementThreads.begin(); itThread != reffinementThreads.end(); ++itThread)
		{
			(*itThread)->start(MESHING_THREAD_PRIORITY);
		}

		/// wait until all threads process
		for(itThread = reffinementThreads.begin(); itThread != reffinementThreads.end(); ++itThread)
		{
			(*itThread)->wait();
		}

		//delete reffinement thread
		for( itThread = reffinementThreads.begin(); itThread != reffinementThreads.end(); ++itThread)
		{
			delete *itThread;
			*itThread = NULL;
		}
	}
	neighboursCell.clear();
	return cells;
}

//////////////////////////////////////////////////////////////////////////////
/// \param pPath Theoputput path file
/// \param pDivided True if we want to create a new file .off for each poly
/// \return return values : 
///					- 0 : success
///					- 1 : not implemented yet
///					- 2 : failed during export
//////////////////////////////////////////////////////////////////////////////
#include <CGAL/Arrangement_2.h>
#include <CGAL/Direction_2.h>
int Voronoi_2D_Mesh::exportToFileOff(QString pPath, bool pDivided ) 
{
	std::vector<DiscoidalCell*> cells = generateMesh();
	int error = 0;
	if(pDivided)
	{
		error = exportToFileOff_divided(pPath, &cells);
	}else
	{
		error = exportToFileOff_undivided(pPath, &cells);
	}

	return error;	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
/// \param pPath Theoputput path file
/// \param cells The list of cell to export
/// \return return values : 
///					- 0 : success
///					- 1 : not implemented yet
///					- 2 : failed during export
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
int Voronoi_2D_Mesh::exportToFileOff_divided(QString pPath, std::vector<DiscoidalCell*>* cells)
{
	map<Point_3, unsigned long int> indexes;
	unsigned long int iFile = 0;
	std::vector<DiscoidalCell*>::iterator itCell;
	for(itCell = (*cells).begin(); itCell != (*cells).end(); ++itCell)
	{
		QString fileName = pPath + "_" + QString::number(iFile);
		std::vector<DiscoidalCell*> cell;
		cell.push_back(*itCell);
		if(exportToFileOff_undivided(fileName, &cell) != 0 )
		{
			return 2;
		}
		iFile++;
	}
	return 0;
}	

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
/// \param pPath Theoputput path file
/// \param cells The list of cell to export
/// \return return values : 
///					- 0 : success
///					- 1 : not implemented yet
///					- 2 : failed during export
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
int Voronoi_2D_Mesh::exportToFileOff_undivided(QString pPath, std::vector<DiscoidalCell*>* cells)
{
	map<Point_2, unsigned long int> indexes;
	ofstream* voronoiOut = NULL;

	std::vector<DiscoidalCell*>::iterator itCell;
	pPath += ".off";
	voronoiOut = IO::OFF::createOffFileWithHeader(pPath.toStdString());

	/// export voronoi points
	set<Point_2> points;
	std::vector<Segment_2>::iterator itPolyPts;
	for(itCell = (*cells).begin(); itCell != (*cells).end(); ++itCell)
	{
		// add shape points
		for(itPolyPts = (*itCell)->shape_begin(); itPolyPts != (*itCell)->shape_end(); ++itPolyPts)
		{
			points.insert(itPolyPts->source());
		}
	}
	*voronoiOut << points.size() << " " << cells->size() << " 0" << endl;
	IO::OFF::exportVerticesToOff(points, indexes, voronoiOut);

	// export polyhedrons
	for(itCell = (*cells).begin(); itCell != (*cells).end(); ++itCell)
	{
		*voronoiOut << (*itCell)->shape_size();
		for(itPolyPts = (*itCell)->shape_begin(); itPolyPts != (*itCell)->shape_end(); ++itPolyPts)
		{
			*voronoiOut << " " << indexes[itPolyPts->source()];
		}
		*voronoiOut << " " << IO::exportColor((*itCell)->getColor()).toStdString() << std::endl; /// add a random color to each voronoi cell
	}
	voronoiOut->close();
	delete voronoiOut;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
bool Voronoi_2D_Mesh::isValid()
{
	/// \todo
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
set<t_Cell_2*> Voronoi_2D_Mesh::getCells() const
{
	set<t_Cell_2*> cells;
	std::map<const t_SpatialableAgent_2*, DiscoidalCell*>::const_iterator itCell;
	for(itCell = mConstCellToDiscoidal.begin(); itCell!= mConstCellToDiscoidal.end(); ++itCell)
	{
		assert(itCell->second);
		cells.insert(itCell->second);
	}
	return cells;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
std::set<t_Cell_2*> Voronoi_2D_Mesh::getCellsWithShape() const
{
	set<t_Cell_2*> cells;
	std::map<const t_SpatialableAgent_2*, DiscoidalCell*>::const_iterator itCell;
	for(itCell = mConstCellToDiscoidal.begin(); itCell!= mConstCellToDiscoidal.end(); ++itCell)
	{
		assert(itCell->second);
		if(itCell->second->hasMesh())
		{
			cells.insert(itCell->second);
		}
	}
	return cells;
}
