#include "Utils_Basic_Color.hh"

static double _RED[3] = {1., 0., 0.};
static double _BLUE[3] = {0., 1., 0.};
static double _GREEN[3] = {0., 0., 1.};

namespace Utils::Color {

/// \brief the standard red color
double* Red()	{return _RED; }
/// \brief the standard blue color
double* Blue()	{return _BLUE; }
/// \brief the standard green color
double* Green()	{return _GREEN; }

}
