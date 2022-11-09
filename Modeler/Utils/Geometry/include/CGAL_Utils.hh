/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef CGAL_UTILS_HH
#define CGAL_UTILS_HH

#include <CGAL/Polyhedron_3.h>

#include "CPOP_Circle.hh"
#include "CPOP_Triangle.hh"
#include "File_Utils.hh"
#include "Mesh3DSettings.hh"

#include "Debug_Settings.hh"

#include <vector>
#include <map>
#include <set>

#include <QString>

//////////////////////////////////////////////////////////////////////////////
/// \brief all utils functions
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
namespace Utils
{
//////////////////////////////////////////////////////////////////////////////
/// \brief utils functions relative to CGAL developped for CPOP
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
namespace myCGAL
{
using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh3D;

/// \brief convert a vector of segment_3 to a set of poit_3
void convertVectOfSegToListOfPts(std::vector<Segment_3>&, std::set<Point_3>&);
/// \brief return the index of the longest arc circle between two points.
int getLongestAngle(Point_2 origin, std::map<Point_2, bool>& pointMap, std::vector<Point_2>& pts);
int getLongestSegment(std::map<Point_2, bool>& pointMap, std::vector<Point_2>& pts);
/// \brief return true if an intersection exists.
bool planeSegmentIntersection(Segment_3, Plane_3);
// --- BOUNDING BOXES
/// \brief rturn true if the point is on the Bounding box
bool isOnBoundingBox(K::Iso_rectangle_2 bb, Point_2 toCheck);
// --- POLYHEDRON MANAGEMENT
/// \brief return the number of facets of the polyhedron
int getNumberOfFacets(const Polyhedron_3*);
/// --- DISPLAY ENTITIES
/// \brief print the polyhedron on the output
void displayPolyhedron(Polyhedron_3*, std::map<Point_3, bool> = std::map<Point_3, bool>());
/// \brief print the CPOP_Triangle
void displayCPOP_Triangle(const CPOP_Triangle*);

/// \brief return a normalized Vector_3
template<typename Vector>
Vector normalize(Vector) {return Vector();}
template<>
Vector_2 normalize(Vector_2 v);
template<>
Vector_3 normalize(Vector_3 v);
/// \brief rotation function for a 3D vector
Vector_2 rotateVect2D(Vector_2 v, double signedAngle);

// return the absolute angle between the two vector.
double getAngle(Vector_2 v1, Vector_2  v2);
/// \brief return the signed angle between two vector.
double getSignedAngle(Vector_2 v1, Vector_2 v2);
/// \brief return a random point set on the facet
Point_3 getSpotOnFacet(Polyhedron_3::Facet);
Point_3 getSpotOnTriangle(const Triangle_3* pTri);

/// \brief return the volume for a given CONVEX polyhedron
double getConvexPolyhedronVolume(const Polyhedron_3*, Point_3);


/// \brief convert the given point to box of given width
vector<Point_2> convertPointToBox(Point_2 point, double width);

/// \brief convert the given point to box of given width
set<Point_3> convertPointToBox(Point_3 point, double width);
/// \brief option to save some operation made in CGAL_UTILS
void setDebugOutput(bool);

/// \brief convert a Point in CPOP unit to G4
Point_3 to_G4(const Point_3& point);
/// \brief convert a Point in G4 unit to CPOP
template <typename Point>
Point_3 to_CPOP(const Point& point){
    double conversionFrmG4ToCPOP = UnitSystemManager::getInstance()->getConversionFromG4();
    return Point_3(
                point.x()*conversionFrmG4ToCPOP,
                point.y()*conversionFrmG4ToCPOP,
                point.z()*conversionFrmG4ToCPOP
                );
}

}
}

#endif // CGAL_UTILS_HH
