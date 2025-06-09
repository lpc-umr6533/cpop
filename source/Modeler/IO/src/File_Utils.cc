#include "File_Utils.hh"
#include "RandomEngineManager.hh"

#include <QStringList>
#include <QTextStream>
#include <iostream>
#include <fstream>
#include <QFile>

namespace IO {

/// \param pColor the color to export
/// \return QString The random color exported 
QString exportColor(CGAL::Color pColor) {
	QString res =	QString::number(pColor.red()) + " " +
		QString::number(pColor.green()) + " " +
		QString::number(pColor.blue());
	return res;
}

std::vector<Point_3> getPositions(QString pFile) {
	std::vector<Point_3> points;

	QFile file(pFile); // this is a name of a file text1.txt sent from main method
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return points;

	QTextStream in(&file);
	while(!in.atEnd()) {
		QString line = in.readLine();    
		QStringList fields = line.split("\t");    
		assert(fields.size() == 3);
		if(fields.size() == 3) {
			points.emplace_back(
				fields[0].toDouble(),
				fields[1].toDouble(),
				fields[2].toDouble()
			);
		}  
	}
	return points;
}

void savePositions(QString pFile, const std::vector<Point_3>* pPositions) {
	std::ofstream output(pFile.toStdString());
	for(const auto& pPosition : *pPositions)
		output << pPosition.x() << "\t" << pPosition.y() << "\t" << pPosition.z() << std::endl;
	output.close();
}

/// \return CGAL::Color return a rando color
CGAL::Color getRandomColor() {
	return {
		static_cast<unsigned char>(RandomEngineManager::getInstance()->randi(0, 255)),
		static_cast<unsigned char>(RandomEngineManager::getInstance()->randi(0, 255)),
		static_cast<unsigned char>(RandomEngineManager::getInstance()->randi(0, 255))
	};
}

}
