#ifndef CPOP_Triangle_HH
#define CPOP_Triangle_HH

#include "Mesh3DSettings.hh"

using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh3D;

/// \brief Define CPOP facet. Used to know if we can reffine her or no.
/// @author Henri Payno
class CPOP_Triangle {
public:
	CPOP_Triangle(Point_3 = Point_3(0., 0., 0.), Point_3 = Point_3(0., 0., 0.), Point_3 = Point_3(0., 0., 0.), bool reffinable = false);

	/// \brief set the first point
	void setA(Point_3 a) { _points[0] = a; }
	/// \brief set the second point of the facet
	void setB(Point_3 b) { _points[1] = b; }
	/// \brief set the third point of the facet
	void setC(Point_3 c) { _points[2] = c; }
	/// \brief point setter
	void set(unsigned int i, Point_3 pt);
	/// \brief return the first point
	[[nodiscard]] Point_3 getA() const	{ return _points[0]; }
	/// \brief return the second point of the facet
	[[nodiscard]] Point_3 getB() const	{ return _points[1]; }
	/// \brief return the third point of the facet
	[[nodiscard]] Point_3 getC() const 	{ return _points[2]; }
	/// \brief point getter
	[[nodiscard]] Point_3 get(unsigned int) const;

	/// \brief is the facet a reffinable one
	[[nodiscard]] bool canReffine() const {return reffinable;};
	/// \brief reffinement settter
	void setReffinement(bool b) {reffinable = b;};

	/// \brief comparator
	bool operator==(const CPOP_Triangle&) const;
	/// \brief comparator
	bool operator<(const CPOP_Triangle&) const;

	/// \brief return the barycenter of the facet
	[[nodiscard]] Point_3 getBarycenter() const;
	/// \brief potential win setter
	[[nodiscard]] double get16squareA() const;

private:
	std::vector<Point_3> _points; ///< \brief the list of vertex
	bool reffinable;              ///< \brief can we reffine this triangle.
};

#endif
