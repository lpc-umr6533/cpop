/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef SPATIAL_DELIMITATION_HH
#define SPATIAL_DELIMITATION_HH

#include "ActiveDelimitation.hh"
#include "ADistribution.hh"
#include "BoundingBox.hh"
#include "DistributionType.hh"
#include "Writable.hh"

#include <set>

template <typename Kernel, typename Point, typename Vector>
class ADistribution;

//////////////////////////////////////////////////////////////////////////////
/// \brief define a spatial delimitation.
/// The space definied is the space between those two parts : 
///		- the internal boundary/delimitation
///		- the external boundary/delimitation
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point, typename Vector>
class SpatialDelimitation : public Writable
{
public:
	/// \brief constructor
	SpatialDelimitation( 
		Point pOrigin = Point(),
		std::set<ActiveDelimitation<Kernel, Point, Vector> *> pExternal= std::set<ActiveDelimitation<Kernel, Point, Vector> *>(), 
		std::set<ActiveDelimitation<Kernel, Point, Vector> *> pInternal = std::set<ActiveDelimitation<Kernel, Point, Vector> *>());
	
	/// \brief destructor
	virtual ~SpatialDelimitation();

	/// \brief internal delimitation setter
	void setSpatialInternalDelimitation(std::set<ActiveDelimitation<Kernel, Point, Vector> *> pDelimitation)	{internalDelimitation = pDelimitation;};
	/// \brief internal delimitation getter
	std::set<ActiveDelimitation<Kernel, Point, Vector> *> getSpatialInternalDelimitation()						{return internalDelimitation;};

	/// \brief internal delimitation setter
	void setSpatialExternalDelimitation(std::set<ActiveDelimitation<Kernel, Point, Vector> *> pDelimitation)	{externalDelimitation = pDelimitation;};
	/// \brief internal delimitation getter
	std::set<ActiveDelimitation<Kernel, Point, Vector> *> getSpatialExternalDelimitation()						{return externalDelimitation;};

	/// \brief return a position inside this spatial delimitaion according to a distribution type.
	virtual Point getSpot(Distribution::DistributionType type) const = 0;

	/// \brief return true if the given point is inside the spatial delimitation
	virtual bool isIn(Point) const = 0;
	/// \brief print cell information (used also to save the cell on a .txt file)
	virtual void write(QXmlStreamWriter&) const = 0;

	/// \brief bounding box getter
	BoundingBox<Point> getBoundingBox() const { return boundingBox;}
	/// \brief spatialDelimitation origin getter
	void setOrigin( Point pOrigin)			{ origin = pOrigin; }
	/// \brief spatialDelimitation getter
	Point getOrigin() const 				{ return origin;}

protected:
	/// \brief function defining the bounding box of the world
	virtual BoundingBox<Point> createBoundingBox() = 0;

protected:
	Point origin;																///< \brief the center of the spatial delimitation
	std::set<ActiveDelimitation<Kernel, Point, Vector> *> internalDelimitation;	///< \brief the set of ActiveDelimitation composing the internal delimitation
	std::set<ActiveDelimitation<Kernel, Point, Vector> *> externalDelimitation; ///< \brief the set of ActiveDelimitation composing the external delimitation

	BoundingBox<Point> boundingBox;												///< \brief the minimal bounding box including the spatial delimitation

};


////////////////////// FUNCTION DEFINITIONS  ////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
/// \param pOrigin the center point (spatial anchor) of the spatial delimitation
/// \param pExternal the set of ActiveDelimitation composing the external delimitation
/// \param pInternal the set of ActiveDelimitation composing the internal delimitation
///////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point, typename Vector>
SpatialDelimitation<Kernel, Point, Vector>::SpatialDelimitation(
	Point pOrigin,
	std::set<ActiveDelimitation<Kernel, Point, Vector> *> pExternal, 
	std::set<ActiveDelimitation<Kernel, Point, Vector> *> pInternal):
	Writable(),
	origin(pOrigin),
	internalDelimitation(pInternal), 
	externalDelimitation(pExternal)
{
	
}

/////////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point, typename Vector>
SpatialDelimitation<Kernel, Point, Vector>::~SpatialDelimitation()
{
	typename std::set<ActiveDelimitation<Kernel, Point, Vector> *>::iterator it;
	for(it = internalDelimitation.begin(); it != internalDelimitation.end(); ++it)
	{
		if(*it)
		{
			delete (*it);
		}
	}

	for(it = externalDelimitation.begin(); it != externalDelimitation.end(); ++it)
	{
		if(*it)
		{
			delete (*it);
		}
	}
	internalDelimitation.clear();
	externalDelimitation.clear();
}


#endif