/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef DISCOIDAL_CELL_MESH_HH
#define DISCOIDAL_CELL_MESH_HH

#include "Voronoi_2D_Mesh.hh"

#include "CellSettings.hh"
#include "CellMesh.hh"
#include "CellProperties.hh"
#include "DiscoidalCell.hh"
#include "Nucleus.hh"

using namespace Settings::nCell;

//////////////////////////////////////////////////////////////////////////////
/// \brief The specific mesher for the discoidal cell.
/// \warning the export will probably modify the nucleus radius and the nucleus
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class DiscoidalCellMesh : public Voronoi_2D_Mesh, public CellMesh
{
public:
	/// \brief constructor
	DiscoidalCellMesh(unsigned int nbFacetPerCell, double delta, std::set<t_Cell_2* > pCells = std::set<t_Cell_2*>());
	/// \brief destructor
	virtual ~DiscoidalCellMesh();
	
	/// \brief the mesh exporter
	virtual int exportToFile(QString, MeshOutFormats::outputFormat, bool pDivided = false);

	/// \brief export all cells on the same file
	virtual int exportToFileOff_undivided(QString pPath, std::vector<DiscoidalCell*>* );
	// /// \brief return the spheroidal cell structure.
	// virtual DiscoidalCell* getDiscoidalCell(Point_2 ptOrigin);
	/// \brief clean data structures
	virtual void clean();
	/// \brief get all polygons from the delaunay mesh.
	virtual std::vector<DiscoidalCell*> generateMesh();
	
};

#endif // DISCOIDAL_CELL_MESH_HH