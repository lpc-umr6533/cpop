#ifndef SPHEROIDREGION_HH
#define SPHEROIDREGION_HH

#include <unordered_set>
#include <utility>

#include "CellSettings.hh"

namespace cpop {

/*! \class SpheroidRegion
   * \brief Class representing a sphere region with an internal and external radius
   */
class SpheroidRegion {
public:
	template <typename CellIt>
	SpheroidRegion(
		std::string name, const Settings::Geometry::Point_3& center , double internal_radius, double external_radius, CellIt begin, CellIt end
	);

	bool isInRegion(const Settings::nCell::t_Cell_3* cell) const;

	bool isSampled(const Settings::nCell::t_Cell_3* cell) const;
	const std::unordered_set<const Settings::nCell::t_Cell_3*>& sample(int nSample = -1);

	Settings::Geometry::Point_3 center() const;

	double internal_radius() const;

	double external_radius() const;

	std::string name() const;

	std::vector<const Settings::nCell::t_Cell_3 *> const& cells_in_region() const;

private:
	std::string _name; /*!< Region name*/
	Settings::Geometry::Point_3 _center; /*!< Region center*/
	double _internalRadius; /*!< Internal radius in G4 unit of the region */
	double _externalRadius; /*!< External radius in G4 unit of the region */

	std::vector<const Settings::nCell::t_Cell_3*> _cellsInRegion; /*!< Cells contained in the region*/
	std::unordered_set<const Settings::nCell::t_Cell_3*> _sampledCells;   /*!< Sampled cells in the region*/
};

template<typename CellIt>
SpheroidRegion::SpheroidRegion(
	std::string name, const Settings::Geometry::Point_3 &center, double internal_radius, double external_radius, CellIt begin, CellIt end
):
	_name(std::move(name)),
	_center(center),
	_internalRadius(internal_radius),
	_externalRadius(external_radius)
{
	for(auto it = begin; it != end; ++it) {
		if(isInRegion(*it))
			_cellsInRegion.push_back(*it);
	}
}

}

#endif
