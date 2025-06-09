#ifndef GRID_ELEMENT_TRAITS_HH
#define GRID_ELEMENT_TRAITS_HH

#include <cassert>
#include <vector>

/// \brief  deifne grid element traits such as size on axis
/// @author Henri Payno
template <typename Kernel>
class GridElementTraits {
public:
	GridElementTraits(std::vector<Kernel>);

	/// \brief return the value on the requested axis
	Kernel getLengthOnAxis(unsigned int) const;

private:
	std::vector<Kernel> _sizes;	///< \brief size on each axis
};

/////////////////////////////////////// implementation ///////////////////////////////////

template <typename Kernel>
GridElementTraits<Kernel>::GridElementTraits(std::vector<Kernel> pSizes):
	_sizes(pSizes)
{
}

template <typename Kernel>
Kernel GridElementTraits<Kernel>::getLengthOnAxis(unsigned int pAxis) const {
	assert(pAxis < _sizes.size());
	if(pAxis >= _sizes.size()) {
		return {};
	} else {
		return _sizes[pAxis];
	}
}

#endif
