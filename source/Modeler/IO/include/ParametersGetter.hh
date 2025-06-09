#ifndef PARAMETERS_GETTER_HH
#define PARAMETERS_GETTER_HH

#include "ECellTypes.hh"
#include <QString>

#include "GeometrySettings.hh"
using namespace Settings::Geometry;

namespace IO::Parameters {

/// \brief return the position from the ist of potential parameter
template<typename Point>
Point findPosition(QString);
/// \brief return the radius from the ist of potential parameter
template<typename Kernel>
Kernel findRadius(QString);
/// \brief return the nucleus radius from the ist of potential parameter
template<typename Kernel>
Kernel findNucleusRadius(QString);
/// \brief return the weight from the ist of potential parameter
template<typename Kernel>
Kernel findWeight(QString);
/// \brief return the parameter string for a requested parameter type
QString getParameter(QString, QString);
/// \brief QString return the next value for a parameter string
QString getParameterValue(QString, unsigned int);
/// \brief return the cell type requested
CellType getCellType(QString);
/// specialization
template<>
Point_2 findPosition(QString);
template<>
Point_3 findPosition(QString);

}

namespace IO::Parameters {

/// \warning parameter identifiant must always be preceded by a space.
template<typename Kernel, typename Point>
Point findPosition(QString parameters) {
	return {};
}

template<typename Kernel>
Kernel findRadius(QString parameters) {
	QString parameter = getParameter(parameters, " radius");
	if(parameter == "")
		return {};

	QString val = getParameterValue(parameter, 1);
	// std::cout << "value returned for radius is :" << val.toStdString() << std::endl;
	return Kernel(val.toDouble());
}

template<typename Kernel>
Kernel findNucleusRadius(QString parameters) {
	QString parameter = getParameter(parameters, " nucleusRadius");
	if(parameter == "")
		return {};

	QString val = getParameterValue(parameter, 1);
	// std::cout << "value returned for nucleus radius is :" << val.toStdString() << std::endl;
	return Kernel(val.toDouble());
}

template<typename Kernel>
Kernel findWeight(QString parameters) {
	QString parameter = getParameter(parameters, " weight");
	if(parameter == "")
		return {};

	QString val = getParameterValue(parameter, 1);
	return Kernel(val.toDouble());
}

}

#endif
