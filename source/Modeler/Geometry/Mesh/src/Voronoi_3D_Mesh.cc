/*----------------------
Copyright (C): Henri Payno, Axel Delsol,
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "Voronoi_3D_Mesh.hh"

#include "Agent.hh"
#include "CellMesh.hh"
#include "CGAL_Utils.hh"
#include "EngineSettings.hh"
#include "File_Utils.hh"
#include "File_Utils_OFF.hh"
#include "Round_Shape.hh"
#include "Voronoi3DCellMeshSubThread.hh"

#include "Spatialable.hh"

#include <CGAL/Polyhedron_3.h>
#include <CGAL/convex_hull_3.h>

#include <string>
#include <fstream>

#ifndef NDEBUG
	#define VORONOI_3D_MESH_DEBUG 0
#else
	#define VORONOI_3D_MESH_DEBUG 0	// must always stay at 0
#endif

using namespace std;
#undef max

#include <iostream>
#include <string>
#include <cstdlib>
#include <string>


//////////////////////////////////////////////////////////////////////////////
/// \param pInitSpatialables The set of spatialable to include on the mesh.
/// \param pMaxNbFacet The maximal number of facet a cell must contained
/// \param delta The minimal value for which we continu to reffine
//////////////////////////////////////////////////////////////////////////////
Voronoi_3D_Mesh::Voronoi_3D_Mesh(
	unsigned int pMaxNbFacet,
	double delta,
	set<t_Cell_3* > pInitSpatialables ):
	Mesh<double, Point_3, Vector_3>(MeshTypes::Weighted_Voronoi_Tesselation),
	Delaunay_3D_SDS("Voronoi_based_Mesh_3D"),
	maxNumberOfFacetPerCell(pMaxNbFacet),
	deltaGain(delta)
{
	minWeight = numeric_limits<double>::max();
	maxWeight = -1;
	set<t_Cell_3* >::const_iterator itS;
	for(itS = pInitSpatialables.begin(); itS != pInitSpatialables.end(); ++itS)
	{
		add(*itS);
	}
}

//////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////
Voronoi_3D_Mesh::~Voronoi_3D_Mesh()
{
	clean();
}

//////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////
K::Iso_cuboid_3 Voronoi_3D_Mesh::getBoundingBox() const
{
	// create the Bounding box.
	vector<Point_3> pts;
	RT_3::Finite_vertices_iterator itVertex;
	for(itVertex = delaunay.finite_vertices_begin(); itVertex != delaunay.finite_vertices_end(); ++itVertex)
	{
		pts.push_back(itVertex->point().point());
	}
	K::Iso_cuboid_3 bb = CGAL::bounding_box( pts.begin(), pts.end());
	// add the potential next point from the voronoi cell and sphere shape
	return K::Iso_cuboid_3( Point_3(bb.xmin()-2*maxWeight, bb.ymin()-2*maxWeight, bb.zmin()-2*maxWeight),
						 	Point_3(bb.xmax()+2*maxWeight, bb.ymax()+2*maxWeight, bb.zmax()+2*maxWeight) );	// for now weight correspond to the cell radius.
																											// * 2 because is own radius and the radius of bounding point
}

///////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////
void Voronoi_3D_Mesh::clean()
{
	Delaunay_3D_SDS::clean();
	neighboursCell.clear();
}

//////////////////////////////////////////////////////////////////////////////
/// \brief the spatialable to add on the mesh
/// \warning :  The weight corresponf to the cell diameter because of the CGAL implementation.
//////////////////////////////////////////////////////////////////////////////
bool Voronoi_3D_Mesh::add(t_Cell_3* pToAdd)
{

	assert(pToAdd);
	assert(CGAL::is_finite(pToAdd->getPosition().x()));
	assert(CGAL::is_finite(pToAdd->getPosition().y()));
	assert(CGAL::is_finite(pToAdd->getPosition().z()));

	SpheroidalCell* cell = dynamic_cast<SpheroidalCell*> (pToAdd);

	if(!cell)
	{
		QString mess = "Unable to add the cell " + QString::number(cell->getID()) + ", none spheroidal cell";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "Voronoi_3D_Mesh");
		return false;
	}

	if(VORONOI_3D_MESH_DEBUG)
	{
		QString mess = "adding the point at " + QString::number(pToAdd->getPosition().x()) + ", " + QString::number(pToAdd->getPosition().y()) +
			" to the voronoi with weight : " + QString::number(cell->getRadius());
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "Weighted Voronoi 3D");
	}

	if(! Delaunay_3D_SDS::add(pToAdd))
	{
		return false;
	}

	mConstCellToSpheroidal.insert(std::pair<const t_Cell_3*, SpheroidalCell*>(
		const_cast<const t_Cell_3*>(pToAdd),
		cell
		));

	// deal with weights
	weights[pToAdd->getPosition()] = cell->getRadius();
	if( cell->getRadius() < minWeight )
	{
		minWeight = cell->getRadius();
	}

	if( cell->getRadius() > maxWeight )
	{
		maxWeight = cell->getRadius();
	}



	/// init cell mesh to empty
	pToAdd->resetMesh();
	return true;
}

//////////////////////////////////////////////////////////////////////////////
/// \param pSpatialable The spatialable to remove from the mesh
//////////////////////////////////////////////////////////////////////////////
void Voronoi_3D_Mesh::remove(t_Cell_3* pSpatialable)
{
	assert(pSpatialable);
	SpheroidalCell* cell = dynamic_cast<SpheroidalCell*> (pSpatialable);
	if(!cell)
	{
		return;
	}
	Delaunay_3D_SDS::remove(pSpatialable);
	mConstCellToSpheroidal.erase(const_cast<const t_SpatialableAgent_3*>(
									static_cast<t_SpatialableAgent_3*>(pSpatialable))
								);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
/// \param pPath The output path file
/// \param cells The list of cell to export
/// \param pDivided True if we want to create a nbew file for each polyhedron
/// \return return values
///					- 0 : success
///					- 1 : not implemented yet
///					- 2 : failed during export
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
int Voronoi_3D_Mesh::exportToFileOff(QString pPath, vector<SpheroidalCell*> cells, bool pDivided)
{
	if(pDivided)
	{
		return exportToFileOff_divided(pPath, &cells);
	}else
	{
		return exportToFileOff_undivided(pPath, &cells);
	}
}

//////////////////////////////////////////////////////////////////////////////
/// \param pPath The output path file
/// \param pFormat The requested file format
/// \param pDivided True if we want to create a file for each cell
/// \return return values :
///					- 0 : success
///					- 1 : not implemented yet
///					- 2 : failed during export
//////////////////////////////////////////////////////////////////////////////
int Voronoi_3D_Mesh::exportToFile(QString pPath, MeshOutFormats::outputFormat pFormat, bool pDivided)
{
	assert(delaunay.is_valid());

	switch(pFormat)
	{
		case MeshOutFormats::OFF :
		{
			vector<SpheroidalCell*> cells = generateMesh();
			vector<const SpheroidalCell*> constCells;
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
/// \param pPath The output path file
/// \param cells The list of cells to export
/// \return values :
///					- 0 : success
///					- 1 : not implemented yet
///					- 2 : failed during export
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
int Voronoi_3D_Mesh::exportToFileOff_undivided(QString pPath, std::vector<SpheroidalCell*>* cells)
{
	unsigned long int nbFacets = 0;

	set<Point_3, comparePoint_3> points;
	map<Point_3, unsigned long int, comparePoint_3> indexes;

	std::vector<SpheroidalCell*>::iterator itCell;
	pPath += ".off";
	ofstream* voronoiOut = IO::OFF::createOffFileWithHeader(pPath.toStdString());
	Polyhedron_3::Point_iterator itPolyPts;
	for(itCell = (*cells).begin(); itCell != (*cells).end(); ++itCell)
	{
		// add shape points
		for(itPolyPts = (*itCell)->shape_points_begin(); itPolyPts != (*itCell)->shape_points_end(); ++itPolyPts)
		{
			points.insert(*itPolyPts);
		}
		nbFacets += Utils::myCGAL::getNumberOfFacets((*itCell)->getShape());
	}
	*voronoiOut << points.size() << " " << nbFacets << " 0" << endl;
	IO::OFF::exportVerticesToOff(points, indexes, voronoiOut);

	// export each cell to OFF
	for(itCell = (*cells).begin(); itCell != (*cells).end(); ++itCell)
	{
	    IO::OFF::exportPolyhedronToOff((*itCell)->getShape(), voronoiOut, indexes);
	}
	voronoiOut->close();
	delete voronoiOut;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
/// \param pPath Theoputput path file
/// \param cells The list of cells to export
/// \return return values :
///					- 0 : success
///					- 1 : not implemented yet
///					- 2 : failed during export
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
int Voronoi_3D_Mesh::exportToFileOff_divided(QString pPath, std::vector<SpheroidalCell*>* cells)
{
	map<Point_3, unsigned long int> indexes;
	unsigned long int iFile = 0;
	std::vector<SpheroidalCell*>::iterator itCell;
	for(itCell = (*cells).begin(); itCell != (*cells).end(); ++itCell)
	{
		QString fileName = pPath + "_" + QString::number(iFile);
		std::vector<SpheroidalCell*> cell;
		cell.push_back(*itCell);
		if(exportToFileOff_undivided(fileName, &cell) != 0 )
		{
			return 2;
		}
		iFile++;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////
/// for example if a cell is at (0, 0, 0) with a radius of 10 and an other at (1, 0, 0,) with a radius of 1.
/// The second cell will be removed.
//////////////////////////////////////////////////////////////////////////////
void Voronoi_3D_Mesh::removeConflicts()
{

 	/// \todo : remove conflict without weights : must be done, check it

	assert(delaunay.is_valid());
	// create shape
	set<Vertex_3_handle> verticesToRemove; // we will remove all vertices which aren'Gabriel

	RT_3::Finite_edges_iterator itEdge;

	ofstream id_cell_file("../../example/TXT/IDCell.txt", fstream::app);

	id_cell_file << " " ;

	for(itEdge = delaunay.finite_edges_begin(); itEdge != delaunay.finite_edges_end(); ++itEdge)
	{
		Vertex_3_handle v1 = itEdge->first->vertex(itEdge->second);
		Vertex_3_handle v2 = itEdge->first->vertex(itEdge->third);
		double maxLength = max(weights[v1->point().point()], weights[v2->point().point()]);
		RT_3::Vertex_handle vToRemove;

		// if the two points are in conflict
		if(sqrt(CGAL::squared_distance(v1->point().point(), v2->point().point())) < maxLength)
		{
			// pick the vertex to remove
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
			id_cell_file << vToRemove->info()->getID() << " ";
			verticesToRemove.insert(vToRemove);

		}
	}


	// remove conflicting vertex ( cell )
	int nbConflict = verticesToRemove.size();
	set<RT_3::Vertex_handle>::iterator itVertexToRmv = verticesToRemove.begin();
	while(itVertexToRmv != verticesToRemove.end())
	{
		remove(mConstCellToSpheroidal[(*itVertexToRmv)->info()]);
		verticesToRemove.erase(itVertexToRmv++);
	}

	assert(delaunay.is_valid());
	// check if conflict remain, if it is the case, do removeConflict againt
	if(nbConflict > 0)
	{
		removeConflicts();
	}
}

#include <CGAL/Direction_3.h>
//////////////////////////////////////////////////////////////////////////////////////////////////
/// \details reference all the neighbouring cell for each cell
//////////////////////////////////////////////////////////////////////////////////////////////////
void Voronoi_3D_Mesh::generateNeighbourhood()
{
	assert(delaunay.is_valid());
	/// by default no hidden point, CGAL triangulation by default set the weight of this kind of point to 0
	RT_3::Finite_vertices_iterator itVertex;
	for(itVertex = delaunay.finite_vertices_begin(); itVertex != delaunay.finite_vertices_end(); ++itVertex)
	{
		const t_SpatialableAgent_3* currentSpa = itVertex->info();
		assert(currentSpa);
		SpheroidalCell* currentCell = mConstCellToSpheroidal[currentSpa];
		assert(currentCell);
		neighboursCell.insert( make_pair(currentCell, set<const SpheroidalCell*>() ) );

		// add each neighbours
		{
			std::set<const t_SpatialableAgent_3*> neighbours = getNeighbours(currentCell);
			std::set<const t_SpatialableAgent_3*>::iterator itSC;
			for(itSC = neighbours.begin(); itSC != neighbours.end(); ++itSC)
			{
				assert(*itSC);
				if(mConstCellToSpheroidal.find(*itSC) != mConstCellToSpheroidal.end() )
				{
					assert(mConstCellToSpheroidal[*itSC ]);
					neighboursCell[currentCell].insert( const_cast<const SpheroidalCell*> (mConstCellToSpheroidal[*itSC ]) );
				}else
				{
					// std::cout << " failed to find the pos : " << endl;
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/// \return The vector corresponding to the cell structure.
//////////////////////////////////////////////////////////////////////////////////////////////////
vector<SpheroidalCell*> Voronoi_3D_Mesh::getCellsStructure()
{
	generateNeighbourhood();

	/// we will only export unhidden vertices ()
	std::vector<SpheroidalCell*> results;
	std::map<SpheroidalCell*, std::set<const SpheroidalCell*> >::iterator itCell;
	RT_3::Finite_vertices_iterator itVertex;
	for(itVertex = delaunay.finite_vertices_begin(); itVertex != delaunay.finite_vertices_end(); ++itVertex)
	{
		assert(itVertex->info());
		results.push_back(mConstCellToSpheroidal[itVertex->info()]);
	}
	return results;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<SpheroidalCell*> Voronoi_3D_Mesh::generateMesh()
{
	assert( delaunay.is_valid() );
	// remove conflicting cells ( if one is included into an other one )
	// G4cout << "\n\n\n generateMesh :: Voronoi3DMesh" << G4endl;
	removeConflicts();

	QString mess = "start exporting for "  + QString::number(delaunay.number_of_vertices() ) + " Cell(s) ";
	InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "Voronoi_3DMesh");

	vector<SpheroidalCell*> cells = getCellsStructure();

	map<SpheroidalCell*, set<const SpheroidalCell*> > neighbours = static_cast<const map<SpheroidalCell*, set<const SpheroidalCell*> > > (neighboursCell );

	// if not using thread
	if(!USE_THREAD_FOR_MESH_SUBDVN)
	{
		//cout << "\n\n\n not using THREAD_FOR_MESH_SUBDVN : " << endl;
		Voronoi3DCellMeshSubThread reffinement(	0,
												getMaxNbFacetPerCell(),
												getDeltaWin(),
												&neighbours);
		for(vector<SpheroidalCell*>::iterator itCell = cells.begin(); itCell != cells.end(); ++itCell)
		{
			reffinement.reffineCell(*itCell);
		}
	}else
	{
		//cout << "\n\n\n using THREAD_FOR_MESH_SUBDVN : " << endl;
		vector<Voronoi3DCellMeshSubThread*> reffinementThreads;
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
				if(VORONOI_3D_MESH_DEBUG)
				{
					mess = "create a new thread of ID "  + QString::number(threadID);
					InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "Voronoi_3DMesh");
				}

				reffinementThreads.push_back(	new Voronoi3DCellMeshSubThread(	threadID,
																				getMaxNbFacetPerCell(),
																				getDeltaWin(),
																				&neighbours
																				));

				threadID++;
			}
		}

		// add cells to threads
		{
			vector<SpheroidalCell*>::iterator itCell;
			int iCell = 0;
			for(itCell = cells.begin(); itCell != cells.end(); ++itCell)
			{
				//IDManager::getInstance()->setID(this);
				// itCell->forceID(iCell)
				reffinementThreads[iCell%nbThreadToCreate]->addCell(*itCell);
				iCell++;
			}
		}

		// run threads.
		vector<Voronoi3DCellMeshSubThread*>::iterator itThread;
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
bool Voronoi_3D_Mesh::isValid()
{
	/// \todo
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
set<t_Cell_3*> Voronoi_3D_Mesh::getCells() const
{
	set<t_Cell_3*> cells;
	std::map<const t_SpatialableAgent_3*, SpheroidalCell*>::const_iterator itCell;
	for(itCell = mConstCellToSpheroidal.begin(); itCell!= mConstCellToSpheroidal.end(); ++itCell)
	{
		assert(itCell->second);
		cells.insert(itCell->second);
	}
	return cells;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
std::set<t_Cell_3*> Voronoi_3D_Mesh::getCellsWithShape() const
{
	set<t_Cell_3*> cells;
	std::map<const t_SpatialableAgent_3*, SpheroidalCell*>::const_iterator itCell;
	for(itCell = mConstCellToSpheroidal.begin(); itCell!= mConstCellToSpheroidal.end(); ++itCell)
	{
		assert(itCell->second);
		if(itCell->second->hasMesh())
		{
			cells.insert(itCell->second);
		}
	}
	return cells;
}
