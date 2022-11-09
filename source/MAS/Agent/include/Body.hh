/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef BODY_HH
#define BODY_HH

//////////////////////////////////////////////////////////////////////////////
/// \brief The body class : define an agent body
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class Body
{
public:
	/// \brief constructor
	Body(bool pCrossable = false, bool pDeformable = false);
	/// \brief destructor
	virtual ~Body();

	/// \brief function called to draw the body 
	virtual void draw() const =0 ;

	/// \brief crossable setter
	void setCrossable(bool b)		{crossable = b;};
	/// \brief is the body crossable ?
	inline bool isCrossable()	const		{return crossable;};

	/// \brief deformable setter
	void setDeformable(bool b)		{deformable = b;};
	/// \brief is the Body deformable ?
	inline bool isDeformable()	const		{return deformable;};

	/// \brief weight setter
	void setWeight(float pWeight)	{weight = pWeight;};
	/// \brief weight getter
	inline double getWeight() const			{return weight;};
	
protected:
	bool crossable;		///< \brief boolean defining if other body can cross this body
	bool deformable;	///< \brief boolean defining if other body can intersect this body

	double weight;		///< \brief The body weight ( in gramme ?)

};

#endif // BODY_HH