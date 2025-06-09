#include "CPOP_Segment_2.hh"

CPOP_Segment_2::CPOP_Segment_2(Segment_2 seg, bool pRefinable):
	_segment(seg),
	_refinable(pRefinable)
{
}

/// \brief return the potential win.
double CPOP_Segment_2::getPotentialWin() const {
	if(!_refinable) {
		return 0.;
	} else {
		/// this is used to know the mesh reffinement. This kind of segment ar between two point which are set on a arc circle.
		/// that we want to reffine.
		return _segment.squared_length();
	}
}

bool CPOP_Segment_2::operator < (const CPOP_Segment_2& segment2) const {
	return (this->getPotentialWin() < segment2.getPotentialWin());
}
