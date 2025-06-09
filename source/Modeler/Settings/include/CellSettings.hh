#ifndef CELL_SETTINGS_HH
#define CELL_SETTINGS_HH

#include "Cell.hh"
#include "GeometrySettings.hh"
#include "RoundCell.hh"
#include "RoundNucleus.hh"

/// \brief cell settings
namespace Settings::nCell {

using namespace Settings::Geometry;

using t_Cell_2 = Cell<double, Point_2, Vector_2>;                 ///< \brief 2D Cell
using t_Cell_3 = Cell<double, Point_3, Vector_3>;                 ///< \brief 3D Cell

using t_RoundCell_2 = RoundCell<double, Point_2, Vector_2>;       ///< \brief 2D round cell
using t_RoundCell_3 = RoundCell<double, Point_3, Vector_3>;       ///< \brief 3D round cell

using t_Nucleus_2 = Nucleus<double, Point_2, Vector_2>;           ///< \brief 2D nucleus
using t_Nucleus_3 = Nucleus<double, Point_3, Vector_3>;           ///< \brief 3D nucleus

using t_RoundNucleus_2 = RoundNucleus<double, Point_2, Vector_2>; ///< \brief 2D Round nucleus
using t_RoundNucleus_3 = RoundNucleus<double, Point_3, Vector_3>; ///< \brief 3D Round nucleus

}

#endif
