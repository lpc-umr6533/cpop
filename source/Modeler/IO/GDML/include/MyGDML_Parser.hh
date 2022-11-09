/*----------------------
Copyright (C): Henri Payno, Axel Delsol,
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef MY_GDML_PARSER_HH
#define MY_GDML_PARSER_HH


#ifdef WITH_GEANT_4	// if using directly G4
	#include "G4PVPlacement.hh"
	#include <G4GDMLParser.hh>
#else			// else using our own G4 definition from CAD Mesh
	#include "geometry/volume/G4PVPlacement.hh"
	#include "persistency/G4GDMLParser.hh"
#endif


#include <QString>

////////////////////////////////////////////////////////////////////////////////
/// \brief create a top class wich will call the G4 GDML parser
////////////////////////////////////////////////////////////////////////////////
class MyGDML_Parser
{
public:
	/// \brief constructor
	MyGDML_Parser();
	/// \ßrief destructor
	~MyGDML_Parser();
	/// \brief write a G4World to a given file path
	bool write(QString path, G4PVPlacement* topWorld);
	/// \brief close the GDML
	void close();

private:
	G4GDMLParser parser;	///< \brief the parser
};

#endif
