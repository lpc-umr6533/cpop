#include "Cell.hh"

template<>
Dimension Cell<double, Point_2, Vector_2>::getDimension() const {
	return _2D;
}

template<>
Dimension Cell<double, Point_3, Vector_3>::getDimension() const {
	return _3D;
}

template<>
std::string Cell<double, Point_2, Vector_2>::getPositionForStates() const {
	return std::to_string(getPosition().x()) + "\t" + std::to_string(getPosition().y());
}

template<>
std::string Cell<double, Point_3, Vector_3>::getPositionForStates() const {
	return std::to_string(getPosition().x()) +
		"\t" + std::to_string(getPosition().y()) +
		"\t" + std::to_string(getPosition().z());
}

template<>
std::string Cell<double, Point_2, Vector_2>::getPositionFlag() const {
	return {"x_pos \ty_pos"};
}

template<>
std::string Cell<double, Point_3, Vector_3>::getPositionFlag() const {
	return {"x_pos \ty_pos \tz_pos"};
}
