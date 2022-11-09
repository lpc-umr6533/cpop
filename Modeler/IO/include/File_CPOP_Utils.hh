/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef FILE_CPOP_UTILS_HH
#define FILE_CPOP_UTILS_HH

#include "GeometrySettings.hh"
#include "Writable.hh"

#include <QString>

//////////////////////////////////////////////////////////////////////////////
/// @namespace IO
///////////////////////////////////////////////////////////////////////////////
namespace IO
{
	//////////////////////////////////////////////////////////////////////////////
	/// @namespace CPOP
	//////////////////////////////////////////////////////////////////////////////		
	namespace CPOP
	{
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
}	

#endif // FILE_CPOP_UTILS_HH

