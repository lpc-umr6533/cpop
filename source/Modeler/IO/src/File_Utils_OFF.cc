#include "File_Utils_OFF.hh"
#include "File_Utils.hh"
#include "Geometry_Utils_Sphere.hh"
#include "CGAL_Utils.hh"
#include "CellSettings.hh"

#include <fstream>

#include <CGAL/convex_hull_3.h>


namespace IO::OFF
{

using namespace Settings::nCell;
////////////////////////////////////////////////////////////////////////////////
/// \param itFacet The polyhedron facet to export
/// \param toExport The "output"
/// \param indexes The map of point to index for the export on .off
/// \param color The color to set to the facet
/// \return true if success
////////////////////////////////////////////////////////////////////////////////
/// \todo remove facet iterator to use a pointer
bool exportFacetToOff(Polyhedron_3::Facet_iterator itFacet, std::ofstream* toExport, std::map<Point_3, unsigned long int, comparePoint_3>& indexes, QString color) {
	// insert indexes if needed
	if(indexes.find(itFacet->halfedge()->vertex()->point()) == indexes.end())
		indexes.insert(std::pair<Point_3, unsigned long int>(itFacet->halfedge()->vertex()->point(), indexes.size()));

	*toExport	<< 3
		<< " " << indexes[itFacet->halfedge()->vertex()->point()]
		<< " " << indexes[itFacet->halfedge()->next()->vertex()->point()]
		<< " " << indexes[itFacet->halfedge()->next()->next()->vertex()->point()]
		<< " " << color.toStdString()
		<< std::endl;

	return true;
}

/// \param cell The cell to export
/// \param toExport The "output"
/// \param indexes The map of point to index for the export on .off
/// \param nucleusDegree The number of point we want to create on the nuclei. Not used if nuclei mesh is given on parameter
/// \param nucleusPoly The mesh of the nuclei if know. If setted, parameter nucleusDegree will be ignore
/// \return true if success
/// \todo : mesh must be const
bool exportSpheroidalCellToOff(SpheroidalCell* cell, std::ofstream* toExport, std::map<Point_3, unsigned long int, comparePoint_3>& indexes, std::vector< Polyhedron_3*> nucleusPoly) {
	// export shape
	if(!exportPolyhedronToOff(cell->getShape(), toExport, indexes, false, cell->getColor()))
		return false;

	// export nuclei
	/// case know meshes
	if(nucleusPoly.size() != 0) {
		for(auto const& itPoly : nucleusPoly) {
			if(!exportPolyhedronToOff(itPoly, toExport, indexes, false, cell->getColor()))
				return false;
		}
	} else {
		std::vector<t_Nucleus_3*> lNuclei = cell->getNuclei();
		for(auto const& itNucleus : lNuclei) {
			std::vector<Point_3> lCellNucleusPoints = itNucleus->getShapePoints();

			Polyhedron_3 polyNucleus;
			CGAL::convex_hull_3(lCellNucleusPoints.begin(), lCellNucleusPoints.end(), polyNucleus);

			if(!exportPolyhedronToOff(&polyNucleus, toExport, indexes, false, cell->getColor()))
				return false;
		}
	}

	return true;
}

/// \param poly The polyhedron to export
/// \param toExport The "output"
/// \param indexes The map of point to index for the export on .off
/// \param randomColor true if we want to set a random color to the polyhedron. Else take pColor
/// \param pColor The color to set to the mesh
/// \return true if success
/// TODO : set polyhedron to const
bool exportPolyhedronToOff(Polyhedron_3* poly, std::ofstream* toExport, std::map<Point_3, unsigned long int, comparePoint_3>& indexes, bool randomColor, CGAL::Color pColor) {
	QString color_associated = randomColor ? IO::exportColor(IO::getRandomColor()) : IO::exportColor(pColor);
	// export facets
	for(auto itFacet = poly->facets_begin(); itFacet != poly->facets_end(); ++itFacet)
		exportFacetToOff(itFacet, toExport, indexes, color_associated);

	return true;
}

/// \param pPath The path of the .off file
/// \return the ofstream created. Have to be deleted.
std::ofstream* createOffFileWithHeader(std::string pPath) {
	/// \todo : check pPath exists and end with .off
	auto* out  = new std::ofstream(pPath);
	*out << "OFF" << std::endl;
	*out << "# cells meshes from cpop";
	*out << std::endl;
	return out;
}

/// \warning if indexes not empty then al index included must be include from 0 to indexes.size()
/// otherwise this will not export well the mesh.
/// \return true if success
bool exportVerticesToOff(std::set<Point_3, comparePoint_3>& toExport, std::map<Point_3, unsigned long int, comparePoint_3>& indexes, std::ofstream* out) {
	unsigned long int id = indexes.size();
	for(const auto & it : toExport) {
		indexes.insert(std::pair<Point_3, unsigned long int>(it, id));
		*out << it << std::endl;
		id++;
	}

	*out << std::endl;

	return true;
}

/// \warning if indexes not empty then al index included must be include from 0 to indexes.size()
/// otherwise this will not export well the mesh.
/// \return true if success
bool exportVerticesToOff(std::set<Point_2>& toExport, std::map<Point_2, unsigned long int>& indexes, std::ofstream* out) {
	unsigned long int id  = indexes.size();
	for(auto it : toExport) {
		indexes.insert(std::pair<Point_2, unsigned long int>(it, id));
		*out << it << " 0.0" <<  std::endl;
		id++;
	}

	*out << std::endl;

	return true;
}

/// \warning if indexes not empty then al index included must be include from 0 to indexes.size()
/// otherwise this will not export well the mesh.
/// \return true if success
bool exportVertices(std::map<Point_3, unsigned long int, comparePoint_3>& indexes, std::ofstream* out) {
	std::map<unsigned long int, Point_3> secondMap;

	for(auto const& indexe : indexes) // increment ID and not nbExported
		secondMap.insert(std::pair<unsigned long int, Point_3>(indexe.second, indexe.first));

	// then export
	for(auto const& itSecondMap : secondMap)
		*out << itSecondMap.second << std::endl;
	*out << std::endl;

	return true;
}

}
