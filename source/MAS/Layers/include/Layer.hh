#ifndef LAYER_HH
#define LAYER_HH

#include "Agent.hh"
#include "SpatialDataStructure.hh"

#include <QString>

#include <map>
#include <set>
#include <vector>

/// \brief Define a layer.
/// a layer handle mainly the display procedures.
/// Will be herited from World and WorldLayer.
/// World will delete agent, no worldLayer,
/// World's agents will be set from is initialisation.
/// WorldLayer's agents can be add on the fly before world intialisation
/// @author Henri Payno
class Layer {
public:
	/// \brief define the potential layer types
	enum LayerType {
		ROOT,	///< The root/top node,
		NODE, 	///< a node including leaf(s) and included inside node
		LEAF 	///< the bottom entity
	};

public:
	Layer(QString, Layer* parent = nullptr, LayerType pType = LEAF);
	virtual ~Layer();

	/// \brief alpha display parameter getter
	[[nodiscard]] float getAlpha() const { return _alpha; }
	/// \brief alpha display parameter setter
	void setAlpha(float pAlpha) { _alpha = pAlpha; }

	/// \brief return the layer name / ID
	[[nodiscard]] QString getName() const { return _name; }

	/// \brief display the layer and all the agent included
	virtual void draw() const;

	/// \brief return the list of agent contained inside the layer
	[[nodiscard]] inline virtual std::set<Agent*> getAgents() const { return _agents; }
	/// \brief return the number of agent contained on the world
	[[nodiscard]] inline unsigned long int getNbAgent() const	{ return (unsigned long int) _agents.size(); }
	/// \brief return the set of agents contained inside sub layer with a unicity constrain
	[[nodiscard]] std::set<Agent*> getUniqueSubAgents() const;
	/// \brief return all the contained agents from this layer to the sublayers
	[[nodiscard]] std::set<Agent*> getUniqueAgentsAndSubAgents() const;

	/// \brief return true if contains the layer requested
	virtual bool contains(Layer*) const;

	/// \brief set color for the draw from the rgb sytem.
	void setColor(float r, float g, float b);

	//// \brief function called to initialize agent inside a layer and make sure he his unique and execute once only.
	virtual void init();

	/// \brief layers getter
	[[nodiscard]] std::map<QString, Layer*> getChilds() const	{return _childLayers;};
	/// \brief layer getter from hisID
	[[nodiscard]] Layer* getChild(QString) const;
	/// \brief return the fisrst child of the layer in the alphabetical order, Null if none
	[[nodiscard]] Layer* getFirstChild() const;
	/// \brief return n agent randomly. We assure their unicity
	[[nodiscard]] std::set<Agent*> getNRandomAgent(unsigned int) const;
	/// \brief return one agent randomly
	[[nodiscard]] Agent* getOneRandomAgent() const;

	[[nodiscard]] float getColor(int index) const { return _color[index]; }

protected:
	/// \brief set of agent included on the layer.
	/// no setter because the WorldLayer will be able to add some but not the world
	std::set<Agent*> _agents;

	QString _name;                            ///< \brief the name of the layer
	float _alpha;                             ///< \brief the alpha parameter for the display
	std::map<QString, Layer*>	_childLayers;   ///< \brief layers included inside this layer
	std::vector<float> _color;		            ///< \brief the color of the layer for display

	LayerType _layerType;                     ///< \brief the position of the layer inside the layer tree

	/// \brief include all agents contained inside sub layer on this layer
	void includeSubLayersAgents();

	/// \brief adding a layer child
	bool addChild(Layer*);
	/// \brief removing a layer child
	void removeChild(Layer*);
};

#endif
