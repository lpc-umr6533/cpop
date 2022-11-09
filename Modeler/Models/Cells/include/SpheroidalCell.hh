/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef SPHEROIDAL_CELL_HH
#define SPHEROIDAL_CELL_HH

#include "RoundCell.hh"

#include "CellSettings.hh"
#include "Mesh3DSettings.hh"
#include "MeshOutFormats.hh"

#include<map>

using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh3D;
using namespace Settings::nCell;
using namespace std;
////////////////////////////////////////////////////////////////////////////
/// \brief A spheroidal cell is defined by her cell membrane represented by a deformable spheroid.
/// \details spheroidal cells contained n organelles.
/// @author Henri Payno
////////////////////////////////////////////////////////////////////////////
class SpheroidalCell : public RoundCell<double, Point_3, Vector_3>
{
	typedef Mesh3D::Polyhedron_3::Point_iterator shapePtIt;		///< \brief 3D polyhedron point iterator
	typedef Mesh3D::Polyhedron_3::Facet_iterator shapeFacetIt;	///< \brief 3D polyhedron facet iterator
public:
	/// \brief constructor	
	SpheroidalCell(const CellProperties*, Point_3 pOrigin, double pSpheroidRadius, double pMass, Mesh3D::Polyhedron_3 pMembraneShape);
	/// \brief destructor
	virtual ~SpheroidalCell();

	/// \brief membrane volume getter
	double getCytoplasmVolume() const;
	
	/// \brief iterator to the shape point begin
	shapePtIt shape_points_begin()				{ return shape->points_begin(); };
	/// \brief iterator to the shape point begin
	shapePtIt shape_points_end()				{ return shape->points_end(); };
	/// \brief shape getter
	Mesh3D::Polyhedron_3* getShape()			{ return shape; };
	
	/// \brief shape facet begin getter 
	shapeFacetIt shape_facets_begin()			{ return shape->facets_begin(); };
	/// \brief shape facet end getter 
	shapeFacetIt shape_facets_end()				{ return shape->facets_end(); };
	/// \brief return a random point on/inside the membrane
	virtual Point_3 getSpotOnCellMembrane() const;
	/// \brief return a random point inside the cytoplasm, avoiding membrane and nucleus
	virtual Point_3 getSpotOnCytoplasm() const;
	/// \brief return a random point on/inside the nucleus
	virtual Point_3 getSpotOnNuclei() const;
	/// \brief return a random point on/inside the nucleus
	virtual Point_3 getSpotInNuclei() const;
	/// \brief return true if the point is inside the cell
	virtual bool hasIn(Point_3) const;
	/// \brief will generate the nucleus shape
	virtual void generateNuclei(vector<Plane_3*>) = 0;	

	/// \brief return the position of the nucleus according to his type of nucleus position
	virtual Point_3 getNucleusCenter(eNucleusPosType nucleusPositionType) const = 0;
	/// \brief set the position of the nucleus according to his type of nucleus position
	virtual void setNucleusCenter() = 0;
	/// \brief return true if nuclei radius are coherent
	virtual bool checkNucleiRadius() const = 0;

	/// \brief reset the mesh
	virtual void resetMesh();
	/// \brief return the statistic about the current mesh
	QString getMeshStats(MeshOutFormats::outputFormat meshType) const;
	/// \brief return the cell description
	virtual QString getDescription() const = 0;
	/// \brief return the volume defined by the mesh
	double getMeshVolume(MeshOutFormats::outputFormat meshType) const;
	// \brief return the volume occupy by the cell mesh without nuclei meshes volumes
	double getCytoplasmMeshVolume(MeshOutFormats::outputFormat meshType) const;
	/// \brief return the volume occupy by the nuclei meshes
	double getNucleiMeshesSumVolume(MeshOutFormats::outputFormat meshType) const;
	/// \brief return the surface represented by the mesh
	double getMembraneMeshSurfaceArea() const 						{ return sumMembraneMeshArea;}
	/// \brief compute the mesh surface
	void computeMembraneSurfaceArea();
	/// \brief return true if the cell own a mesh
	virtual bool hasMesh() const ;

	/// \brief return true if the spatialable cross the bounding box
	virtual bool cross(const BoundingBox<Point_3>*) const;
	/// \brief return true if the point is in the bounding box
	virtual bool isIn(const BoundingBox<Point_3>*) const;	

#if defined(WITH_GEANT_4) || defined(WITH_GDML_EXPORT)
	/// \brief convert the membrane shape to a G4 entity
	virtual G4LogicalVolume* convertMembraneToG4(QString);
	/// \brief convert the cell geometries (including nuclei) to G4 geometries
	virtual G4PVPlacement* convertToG4Structure(
			G4LogicalVolume* pMother, 
			QString pName,
			bool checkOverLaps,
			const map<SpheroidalCell*, set<const SpheroidalCell*> >* pNeighbourCells,
			unsigned int pNbFacet,
			double pDeltaWin,
			map<const G4LogicalVolume*, const t_Cell_3* >* pCellMap = NULL,	
			map<const G4LogicalVolume*, const t_Nucleus_3*>* pNucleiMap = NULL,
			bool pExportNuclei = true);
#endif

protected:
	/// \brief The polyhedron representing the cell boundaries.
	Mesh3D::Polyhedron_3* shape;	

	/// \brief link sum of facet areas to facet
	/// used to obtain uniform spot on facet. 
	map<double, Triangle_3> areasToFacet;

	double sumMembraneMeshArea;	///< \brief the membrane mesh surface ( sum of all facet surfaces )

};

#endif // SPHEROIDAL_CELL_STRUCTURE_HH