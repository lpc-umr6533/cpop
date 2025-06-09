#include "Spheroid.hh"
#include "MeshFactory.hh"

static const unsigned int maxNbAgentPerNode = 2000;

/// \param error returned value accirding to well-proceedings of the mesh creation
/// \param pNbMaxFacetPerPoly the maximal number we want to have on a polyhedron.
/// \warning This value can be cross other because we stop mesh subdivision until this value is reach but a division increase the number of facet per 3
/// \param pDdeltaReffinement The minimal gain we want to have for the better subdivision
/// \return Mesh the mesh generated
template<>
t_Mesh_3* Spheroid<SimpleSpheroidalCell>::getMesh(int* error, unsigned int pNbMaxFacetPerPoly, double pDdeltaReffinement) {
	assert(_spheroidSubEnvironment);
	return MeshFactory::getInstance()->create_3DMesh(error, _spheroidSubEnvironment, MeshTypes::Round_Cell_Tesselation, pNbMaxFacetPerPoly, pDdeltaReffinement);
}

/// \return the bounding box including the spheroid
template<>
BoundingBox<Point_3> Spheroid<SimpleSpheroidalCell>::getBoundingBox() const {
	double maxRadius = -1;
	std::vector<Point_3> pts;

	std::set<Cell<double, Point_3, Vector_3>*>::iterator itCell;
	for(itCell = cells.begin(); itCell != cells.end(); ++itCell) {
		pts.push_back( (*itCell)->getPosition());
		if( dynamic_cast<SimpleSpheroidalCell*>(*itCell)->getRadius() > maxRadius)
			maxRadius = dynamic_cast<SimpleSpheroidalCell*>(*itCell)->getRadius();
	}

	K::Iso_cuboid_3 bb = CGAL::bounding_box( pts.begin(), pts.end());
	// add the potential next point from the voronoi cell and sphere shape
	return { 
		Point_3(bb.xmin()-maxRadius, bb.ymin()-maxRadius, bb.zmin()-maxRadius),
		Point_3(bb.xmax()+maxRadius, bb.ymax()+maxRadius, bb.zmax()+maxRadius)
	}; // for now weight correspond to the cell radius.
}

/// \return the spatial data structure used for this spheroid
template<>
t_SpatialDataStructure_3* Spheroid<SimpleSpheroidalCell>::getSpatialDataStructure() const {
	return new Delaunay_3D_SDS( "spatial sata structure for spheroid with SimpleSpheroidalCell");
}
