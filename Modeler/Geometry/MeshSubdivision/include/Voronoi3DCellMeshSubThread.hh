/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef VORONOI_3D_CELL_MESH_SUBDIVION_THREAD_HH
#define VORONOI_3D_CELL_MESH_SUBDIVION_THREAD_HH

#include "CellMeshSettings.hh"
#include "CPOP_Circle.hh"
#include "CPOP_Triangle.hh"
#include "Nucleus.hh"
#include "Mesh3DSettings.hh"
#include "SpheroidalCell.hh"
#include "RefinementThread.hh"

#include <map>
#include <vector>

#include <QThread>

using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh3D;

//////////////////////////////////////////////////////////////////////////////
/// \brief The thread to refine spheroid cell from voronoi cell.
/// ideally to SpheroidalCell must be Sphere_3. But to win dev time we heritate from 
/// SpheroidalCellMeshSubThread and do the same action but without nucleus.
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class Voronoi3DCellMeshSubThread : public RefinementThread
{
	Q_OBJECT

public:
	/// \brief constructor
	Voronoi3DCellMeshSubThread(unsigned int, unsigned int, double, 
		const std::map<SpheroidalCell*, std::set<const SpheroidalCell*> >* ,					
		std::vector<SpheroidalCell*> pCellsToReffine = std::vector<SpheroidalCell*>(),
		double pSpaceBetweenCells = 0.);
	/// \brief destructor
	virtual ~Voronoi3DCellMeshSubThread();
	/// \brief add a spheroidal cell struct to reffine
	void addCell(SpheroidalCell*);
	/// \brief run of the thread.
	void run();
	/// \brief main function called to reffine a specific cell
	virtual bool reffineCell(SpheroidalCell* cell);

	/// \brief space between cell setter
	void setSpaceBetweenCell(double pSpace) 			{ spaceBetweenCells = pSpace;}
	/// \brief space between cell getter
	double getSpaceBetweenCell() const 					{ return spaceBetweenCells;}

protected:
	/// \brief generate the intersections planes for the given cell
	bool generateIntersectionPlane(SpheroidalCell* cell);
	/// \brief gnerate the smallest membrane mesh possible for the cell
	std::vector<Point_3> generateBasicCellMembraneMesh(SpheroidalCell* cell);
	/// \brief subdivided the membrane mesh according to the delta and maxNumberFacet parameters.
	bool subdivseCellMembraneMesh(SpheroidalCell* cell);
	/// \brief clean data structure and pointer after process
	void clean();	

private:
	/// \brief update the point of the mesh according to intersections.
	inline void adjustPointFromIntersections( Point_3& ptToCheck );
	/// \brief evaluate if point are interesting enought to be include on the mesh.
	inline bool ptsAreInteresting(const CPOP_Triangle*facet, std::vector<Point_3> pts);
	/// \brief  return the point to add when subdivizing a facet
	inline Point_3 getPointNewPointFromSeg(int indexVertex1, int indexVertex2, const CPOP_Triangle* triangle, Point_3 sphereCenter, double sphereRadius);
	/// \brief compute the error due to the approximation.
	// virtual void computeGeometryInformation();

protected:
	std::map<Point_3, int> reffinementValues;		///< \brief map each point to the plane he is part of. If -1 : not par of an intersection plane but on the theorical sphere
	/// \brief memorise the interesting intersection plane from the voronoi cell.
	/// \warning emake sur the plane has his normal on the opposite direction of the sphere center.
	std::vector<Plane_3*> intersections; 
	/// \brief map of plane with there given IDs
	std::map<Plane_3*, int> intersectionIDMap;	

	std::vector<SpheroidalCell*> cellsToReffine;										///< \brief all the cell to reffine.
	/// \brief the list of neighbour and there weight.	
	const std::map<SpheroidalCell*, std::set<const SpheroidalCell* > >* neighbourCells;	
	/// \brief the minimal space to generate betwen each cell. Half ot his distance will be directly removed from the cells radius.
	double spaceBetweenCells;									
};

#endif // SPHEROIDAL_3D_CELL_MESH_THREAD_HH