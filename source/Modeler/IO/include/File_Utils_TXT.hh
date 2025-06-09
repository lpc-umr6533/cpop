#ifndef UTILS_TXT_HH
#define UTILS_TXT_HH

#include <fstream>
#include <QString>

namespace IO::TXT {

/// \brief create a standard .txt file with a predefined header for CPOP
std::ofstream* createTxtFileWithHeader(QString path);

}

#endif
