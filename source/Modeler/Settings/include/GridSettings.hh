#ifndef GRID_SETTINGS_HH
#define GRID_SETTINGS_HH

#include "Grid.hh"

/// \brief grid settings
namespace Settings::nGrid {

using namespace Settings::Geometry;

using t_Grid_2 = Grid<double, Point_2, Vector_2>;				///< \brief 2D Grid
using t_Grid_3 = Grid<double, Point_3, Vector_3>;				///< \brief 3D Grid
using t_GridElementTraits = GridElementTraits<double>;	///< \brief element traits used on grid
using t_GridElement_2 = GridElement<double, Point_2>;		///< \brief 2D Grid Element
using t_GridElement_3 = GridElement<double, Point_3>;		///< \brief 3D Grid Element

}

#endif
