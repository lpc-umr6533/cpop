/*----------------------
Copyright (C): Henri Payno, Axel Delsol,
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "RandomEngineManager.hh"
#include "CellMeshSettings.hh"
#include "File_Utils_TXT.hh"
#include "Geometry_Utils_Sphere.hh"
#include "SpheroidalCell.hh"

#include "Debug_Settings.hh"

#include <CGAL/centroid.h>
#include <CGAL/convex_hull_3.h>
#include <CGAL/Object.h>
#include <CGAL_Utils.hh>

#include <QString>

#include "Nucleus.hh"

#include "G4UnitsTable.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "analysis.hh"
#include "RunAction.hh"

#include <string>
#include <fstream>


#if defined(WITH_GEANT_4) || defined(WITH_GDML_EXPORT)
	#include "G4VPhysicalVolume.hh"
	#include "G4LogicalVolume.hh"
	#include "G4PVPlacement.hh"
	#include "UnitSystemManager.hh"
	#include "Voronoi3DCellMeshSubThread.hh"
#ifdef WITH_GEANT_4
	#include "G4TriangularFacet.hh"
	#include "G4Orb.hh"
	#include "G4TessellatedSolid.hh"
#else
	#include "geometry/solid/specific/G4TriangularFacet.hh"
	#include "geometry/solid/G4Orb.hh"
	#include "geometry/solid/specific/G4TessellatedSolid.hh"
#endif
#endif

static const int maxTry = 7;	// The maximal number of iteration we are ready to made to made to remove overlaps
static const double stepSizeReductionPercent = 2.;	// at each iteration will reduce the size of cell each of stepSizeReductionPercent

#include <iostream>

#if DEBUG_SPHEROIDAL_CELL == 1
	static std::ofstream outputDebug("SpheoidalCellMembraneDebug");
	static unsigned int nbCallToGetSpot = 0;
#endif

///////////////////////////////////////////////////////////////////////////////
/// \param pCellProperties the cell properties of the cell
/// \param pOrigin the center/origin of the cell
/// \param pSpheroidRadius the radius of the spheroid
/// \param pWeight weight of the cell
/// \param pMembraneShape the initial membrane mesh
///////////////////////////////////////////////////////////////////////////////
SpheroidalCell::SpheroidalCell(const CellProperties* pCellProperties, Point_3 pOrigin, double pSpheroidRadius, double pWeight, Mesh3D::Polyhedron_3 pMembraneShape):
	RoundCell<double, Point_3, Vector_3>(pCellProperties, pOrigin, pSpheroidRadius, pWeight)
{
	shape = new Mesh3D::Polyhedron_3(pMembraneShape);
}

///////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////
SpheroidalCell::~SpheroidalCell()
{
	delete shape;
	shape = NULL;
}

//////////////////////////////////////////////////////////////////
/// \return the cytoplsam volume
//////////////////////////////////////////////////////////////////
double SpheroidalCell::getCytoplasmVolume() const
{
	/// create an estimable geometry
	DT_3 triangulation;
	Polyhedron_3 polyCopy = *shape;
	Polyhedron_3::Vertex_iterator itVertex;
	for(itVertex = polyCopy.vertices_begin(); itVertex != polyCopy.vertices_end(); ++itVertex)
	{
		triangulation.insert( itVertex->point() );
	}

	/// compute volume.
	double volume = 0;
	DT_3::Finite_cells_iterator itCell;
    for ( itCell = triangulation.finite_cells_begin(); itCell != triangulation.finite_cells_end(); ++itCell)
	{
		Tetrahedron_3 t(itCell->vertex(0)->point(), itCell->vertex(1)->point(), itCell->vertex(2)->point(), itCell->vertex(3)->point() );
		volume += t.volume();
	}
	return volume;
}

///////////////////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////////////////
void SpheroidalCell::resetMesh()
{
	areasToFacet.clear();
	delete shape;
	shape = new Mesh3D::Polyhedron_3();
}

#include <CGAL/intersections.h>
///////////////////////////////////////////////////////////////////////////////////////////
/// \return A random spot requested on the cytoplasm
///////////////////////////////////////////////////////////////////////////////////////////
Point_3 SpheroidalCell::getSpotOnCellMembrane() const
{
	//G4cout<< "\n\n\n getSpotOnCellMembrane" << G4endl;

	if( shape->size_of_facets() < 1)
	{
		QString mess = "Unvalid shape, unable to compute a spot on the membrane";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "SpheroidalCell");
		return Point_3(0., 0., 0.);
	}else
	{
		// get a random point :
		// first version : pick a random facet and pick a random point on the facet
		// Polyhedron_3::Facet_const_iterator itFacet = shape->facets_begin();
		// int indexFacet = randi(0, shape->size_of_facets() -1);
		// std::advance(itFacet, indexFacet);
		// return Utils::myCGAL::getSpotOnFacet(*itFacet);
		// ==> fail because on cell intersection we dont subdivise triangles because no win ==> give more chance to pick nanoparticles on area without neighbors

		// second version : get a random ray and pick the intersection with the triangle

		// Ray_3 randomRay( getPosition(), Point_3(randd(-1. ,1.), randd(-1. ,1.), randd(-1. ,1.)) );

		// Polyhedron_3::Facet_const_iterator itFacet = shape->facets_begin();
		// while( itFacet != shape->facets_end())
		// {
		// 	Triangle_3 facet(  	itFacet->halfedge()->vertex()->point(),
		// 						itFacet->halfedge()->next()->vertex()->point(),
		// 						itFacet->halfedge()->next()->next()->vertex()->point());
		// 	result_of<K::Intersect_3(Ray_3, Triangle_3)>::type lPt = intersection(randomRay, facet); // CGAL::CommonKernelFunctors::Intersect_3(randomRay, facet);

		// 	if( lPt )
		// 	{
		// 		const Point_3* pt = boost::get<Point_3>(&*lPt);
		// 		if( pt )
		// 		{
		// 			return *pt;
		// 		}
		// 	}
		// 	++ itFacet;
		// }
		// ==> fail because we pick a random point on a box ==> more density on the center of intersection planes

		// third version : give a weight depending on triangle area.
		// go throught all facet until surface area is reach
		double areaToReach = RandomEngineManager::getInstance()->randd(0., sumMembraneMeshArea);
		map<double, Triangle_3>::const_iterator itAreaFacet = areasToFacet.lower_bound(areaToReach);

		assert(itAreaFacet != areasToFacet.end());
		const Triangle_3* lTri = &itAreaFacet->second;
		assert(lTri);
		return (Utils::myCGAL::getSpotOnTriangle(lTri));
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
/// \return A random spot requested on the cytoplasm
///////////////////////////////////////////////////////////////////////////////////////////
Point_3 SpheroidalCell::getSpotOnCytoplasm() const
{
	// pick a point on the membrane, then pick a point on the segment [MembranePoint/cellOrigin] Until we found one not on the nucleus
	// because shape is convex we are sure the picked point is on the cytoplasm (work until no other organ are defined)

  //G4cout<< "\n\n\n getSpotOnCytoplasm" << G4endl;

	Vector_3 v( getSpotOnCellMembrane() - getPosition());
	double x = RandomEngineManager::getInstance()->randd(0., 1.);
	Point_3 res = getPosition() + x*(v);

	std::vector<Nucleus<double, Point_3, Vector_3> *>::const_iterator itNuc;
	for(itNuc = nuclei.begin(); itNuc != nuclei.end(); ++itNuc)
	{
		// if point is on a nucleus, pick an other one
		if((*itNuc)->hasIn(res))
		{
			return getSpotOnCytoplasm();
		}
	}
	return res;
}

///////////////////////////////////////////////////////////////////////////////////////////
/// \return A random spot requested on a nuclei
///////////////////////////////////////////////////////////////////////////////////////////
Point_3 SpheroidalCell::getSpotOnNuclei() const
{
  //G4cout<< "\n\n\n getSpotOnNuclei" << G4endl;

	if(nuclei.size() < 1)
	{
		QString mess = "No nuclei, unable to compute a spot on a  nuclei";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "SpheroidalCell");
		return Point_3(0., 0., 0.);
	}else
	{
		int indexShape = RandomEngineManager::getInstance()->randi(0, nuclei.size() -1);
		assert(nuclei[indexShape]);
		return nuclei[indexShape]->getSpotOn();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
/// \return A random spot requested in a nuclei
///////////////////////////////////////////////////////////////////////////////////////////
Point_3 SpheroidalCell::getSpotInNuclei() const
{
  //G4cout<< "\n\n\n getSpotInNuclei" << G4endl;

	if(nuclei.size() < 1)
	{
		QString mess = "No nuclei, unable to compute a spot on a  nuclei";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "SpheroidalCell");
		return Point_3(0., 0., 0.);
	}else
	{
		int indexShape = RandomEngineManager::getInstance()->randi(0, nuclei.size() -1);
		assert(nuclei[indexShape]);
		return nuclei[indexShape]->getSpotIn();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param ptToCheck the point to check
/// \return true if the point is inside the cell
/// \warning will work only for convex hull for now. Work in our case
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SpheroidalCell::hasIn(Point_3 ptToCheck) const
{
	/// \todo speed up this process using ray tracing
	// check if ordered
	if(!hasMesh())
	{
		return false;
	}

	// if isn't on the theorical cell
	if( CGAL::squared_distance(ptToCheck, getPosition()) >= getSquareRadius() )
	{
		return false;
	}

	// if is in the mesh
	Polyhedron_3::Facet_iterator itFacet;
	for( itFacet = shape->facets_begin(); itFacet != shape->facets_end(); ++itFacet)
    {
    	Plane_3 facetPlane(
    		itFacet->halfedge()->vertex()->point(),
    		itFacet->halfedge()->next()->vertex()->point(),
			itFacet->halfedge()->next()->next()->vertex()->point() );

    	CGAL::Oriented_side oriSide = facetPlane.oriented_side(ptToCheck);
    	if( (oriSide != CGAL::ON_ORIENTED_BOUNDARY ) && ( oriSide != facetPlane.oriented_side(this->getPosition()) ) )
    	{
    		return false;
    	}
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param meshFormat the format of the mesh we want information for
/// \return inforamtion about the mesh
////////////////////////////////////////////////////////////////////////////////////////////////////////
QString SpheroidalCell::getMeshStats(MeshOutFormats::outputFormat meshFormat) const
{
	QString res;
	res += " --- start cell #" + QString::number(getID()) + "_stats --- \n";
	res += " ID : " + QString::number(getID()) + "\n";
	res += " type_of_cell_is : " 		+ getDescription()						+ "\n";
	res += " position :" 				+ QString::number( getPosition().x() )
			+ ", " 						+ QString::number( getPosition().y() )
			+ ", "						+ QString::number( getPosition().z() )	+ "\n";
	res += " approximated_volume : "	+ QString::number(getMeshVolume(meshFormat)) 			+ "\n";
	res += "    --- start NucleiStats --- \n";
	std::vector<Nucleus<double, Point_3, Vector_3> *>::const_iterator itN;	/// \brief the list of nuclei owned by the cell
	unsigned int iNuclei = 0;
	for(itN = nuclei.begin(); itN != nuclei.end(); ++itN)
	{
		assert(*itN);
		res += "      Nuclei #" + QString::number(iNuclei) + "  : " + (*itN)->getMeshVolume(meshFormat);
		++iNuclei;
	}
	res += "    --- end Nuclei_stats --- \n";
	res += " --- end cell" + QString::number(getID()) + "_stats  --- \n";

	return res;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param meshFormat the format of the mesh we want information for
/// \return the membrane mesh volume
////////////////////////////////////////////////////////////////////////////////////////////////////////
double SpheroidalCell::getMeshVolume(MeshOutFormats::outputFormat meshFormat) const
{
	/// \todo : optimization : if Geant4 or GATE or GDML : if no neighbour (intersection line : export a G4Orb ?)
	switch(meshFormat)
	{
		case MeshOutFormats::GDML :
		case MeshOutFormats::GEANT_4 :
		case MeshOutFormats::GATE :
		case MeshOutFormats::OFF :
		{
			return Utils::myCGAL::getConvexPolyhedronVolume(shape, getPosition());
		}
		case MeshOutFormats::Unknow :
		default :
		{
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "unable to generate mesh statistics, unknow calculation methods", "SpheroidalCell");
			return 0.;
		}
	}
}

#if defined(WITH_GEANT_4) || defined(WITH_GDML_EXPORT)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pName The prefix name to give to the G4entities
/// \return The G4LogicalVolume* representing the membrane
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
G4LogicalVolume* SpheroidalCell::convertMembraneToG4(QString pName)
{
	G4double convertToG4 = G4double(UnitSystemManager::getInstance()->getConversionToG4());

	Polyhedron_3::Vertex_iterator itVertex;
	// define the tesselated solid
	G4TessellatedSolid* membraneSolid = new G4TessellatedSolid(pName.toStdString());
	membraneSolid->SetSolidClosed(false);

	// add all external facets
    Point_3 p1, p2, p3;
	// export facets
	Polyhedron_3::Facet_iterator itFacet;
	for( itFacet = shape_facets_begin(); itFacet != shape_facets_end(); ++itFacet)
    {
    	p1 = itFacet->halfedge()->vertex()->point();
    	p2 = itFacet->halfedge()->next()->vertex()->point();
    	p3 = itFacet->halfedge()->next()->next()->vertex()->point();
    	// we only export external facets
    	assert(p1 != getOrigin());
    	assert(p2 != getOrigin());
    	assert(p3 != getOrigin());

		Vector_3 v1 = p1 - getOrigin();
		Vector_3 v2 = p2 - getOrigin();
		Vector_3 v3 = p3 - getOrigin();

		assert(p1 != p2);
		assert(p2 != p3);
		assert(p3 != p1);

		// check facet orientation
		double determinant = CGAL::determinant(v1, v2, v3);
		if(determinant < 0.f)
		{
			Point_3 tmp = p1;
			p1 = p2;
			p2 = tmp;
		}

    	G4TriangularFacet* facet = new G4TriangularFacet( 	G4ThreeVector( 	G4double(	p1.x())*convertToG4,
    																		G4double( 	p1.y())*convertToG4,
    																		G4double(	p1.z())*convertToG4 	),
    														G4ThreeVector( 	G4double(	p2.x())*convertToG4,
    																		G4double( 	p2.y())*convertToG4,
    																		G4double(	p2.z())*convertToG4 	),
    														G4ThreeVector( 	G4double(	p3.x())*convertToG4,
    																		G4double( 	p3.y())*convertToG4,
    																		G4double(	p3.z())*convertToG4 	),
    														ABSOLUTE);

    	assert(facet);
    	assert(static_cast<G4VFacet*>(facet));
        if(!membraneSolid->AddFacet(static_cast<G4VFacet*>(facet)))
        {
        	std::cout << "error during facet addition" << std::endl;
        	return NULL;
        }
    }

	// very important command because otherwise, Geant4 will see this entity as boundless
    membraneSolid->SetSolidClosed(true);
	assert(membraneSolid);

	if(membraneSolid->GetNumberOfFacets() < 4)
	{
		std::cout << "error during creation, cannot create a tesselated solid without facets" << std::endl;
		return NULL;
	}

	// define all facet and add them.
	QString logicalVolName = "LV_" + pName;
	G4Material* lCellMat = this->getCellProperties()->getCytoplasmMaterial(this->getLifeCycle());
	if(!lCellMat)
	{
		lCellMat = MaterialManager::getInstance()->getDefaultMaterial();
	}

	assert(lCellMat);
	return new G4LogicalVolume(membraneSolid, lCellMat, logicalVolName.toStdString(), 0, 0, 0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pMother 			The mother logical volume
/// \param pName 			The name to set to the G4entity
/// \param checkOverLaps 	true if we want to check geometry overlap by the G4 process
/// \param pNeighbourCells	The structure containing all cell connections
/// \param pNbFacet 		The maximal number of facet a cell must contained
/// \param pDeltaWin 		The minimal value for which we continu to reffine
/// \param pCellMap			The map containing relashionship between G4LogicalVolume and cell
/// \param pNucleiMap		The map containing relashionship between G4LogicalVolume and nucleus
/// \param pExportNuclei true if we want to export nuclei as well to G4
/// \return The G4Vplacement* generated for the G4ent
//////////////////////////////////////////////////////////////////////////////////////////////////
// TODO : appeler ca convertToG3Entity
G4PVPlacement* SpheroidalCell::convertToG4Structure(
	G4LogicalVolume* pMother,
	QString pName,
	bool checkOverLaps,
	const std::map<SpheroidalCell*, std::set<const SpheroidalCell*> >* pNeighbourCells,
	unsigned int pNbFacet,
	double pDeltaWin,
	map<const G4LogicalVolume*, const t_Cell_3* >* pCellMap,
	map<const G4LogicalVolume*, const t_Nucleus_3*>* pNucleiMap,
	bool pExportNuclei
	)
{
	assert(pMother);
	assert(pNeighbourCells);

	G4LogicalVolume* membraneLogicVol = convertMembraneToG4(pName);

	QString physVolName = "PV_" + pName;
	// std::cout << '\n' << " physVolName " << printf(physVolName.toStdString().c_str()) <<'\n';

	G4PVPlacement* vpPalcement = new G4PVPlacement(	G4Transform3D(),			// no rotation
						membraneLogicVol,     		// its logical volume
						physVolName.toStdString(), 	// its name
						pMother,					// its mother  volume
						false,         				// no boolean operations
						0);           				// copy number

	// try to remove overlaps by reducing membrane radius
	if(checkOverLaps)
	{
		int nbTry = 0;
		Voronoi3DCellMeshSubThread cellMeshSub(0, pNbFacet, pDeltaWin, pNeighbourCells);
		bool overlap;
		try
		{
			overlap = vpPalcement->CheckOverlaps( 1000, overlapToleranceForG4, false);
		}catch(...)
		{
			throw;
		}

		while(nbTry < maxTry && (overlap))
		{
			nbTry ++;
			this->resetMesh();

			double newStepSize = stepSizeReductionPercent / 100. * nbTry * this->getRadius();

			cellMeshSub.setSpaceBetweenCell( newStepSize);
			cellMeshSub.reffineCell( this );

			membraneLogicVol = this->convertMembraneToG4(pName);
			try
			{
				overlap = vpPalcement->CheckOverlaps( 1000, overlapToleranceForG4, false);
			}catch(...)
			{
				throw;
			}
		}

		// if some operations made to remove overlaps : set status
		if(nbTry > 0)
		{
			if( (!overlap) )
			{
				cout << " suceeded to remove an overlap" << endl;
				std::cout << "  Masse cell " << membraneLogicVol->GetMass()  <<'\n';
			}else
			{
				cout << " failed to removed an overlap. Removing the G4PVPlacement" << endl;
				// remove mesh
				this->resetMesh();
				// remove the G4VPlacement entity
				pMother->RemoveDaughter(vpPalcement);
				delete vpPalcement;
			}
		}

		if(overlap)
		{
			return NULL;
		}
	}

	// if we want to register cells to the map
	if(pCellMap)
	{
		pCellMap->insert(make_pair(	const_cast<const G4LogicalVolume*>(membraneLogicVol),
                                    const_cast<const t_Cell_3*>(static_cast<t_Cell_3*>(this)) ));
	}

	// deal with nuclei
	if(pExportNuclei)
	{
		std::vector<Nucleus<double, Point_3, Vector_3>* > ::iterator itNucleus;
		unsigned int iNucleus = 0;
		G4Material* lNucleusMat = getCellProperties()->getNucleusMaterial(getLifeCycle());
		assert(getCellProperties());

		if(!lNucleusMat) lNucleusMat = MaterialManager::getInstance()->getDefaultMaterial();

		assert(lNucleusMat);

		G4int err = 0;

		for(itNucleus = nuclei.begin(); itNucleus != nuclei.end(); ++itNucleus)
		{
			QString nucleusName = nucleusNamePrefix + pName + QString::number(iNucleus);
			iNucleus++;
			G4PVPlacement* nucPlacement = (*itNucleus)->convertToG4Entity(nucleusName, membraneLogicVol, lNucleusMat, checkOverLaps);
			// std::cout << "  Masse cell " << membraneLogicVol->GetMass()  <<'\n';
			assert(nucPlacement);

			// if we want to register nuclei on a map
	        if(pNucleiMap)
	        {
	            pNucleiMap->insert(make_pair(const_cast<const G4LogicalVolume*>(nucPlacement->GetLogicalVolume()), const_cast<const t_Nucleus_3*>(*itNucleus) ));
	        }

			ofstream fw("MassesCell.txt", fstream::app);

			if (fw.is_open())
			{
			    fw << G4BestUnit(((nucPlacement->GetLogicalVolume())->GetMass()), "Mass") <<" ";
					fw << G4BestUnit((membraneLogicVol->GetMass()), "Mass") << "\n" ;
			  fw.close();
			}
			else err=1;


		}

		if (err == 1) {G4cout << "error happened during writing of MassesCell.txt"<<
								G4endl;}
	}

	return vpPalcement;
}

#endif	// WITH_GEANT_4

//////////////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////////////
void SpheroidalCell::computeMembraneSurfaceArea()
{
	areasToFacet.clear();
	// map<double, Polyhedron_3::Facet_const_iterator> areasToFacet;

	// reset area
	sumMembraneMeshArea = 0;

	// add area of eah facet
	Polyhedron_3::Facet_const_iterator itFacet;
	for( itFacet = shape->facets_begin(); itFacet != shape->facets_end(); ++itFacet)
	{
		Triangle_3 lTri(
			itFacet->halfedge()->vertex()->point(),
    		itFacet->halfedge()->next()->vertex()->point(),
    		itFacet->halfedge()->next()->next()->vertex()->point() );
		// here we can't use the squared_area because we want want to pick uniformely spot on the membrane
		sumMembraneMeshArea += sqrt(lTri.squared_area());
		areasToFacet.insert(make_pair(sumMembraneMeshArea, lTri));
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \return true if the cell has a mesh
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SpheroidalCell::hasMesh() const
{
	assert(shape);
	if(shape && !shape->empty())
	{
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param meshFormat the format of the mesh we want volume for
/// \return the volume
////////////////////////////////////////////////////////////////////////////////////////////////////////
double SpheroidalCell::getNucleiMeshesSumVolume(MeshOutFormats::outputFormat meshFormat) const
{
	double volume = 0.;
	std::vector<Nucleus<double, Point_3, Vector_3> *>::const_iterator itNuc;
	for(itNuc = nuclei.begin(); itNuc != nuclei.end(); ++itNuc)
	{
		volume += (*itNuc)->getMeshVolume( meshFormat);
	}

	G4cout<< "\n SpheroidalCell::getNucleiMeshesSumVolume : " << volume <<G4endl;

	return volume;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param meshFormat the format of the mesh we want volume for
/// \return the volume
////////////////////////////////////////////////////////////////////////////////////////////////////////
double SpheroidalCell::getCytoplasmMeshVolume(MeshOutFormats::outputFormat meshFormat) const
{
	return this->getMeshVolume(meshFormat) - this->getNucleiMeshesSumVolume(meshFormat);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SpheroidalCell::cross(const BoundingBox<Point_3>* pBB) const
{
	Point_3 pt = this->getPosition();
	double radius = getRadius();

	return 	(
		(pt.x() <= (pBB->getTopRight().x() 		+ radius)) &&
		(pt.x() >= (pBB->getBottomLeft().x()	- radius)) &&
		(pt.y() <= (pBB->getTopRight().y()		+ radius)) &&
		(pt.y() >= (pBB->getBottomLeft().y() 	- radius)) &&
		(pt.z() <= (pBB->getTopRight().z()		+ radius)) &&
		(pt.z() >= (pBB->getBottomLeft().z() 	- radius)) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SpheroidalCell::isIn(const BoundingBox<Point_3>* pBB) const
{
	Point_3 pt = this->getPosition();
	double radius = getRadius();

	return 	(
		((pt.x() - radius) < pBB->getTopRight().x()		) &&
		((pt.x() + radius) > pBB->getBottomLeft().x() 	) &&
		((pt.y() - radius) < pBB->getTopRight().y()		) &&
		((pt.y() + radius) > pBB->getBottomLeft().y() 	) &&
		((pt.z() - radius) < pBB->getTopRight().z()		) &&
		((pt.z() + radius) > pBB->getBottomLeft().z() 	) );
}
