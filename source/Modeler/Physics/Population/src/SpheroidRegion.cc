#include "SpheroidRegion.hh"

#include "CGAL_Utils.hh"
#include "RandomEngineManager.hh"
#include "Randomize.hh"

namespace cpop {

bool SpheroidRegion::isInRegion(const Settings::nCell::t_Cell_3 *cell) const
{
    double squaredistToCenter = CGAL::squared_distance(Utils::myCGAL::to_G4(cell->getPosition()), center_);

    bool above_internal_radius = squaredistToCenter  >= internal_radius_*internal_radius_ - 10E-9; // can be a bit below due to rounding errors
    bool below_external_radius = squaredistToCenter  <= external_radius_*external_radius_ + 10E-9; // can be a bit above due to rounding errors

    return above_internal_radius && below_external_radius;
}

bool SpheroidRegion::isSampled(const Settings::nCell::t_Cell_3 *cell) const
{
    auto found_cell = sampled_cells_.find(cell);

    return isInRegion(cell) && found_cell != sampled_cells_.end();
}

const std::unordered_set<const Settings::nCell::t_Cell_3 *> &SpheroidRegion::sample(int nSample)
{
    if(sampled_cells_.empty()) {

        if(nSample <= 0) { // No sampling : all cells are observed
            sampled_cells_.insert(cells_in_region_.begin(), cells_in_region_.end());
        } else {
            int max_cells = cells_in_region_.size();
            if(nSample > max_cells) {
                std::string msg = "Can not select " + std::to_string(nSample) + " cells because region contains " + std::to_string(max_cells) + " cells.\n";
                throw std::runtime_error(msg.c_str());
            }

            unsigned int indexCell = 0;
            while(sampled_cells_.size() < nSample) {
                indexCell = RandomEngineManager::getInstance()->randi(0, max_cells -1);
                sampled_cells_.insert(cells_in_region_[indexCell]);
            }

        }
    }

    return sampled_cells_;
}

Settings::Geometry::Point_3 SpheroidRegion::center() const
{
    return center_;
}

double SpheroidRegion::internal_radius() const
{
    return internal_radius_;
}

double SpheroidRegion::external_radius() const
{
    return external_radius_;
}

std::string SpheroidRegion::name() const
{
    return name_;
}

std::vector<const Settings::nCell::t_Cell_3 *> SpheroidRegion::cells_in_region() const
{
    return cells_in_region_;
}

}
