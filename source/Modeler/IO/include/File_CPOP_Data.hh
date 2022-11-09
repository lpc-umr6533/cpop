/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef FILE_CPOP_DATA_HH
#define FILE_CPOP_DATA_HH

#include "CellSettings.hh"
#include "Writable.hh"
#include "EnvironmentSettings.hh"

#include <QString>
#include <set>

using namespace Settings::nEnvironment;

//////////////////////////////////////////////////////////////////////////////
/// \brief file management for Inputs and Outputs
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
namespace IO
{
	//////////////////////////////////////////////////////////////////////////////
	/// \brief Utils for the save population modeler
	/// @author Henri Payno
	//////////////////////////////////////////////////////////////////////////////	
	namespace CPOP
	{
		using namespace Settings::nCell;
		/// \brief this can write/save any type of writable (environment, cell...)
		/// \warning for now this function can at top save an environment with only one "level" of similated sub environment
		void save(Writable*, QString path);
	}
}

#endif // FILE_CPOP_DATA_HH