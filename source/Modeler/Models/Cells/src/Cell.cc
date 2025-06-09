#include "Cell.hh"

template<>
Dimension Cell<double, Point_2, Vector_2>::getDimension() const {
	return _2D;
}

template<>
Dimension Cell<double, Point_3, Vector_3>::getDimension() const {
	return _3D;
}

template<>
QString Cell<double, Point_2, Vector_2>::getPositionForStates() const {
	return QString(QString::number(getPosition().x())) +
		QString("\t" + QString::number(getPosition().y()));
}

template<>
QString Cell<double, Point_3, Vector_3>::getPositionForStates() const {
	return QString(QString::number(getPosition().x())) +
		QString("\t" + QString::number(getPosition().y())) +
		QString("\t" + QString::number(getPosition().z()));
}

template<>
QString Cell<double, Point_2, Vector_2>::getPositionFlag() const {
	return {"x_pos \ty_pos"};
}

template<>
QString Cell<double, Point_3, Vector_3>::getPositionFlag() const {
	return {"x_pos \ty_pos \tz_pos"};
}
