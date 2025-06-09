#ifndef ACTIVE_DELIMITATION_HH
#define ACTIVE_DELIMITATION_HH

#include "SpatialableAgent.hh"

/// \brief This active delimitation is a simple spatial delimitation (plane, sphere...) which is combined with a spatialable agent.
/// This relathionship permits active delimitation to be included on force simulation and to attrack/repulse other agent.
/// @author Henri Payno
template <typename Kernel, typename Point, typename Vector>
class ActiveDelimitation : public SpatialableAgent<Kernel, Point, Vector> {
	/// TODO : le simulation manager doit hierrachiser les agents et exécuter les Active délimitation à la fin pour réajuster la position des agents.
public:
	/// \param pAnimat The animation of the activeDelimitation
	/// \param pBody The body (normaly shape) of an active delimitation
	/// \param pPosition The position of the delimitation
	/// \param pOrientation The orientation of the agent
	ActiveDelimitation(
		Body* pBody,
		Point pPosition = Point(),
		Vector pOrientation = Vector()
	):
		SpatialableAgent<Kernel, Point, Vector>(pBody, pPosition, pOrientation)
	{}

	virtual ~ActiveDelimitation() = default;

	/// \brief the process called at each simulation iteration
	int exec() { return 0; }

	/// \brief function called before launching the agent on the simulation
	int init() { return 0; }

	/// \brief update agt position according to this spatial delimitation
	/// \details the idea is to make sur he is still on delimitation according to his previous and current position ( check crossing )
	virtual void updateAgtPosition(SpatialableAgent<Kernel, Point, Vector>) {};	/// TODO
};

#endif
