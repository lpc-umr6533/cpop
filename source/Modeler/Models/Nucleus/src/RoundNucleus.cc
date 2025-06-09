#include "CellMeshSettings.hh"
#include "CGAL_Utils.hh"
#include "RoundNucleus.hh"
#include "RandomEngineManager.hh"
#include "Geometry_Utils_Circle.hh"
#include "Geometry_Utils_Sphere.hh"
#include "UnitSystemManager.hh"

#include <CGAL/convex_hull_3.h>

#if ( defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) )
	#define _USE_MATH_DEFINES
	#include <math.h>
#endif

#ifdef CONVERT_TO_G4

/// \param name 			The name to set to the nucleus
/// \param motherVolume 	The mother volume, NULL if none
/// \param pNucleusMat 		The material to set to the nucleus
/// \param checkOverLaps 	The material to set to the nucleus
template<>
G4PVPlacement* RoundNucleus<double, Point_3, Vector_3>::convertToG4Entity(QString name, G4LogicalVolume* motherVolume, G4Material* pNucleusMat, bool checkOverLaps) const {
	auto convertToG4 = G4double(UnitSystemManager::getInstance()->getConversionToG4());
	G4LogicalVolume* logicVol = convertToG4LogicalVolume(name, pNucleusMat);

	QString physVolName = "PV_" + name;
	auto* physVol = new G4PVPlacement(
		G4Translate3D(_origin.x()*convertToG4, _origin.y()*convertToG4, _origin.z()*convertToG4),	// no rotation
		logicVol,                                                                                 // its logical volume
		physVolName.toStdString(),                                                                // its name
		(motherVolume == nullptr ) ? nullptr : motherVolume,                                      // its mother  volume
		false,                                                                                    // no boolean operations
		0,                                                                                        // copy number
		checkOverLaps                                                                             // surface overlaps
	);

	return physVol;
}

/// \param name 			The name to set to the nucleus
/// \param pNucleusMat 		The material to set to the nucleus
/// \param checkOverLaps 	The material to set to the nucleus
template<>
G4LogicalVolume* RoundNucleus<double, Point_3, Vector_3>::convertToG4LogicalVolume(QString name, G4Material* pNucleusMat) const {
	assert(pNucleusMat);

	auto convertToG4 = G4double(UnitSystemManager::getInstance()->getConversionToG4());
	auto* nucleusSolid = new G4Orb(name.toStdString(), getRadius()*convertToG4);

	assert(nucleusSolid);

	QString logicalVolName = "LV_" + name;

	return new G4LogicalVolume(nucleusSolid, pNucleusMat, logicalVolName.toStdString(), nullptr, nullptr, nullptr);
}

#endif

/// Get a random spot on the nucleus
template<>
Point_3 RoundNucleus<double, Point_3, Vector_3>::getSpotOn() const {
	return Utils::Geometry::Sphere::getSpotOnSphere(_radius, _origin);
}

/// Get a random spot on the nucleus
template<>
Point_2 RoundNucleus<double, Point_2, Vector_2>::getSpotOn() const {
	return Utils::Geometry::Circle::getSpotOnDisc(_radius, _origin);
}

/// Get a random spot in the nucleus
template<>
Point_3 RoundNucleus<double, Point_3, Vector_3>::getSpotIn() const {
	Vector_3 randDir(
		RandomEngineManager::getInstance()->randd(-1., 1.),
		RandomEngineManager::getInstance()->randd(-1., 1.),
		RandomEngineManager::getInstance()->randd(-1., 1.)
	);
	return Utils::Geometry::Sphere::getIntersecPtWithSphere( getOrigin(), getRadius(), randDir);
}

/// Get a random spot in the nucleus
template<>
Point_2 RoundNucleus<double, Point_2, Vector_2>::getSpotIn() const {
	Vector_2 randDir(
		RandomEngineManager::getInstance()->randd(-1., 1.),
		RandomEngineManager::getInstance()->randd(-1., 1.)
	);
	return Utils::Geometry::Circle::getIntersecPtWithCircle(getOrigin(), getRadius(), randDir);
}

/// Retuen a list of Point defining the shape
template<>
std::vector<Point_2> RoundNucleus<double, Point_2, Vector_2>::getShapePoints() const {
	return Utils::Geometry::Circle::getNPointOnCircle( getOrigin(), getRadius(), NUCLEUS_SPHERE_NB_POINT);
}

/// Retuen a list of Point defining the shape
template<>
std::vector<Point_3> RoundNucleus<double, Point_3, Vector_3>::getShapePoints() const {
	return Utils::Geometry::Sphere::getNPointOnSphere( getOrigin(), getRadius(), NUCLEUS_SPHERE_NB_POINT);
}

/// \cond
template<>
double RoundNucleus<double, Point_2, Vector_2>::getMeshVolume(MeshOutFormats::outputFormat) const {
	return -1.;
}
/// \endcond

template<>
double RoundNucleus<double, Point_3, Vector_3>::getMeshVolume(MeshOutFormats::outputFormat meshType) const {
	switch(meshType) {
		case MeshOutFormats::GDML:
		case MeshOutFormats::GEANT_4:
		case MeshOutFormats::GATE:
			/// in this case we export a perfect sphere
			return (4./3.* M_PI * (getRadius() * getRadius() * getRadius() ) );
		case MeshOutFormats::OFF:
		{
			auto const& lCellNucleusPoints = getShapePoints();
			Polyhedron_3 polyNucleus;
			CGAL::convex_hull_3(lCellNucleusPoints.begin(), lCellNucleusPoints.end(), polyNucleus);
			return Utils::myCGAL::getConvexPolyhedronVolume(&polyNucleus, getOrigin());
		}
		case MeshOutFormats::Unknow:
		default :
		{
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "unable to generate mesh statistics, unknow calculation methods", "RoundNucleus");
			return -1.;
		}
	}
}
