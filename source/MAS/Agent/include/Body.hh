#ifndef BODY_HH
#define BODY_HH

/// \brief The body class : define an agent body
/// @author Henri Payno
class Body {
public:
	Body(bool pCrossable = false, bool pDeformable = false):
		_crossable(pCrossable),
		_deformable(pDeformable)
	{}

	virtual ~Body() = default;

	/// \brief function called to draw the body
	virtual void draw() const = 0;

	/// \brief crossable setter
	void setCrossable(bool b) { _crossable = b; }
	/// \brief is the body crossable ?
	[[nodiscard]] inline bool isCrossable()	const	{return _crossable; }

	/// \brief deformable setter
	void setDeformable(bool b) { _deformable = b; }
	/// \brief is the Body deformable?
	[[nodiscard]] inline bool isDeformable() const { return _deformable; }

	/// \brief weight setter
	void setWeight(float pWeight)	{ _weight = pWeight; }
	/// \brief weight getter
	[[nodiscard]] inline double getWeight() const { return _weight; }

protected:
	bool _crossable;  ///< \brief boolean defining if other body can cross this body
	bool _deformable; ///< \brief boolean defining if other body can intersect this body

	double _weight;   ///< \brief The body weight ( in gramme ?)

};

#endif
