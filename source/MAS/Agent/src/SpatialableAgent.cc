#include "SpatialableAgent.hh"

/// \brief the 2D specialization for drawing
template<>
void SpatialableAgent<double, Point_2, Vector_2>::draw() const {
	if(_body) {
		glPushMatrix();
		glTranslatef(
			static_cast<GLfloat> (_position.x()),
			static_cast<GLfloat> (_position.y()),
			0.f
		);
		_body->draw();
		glPopMatrix();
	}
}

//// 3D
template<>
void SpatialableAgent<double, Point_3, Vector_3>::draw() const {
	if(_body) {
		glPushMatrix();
		glTranslatef(
			static_cast<GLfloat> (_position.x()),
			static_cast<GLfloat> (_position.y()),
			static_cast<GLfloat> (_position.z())
		);
		_body->draw();
		glPopMatrix();
	}
}

/// \brief the 2D specialization for drawing
template<>
bool SpatialableAgent<double, Point_2, Vector_2>::cross(const BoundingBox<Point_2>* pBB) const {
	Point_2 pt = this->getPosition();
	return (
		!isIn(pBB) &&
		(	( pt.x() <= pBB->getTopRight().x() && pt.x() >= pBB->getBottomLeft().x() ) &&
			( pt.y() <= pBB->getTopRight().y() && pt.y() >= pBB->getBottomLeft().y() )
		)
	);
}

//// 3D
template<>
bool SpatialableAgent<double, Point_3, Vector_3>::cross(const BoundingBox<Point_3>* pBB) const {
	Point_3 pt = this->getPosition();
	return (
		!isIn(pBB) &&
		(	( pt.x() <= pBB->getTopRight().x() && pt.x() >= pBB->getBottomLeft().x() )&&
			( pt.y() <= pBB->getTopRight().y() && pt.y() >= pBB->getBottomLeft().y() )&&
			( pt.z() <= pBB->getTopRight().z() && pt.z() >= pBB->getBottomLeft().z() )
		)
	);
}

/// stricklty inside
template<>
bool SpatialableAgent<double, Point_2, Vector_2>::isIn(const BoundingBox<Point_2>* pBB) const {
	Point_2 pt = this->getPosition();
	return (
		( pt.x() < pBB->getTopRight().x() && pt.x() > pBB->getBottomLeft().x() )&&
		( pt.y() < pBB->getTopRight().y() && pt.y() > pBB->getBottomLeft().y() )
	);
}

//// 3D
template<>
bool SpatialableAgent<double, Point_3, Vector_3>::isIn(const BoundingBox<Point_3>* pBB) const {
	Point_3 pt = this->getPosition();
	return (
		( pt.x() < pBB->getTopRight().x() && pt.x() > pBB->getBottomLeft().x() )&&
		( pt.y() < pBB->getTopRight().y() && pt.y() > pBB->getBottomLeft().y() )&&
		( pt.z() < pBB->getTopRight().z() && pt.z() > pBB->getBottomLeft().z() )
	);
}
