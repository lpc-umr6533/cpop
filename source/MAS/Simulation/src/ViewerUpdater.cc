#include "ViewerUpdater.hh"

/// \brief signal listener than an update has been done (Viewers for example)
void ViewerUpdater::sl_updated() {
	emit si_updated();
}
