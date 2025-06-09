#include "CPOP_SpectrumRange.hh"

#include <cassert>

CPOP_SpectrumRange::CPOP_SpectrumRange(G4double pProbaLowBound, G4double pProbaHighBound):
	_probaLowBound(pProbaLowBound),
	_probaHighBound(pProbaHighBound)
{
	assert(pProbaLowBound < pProbaHighBound);
}

bool CPOP_SpectrumRange::operator < (const CPOP_SpectrumRange& pSecond) {
	return (_probaHighBound < pSecond._probaHighBound);
}
