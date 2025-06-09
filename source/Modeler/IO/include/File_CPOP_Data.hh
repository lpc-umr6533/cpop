#ifndef FILE_CPOP_DATA_HH
#define FILE_CPOP_DATA_HH

#include "Writable.hh"
#include "EnvironmentSettings.hh"

#include <QString>

using namespace Settings::nEnvironment;

namespace IO::CPOP {

using namespace Settings::nCell;

/// \brief this can write/save any type of writable (environment, cell...)
/// \warning for now this function can at top save an environment with only one "level" of similated sub environment
void save(Writable*, QString path);

}

#endif
