/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "Cell.hh"

///////////////////////////////////////////////////////////////////////////////////:::
// 				GetDimension
///////////////////////////////////////////////////////////////////////////////////:::
template<>
Dimension Cell<double, Point_2, Vector_2>::getDimension() const
{
	return _2D;
}

template<>
Dimension Cell<double, Point_3, Vector_3>::getDimension() const
{
	return _3D;
}



///////////////////////////////////////////////////////////////////////////////////:::
// 				getPositionForStates
///////////////////////////////////////////////////////////////////////////////////:::
template<>
QString Cell<double, Point_2, Vector_2>::getPositionForStates() const
{
	return QString(QString::number(getPosition().x())) 	+ 
		QString("\t" + QString::number(getPosition().y()));
}

template<>
QString Cell<double, Point_3, Vector_3>::getPositionForStates() const
{
	return QString(QString::number(getPosition().x())) 	+ 
		QString("\t" + QString::number(getPosition().y()))		+
		QString("\t" + QString::number(getPosition().z()));
}

///////////////////////////////////////////////////////////////////////////////////:::
// 				getPositionForStates
///////////////////////////////////////////////////////////////////////////////////:::
template<>
QString Cell<double, Point_2, Vector_2>::getPositionFlag() const
{
	return QString("x_pos \ty_pos");
}

template<>
QString Cell<double, Point_3, Vector_3>::getPositionFlag() const
{
	return QString("x_pos \ty_pos \tz_pos");
}