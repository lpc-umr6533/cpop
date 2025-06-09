#ifndef CELL_VARIABLE_ATTRIBUTE_HH
#define CELL_VARIABLE_ATTRIBUTE_HH

/// \brief a simple class to store a min and a max value inside
/// used for masses, volumes...
/// @author Henri Payno
template <class T> class CellVariableAttribute {
public:
	/// \brief consstructor
	CellVariableAttribute(T pMin, T pMax):
		_min{pMin},
		_max{pMax}
	{}

	/// \brief return the max value.
	[[nodiscard]] T var_max() const { return _max; }
	/// \brief return the min value.
	[[nodiscard]] T var_min() const { return _min; }

	/// \brief minimal value setter
	void setMin(T pMin)	{ _min = pMin; }

	/// \brief maximal value setter
	void setMax(T pMax)	{ _max = pMax; }

private:
	T _min;	///< \brief minimal value
	T _max;	///< \brief maximal value
};

#endif
