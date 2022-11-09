/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "File_Utils.hh"
#include "RandomEngineManager.hh"

#include <QStringList>
#include <QTextStream>
#include <iostream>
#include <fstream>
#include <QFile>

namespace IO
{
	/////////////////////////////////////////////////////////////////////////////////
	/// \param pColor the color to export
	/// \return QString The random color exported 
	/////////////////////////////////////////////////////////////////////////////////
	QString exportColor(CGAL::Color pColor)
	{
		QString res = 	QString::number(pColor.red()) 	+ " " +
						QString::number(pColor.green()) + " " +
						QString::number(pColor.blue());
		return res;		
	}

	/////////////////////////////////////////////////////////////////////////////////
	///
	/////////////////////////////////////////////////////////////////////////////////
	std::vector<Point_3> getPositions(QString pFile)
	{
		std::vector<Point_3> points;

		QFile file(pFile); // this is a name of a file text1.txt sent from main method
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
		    return points;
		}

		QTextStream in(&file);
		while(!in.atEnd()) {
		    QString line = in.readLine();    
		    QStringList fields = line.split("\t");    
		    assert(fields.size() == 3);
		    if(fields.size() == 3)
		    {
		    	points.push_back(
		    		Point_3( 	fields[0].toDouble(),
		    					fields[1].toDouble(),
		    					fields[2].toDouble() ) );
		    }  
		}
		return points;
	}

	/////////////////////////////////////////////////////////////////////////////////
	///
	/////////////////////////////////////////////////////////////////////////////////
	void savePositions(QString pFile, const std::vector<Point_3>* pPositions)
	{
		ofstream output(pFile.toStdString());
		std::vector<Point_3>::const_iterator itPos;
		for(itPos = pPositions->begin(); itPos != pPositions->end(); ++itPos)
		{
			output << itPos->x() << "\t" << itPos->y() << "\t" << itPos->z() << endl;
		}
		output.close();
	}
	
	/////////////////////////////////////////////////////////////////////////////////
	/// \return CGAL::Color return a rando color
	/////////////////////////////////////////////////////////////////////////////////
	CGAL::Color getRandomColor()
	{
		return CGAL::Color(
			RandomEngineManager::getInstance()->randi(0, 255),
			RandomEngineManager::getInstance()->randi(0, 255),
			RandomEngineManager::getInstance()->randi(0, 255) );
	}

}