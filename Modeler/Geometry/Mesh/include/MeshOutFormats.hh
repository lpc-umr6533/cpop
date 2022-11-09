/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef MESH_OUT_FORMAT_HH
#define MESH_OUT_FORMAT_HH

#include <QString>

//////////////////////////////////////////////////////////////////////////////
/// \brief The known put mesh formats
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
namespace MeshOutFormats
{
	/// \brief output format handle by CPOP
	enum outputFormat
	{
		OFF,	///< \brief Object File Format
		GDML,	
		GEANT_4,
		GATE,
		Unknow	///< \warning Unknow foramt must always stay last
	};

	/// \brief return the format name based on is enum value
	QString getFormatName(outputFormat);
}

#endif // MESH_FORMAT_HH