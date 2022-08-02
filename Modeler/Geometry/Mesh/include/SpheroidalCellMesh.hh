/*----------------------
Copyright (C): Henri Payno, Axel Delsol,
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/

#include<iostream>
#include<stdio.h>

#ifndef SPHEROIDAL_CELL_MESH_HH
#define SPHEROIDAL_CELL_MESH_HH

#include "CellMesh.hh"
#include "Voronoi_3D_Mesh.hh"

#include "CellSettings.hh"
#include "CellProperties.hh"
#include "SpheroidalCell.hh"
#include "Nucleus.hh"

#ifdef WITH_GEANT_4
	#include "G4LogicalVolume.hh"
	#include "G4PVPlacement.hh"
	#include "G4LogicalVolume.hh"
#endif // WITH_GEANT_4

using namespace Settings::nCell;
using namespace std;

#ifdef NDEBUG
	#define DEFAULT_OVER_LAP = 1;
#else
	#define DEFAULT_OVER_LAP = 0;
#endif

//////////////////////////////////////////////////////////////////////////////
/// \brief The specific mesher for the spheroidal cell.
/// \warning the export will probably modify the nucleus radius and the nucleus
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class SpheroidalCellMesh : public Voronoi_3D_Mesh, public CellMesh
{

public:
	/// \brief constructor
	SpheroidalCellMesh(unsigned int nbFacetPerCell, double delta, std::set<t_Cell_3* > pCells = std::set<t_Cell_3*>());
	/// \brief destructor
	virtual ~SpheroidalCellMesh();

	/// \brief the mesh exporter
	virtual int exportToFile(QString, MeshOutFormats::outputFormat, bool pDivided = false);
	/// \brief return curently the number of visible cell
	unsigned int getNumberOfVisibleCell()	{ return Voronoi_3D_Mesh::delaunay.number_of_vertices();};
	/// \brief generate all cell structures.
	virtual std::vector<SpheroidalCell*> generateMesh();

#if defined(WITH_GEANT_4) || defined(WITH_GDML_EXPORT)
	/// \brief export the configuration to a G4PVPlacement. The one returned is the "world"/top G4 entity
	virtual G4PVPlacement* convertToG4World(bool checkOverlaps = false, G4Material* pMaterialBetwwenCell = NULL,
		map<const G4LogicalVolume*, const t_Cell_3* > * mapCells		= NULL,
		map<const G4LogicalVolume*, const t_Nucleus_3*> * mapNuclei  	= NULL,
		bool exportNuclei = true);

	/// \brief return the bounding box englobing the spheroid. Without converting cell into G4 entities
	virtual G4LogicalVolume* getG4BoundingLogicalVolume( G4ThreeVector&, G4Material* pMaterialBetwwenCell = NULL);

	/// \brief export the configuration to a G4LogicalVolume. The one returned is the "world"/top G4 entity
	virtual G4LogicalVolume* convertToG4Logical( G4LogicalVolume* parent, bool checkOverlaps = false, G4Material* pMaterialBetwwenCell = NULL,
		map<const G4LogicalVolume*, const t_Cell_3* > * mapCells		= NULL,
		map<const G4LogicalVolume*, const t_Nucleus_3*> * mapNuclei  	= NULL,
		bool pExportNuclei = true);
#endif

protected:
	/// \brief export all cells on the same file
	virtual int exportToFileOff_undivided(QString, std::vector<SpheroidalCell*>* );
	/// \brief clean data structures
	virtual void clean();

#ifdef WITH_GDML_EXPORT
	/// \brief export to a GDML file
	virtual int exportToFileGDML(QString, std::vector<SpheroidalCell*>, bool);
#endif

};

#endif // SPHEROIDAL_CELL_MESH_HH
