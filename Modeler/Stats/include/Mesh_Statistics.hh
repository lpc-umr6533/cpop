/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef MESH_STATISTICS_HH
#define MESH_STATISTICS_HH

#include "CellSettings.hh"
#include "MeshOutFormats.hh"

#include <iostream>
#include <vector>

using namespace std;
using namespace Settings::nCell;

//////////////////////////////////////////////////////////////////////////////
/// \brief general utils
/// \details extract data to the R software format : http://www.r-project.org/
/// 
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
namespace Statistics
{
    /// \brief export cell meshes statistics to the given ofstream
    bool generateMeshStats(vector<const t_Cell_3*> pCells, MeshOutFormats::outputFormat pFormat, ofstream* pCellOut=NULL, ofstream* pNucleiOut=NULL);
    /// \brief create header fot the cell statistic file
    void writeCellStatsHeader(MeshOutFormats::outputFormat pFormat, ofstream* pOut);
    /// \brief create header fot the cell nuclei statistic file
    void writeCellNucleiStatsHeader(MeshOutFormats::outputFormat pFormat, ofstream* pOut);
}

#endif // UTILS_HH
