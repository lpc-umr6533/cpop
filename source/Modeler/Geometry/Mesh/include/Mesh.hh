#ifndef MESH_HH
#define MESH_HH

#include "MeshTypes.hh"
#include "MeshOutFormats.hh"
#include "Cell.hh"

#include <CGAL/IO/Color.h>

#ifdef WITH_GDML_EXPORT
	#define G4_LINK
#endif

#ifdef WITH_GEANT_4
	#define G4_LINK
#endif

#if ( defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) )
	#define is_nan _isnan
#else
	#define is_nan std::isnan
#endif

/// \brief The mesh contains points, edges ands faces
/// @author Henri Payno
template <typename Kernel, typename Point, typename Vector>
class Mesh {
	using t_Cell = Cell<Kernel, Point, Vector>;	///< \brief a cell manage by the Mesh
public:
	Mesh(MeshTypes::MeshType);
	virtual ~Mesh() = default;

	/// \brief mesh type getter
	MeshTypes::MeshType getMeshType() { return meshType; }
	/// \brief add a point to the mesh
	virtual bool add(t_Cell*) = 0;
	/// \brief remove a point to the mesh
	virtual void remove(t_Cell*) = 0;

	/// \brief the mesh exporter
	virtual int exportToFile(QString, MeshOutFormats::outputFormat, bool = false);
	/// \brief return the cell contained on the mesh
	virtual std::set<t_Cell*> getCells() const = 0;
	/// \brief return the cell contained on the mesh
	virtual std::set<t_Cell*> getCellsWithShape() const = 0;
	/// \brief add a markup point
	/// \param pPoint 	the point location to remove
	/// \param pColor 	the color used to potential display
	/// \param pWidth 	the potential bow width for display
	void addMarkUpPoint(Point pPoint, CGAL::IO::Color pColor = CGAL::IO::red(), double pWidth = 0.5) {
		markupPoints.insert(std::make_pair(pPoint, std::make_pair(pColor, pWidth)));
	};
	/// \brief remove a mark up point
	void removeMakUpPoint(Point pt);

private:
	MeshTypes::MeshType meshType; ///< the mesh type : Delaunay, voronoi...

protected:
	std::map<Point, std::pair<CGAL::IO::Color, double> > markupPoints;	///< Point mark by the user. Include the point, the color of the point and the length of the point
};

///////////////////////////////////// FUNCTION IMPLEMENTATION //////////////////////////////////////////
#include "InformationSystemManager.hh"

/// \param pMeshType The type of the mesh
template <typename Kernel, typename Point, typename Vector>
Mesh<Kernel, Point, Vector>::Mesh(MeshTypes::MeshType pMeshType ) :
	meshType(pMeshType)
{
}

/// \param pFileName 		The output file name of the mesh
/// \param pOutputformat 	The output format of the mesh
/// \param b 				true if we want to create a file for each cell
/// \return {return values :
///					- 0 : success
///					- 1 : not implemented yet
///				}
template <typename Kernel, typename Point, typename Vector>
int Mesh<Kernel, Point, Vector>::exportToFile(QString, MeshOutFormats::outputFormat, bool) {
	InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "export hasn't been implemented for this type of mesh yet", "Mesh");
	return 1;
}

/// \param pt the point location to remove
template <typename Kernel, typename Point, typename Vector>
void Mesh<Kernel, Point, Vector>::removeMakUpPoint(Point pt) {
	if(markupPoints.find(pt) != markupPoints.end())
		markupPoints.erase(pt);
}

#endif
