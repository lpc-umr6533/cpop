#include "File_Utils_TXT.hh"

namespace IO::TXT
{

std::ofstream* createTxtFileWithHeader(QString path) {
	auto* out = new std::ofstream(path.toStdString().c_str());
	return out;
}

}

