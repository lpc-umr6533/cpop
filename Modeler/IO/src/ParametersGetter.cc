/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "ParametersGetter.hh"

#include <QStringList>

namespace IO
{
	namespace Parameters
	{
		//////////////////////////////////////////////////////////////////////:
		///
		//////////////////////////////////////////////////////////////////////:
		QString getParameter(QString parameters, QString parameterRequested)
		{
			if(parameters.contains(parameterRequested, Qt::CaseInsensitive))
			{
				QStringList parameter = parameters.split(parameterRequested, QString::SkipEmptyParts, Qt::CaseInsensitive);
				parameter = parameter.back().split(")", QString::SkipEmptyParts);
				return parameter.front();
			}
			/// if not found return an empty string
			return QString();
		}

		//////////////////////////////////////////////////////////////////////:
		/// first parameter has index of 1
		//////////////////////////////////////////////////////////////////////:
		QString getParameterValue(QString parameter, unsigned int index)
		{
			// std::cout << " parameter requested : " << parameter.toStdString() << std::endl;
			parameter.replace("(", "");
			parameter.replace(")", "");
			parameter.replace("[", "");
			parameter.replace("]", "");
			parameter.replace("{", "");
			parameter.replace("}", "");
			parameter.replace(" ", "");
			QStringList values = parameter.split(",");

			if( index > (unsigned int)values.size())
			{
				return QString();
			}else
			{
				assert(values.size() >= (index-1));
				return values[index -1];
			}
		}

		//////////////////////////////////////////////////////////////////////:
		///
		//////////////////////////////////////////////////////////////////////:
		CellType getCellType(QString line)
		{
			int cellType = 0;
			while(static_cast<CellType>(cellType) != UNKNOW)
			{
				if(line.contains(
					getCellTypeName(static_cast<CellType>(cellType)), Qt::CaseInsensitive)
					)
				{
					return static_cast<CellType>(cellType);
				}
				cellType++;
			}
			return UNKNOW;
		}

		//////////////////////////////////////////////////////////////////////:
		///
		//////////////////////////////////////////////////////////////////////:
		template<>
		Point_2 findPosition(QString parameters)
		{

			QString parameter = getParameter(parameters, " position");
			if(parameter == QString())
			{
				return Point_2();
			}
			return Point_2(	getParameterValue(parameter, 1).toDouble(), 
							getParameterValue(parameter, 2).toDouble());
		}

		//////////////////////////////////////////////////////////////////////:
		///
		//////////////////////////////////////////////////////////////////////:
		template<>
		Point_3 findPosition(QString parameters)
		{
			QString parameter = getParameter(parameters, " position");
			if(parameter == QString())
			{
				return Point_3(0., 0., 0.);
			}
			return Point_3(	getParameterValue(parameter, 1).toDouble(), 
							getParameterValue(parameter, 2).toDouble(),
							getParameterValue(parameter, 3).toDouble());
		}
	}//namepsace parameters
} // namepsace IO
