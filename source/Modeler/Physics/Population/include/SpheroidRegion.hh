#ifndef SPHEROIDREGION_HH
#define SPHEROIDREGION_HH

#include <unordered_set>

#include "Mesh3DSettings.hh"
#include "CellSettings.hh"

namespace cpop {

/*! \class SpheroidRegion
   * \brief Class representing a sphere region with an internal and external radius
   *
   *
   */
class SpheroidRegion
{
public:
    template <typename CellIt>
    SpheroidRegion(const std::string& name, const Settings::Geometry::Point_3& center , double internal_radius, double external_radius, CellIt begin, CellIt end);

    bool isInRegion(const Settings::nCell::t_Cell_3* cell) const;

    bool isSampled(const Settings::nCell::t_Cell_3* cell) const;
    const std::unordered_set<const Settings::nCell::t_Cell_3*>& sample(int nSample = -1);

    Settings::Geometry::Point_3 center() const;

    double internal_radius() const;

    double external_radius() const;

    std::string name() const;

    std::vector<const Settings::nCell::t_Cell_3 *> cells_in_region() const;

private:
    std::string name_; /*!< Region name*/
    Settings::Geometry::Point_3 center_; /*!< Region center*/
    double internal_radius_; /*!< Internal radius in G4 unit of the region */
    double external_radius_; /*!< External radius in G4 unit of the region */

    std::vector<const Settings::nCell::t_Cell_3*> cells_in_region_; /*!< Cells contained in the region*/
    std::unordered_set<const Settings::nCell::t_Cell_3*> sampled_cells_;   /*!< Sampled cells in the region*/
};

template<typename CellIt>
SpheroidRegion::SpheroidRegion(const std::string &name, const Settings::Geometry::Point_3 &center, double internal_radius, double external_radius, CellIt begin, CellIt end)
    :name_(name), center_(center), internal_radius_(internal_radius), external_radius_(external_radius)
{
    for(auto it = begin; it != end; ++it) {
        if(isInRegion(*it))
            cells_in_region_.push_back(*it);
    }
}

}
#endif // SPHEROIDREGION_HH
