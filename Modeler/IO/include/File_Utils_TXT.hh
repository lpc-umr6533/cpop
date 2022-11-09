/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef UTILS_TXT_HH
#define UTILS_TXT_HH

#include <iostream>
#include <fstream>

#include <QString>

//////////////////////////////////////////////////////////////////////////////
/// @namespace IO
//////////////////////////////////////////////////////////////////////////////
namespace IO
{
	//////////////////////////////////////////////////////////////////////////////
	/// \brief describe file management for text files
	/// @author Henri Payno
	//////////////////////////////////////////////////////////////////////////////	
	namespace TXT
	{
		/// \brief create a standard .txt file with a predefined header for CPOP
		std::ofstream* createTxtFileWithHeader(QString path);
	}
}

#endif // UTILS_TXT_HH