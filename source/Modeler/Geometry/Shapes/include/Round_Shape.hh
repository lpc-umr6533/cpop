#ifndef ROUND_SHAPE_HH
#define ROUND_SHAPE_HH

#include "Shape.hh"
#include "GeometrySettings.hh"

#include <InformationSystemManager.hh>

using namespace Settings::Geometry;

/// \brief The agent class : define a round shape
/// @author Henri Payno
template<typename Kernel, typename Point, typename Vector>
class Round_Shape : public Shape {
public:
	Round_Shape(Kernel = 0.);

	/// \brief called when require to draw the body
	void draw() const override;
	/// \brief radius getter of the sphere.
	[[nodiscard]] Kernel getRadius() const { return _radius; }
	/// \brief radius setter
	void setRadius(Kernel pRadius) { _radius = pRadius; }

private:
	Kernel _radius;	///< The radius of the sphere
};

/////////////////////////// FUNCTION DEFINITIONS ///////////////////////////

template<typename Kernel, typename Point, typename Vector>
Round_Shape<Kernel, Point, Vector>::Round_Shape(Kernel pRadius):
	Shape(ShapeTypes::ROUND),
	_radius(pRadius)
{
}

template<typename Kernel, typename Point, typename Vector>
void Round_Shape<Kernel, Point, Vector>::draw() const {
	std::string mess = "unable to draw shape for this king of template parameter";
	InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess, "Round shape");
}

/// 2D specialization
template<>
void Round_Shape<float, Point_2, Vector_2>::draw() const;

/// 3D specialization
template<>
void Round_Shape<float, Point_3, Vector_3>::draw() const;

#endif
