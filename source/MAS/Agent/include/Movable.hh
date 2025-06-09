#ifndef MOVABLE_HH
#define MOVABLE_HH

/// \brief The movable class. Define the movement from a Vec3f representing the speed
/// @author Henri Payno
#include <utility>
template <typename Kernel, typename Point, typename Vector>
class Movable {
public:
	/// \brief constructor
	/// \param pDirection 	The intial direction of the movable
	/// \param pSpeed 		The initial speed
	/// \param pOrientation The initial orientation of the movable
	Movable(Vector pDirection = Vector(), Kernel pSpeed = Kernel(), Vector pOrientation = Vector()):
		_direction(pDirection), _speed(pSpeed), _orientation(pOrientation)
	{
	}

	virtual ~Movable() = default;

	/// \brief return the direction of the object
	[[nodiscard]] inline Vector const* getDirection() const { return &_direction; }
	/// \brief set the current direction of the Movable
	void setDirection(Vector pDir) { _direction = std::move(pDir); }
	/// \brief return the speed of the object ( in m/s)
	inline Kernel getSpeed() const { return _speed; }
	/// \brief the speed setter
	void setSpeed(Kernel pSpeed) { _speed = pSpeed; }
	/// \brief return the orientation of the object (in rad)
	[[nodiscard]] inline Vector const* getOrientation() const { return &_orientation; };
	/// \brief The orienattion setter
	void setOrientation(Vector pOrientation) { _orientation = pOrientation; };

	/// \brief direction setter
	void setForce(Vector pDir) { _direction = std::move(pDir); } // TODO bad variable?
	/// \brief reset the force on the movable
	void resetForce() { _actingForce = Vector(); }
	/// \brief add a force to the movable
	void addForce(Vector const& pNewForce) { _actingForce = _actingForce + pNewForce; }
	/// \brief Cmopute the new position according to the deltaT value.
	inline virtual void updatePosition(double deltaT) = 0;

protected:
	Vector _direction;    ///< The direction of the movable
	Kernel _speed;        ///< The movable speed (in m/s)
	Vector _orientation;  ///< The orientation of the movable
	Vector _actingForce;  ///< The force actually acting
};

#endif
