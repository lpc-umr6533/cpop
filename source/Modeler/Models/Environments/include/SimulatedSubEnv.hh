#ifndef SIMULATED_SUB_ENVIRONMENT_HH
#define SIMULATED_SUB_ENVIRONMENT_HH

#include "SpatialDelimitation.hh"

#include "Environment.hh"
#include "Portal.hh"
#include "SubEnvironment.hh"
#include "Utils_Basic_Color.hh"

#include <set>

template <typename Kernel, typename Point, typename Vector>
class SpatialDelimitation;

/// \brief this is one of the two kind of SubEnvironment. The SimulatedSubEnvironment as is
/// name notice it will be Simulate (agent contained will be executed by the MASPlatform ).
/// This object is in opposition of the MirroredSubEnvironment
/// @author Henri Payno
template <typename Kernel, typename Point, typename Vector>
class SimulatedSubEnv : public SubEnvironment<Kernel, Point, Vector>
{
	using t_WorldLayer = WorldLayer<Kernel, Point, Vector>;   ///< \brief world layer according to templates
	using t_Portal = Portal<Kernel, Point, Vector>;           ///< \brief portal according to templates

public:
	SimulatedSubEnv(
		Environment<Kernel, Point, Vector>* pParent,
		std::string pName,
		SpatialDelimitation<Kernel, Point, Vector>* pDelimitation,
		std::set<t_Portal *> pPortal = std::set<t_Portal *>()
	);

	/// \brief add a portal to the simulation
	void addPortal(t_Portal* portal);
	/// \brief remove a portal
	void removePortal(t_Portal*);

	/// \brief define the color of the delimitations
	void setDelimitationsColor(double*);
	void setDelimitationsColor(double, double, double);
	void setDelimitationsColor(std::vector<double>);

	/// \brief return the spatial delimitaion of the environment
	SpatialDelimitation<Kernel, Point, Vector>* getSpatialDelimitation() const { return _delimitation; };

	/// \brief draw the simulated sub environment
	void draw() const override;

	/// \brief print cell information (used also to save the cell on a .txt file)
	void write(QXmlStreamWriter&) const override;

private:
	SpatialDelimitation<Kernel, Point, Vector>* _delimitation;  ///< \brief the spatial delimitation of this sub environment.
	std::set<t_Portal*> _portals;                               ///< \brief all the portals contained on this simulated environments
};

/////////////////////////////////////// FUNCTION DEFINITIONS  ////////////////////////////////////////////

#include <cassert>

static constexpr char LAYER_DELIMITATION_NAME[] = "Delimitation";

/// \param pParent The parent of the Simulated SubEnv
/// \param pName The name of the Simulated SubEnv
/// \param pDelimitation initial spatial delimiation
/// \param pCPops The uniques cell populations.
/// \param pPortal The portals included on this environement.
template <typename Kernel, typename Point, typename Vector>
SimulatedSubEnv<Kernel, Point, Vector>::SimulatedSubEnv(
	Environment<Kernel, Point, Vector>* pParent,
	std::string pName,
	SpatialDelimitation<Kernel, Point, Vector>* pDelimitation,
	std::set<t_Portal*> pPortal) :
	SubEnvironment<Kernel, Point, Vector>(pParent, std::move(pName)),
	_delimitation(pDelimitation),
	_portals(pPortal)
{
	/// create the World Layer specifique to the spatial delimitation.
	Layer::addChild(new t_WorldLayer(LAYER_DELIMITATION_NAME, this));
	// internal
	auto lAD = _delimitation->getSpatialInternalDelimitation();
	for(auto itAD = lAD.begin(); itAD != lAD.end(); ++itAD)
		static_cast<t_WorldLayer*>(Layer::_childLayers[LAYER_DELIMITATION_NAME])->addAgent(*itAD);

	// internal
	lAD = _delimitation->getSpatialExternalDelimitation();
	for(auto itAD = lAD.begin(); itAD != lAD.end(); ++itAD)
		static_cast<t_WorldLayer*>(Layer::_childLayers[LAYER_DELIMITATION_NAME])->addAgent(*itAD);

	/// by default delimitaion layer is red
	setDelimitationsColor(Utils::Color::Red());
}

/// \param portal The portal to ad to the sub environement
template <typename Kernel, typename Point, typename Vector>
void SimulatedSubEnv<Kernel, Point, Vector>::addPortal(t_Portal* portal) {
	assert(portal);
	if(_portals.find(portal) == _portals.end())
		_portals.insert(portal);
}

///	\param portal The portal to remove
template <typename Kernel, typename Point, typename Vector>
void SimulatedSubEnv<Kernel, Point, Vector>::removePortal(t_Portal* portal) {
	assert(portal);
	if(_portals.find(portal) != _portals.end())
		_portals.erase(portal);
}

template <typename Kernel, typename Point, typename Vector>
void SimulatedSubEnv<Kernel, Point, Vector>::draw() const {
	SubEnvironment<Kernel, Point, Vector>::draw();
}

/// The colors are based on the rgb system
/// \param pColor The color to set
template <typename Kernel, typename Point, typename Vector>
void SimulatedSubEnv<Kernel, Point, Vector>::setDelimitationsColor(std::vector<double> pColor) {
	if(pColor.size() == 3)
		setDelimitationsColor(pColor[0], pColor[1], pColor[2]);
}

/// The colors are based on the rgb system
/// \param r The red factor from 0 .to 1.
/// \param g The green factor from 0 .to 1.
/// \param b The blue factor from 0 .to 1.
template <typename Kernel, typename Point, typename Vector>
void SimulatedSubEnv<Kernel, Point, Vector>::setDelimitationsColor(double r, double g, double b) {
	Layer::_childLayers[LAYER_DELIMITATION_NAME]->setColor(r, g, b);
}

/// The colors are based on the rgb system
/// \param pColor the color to set to the delimiation
template <typename Kernel, typename Point, typename Vector>
void SimulatedSubEnv<Kernel, Point, Vector>::setDelimitationsColor(double* pColor) {
	assert(pColor);
	Layer::_childLayers[LAYER_DELIMITATION_NAME]->setColor(pColor[0], pColor[1], pColor[2]);
}

/// \param writer the writer to redirect the information
template <typename Kernel, typename Point, typename Vector>
void SimulatedSubEnv<Kernel, Point, Vector>::write(QXmlStreamWriter& writer) const {
	/// write himself
	writer.writeStartElement(simulated_sub_env_flag);
	writer.writeAttribute(name_flag, QString::fromStdString(Layer::getName()));

	/// ask children to write themself
	auto const& childs = Layer::getChilds();
	for(auto const& child : childs) {
		if(dynamic_cast<Writable*>(child.second))
			dynamic_cast<Writable*>(child.second)->write(writer);
	}

	/// write spatial delimitation
	assert(_delimitation);
	_delimitation->write(writer);

	/// write Cell ID contained here
	{
		writer.writeStartElement(contained_agent_flag);
		std::set<Agent*> lAgents = Layer::getAgents();
		for(auto const& lAgent : lAgents)
			writer.writeTextElement(agent_ID_flag, QString::number(lAgent->getID()));
		writer.writeEndElement(); // "contained_agent"
	}

	writer.writeEndElement(); // "simulated_sub_env_flag"
}

#endif
