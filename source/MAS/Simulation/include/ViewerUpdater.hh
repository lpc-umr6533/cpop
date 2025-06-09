#ifndef VIEWER_UPDATER_HH
#define VIEWER_UPDATER_HH

#include <QObject>
#include <QString>

#include "Layer.hh"

/// \brief The world calss, defining the agent's world.
/// \details
/// A world is composed of an infinite number of layer.
/// A layer include agent.
/// An agent can be included on 0..n Layer.
/// Layers are use to display world and to assure relationship between simulation actors.
///	Heritate from Layer
/// World will delete agent, no worldLayer, He will not display agent in the contrary of WorldLayer
/// World's agents will be set from is initialisation.
/// WorldLayer's agents can be add on the fly before world intialisation
/// @author Henri Payno
class ViewerUpdater : public QObject
{
	Q_OBJECT
	friend class MASPlatform;

public slots:
	void sl_updated();	///< \brief slot called when updated

signals:
	void si_updated();	///< signal send to the listener ( viewers )

};

#endif
