#include "File_CPOP_Utils.hh"
#include "File_Utils_TXT.hh"

#include <QFile>

namespace IO::CPOP
{

template<>
void writePoint(QString pointID, QXmlStreamWriter& writer, Point_2 v) {
	writer.writeStartElement(pointID);
	writer.writeTextElement("x", QString::number(v.x()) );
	writer.writeTextElement("y", QString::number(v.y()) );
	writer.writeEndElement();
}

template<>
void writePoint(QString pointID, QXmlStreamWriter& writer, Point_3 v) {
	writer.writeStartElement(pointID);
	writer.writeTextElement("x", QString::number(v.x()) );
	writer.writeTextElement("y", QString::number(v.y()) );
	writer.writeTextElement("z", QString::number(v.z()) );
	writer.writeEndElement();
}

}
