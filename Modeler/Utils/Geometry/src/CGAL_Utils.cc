/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "CGAL_Utils.hh"
#include "File_Utils.hh"

#include "InformationSystemManager.hh"
#include "RandomEngineManager.hh"

#include <CGAL/Polyhedron_3.h>

#ifndef NDEBUG
#define DEBUG_CGAL_UTILS 1
#else
#define DEBUG_CGAL_UTILS 0	// must always stay at 0
#endif

#if ( defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) )
#define _USE_MATH_DEFINES
#include <math.h>
#endif

#include <iostream>

static std::ofstream* outputDebug = NULL;
static const QString nameOutDebug = "debugCGAL_Utils";

using namespace std;

namespace Utils
{
namespace myCGAL
{
////////////////////////////////////////////////////////////////////////////////
/// \param pPoly The polyhedron we want to know the number of facets for.
/// \return the number of facet contained on the polyhedron
////////////////////////////////////////////////////////////////////////////////
int getNumberOfFacets(const Polyhedron_3* pPoly)
{
    assert(pPoly);
    return std::distance( pPoly->facets_begin(), pPoly->facets_end());
}

////////////////////////////////////////////////////////////////////////////////
/// \param segments The segments to convert
/// \param points the set of points to store points issued from segments
////////////////////////////////////////////////////////////////////////////////
void convertVectOfSegToListOfPts(std::vector<Segment_3>& segments, std::set<Point_3>& points)
{
    std::vector<Segment_3>::const_iterator itSeg;
    for(itSeg = segments.begin(); itSeg != segments.end(); ++itSeg)
    {
        points.insert(itSeg->source());
        points.insert(itSeg->target());
    }
}

/// \warning points must be set ordered
/// TODO : if ok, créer une structure basée sur les segments qui sont
/// segmentable ou non et que l'on tri par longueur.
int getLongestSegment(std::map<Point_2, bool>& pointMap, std::vector<Point_2>& pts)
{
    int index = -1;
    double bestLength = -1.;
    for(unsigned int iPt = 0; iPt < pts.size(); iPt ++)
    {
        if(pointMap[pts[iPt]] == true)
        {
            unsigned int otherVertexSegment = iPt + 1;
            if(otherVertexSegment == pts.size()) otherVertexSegment = 0;

            if(pointMap[pts[otherVertexSegment]] == true)
            {
                if(squared_distance(pts[otherVertexSegment], pts[iPt]) > bestLength)
                {
                    bestLength = squared_distance(pts[otherVertexSegment], pts[iPt]);
                    index = iPt;
                }
            }
        }
    }
    return index;
}

#define MIN_DELTA_ANGLE 0.001
////////////////////////////////////////////////////////////////////////////////////////////////
/// \param origin The origin of the disc
/// \param pointMap The map of point and if we can refine on their neighborhood
/// \param pts The vector of point composing the shape of the cell
/// \return the index of the longest angle
////////////////////////////////////////////////////////////////////////////////////////////////
int getLongestAngle(Point_2 origin, std::map<Point_2, bool>& pointMap, std::vector<Point_2>& pts)
{
    double maxAngle = -1.;
    unsigned int index = -1;
    for(unsigned int iPt = 0; iPt < pts.size(); iPt ++)
    {
        unsigned int otherVertexSegment = iPt + 1;
        if(otherVertexSegment == pts.size()) otherVertexSegment = 0;

        // we don't affine only between two fix point.
        if((pointMap[pts[iPt]] == true) || (pointMap[pts[otherVertexSegment]] == true ))
        {
            double lAngle = getSignedAngle(Vector_2( pts[iPt] - origin), Vector_2( pts[otherVertexSegment] - origin));
            // std::cout << "langle : " << lAngle;
            {
                lAngle += 2.* M_PI;
            }

            if(lAngle > maxAngle)
            {
                maxAngle = lAngle;
                index = iPt;
            }
        }
    }

    if(maxAngle < MIN_DELTA_ANGLE)
        return -1;

    return index;
}

////////////////////////////////////////////////////////////////////////////////////////
/// \param bb The bounding box
/// \param toCheck The point to check
/// \return True if the point is inside the bounding box.
////////////////////////////////////////////////////////////////////////////////////////
bool isOnBoundingBox(K::Iso_rectangle_2 bb, Point_2 toCheck)
{
    return (bb.bounded_side(toCheck) != CGAL::ON_UNBOUNDED_SIDE);
}

/////////////////////////////////////////////////////////////////////////////////////////
/// \param pPoly The polyhedron to display
/// \param reffinement The map storing if the points can be reffine or not.
/////////////////////////////////////////////////////////////////////////////////////////
void displayPolyhedron(Polyhedron_3* pPoly, std::map<Point_3, bool> reffinement)
{
    Polyhedron_3::Point_iterator itPolyPts;
    for(itPolyPts = pPoly->points_begin(); itPolyPts != pPoly->points_end(); ++itPolyPts)
    {
        cout << "      " << *itPolyPts ;
        if(reffinement.find(*itPolyPts) != reffinement.end())
        {
            cout << (reffinement[*itPolyPts] ? " can " : " can' t ") << " reffine this points";
        }
        cout << std::endl;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
/// \param triangle The triangle to diaply
/////////////////////////////////////////////////////////////////////////////////////////
void displayCPOP_Triangle(const CPOP_Triangle* triangle)
{
    cout << "   " << triangle->getA() << endl;
    cout << "   " << triangle->getB() << endl;
    cout << "   " << triangle->getC() << endl;
}

/////////////////////////////////////////////////////////////////////////////////////////
/// \param v The vector to normalize
/// \return Vector_3 The vector normalized
/////////////////////////////////////////////////////////////////////////////////////////
template<>
Vector_3 normalize(Vector_3 v)
{
    double length = pow(v.squared_length(), 0.5);
    return Vector_3(
                v.x() / length,
                v.y() / length,
                v.z() / length
                );
}

/////////////////////////////////////////////////////////////////////////////////////////
/// \param v The vector to normalize
/// \return Vector_2 The vector normalized
/////////////////////////////////////////////////////////////////////////////////////////
template<>
Vector_2 normalize(Vector_2 v)
{
    double length = pow(v.squared_length(), 0.5);
    return Vector_2(
                v.x() / length,
                v.y() / length
                );
}

/////////////////////////////////////////////////////////////////////////////////////////
/// \param v in The vector to rotate.
/// \param signedAngle in The angle we want to rotate the vector of ( in radian)
/// \return Vector_2 The vector v rotated
/////////////////////////////////////////////////////////////////////////////////////////
Vector_2 rotateVect2D(Vector_2 v, double signedAngle)
{
    return Vector_2(
                v.x()*std::cos(signedAngle) - v.y()*std::sin(signedAngle),
                v.x()*std::sin(signedAngle) + v.y()*std::cos(signedAngle)
                );
}

/////////////////////////////////////////////////////////////////////////////////////////
/// \param v1 the first vector we want the angle for
/// \param v2 the second vector we want the angle for
/// \return double the angle on radian between the two vector. Not signed, in [0, pi]
/////////////////////////////////////////////////////////////////////////////////////////
double getAngle(Vector_2 v1, Vector_2 v2)
{
    v1 = v1 / std::sqrt( v1 * v1);
    v2 = v2 / std::sqrt( v2 * v2);
    return std::acos( v1 * v2 );
}

/////////////////////////////////////////////////////////////////////////////////////////
/// \param v1 the first vector we want the angle for
/// \param v2 the second vector we want the angle for
/// \return double the angle on radian between the two vector. Signed, in [-pi, pi]
/////////////////////////////////////////////////////////////////////////////////////////
double getSignedAngle(Vector_2 v1, Vector_2 v2)
{
    if( determinant(v1, v2) < 0)
    {
        return -getAngle(v1, v2);
    }else
    {
        return getAngle(v1, v2);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////
Point_3 getSpotOnTriangle(const Triangle_3* pTri)
{
    // we pick a random point on the triangle landmark
    double x 	= RandomEngineManager::getInstance()->randd(0., 1.);
    double y 	= RandomEngineManager::getInstance()->randd(0., 1.);

    if( (x+y) > 1.)
    {
        x = 1. -x;
        y = 1. -y;
    }

    Vector_3 X 	= pTri->vertex(1) - pTri->vertex(0);
    Vector_3 Y 	= pTri->vertex(2) - pTri->vertex(0);
    Point_3 res	= pTri->vertex(0) + x*X + y*Y;
    return res;
}
/////////////////////////////////////////////////////////////////////////////////////////
/// \param pFacet the facet from where we want to pick a point
/// \brief return a random point set on the facet
/////////////////////////////////////////////////////////////////////////////////////////
Point_3 getSpotOnFacet(Polyhedron_3::Facet pFacet)
{
    if(pFacet.is_triangle())
    {

        Point_3 p1 = pFacet.halfedge()->vertex()->point();
        Point_3 p2 = pFacet.halfedge()->next()->vertex()->point();
        Point_3 p3 = pFacet.halfedge()->next()->next()->vertex()->point();
        Triangle_3 lTri(p1, p2, p3);
        return getSpotOnTriangle(&lTri);
    }

    /// \todo :
    /// else pick a random point on the BB
    /// \todo : générer un point dans la BB et vérifier si celui-ci se trouve dans la facet ?
    // K::Iso_cuboid_3 bb = CGAL::bounding_box( pts.begin(), pts.end());
    /// check if inside, until not, repick a point
    return Point_3(0., 0., 0.);
}

/////////////////////////////////////////////////////////////////////////////////////////
/// \param poly The polyhedron we want the volume for
/// \param origin The origin of the polyhedron (the volume is the addition of tetrahedron recut of the polyhedron.
///							each tetrahedron will contain this point.)
/// \return double the volume of the polyhedron in um³
/////////////////////////////////////////////////////////////////////////////////////////
double getConvexPolyhedronVolume(const Polyhedron_3* poly, Point_3 origin)
{
    assert(poly);
    double volume = 0.;
    Polyhedron_3::Facet_const_iterator itFacet;
    for( itFacet = poly->facets_begin(); itFacet != poly->facets_end(); ++itFacet)
    {
        volume += CGAL::volume(	origin,
                                itFacet->halfedge()->vertex()->point(),
                                itFacet->halfedge()->next()->vertex()->point(),
                                itFacet->halfedge()->next()->next()->vertex()->point()
                                );
    }
    return volume;
}

////////////////////////////////////////////////////////////////////////////////////////////////:
/// \param point to convert in box (<=> the center of the box)
/// \param width width of the box
/// \return The box created
////////////////////////////////////////////////////////////////////////////////////////////////:
vector<Point_2> convertPointToBox(Point_2 point, double width)
{
    vector<Point_2> res;
    double halfWidth = width / 2.;

    res.push_back( Point_2( point.x() - halfWidth, point.y() + halfWidth )); // top 		left
    res.push_back( Point_2( point.x() + halfWidth, point.y() + halfWidth )); // top 		right
    res.push_back( Point_2( point.x() + halfWidth, point.y() - halfWidth )); // bottom 	right
    res.push_back( Point_2( point.x() - halfWidth, point.y() - halfWidth )); // bottom 	left

    return res;
}

////////////////////////////////////////////////////////////////////////////////////////////////:
/// \param point to convert in box (<=> the center of the box)
/// \param width width of the box
/// \return The box created
////////////////////////////////////////////////////////////////////////////////////////////////:
set<Point_3> convertPointToBox(Point_3 point, double width)
{
    set<Point_3> res;
    double halfWidth = width / 2.;

    res.insert( Point_3( point.x() - halfWidth, point.y() + halfWidth, point.z() - halfWidth )); // top 	left 	back
    res.insert( Point_3( point.x() + halfWidth, point.y() + halfWidth, point.z() - halfWidth )); // top 	right 	back
    res.insert( Point_3( point.x() - halfWidth, point.y() - halfWidth, point.z() - halfWidth )); // bottom 	left 	back
    res.insert( Point_3( point.x() + halfWidth, point.y() - halfWidth, point.z() - halfWidth )); // bottom 	right 	back

    res.insert( Point_3( point.x() - halfWidth, point.y() + halfWidth, point.z() + halfWidth )); // top 	left	front
    res.insert( Point_3( point.x() + halfWidth, point.y() + halfWidth, point.z() + halfWidth )); // top 	right	front
    res.insert( Point_3( point.x() - halfWidth, point.y() - halfWidth, point.z() + halfWidth )); // bottom 	left	front
    res.insert( Point_3( point.x() + halfWidth, point.y() - halfWidth, point.z() + halfWidth )); // bottom 	right	front

    return res;
}

////////////////////////////////////////////////////////////////////////////////////////////////:
/// \param pDebug true if we want to active debug
////////////////////////////////////////////////////////////////////////////////////////////////:
void setDebugOutput(bool pDebug)
{
    if(pDebug)
    {
        if(outputDebug)
        {
            outputDebug->close();
            delete outputDebug;
            outputDebug = NULL;
        }else
        {
            outputDebug = new std::ofstream(nameOutDebug.toStdString().c_str());
        }
    }else
    {
        if(outputDebug)
        {
            outputDebug->close();
            delete outputDebug;
            outputDebug = NULL;
        }
    }
}

Point_3 to_G4(const Point_3 &point)
{
    double conversionFrmCPOPToG4 = UnitSystemManager::getInstance()->getConversionToG4();
    return Point_3(
                point.x()*conversionFrmCPOPToG4,
                point.y()*conversionFrmCPOPToG4,
                point.z()*conversionFrmCPOPToG4
                );
}


} // end namespace myCGAL
} // end namespace Utils
