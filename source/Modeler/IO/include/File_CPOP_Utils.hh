#ifndef FILE_CPOP_UTILS_HH
#define FILE_CPOP_UTILS_HH

#include "GeometrySettings.hh"
#include "Writable.hh"

#include <QString>

namespace IO::CPOP {

using namespace Settings::Geometry;
/// \brief write the vector on the given XML.
template<typename Point>
void writePoint(QString, QXmlStreamWriter&, Point)	{}

/// \brief write the vector on the given XML.
template<>
void writePoint(QString, QXmlStreamWriter&, Point_2);

/// \brief write the vector on the given XML.
template<>
void writePoint(QString, QXmlStreamWriter&, Point_3);

}

#endif

