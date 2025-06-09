#include "ECellTypes.hh"

QString getCellTypeName(CellType type) {
	switch(type) {
		case SIMPLE_ROUND: return "SimpleRound";
		default : return "unknow";
	}
}
