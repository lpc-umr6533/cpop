//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: G4GDMLParser.hh,v 1.58.2.1 2010/03/18 11:08:21 gcosmo Exp $
// GEANT4 tag $Name: geant4-09-03-patch-01 $
//
//
// class G4GDMLParser
//
// Class description:
//
// GDML main parser.

// History:
// - Created.                                  Zoltan Torzsok, November 2007
// -------------------------------------------------------------------------
 
#ifndef _G4GDMLPARSER_INCLUDED_
#define _G4GDMLPARSER_INCLUDED_

//BEGIN CAD-GDML
#include "../geometry/management/G4VPhysicalVolume.hh"
//END CAD-GDML

#include "G4GDMLReadStructure.hh"
#include "G4GDMLWriteStructure.hh"
///#include "G4STRead.hh"

#define G4GDML_DEFAULT_SCHEMALOCATION G4String("http://service-spi.web.cern.ch/service-spi/app/releases/GDML/schema/gdml.xsd")

class G4GDMLParser
{
  public:  // with description

   G4GDMLParser();
///   G4GDMLParser(G4GDMLReadStructure*);
///   G4GDMLParser(G4GDMLReadStructure*, G4GDMLWriteStructure*);
  ~G4GDMLParser();
     //
     // Parser constructors & destructor

   inline void Read(const G4String& filename, G4bool Validate=true);
    
     //
     // Imports a single GDML module, specified by the GDML filename
     // in input. Validation against schema is activated by default.

   inline void Write(const G4String& filename,
                     const G4VPhysicalVolume* const pvol = 0,
                           G4bool storeReferences = true,
                     const G4String& SchemaLocation = G4GDML_DEFAULT_SCHEMALOCATION);
   
   inline G4LogicalVolume* GetVolume(const G4String& name) const;
///
   inline G4VPhysicalVolume* GetWorldVolume(const G4String& setupName="Default") const;

  private:

   G4GDMLReadStructure* reader;
   G4GDMLWriteStructure* writer;
   G4bool urcode, uwcode;

   ///BEGIN CAD-GDML
	public:
	inline G4GDMLReadStructure* GetReader() const;
	///END CAD-GDML
};
   ///BEGIN CAD-GDML
inline 
G4GDMLReadStructure* G4GDMLParser::GetReader() const
{
  return reader;
}
	///END CAD-GDML

#include "G4GDMLParser.icc"

#endif
