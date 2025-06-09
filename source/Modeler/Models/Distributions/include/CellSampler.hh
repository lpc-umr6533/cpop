#ifndef CELL_SAMPLER_HH
#define CELL_SAMPLER_HH

#include "CellSettings.hh"
#include <set>

/// \brief interface for cell sampling
/// @author Henri Payno
template<typename CellType>
class CellSampler {
public:
	virtual ~CellSampler() = default;

	/// \brief function called to sample NCells;
	virtual std::set<const CellType* > sampleNCells(unsigned int) const = 0;
};

#endif
