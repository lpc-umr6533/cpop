/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "DiscoidalCellMesh.hh"

#include "CellMeshSettings.hh"
#include "CGAL_Utils.hh"
#include "EngineSettings.hh"
#include "Geometry_Utils_Circle.hh"
#include "File_Utils.hh"
#include "File_Utils_OFF.hh"
#include "File_Utils_TXT.hh"
#include "Round_Shape.hh"
#include "DiscoidalCell_MeshSub_Thread.hh"


using namespace std;

#ifndef NDEBUG
	#define DISCOIDAL_CELL_MESH_DEBUG 0
#else
	#define DISCOIDAL_CELL_MESH_DEBUG 0	// must always stay at 0
#endif
#if ( defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) )
	#undef max
#endif

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////
/// \param nbFacetPerCell 	The threshold number to stop membrane mesh subdivision
/// \param delta 			The minimal value for which we continu to reffine
/// \param pCells 			The set of cell to include on the mesh.
//////////////////////////////////////////////////////////////////////////////////////////////
DiscoidalCellMesh::DiscoidalCellMesh(	unsigned int nbFacetPerCell, 
										double delta, 
										set<t_Cell_2* > pCells) :
	Voronoi_2D_Mesh(nbFacetPerCell, delta, pCells),
	CellMesh()
{
	
}

//////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////
DiscoidalCellMesh::~DiscoidalCellMesh()
{
	clean();
}

//////////////////////////////////////////////////////////////////////////////////
/// \param pPath 	the output file path
/// \param pFormat 	the output format of the mesh
/// \param pDivided	True if we want to create one file per cell
//////////////////////////////////////////////////////////////////////////////////
int DiscoidalCellMesh::exportToFile(QString pPath, MeshOutFormats::outputFormat pFormat, bool pDivided)
{
	if(pFormat == MeshOutFormats::OFF)
	{
		return exportToFileOff(pPath, pDivided);
	}else
	{
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "Exporter do not deal with this kind of format yet", "DiscoidalCellMesh");
		return 1;
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////
/// \param pPath the output file path
/// \param cells The cells to export 
/// \return 0 if success
//////////////////////////////////////////////////////////////////////////////////
int DiscoidalCellMesh::exportToFileOff_undivided(QString pPath, vector<DiscoidalCell*>* cells)	/// TODO : passer en const
{
	map<Point_2, unsigned long int> indexes;

	/// create the .off file to set the mesh
	pPath += ".off";
	ofstream* voronoiOut = IO::OFF::createOffFileWithHeader(pPath.toStdString());

	// deal with cell
	set<Point_2> points;
	vector<Segment_2>::iterator itPolyPts;
	vector<DiscoidalCell*>::iterator itCell;
	for(itCell = (*cells).begin(); itCell != (*cells).end(); ++itCell)
	{
		// add membrane shape points
		for(itPolyPts = (*itCell)->shape_begin(); itPolyPts != (*itCell)->shape_end(); ++itPolyPts)
		{
			points.insert(itPolyPts->source());
		}

		// add nucleus shape points 
		vector<t_Nucleus_2*> lNuclei = (*itCell)->getNuclei();
		vector<t_Nucleus_2*>::iterator itNucleus;
		for(itNucleus = lNuclei.begin(); itNucleus != lNuclei.end(); ++itNucleus)
		{
			vector<Point_2> lCellNucleusPoints = (*itNucleus)->getShapePoints();
			points.insert(lCellNucleusPoints.begin(), lCellNucleusPoints.end());
		}
	}

	// deal with markup points
	unsigned int nbSegForMarkupPts = 0;
	vector<pair<CGAL::Color, vector<Point_2> > > rectangles;
	{
		// convert all of them to rectangles
		map<Point_2, pair<CGAL::Color, double> >::const_iterator itMarkPoint;
		for( itMarkPoint = markupPoints.begin(); itMarkPoint != markupPoints.end(); ++itMarkPoint)
		{
			vector<Point_2> trianglePoints = Utils::myCGAL::convertPointToBox(itMarkPoint->first, itMarkPoint->second.second);
			rectangles.push_back(make_pair(itMarkPoint->second.first, trianglePoints));
			points.insert( trianglePoints.begin(), trianglePoints.end());
			nbSegForMarkupPts += trianglePoints.size();
		}
	}

	*voronoiOut << points.size() << " " << rectangles.size() + cells->size() * 2 << " 0" << endl;	// *2 because we also include cell nucleus
	IO::OFF::exportVerticesToOff(points, indexes, voronoiOut);

	// export cell shapes
	for(itCell = (*cells).begin(); itCell != (*cells).end(); ++itCell)
	{
		// define membrane shape
		*voronoiOut << (*itCell)->shape_size();
		for(itPolyPts = (*itCell)->shape_begin(); itPolyPts != (*itCell)->shape_end(); ++itPolyPts)
		{
			*voronoiOut << " " << indexes[itPolyPts->source()];
		}
		*voronoiOut << " " << IO::exportColor((*itCell)->getColor()).toStdString() << endl; /// add a random color to each voronoi cell
		
		// define cell nucleus
		vector<t_Nucleus_2*> lNuclei = (*itCell)->getNuclei();
		vector<t_Nucleus_2*>::iterator itNucleus;
		for(itNucleus = lNuclei.begin(); itNucleus != lNuclei.end(); ++itNucleus)
		{
			vector<Point_2> lCellNucleusPoints = (*itNucleus)->getShapePoints();
			vector<Point_2>::iterator itPt;
			*voronoiOut << std::distance(lCellNucleusPoints.begin(), lCellNucleusPoints.end());
			for(itPt = lCellNucleusPoints.begin(); itPt != lCellNucleusPoints.end(); ++itPt)
			{
				*voronoiOut << " " << indexes[*itPt];
			}
		}
		*voronoiOut << " " << IO::exportColor((*itCell)->getColor()).toStdString() << endl; /// add a random color to each voronoi cell
	}

	// export markup point shape
	vector<pair<CGAL::Color, vector<Point_2> > >::iterator itMarkup;
	for(itMarkup = rectangles.begin(); itMarkup != rectangles.end(); ++itMarkup )
	{
		*voronoiOut << 4;
		for(vector<Point_2>::iterator itMarkPt = itMarkup->second.begin(); itMarkPt != itMarkup->second.end(); ++itMarkPt)
		{
			*voronoiOut << " " << indexes[*itMarkPt];
		}
		*voronoiOut << " " << IO::exportColor(itMarkup->first).toStdString() << endl;
	}

	voronoiOut->close();
	delete voronoiOut;
 	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////
void DiscoidalCellMesh::clean()
{
	Voronoi_2D_Mesh::clean();
}

//////////////////////////////////////////////////////////////////////////////
/// \return the cells for which we generate a mesh
//////////////////////////////////////////////////////////////////////////////
vector<DiscoidalCell*> DiscoidalCellMesh::generateMesh()
{
	assert(delaunayTriangulation.is_valid());

	removeConflicts();
	vector<DiscoidalCell*> cells = getCellsStructure();

	const map<DiscoidalCell*, set<const DiscoidalCell*> > neighbours = static_cast<const map<DiscoidalCell*, set<const DiscoidalCell*> > > (neighboursCell );

	if(!USE_THREAD_FOR_MESH_SUBDVN)
	{
		DiscoidalCellMeshSubThread reffinement(	
												0, 
												getMaxNbSegPerCell(),
												getDeltaWin(),
												&neighbours,
												MAX_RATIO_NUCLEUS_TO_CELL
												);
		for(vector<DiscoidalCell*>::iterator itCell = cells.begin(); itCell != cells.end(); ++itCell)
		{
			reffinement.reffineCell(*itCell);
		}
	}else
	{
		vector<DiscoidalCellMeshSubThread*> reffinementThreads;

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
				if(DISCOIDAL_CELL_MESH_DEBUG)
				{
					QString mess = "create a new thread of ID "  + QString::number(threadID);
					InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "DiscoidalCellMesh");
				}

				reffinementThreads.push_back(	new DiscoidalCellMeshSubThread(	
																				threadID,
																				getMaxNbSegPerCell(),
																				getDeltaWin(),
																				&neighbours,
																				MAX_RATIO_NUCLEUS_TO_CELL
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
		vector<DiscoidalCellMeshSubThread*>::iterator itThread;
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