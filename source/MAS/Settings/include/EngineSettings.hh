#ifndef ENGINE_SETTINGS_HH
#define ENGINE_SETTINGS_HH

#include <QThread>

static const unsigned int INITIAL_MAX_THREAD = 12;
static const QThread::Priority SIMU_THREAD_PRIORITY = QThread::LowPriority;

#include "GeometrySettings.hh"
#include "SpatialDataStructure.hh"
#include "WorldLayer.hh"

/// @namespace Settings
namespace Settings {

/// \brief Geometries settngs
namespace SDS {

using t_SpatialDataStructure_2 = SpatialDataStructure<double, Point_2, Vector_2>;	///< \brief 2D spatial data structure
using t_SpatialDataStructure_3 = SpatialDataStructure<double, Point_3, Vector_3>;	///< \brief 3D spatial data structure

}

/// \brief Environement settings
namespace Env {

using t_WorldLayer_2 = WorldLayer<double, Point_2, Vector_2>;	///< \brief 2D world layer
using t_WorldLayer_3 = WorldLayer<double, Point_3, Vector_3>;	///< \brief 3D world layer

}

}

#endif
