#include "RandomEngineManager.hh"
#include "CellMeshSettings.hh"
#include "MaterialManager.hh"
#include "SpheroidalCell.hh"

#include "Debug_Settings.hh"

#include <CGAL/centroid.h>
#include <CGAL/convex_hull_3.h>
#include <CGAL/Object.h>
#include <CGAL_Utils.hh>

#include "Nucleus.hh"

#include "G4UnitsTable.hh"

#include "G4RunManager.hh"

#include <string>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

#if defined(WITH_GEANT_4) || defined(WITH_GDML_EXPORT)
	#include "G4VPhysicalVolume.hh"
	#include "G4LogicalVolume.hh"
	#include "G4PVPlacement.hh"
	#include "UnitSystemManager.hh"
	#include "Voronoi3DCellMeshSubThread.hh"
#ifdef WITH_GEANT_4
	#include "G4TriangularFacet.hh"
	#include "G4TessellatedSolid.hh"
#else
	#include "geometry/solid/specific/G4TriangularFacet.hh"
	#include "geometry/solid/G4Orb.hh"
	#include "geometry/solid/specific/G4TessellatedSolid.hh"
#endif
#endif

static constexpr int maxTry = 7;	// The maximal number of iteration we are ready to made to made to remove overlaps
static constexpr double stepSizeReductionPercent = 2.;	// at each iteration will reduce the size of cell each of stepSizeReductionPercent

#include <iostream>

#if DEBUG_SPHEROIDAL_CELL == 1
	static std::ofstream outputDebug("SpheoidalCellMembraneDebug");
	static unsigned int nbCallToGetSpot = 0;
#endif

/// \param pCellProperties the cell properties of the cell
/// \param pOrigin the center/origin of the cell
/// \param pSpheroidRadius the radius of the spheroid
/// \param pWeight weight of the cell
/// \param pMembraneShape the initial membrane mesh
SpheroidalCell::SpheroidalCell(const CellProperties* pCellProperties, Point_3 pOrigin, double pSpheroidRadius, double pWeight, Mesh3D::Polyhedron_3 pMembraneShape):
	RoundCell<double, Point_3, Vector_3>(pCellProperties, pOrigin, pSpheroidRadius, pWeight)
{
	_shape = new Mesh3D::Polyhedron_3(pMembraneShape);
}

SpheroidalCell::~SpheroidalCell() {
	delete _shape;
}

/// \return the cytoplsam volume
double SpheroidalCell::getCytoplasmVolume() const {
	/// create an estimable geometry
	DT_3 triangulation;
	Polyhedron_3 polyCopy = *_shape;
	Polyhedron_3::Vertex_iterator itVertex;
	for(itVertex = polyCopy.vertices_begin(); itVertex != polyCopy.vertices_end(); ++itVertex)
		triangulation.insert(itVertex->point());

	/// compute volume.
	double volume = 0;
	for(auto itCell = triangulation.finite_cells_begin(); itCell != triangulation.finite_cells_end(); ++itCell) {
		Tetrahedron_3 t(itCell->vertex(0)->point(), itCell->vertex(1)->point(), itCell->vertex(2)->point(), itCell->vertex(3)->point() );
		volume += t.volume();
	}

	return volume;
}

void SpheroidalCell::resetMesh() {
	areasToFacet.clear();
	delete _shape;
	_shape = new Mesh3D::Polyhedron_3;
}

#include <CGAL/intersections.h>

/// \return A random spot requested on the cytoplasm
Point_3 SpheroidalCell::getSpotOnCellMembrane() const {
	if(_shape->size_of_facets() < 1) {
		std::string mess = "Unvalid shape, unable to compute a spot on the membrane";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess, "SpheroidalCell");
		return {0., 0., 0.};
	} else {
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
		double areaToReach = RandomEngineManager::getInstance()->randd(0., _sumMembraneMeshArea);
		auto itAreaFacet = areasToFacet.lower_bound(areaToReach);

		assert(itAreaFacet != areasToFacet.end());
		const Triangle_3* lTri = &itAreaFacet->second;
		assert(lTri);
		return (Utils::myCGAL::getSpotOnTriangle(lTri));
	}
}

/// \return A random spot requested on the cytoplasm
Point_3 SpheroidalCell::getSpotOnCytoplasm() const {
	// pick a point on the membrane, then pick a point on the segment [MembranePoint/cellOrigin] Until we found one not on the nucleus
	// because shape is convex we are sure the picked point is on the cytoplasm (work until no other organ are defined)

	Vector_3 v( getSpotOnCellMembrane() - getPosition());
	double x = RandomEngineManager::getInstance()->randd(0., 1.);
	Point_3 res = getPosition() + x*(v);

	std::vector<Nucleus<double, Point_3, Vector_3> *>::const_iterator itNuc;
	for(auto const& itNuc : _nuclei) {
		// if point is on a nucleus, pick an other one
		if(itNuc->hasIn(res))
			return getSpotOnCytoplasm();
	}

	return res;
}

/// \return A random spot requested on a nuclei
Point_3 SpheroidalCell::getSpotOnNuclei() const {
	if(_nuclei.size() < 1) {
		std::string mess = "No nuclei, unable to compute a spot on a  nuclei";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess, "SpheroidalCell");
		return {0., 0., 0.};
	} else {
		int indexShape = RandomEngineManager::getInstance()->randi(0, _nuclei.size() -1);
		assert(_nuclei[indexShape]);
		return _nuclei[indexShape]->getSpotOn();
	}
}

/// \return A random spot requested in a nuclei
Point_3 SpheroidalCell::getSpotInNuclei() const {
	if(_nuclei.size() < 1) {
		std::string mess = "No nuclei, unable to compute a spot on a  nuclei";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess, "SpheroidalCell");
		return {0., 0., 0.};
	} else {
		int indexShape = RandomEngineManager::getInstance()->randi(0, _nuclei.size() -1);
		assert(_nuclei[indexShape]);
		return _nuclei[indexShape]->getSpotIn();
	}
}

/// \param ptToCheck the point to check
/// \return true if the point is inside the cell
/// \warning will work only for convex hull for now. Work in our case
bool SpheroidalCell::hasIn(Point_3 ptToCheck) const {
	/// \todo speed up this process using ray tracing
	// check if ordered
	if(!hasMesh())
		return false;

	// if isn't on the theorical cell
	if(CGAL::squared_distance(ptToCheck, getPosition()) >= getSquareRadius())
		return false;

	// if is in the mesh
	for(auto itFacet = _shape->facets_begin(); itFacet != _shape->facets_end(); ++itFacet) {
		Plane_3 facetPlane(
			itFacet->halfedge()->vertex()->point(),
			itFacet->halfedge()->next()->vertex()->point(),
			itFacet->halfedge()->next()->next()->vertex()->point()
		);

		CGAL::Oriented_side oriSide = facetPlane.oriented_side(ptToCheck);
		if((oriSide != CGAL::ON_ORIENTED_BOUNDARY ) && ( oriSide != facetPlane.oriented_side(this->getPosition())))
			return false;
	}

	return true;
}

/// \param meshFormat the format of the mesh we want information for
/// \return inforamtion about the mesh
std::string SpheroidalCell::getMeshStats(MeshOutFormats::outputFormat meshFormat) const {
	std::string res;
	res += " --- start cell #"                 + std::to_string(getID())                   + "_stats --- \n";
	res += " ID : "                            + std::to_string(getID())                   + "\n";
	res += " type_of_cell_is : "               + getDescription()                          + "\n";
	res += " position :"                       + std::to_string(getPosition().x())
	    + ", "                                 + std::to_string(getPosition().y())
	    + ", "                                 + std::to_string(getPosition().z())         + "\n";
	res += " approximated_volume : "           + std::to_string(getMeshVolume(meshFormat)) + "\n";
	res += "    --- start NucleiStats --- \n";

	unsigned int iNuclei = 0;
	for(auto const& itN : _nuclei) {
		assert(itN);
		res += "      Nuclei #" + std::to_string(iNuclei) + "  : " + std::to_string(itN->getMeshVolume(meshFormat));
		++iNuclei;
	}

	res += "    --- end Nuclei_stats --- \n";
	res += " --- end cell" + std::to_string(getID()) + "_stats  --- \n";

	return res;
}

/// \param meshFormat the format of the mesh we want information for
/// \return the membrane mesh volume
double SpheroidalCell::getMeshVolume(MeshOutFormats::outputFormat meshFormat) const {
	/// \todo : optimization : if Geant4 or GATE or GDML : if no neighbour (intersection line : export a G4Orb ?)
	switch(meshFormat) {
		case MeshOutFormats::GDML:
		case MeshOutFormats::GEANT_4:
		case MeshOutFormats::GATE:
		case MeshOutFormats::OFF:
		{
			return Utils::myCGAL::getConvexPolyhedronVolume(_shape, getPosition());
		}
		case MeshOutFormats::Unknow:
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
G4LogicalVolume* SpheroidalCell::convertMembraneToG4(std::string const& pName) {
	auto convertToG4 = G4double(UnitSystemManager::getInstance()->getConversionToG4());

	Polyhedron_3::Vertex_iterator itVertex;
	// define the tesselated solid
	auto* membraneSolid = new G4TessellatedSolid(pName);
	membraneSolid->SetSolidClosed(false);

	// add all external facets
	Point_3 p1, p2, p3;
	// export facets
	for(auto itFacet = shape_facets_begin(); itFacet != shape_facets_end(); ++itFacet) {
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
		if(determinant < 0.f) {
			Point_3 tmp = p1;
			p1 = p2;
			p2 = tmp;
		}

		auto* facet = new G4TriangularFacet(
			G4ThreeVector(
				G4double(p1.x())*convertToG4,
				G4double(p1.y())*convertToG4,
				G4double(p1.z())*convertToG4
			),
			G4ThreeVector(
				G4double(p2.x())*convertToG4,
				G4double(p2.y())*convertToG4,
				G4double(p2.z())*convertToG4
			),
			G4ThreeVector(
				G4double(p3.x())*convertToG4,
				G4double(p3.y())*convertToG4,
				G4double(p3.z())*convertToG4
			),
			ABSOLUTE
		);

		assert(facet);
		assert(static_cast<G4VFacet*>(facet));

		if(!membraneSolid->AddFacet(static_cast<G4VFacet*>(facet))) {
			std::cout << "error during facet addition" << std::endl;
			// return nullptr;
		}
	}

	// very important command because otherwise, Geant4 will see this entity as boundless
	membraneSolid->SetSolidClosed(true);
	assert(membraneSolid);

	if(membraneSolid->GetNumberOfFacets() < 4) {
		std::cout << "error during creation, cannot create a tesselated solid without facets" << std::endl;
		return nullptr;
	}

	// define all facet and add them.
	std::string logicalVolName = "LV_" + pName;
	auto* lCellMat = this->getCellProperties()->getCytoplasmMaterial(this->getLifeCycle());
	if(!lCellMat)
		lCellMat = MaterialManager::getInstance()->getDefaultMaterial();

	assert(lCellMat);
	return new G4LogicalVolume(membraneSolid, lCellMat, logicalVolName, nullptr, nullptr, nullptr);
}

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
// TODO : appeler ca convertToG3Entity
G4PVPlacement* SpheroidalCell::convertToG4Structure(
	G4LogicalVolume* pMother,
	std::string const& pName,
	bool checkOverLaps,
	const std::map<SpheroidalCell*, std::set<const SpheroidalCell*> >* pNeighbourCells,
	unsigned int pNbFacet,
	double pDeltaWin,
	std::map<const G4LogicalVolume*, const t_Cell_3*>* pCellMap,
	std::map<const G4LogicalVolume*, const t_Nucleus_3*>* pNucleiMap,
	bool pExportNuclei
	)
{
	assert(pMother);
	assert(pNeighbourCells);

	G4LogicalVolume* membraneLogicVol = convertMembraneToG4(pName);

	std::string physVolName = "PV_" + pName;
	// std::cout << '\n' << " physVolName " << printf(physVolName.toStdString().c_str()) <<'\n';

	auto* vpPalcement = new G4PVPlacement(
		G4Transform3D(),  // no rotation
		membraneLogicVol, // its logical volume
		physVolName,      // its name
		pMother,          // its mother  volume
		false,            // no boolean operations
		0                 // copy number
		, true
	);

	std::string home_path = std::filesystem::current_path().string();

	std::string output_txt_folder = home_path + "/OutputTxt";
	if (!fs::exists(output_txt_folder)) {
		if (!fs::create_directory(output_txt_folder))
		{std::cerr << "Error: Failed to create directory: " << output_txt_folder << "\n";}
	}

	std::ofstream masses_file(output_txt_folder + "/MassesCell.txt", std::ios::app);

	// try to remove overlaps by reducing membrane radius
	if(checkOverLaps) {
		int nbTry = 0;
		Voronoi3DCellMeshSubThread cellMeshSub(0, pNbFacet, pDeltaWin, pNeighbourCells);
		bool overlap;
		try {
			overlap = vpPalcement->CheckOverlaps(1000, overlapToleranceForG4, false);
		} catch(...) {
			throw;
		}

		while(nbTry < maxTry && (overlap)) {
			nbTry ++;
			this->resetMesh();

			double newStepSize = stepSizeReductionPercent / 100. * nbTry * this->getRadius();

			cellMeshSub.setSpaceBetweenCell( newStepSize);
			cellMeshSub.reffineCell( this );

			membraneLogicVol = this->convertMembraneToG4(pName);

			try {
				overlap = vpPalcement->CheckOverlaps( 1000, overlapToleranceForG4, false);
			} catch(...) {
				throw;
			}
		}

		// if some operations made to remove overlaps : set status
		if(nbTry > 0) {
			if((!overlap)) {
				std::cout << " suceeded to remove an overlap" << std::endl;
				std::cout << "  Masse cell " << membraneLogicVol->GetMass()  <<'\n';
			} else {
				std::cout << " failed to removed an overlap. Removing the G4PVPlacement" << std::endl;
				resetMesh();
				pMother->RemoveDaughter(vpPalcement);
				delete vpPalcement;
			}
		}

		if(overlap)
			return nullptr;
	}

	// if we want to register cells to the map
	if(pCellMap) {
		pCellMap->insert(
			// TODO const_cast!
			std::make_pair(
				const_cast<const G4LogicalVolume*>(membraneLogicVol),
				const_cast<const t_Cell_3*>(static_cast<t_Cell_3*>(this))
			)
		);
	}

	// deal with nuclei
	if(pExportNuclei) {
		unsigned int iNucleus = 0;
		auto* lNucleusMat = getCellProperties()->getNucleusMaterial(getLifeCycle());
		assert(getCellProperties());

		if(!lNucleusMat) lNucleusMat = MaterialManager::getInstance()->getDefaultMaterial();
		assert(lNucleusMat);

		G4int err = 0;

		for(auto const& itNucleus : _nuclei) {
			std::string nucleusName = nucleusNamePrefix + pName + std::to_string(iNucleus);
			iNucleus++;
			auto* nucPlacement = itNucleus->convertToG4Entity(nucleusName, membraneLogicVol, lNucleusMat, checkOverLaps);
			// std::cout << "  Masse cell " << membraneLogicVol->GetMass()  <<'\n';
			assert(nucPlacement);

			// if we want to register nuclei on a map
			if(pNucleiMap) {
				pNucleiMap->insert(
					std::make_pair(
						const_cast<const G4LogicalVolume*>(nucPlacement->GetLogicalVolume()),
						const_cast<const t_Nucleus_3*>(itNucleus)
					)
				);
			}

			if(masses_file.is_open()) {
				masses_file << G4BestUnit(((nucPlacement->GetLogicalVolume())->GetMass()), "Mass") <<" ";
				masses_file << G4BestUnit((membraneLogicVol->GetMass()), "Mass") << "\n" ;
			} else {
				err=1;
			}
		}

		if(err)
			G4cout << "error happened during writing of MassesCell.txt" << G4endl;
	}

	masses_file.close();

	return vpPalcement;
}

#endif

void SpheroidalCell::computeMembraneSurfaceArea() {
	areasToFacet.clear();
	// map<double, Polyhedron_3::Facet_const_iterator> areasToFacet;

	// reset area
	_sumMembraneMeshArea = 0;

	// add area of eah facet
	for(auto itFacet = _shape->facets_begin(); itFacet != _shape->facets_end(); ++itFacet) {
		Triangle_3 lTri(
			itFacet->halfedge()->vertex()->point(),
			itFacet->halfedge()->next()->vertex()->point(),
			itFacet->halfedge()->next()->next()->vertex()->point()
		);

		// here we can't use the squared_area because we want want to pick uniformely spot on the membrane
		_sumMembraneMeshArea += sqrt(lTri.squared_area());
		areasToFacet.insert(std::make_pair(_sumMembraneMeshArea, lTri));
	}
}

/// \return true if the cell has a mesh
bool SpheroidalCell::hasMesh() const {
	assert(_shape);
	return _shape && !_shape->empty();
}

/// \param meshFormat the format of the mesh we want volume for
/// \return the volume
double SpheroidalCell::getNucleiMeshesSumVolume(MeshOutFormats::outputFormat meshFormat) const {
	double volume = 0.;
	for(auto const& itNuc : _nuclei)
		volume += itNuc->getMeshVolume( meshFormat);

	G4cout << "\n SpheroidalCell::getNucleiMeshesSumVolume : " << volume << G4endl;

	return volume;
}

/// \param meshFormat the format of the mesh we want volume for
/// \return the volume
double SpheroidalCell::getCytoplasmMeshVolume(MeshOutFormats::outputFormat meshFormat) const {
	return getMeshVolume(meshFormat) - getNucleiMeshesSumVolume(meshFormat);
}

bool SpheroidalCell::cross(const BoundingBox<Point_3>* pBB) const {
	Point_3 pt = this->getPosition();
	double radius = getRadius();

	return (
		(pt.x() <= (pBB->getTopRight().x() 		+ radius)) &&
		(pt.x() >= (pBB->getBottomLeft().x()	- radius)) &&
		(pt.y() <= (pBB->getTopRight().y()		+ radius)) &&
		(pt.y() >= (pBB->getBottomLeft().y() 	- radius)) &&
		(pt.z() <= (pBB->getTopRight().z()		+ radius)) &&
		(pt.z() >= (pBB->getBottomLeft().z() 	- radius))
	);
}

bool SpheroidalCell::isIn(const BoundingBox<Point_3>* pBB) const {
	Point_3 pt = this->getPosition();
	double radius = getRadius();

	return (
		((pt.x() - radius) < pBB->getTopRight().x()		) &&
		((pt.x() + radius) > pBB->getBottomLeft().x() 	) &&
		((pt.y() - radius) < pBB->getTopRight().y()		) &&
		((pt.y() + radius) > pBB->getBottomLeft().y() 	) &&
		((pt.z() - radius) < pBB->getTopRight().z()		) &&
		((pt.z() + radius) > pBB->getBottomLeft().z() 	)
	);
}
