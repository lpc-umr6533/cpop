#include "ENucleusPosType.hh"

/// \brief return the alpha nuemrical value of the nucleus position type
QString getNucleusPosTypeName(eNucleusPosType type) {
	switch(type) {
		case ORIGIN:      return "ORIGIN";
		case BARYCENTER:  return "BARYCENTER";
		default:          return "Unknow";
	}
}
