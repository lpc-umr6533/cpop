/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef MOVABLE_HH
#define MOVABLE_HH

#include "Spatialable.hh"

//////////////////////////////////////////////////////////////////////////////
/// \brief The movable class. Define the movement from a Vec3f representing the speed
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point, typename Vector>
class Movable
{
public:
	///////////////////////////////////////////////////////////////////
	/// \brief constructor
	/// \param pDirection 	The intial direction of the movable
	/// \param pSpeed 		The initial speed
	/// \param pOrientation The initial orientation of the movable
	///////////////////////////////////////////////////////////////////
	Movable(Vector pDirection = Vector(), Kernel pSpeed = Kernel(), Vector pOrientation = Vector()):
		direction(pDirection), speed(pSpeed), orientation(pOrientation)
	{

	}	
	///////////////////////////////////////////////////////////////////
	/// \brief destructor
	///////////////////////////////////////////////////////////////////
	virtual ~Movable()
	{
		
	}

	/// \brief return the direction of the object
	inline Vector* getDirection() 						{return &direction; };
	/// \brief set the current direction of the Movable
	void setDirection(Vector pDir)						{ direction = pDir;}
	/// \brief return the speed of the object ( in m/s)
	inline Kernel getSpeed() const						{return speed; };
	/// \brief the speed setter	
	void setSpeed(Kernel pSpeed)						{speed = pSpeed;};
	/// \brief return the orientation of the object (in rad)
	inline Vector* getOrientation() const				{return &orientation; };
	/// \brief The orienattion setter
	void setOrientation(Vector pOrientation)			{orientation = pOrientation;};

	/// \brief direction setter
	void setForce(Vector pDir)							{direction = pDir;};	
	/// \brief reset the force on the movable
	void resetForce()									{ actingForce = Vector();};
	/// \brief add a force to the movable
	void addForce(Vector pNewForce)						{ actingForce = actingForce + pNewForce;};
	/// \brief Cmopute the new position according to the deltaT value.
	inline virtual void updatePosition(double deltaT) = 0;

protected:
	Vector direction;		///< The direction of the movable
	Kernel speed;			///< The movable speed (in m/s)
	Vector orientation;		///< The orientation of the movable 
	Vector actingForce;		///< The force actually acting
};

#endif // MOVABLE_HH