/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef SPATIALABLE_HH
#define SPATIALABLE_HH

#ifdef USE_EXPORT_KEYWORD
	export
#endif

//////////////////////////////////////////////////////////////////////////////
/// \brief The spatialable class. Define the ppossibility to be localizated
/// and include on a SpatialDataStructure
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point, typename Vector>
class Spatialable
{
public:
	////////////////////////////////////////////////////////
	/// \brief constructor
	/// \param pPosition 	The initial position
	/// \param pOrientation The inital orientation
	////////////////////////////////////////////////////////
	Spatialable(Point pPosition = Point(), Vector pOrientation = Vector()):
		position(pPosition), orientation(pOrientation)
	{

	}

	////////////////////////////////////////////////////////
	/// \brief destructor
	////////////////////////////////////////////////////////
	virtual ~Spatialable()
	{

	};

	/// \brief  return the position of the object
	/// CGALPOINT_µ2 and Point_3 doesn't heritates from an interesting 
	/// common class. We could use point_d but woul'd be heavier.
	inline Point getPosition() const			{return position;}
	/// \brief position stter for the spatialable
	void setPosition(Point pPos)		{position = pPos;};
	/// \brief return the orientation of the spatialable
	inline Vector getOrientation()	const		{return orientation;};
	/// \brief orientation setter
	void setOrientation(Vector pOri)	{orientation = pOri;};

protected:
	/// \brief the position of the spatialable
	Point position;
	/// \brief the orientation of the spatialable
	Vector orientation;

};
#endif // SPATIALABLE_HH

