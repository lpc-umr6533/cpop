/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef CPOP_SEGMENT_2_HH
#define CPOP_SEGMENT_2_HH

#include "GeometrySettings.hh"
#include "Mesh2DSettings.hh"

using namespace Settings::Geometry::Mesh2D;
using namespace Settings::Geometry;

//////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Define CPOP Segment 2. For mesh reffinement 
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////////////////////
class CPOP_Segment_2
{
public:
	/// \brief constructor
	CPOP_Segment_2(Segment_2 seg, bool refinable = true);
	/// \brief destructor
	~CPOP_Segment_2();
	
	/// \brief potential win getter
	double getPotentialWin() const;
	/// \brief segment getter
	Segment_2 getSegment() const	{return segment;};
	/// \brief inferior operator
	bool operator < (const CPOP_Segment_2& ) const;

	/// \brief source getter
	Point_2 source()	{ return segment.source();};
	/// \brief target getter
	Point_2 target()	{return segment.target();};	

private:
	Segment_2 segment;	///< \brief the segment
	bool refinable;		///< \brief is it interesting to refine this segment

};

#endif // CPOP_SEGMENT_2_HH