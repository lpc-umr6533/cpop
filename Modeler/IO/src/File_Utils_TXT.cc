/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "File_Utils_TXT.hh"

namespace IO
{
	namespace TXT
	{
		////////////////////////////////////////////////////////////////////////////////////
		///
		////////////////////////////////////////////////////////////////////////////////////
		std::ofstream* createTxtFileWithHeader(QString path)
		{
			std::ofstream* out  = new std::ofstream(path.toStdString().c_str());
			return out;
		}
	}
}
