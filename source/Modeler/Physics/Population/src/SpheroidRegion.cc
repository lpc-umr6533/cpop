#include "SpheroidRegion.hh"

#include "CGAL_Utils.hh"
#include "RandomEngineManager.hh"

namespace cpop {

bool SpheroidRegion::isInRegion(const Settings::nCell::t_Cell_3 *cell) const {
	double squaredistToCenter = CGAL::squared_distance(Utils::myCGAL::to_G4(cell->getPosition()), _center);

	bool above_internal_radius = squaredistToCenter  >= _internalRadius*_internalRadius - 10E-9; // can be a bit below due to rounding errors
	bool below_external_radius = squaredistToCenter  <= _externalRadius*_externalRadius + 10E-9; // can be a bit above due to rounding errors

	return above_internal_radius && below_external_radius;
}

bool SpheroidRegion::isSampled(const Settings::nCell::t_Cell_3 *cell) const {
	auto found_cell = _sampledCells.find(cell);
	return isInRegion(cell) && found_cell != _sampledCells.end();
}

const std::unordered_set<const Settings::nCell::t_Cell_3 *> &SpheroidRegion::sample(int nSample) {
	if(_sampledCells.empty()) {
		if(nSample <= 0) { // No sampling : all cells are observed
			_sampledCells.insert(_cellsInRegion.begin(), _cellsInRegion.end());
		} else {
			int max_cells = _cellsInRegion.size();
			if(nSample > max_cells) {
				std::string msg = "Can not select " + std::to_string(nSample) + " cells because region contains " + std::to_string(max_cells) + " cells.\n";
				throw std::runtime_error(msg.c_str());
			}

			unsigned int indexCell = 0;
			while(_sampledCells.size() < nSample) {
				indexCell = RandomEngineManager::getInstance()->randi(0, max_cells -1);
				_sampledCells.insert(_cellsInRegion[indexCell]);
			}
		}
	}

	return _sampledCells;
}

Settings::Geometry::Point_3 SpheroidRegion::center() const {
	return _center;
}

double SpheroidRegion::internal_radius() const {
	return _internalRadius;
}

double SpheroidRegion::external_radius() const {
	return _externalRadius;
}

std::string SpheroidRegion::name() const {
	return _name;
}

std::vector<const Settings::nCell::t_Cell_3 *> const& SpheroidRegion::cells_in_region() const {
	return _cellsInRegion;
}

}
