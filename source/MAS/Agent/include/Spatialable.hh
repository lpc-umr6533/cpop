#ifndef SPATIALABLE_HH
#define SPATIALABLE_HH

#ifdef USE_EXPORT_KEYWORD
	export
#endif

/// \brief The spatialable class. Define the ppossibility to be localizated
/// and include on a SpatialDataStructure
/// @author Henri Payno
template <typename Kernel, typename Point, typename Vector>
class Spatialable {
public:
	/// \brief constructor
	/// \param pPosition 	The initial position
	/// \param pOrientation The inital orientation
	Spatialable(Point pPosition = Point(), Vector pOrientation = Vector()):
		_position(pPosition), _orientation(pOrientation)
	{
	}

	virtual ~Spatialable() = default;

	/// \brief  return the position of the object
	/// CGALPOINT_µ2 and Point_3 doesn't heritates from an interesting
	/// common class. We could use point_d but woul'd be heavier.
	[[nodiscard]] inline Point getPosition() const { return _position; }
	/// \brief position stter for the spatialable
	void setPosition(Point pPos) { _position = pPos; }
	/// \brief return the orientation of the spatialable
	[[nodiscard]] inline Vector const& getOrientation() const	{ return _orientation; }
	/// \brief orientation setter
	void setOrientation(Vector pOri) { _orientation = pOri; }

protected:
	/// \brief the position of the spatialable
	Point _position;
	/// \brief the orientation of the spatialable
	Vector _orientation;
};

#endif
