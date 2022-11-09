/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "File_CPOP_Utils.hh"
#include "File_Utils_TXT.hh"

#include <QFile>

namespace IO
{
	namespace CPOP
	{

		///////////////////////////////////////////////////////////////////////
		///
		///////////////////////////////////////////////////////////////////////
		template<>
		void writePoint(QString pointID, QXmlStreamWriter& writer, Point_2 v)
		{
			writer.writeStartElement(pointID);
			writer.writeTextElement("x", QString::number(v.x()) );
			writer.writeTextElement("y", QString::number(v.y()) );
			writer.writeEndElement();	
		}

		///////////////////////////////////////////////////////////////////////
		///
		///////////////////////////////////////////////////////////////////////
		template<>
		void writePoint(QString pointID, QXmlStreamWriter& writer, Point_3 v)
		{
			writer.writeStartElement(pointID);
			writer.writeTextElement("x", QString::number(v.x()) );
			writer.writeTextElement("y", QString::number(v.y()) );
			writer.writeTextElement("z", QString::number(v.z()) );
			writer.writeEndElement();	
		}
	
	} // namespace IO
} // namespace CPOP		