#ifndef MAS_PLATFORM_HH
#define MAS_PLATFORM_HH

#include "Agent.hh"
#include "ConflictSolver.hh"
#include "SimulationManager.hh"
#include "ViewerUpdater.hh"

#include <QList>
#include <QObject>

/// \brief Offer a multithreaded Multi Agent System platform allowing to execute agent and
/// synchronize at each step of the simulation.
/// @author Henri Payno
class MASPlatform: public QObject {
	Q_OBJECT
public:
	MASPlatform(QList<Agent*> pAgents = QList<Agent*>());
	~MASPlatform() override;

	/// \brief simulation starter
	int startSimulation();

	/// \brief define the world we are working on.
	bool connectViewerUpdater(ViewerUpdater*);

	/// \brief define the layer to simulate
	void setLayerToSimulate(Layer*);

	/// \brief define the simulation duration
	void setDuration(double);
	/// \brief set the duration of steps
	void setStepDuration(double);
	/// \brief displacementThreshold setter
	void setDisplacementThreshold(double pThreshold) const;
	/// \brief displacementThreshold getter
	[[nodiscard]] double getDisplacementThreshold() const;

	/// \brief set the duration of steps
	void setNbMaxThreads(int);

	/// \brief setter of the conflict solver on the simulation
	void addConflictSolver(ConflictSolver*);
	/// \brief setter of the conflict solver on the simulation
	void removeConflictSolver(ConflictSolver*);
	/// \brief add a participant to the MAS platform
	int addParticipant(Agent*);

	/// \brief we will execute randomly a limited number of agent 
	void limiteNbAgentToSimulate(unsigned int);
	/// \brief avoid limitation of agent, execute all agent
	void unlimiteNbAgentToSimulate(bool);

private: 
	/// \brief defined the agent to simulate from the layer
	bool initAgentToSimulate();
	/// \brief: The layer to simulate
	/// \warning on order to have a display update the layer to simulate must be part of the world
	Layer* _layerToSimulate;							

public slots:
	/// \brief slot called wwhen simulation start
	void sl_startSimulation() { startSimulation(); }
};

#endif
