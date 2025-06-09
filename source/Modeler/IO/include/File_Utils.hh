#ifndef FILE_Utils_HH
#define FILE_Utils_HH

#include "GeometrySettings.hh"

#include <CGAL/IO/Color.h>
#include <QString>
#include <vector>

using namespace Settings::Geometry;

/// @namespace IO
namespace IO {

/// \brief export to QString the given color
QString exportColor(CGAL::Color pColor);
/// \brief return all point_3 stored on the given file
std::vector<Point_3> getPositions(QString);
/// \brief save the position to the given file name
void savePositions(QString pFile, const std::vector<Point_3>* pPositions);
/// \brief return a random color
CGAL::Color getRandomColor();

}

#endif
