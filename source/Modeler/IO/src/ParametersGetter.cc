#include "ParametersGetter.hh"

#include <QStringList>

namespace IO::Parameters {

QString getParameter(QString parameters, QString parameterRequested) {
	if(parameters.contains(parameterRequested, Qt::CaseInsensitive)) {
		QStringList parameter = parameters.split(parameterRequested, Qt::SkipEmptyParts, Qt::CaseInsensitive);
		parameter = parameter.back().split(")", Qt::SkipEmptyParts);
		return parameter.front();
	}
	/// if not found return an empty string
	return {};
}

/// first parameter has index of 1
QString getParameterValue(QString parameter, unsigned int index) {
	// std::cout << " parameter requested : " << parameter.toStdString() << std::endl;
	parameter.replace("(", "");
	parameter.replace(")", "");
	parameter.replace("[", "");
	parameter.replace("]", "");
	parameter.replace("{", "");
	parameter.replace("}", "");
	parameter.replace(" ", "");
	QStringList values = parameter.split(",");

	if(index > (unsigned int)values.size()) {
		return {};
	} else {
		assert(values.size() >= (index-1));
		return values[index -1];
	}
}

CellType getCellType(QString line) {
	int cellType = 0;
	while(static_cast<CellType>(cellType) != UNKNOW) {
		if(line.contains(getCellTypeName(static_cast<CellType>(cellType)), Qt::CaseInsensitive))
			return static_cast<CellType>(cellType);
		cellType++;
	}

	return UNKNOW;
}

template<>
Point_2 findPosition(QString parameters) {
	QString parameter = getParameter(parameters, " position");
	if(parameter == QString())
		return {};
	return {
		getParameterValue(parameter, 1).toDouble(),
		getParameterValue(parameter, 2).toDouble()
	};
}

template<>
Point_3 findPosition(QString parameters) {
	QString parameter = getParameter(parameters, " position");
	if(parameter == QString())
		return {0., 0., 0.};

	return {
		getParameterValue(parameter, 1).toDouble(),
		getParameterValue(parameter, 2).toDouble(),
		getParameterValue(parameter, 3).toDouble()
	};
}

}
