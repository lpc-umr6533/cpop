#include "MeshOutFormats.hh"

namespace MeshOutFormats {

QString getFormatName(outputFormat pFormat) {
	switch(pFormat) {
		case OFF:
			return "OFF";
		case GDML:
			return "GDML";
		case GEANT_4:
			return "GEANT_4";			
		case GATE:
			return "GATE";
		default:
			return "Unknow";
	}
}

}
