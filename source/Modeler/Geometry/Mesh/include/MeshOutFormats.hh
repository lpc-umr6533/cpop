#ifndef MESH_OUT_FORMAT_HH
#define MESH_OUT_FORMAT_HH

#include <QString>

/// \brief The known put mesh formats
/// @author Henri Payno
namespace MeshOutFormats {

/// \brief output format handle by CPOP
enum outputFormat {
	OFF,	///< \brief Object File Format
	GDML,
	GEANT_4,
	GATE,
	STL,
	Unknow	///< \warning Unknow foramt must always stay last
};

/// \brief return the format name based on is enum value
QString getFormatName(outputFormat);

}

#endif
