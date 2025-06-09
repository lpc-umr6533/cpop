#include "MessengerBase.hh"

#include "G4UIdirectory.hh"

namespace cpop {

G4String MessengerBase::make_directory(G4String base, G4String suffix) {
	G4String dir_name = base + "/" + suffix + "/";
	_dir = std::make_unique<G4UIdirectory>(dir_name); // switch to make unique when C++ 14 is available for G4/Gate

	return base + "/" + suffix;
}

}
