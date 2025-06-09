#ifndef CPOP_SEGMENT_2_HH
#define CPOP_SEGMENT_2_HH

#include "GeometrySettings.hh"
#include "Mesh2DSettings.hh"

using namespace Settings::Geometry::Mesh2D;
using namespace Settings::Geometry;

/// \brief Define CPOP Segment 2. For mesh reffinement
/// @author Henri Payno
class CPOP_Segment_2 {
public:
	CPOP_Segment_2(Segment_2 seg, bool refinable = true);

	/// \brief potential win getter
	[[nodiscard]] double getPotentialWin() const;
	/// \brief segment getter
	[[nodiscard]] Segment_2 getSegment() const { return _segment; }
	/// \brief inferior operator
	bool operator<(const CPOP_Segment_2&) const;

	/// \brief source getter
	Point_2 source() { return _segment.source(); }
	/// \brief target getter
	Point_2 target() {return _segment.target(); }

private:
	Segment_2 _segment; ///< \brief the segment
	bool _refinable;    ///< \brief is it interesting to refine this segment
};

#endif
