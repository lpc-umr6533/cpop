#ifndef BOUNDING_BOX_HH
#define BOUNDING_BOX_HH

template <typename Point>
class BoundingBox {
public:
	BoundingBox(Point = Point(), Point = Point());

	Point getBottomLeft() const 	{ return _bottomLeft; }
	Point getTopRight() const 		{ return _topRight; }

private:
	Point _bottomLeft;  ///< \brief the back bottom left corner
	Point _topRight;    ///< \brief the front top right corner
};

template <typename Point>
BoundingBox<Point>::BoundingBox(Point pbottomLeft , Point pTopRight):
	_bottomLeft(pbottomLeft),
	_topRight(pTopRight)
{
}

#endif
