#ifndef MESH_STATISTICS_HH
#define MESH_STATISTICS_HH

#include "CellSettings.hh"
#include "MeshOutFormats.hh"

#include <vector>

using namespace Settings::nCell;

/// \brief general utils
/// \details extract data to the R software format : http://www.r-project.org/
/// @author Henri Payno
namespace Statistics {

/// \brief export cell meshes statistics to the given ofstream
bool generateMeshStats(std::vector<const t_Cell_3*> pCells, MeshOutFormats::outputFormat pFormat, std::ofstream* pCellOut=nullptr, std::ofstream* pNucleiOut=nullptr);
/// \brief create header fot the cell statistic file
void writeCellStatsHeader(MeshOutFormats::outputFormat pFormat, std::ofstream* pOut);
/// \brief create header fot the cell nuclei statistic file
void writeCellNucleiStatsHeader(MeshOutFormats::outputFormat pFormat, std::ofstream* pOut);
}

#endif
