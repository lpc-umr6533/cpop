#include "ENucleusType.hh"

QString getNucleusTypeName(eNucleusType type) {
	switch(type) {
		case ROUND: return "ROUND";
		default:    return "Unknow";
	}
}
