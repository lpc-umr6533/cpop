#ifndef DIMENSIONED_LAYER_HH
#define DIMENSIONED_LAYER_HH

#include "Layer.hh"
#include "SpatialableAgent.hh"
#include "SpatialDataStructureManager.hh"

/// \brief the Dimension recognized
/// \note we start at 0 to have the correspondance (index)(Dimension) == common denomination value <=> _3D == 3
enum Dimension {
	_0D,
	_1D,
	_2D,
	_3D,
	Unknow_D
};

/// \brief the Layer for a given dimension
/// @author Henri Payno
template<typename Kernel, typename Point, typename Vector>
class Dimensioned_Layer: public Layer {
public:
	using SpatialDataStructureKPV = SpatialDataStructure<Kernel, Point, Vector>;
	using SpatialDataStructureSet = std::set<SpatialDataStructureKPV*>;

public:
	Dimensioned_Layer(std::string, Layer* pParent = nullptr, LayerType pType = LEAF);

	/// \brief return the spatial data structures link to the layer
	[[nodiscard]] auto const& getSpatialDataStructures() const { return _spatialDataStructures; };

	/// \brief add a spatial dta structure to the layer
	bool addSpatialDataStructure(SpatialDataStructureKPV*);
	/// \brief remove the spatial data structure
	void removeSpatialDataStruture(SpatialDataStructureKPV*);

	/// \brief add a layer of this dimensioned layer.
	virtual bool addChild(Dimensioned_Layer*);
	/// \brief remove a layer of this dimensioned layer
	virtual void removeChild(Dimensioned_Layer*);
	/// \brief init the dimensioned layer
	void init() override;
	/// \brief return the dimension of the layer
	[[nodiscard]] Dimension getDimension() const;

protected:
	/// \brief initialize all spatial data structures
	virtual void initSpatialDataStructures();

	/// \brief  the list of available spatial data structure
	SpatialDataStructureSet _spatialDataStructures;
};

////////////////// FUNCTION IMPLEMENTATION ///////////////////////////////////////

#ifndef NDEBUG
	#define DEBUG_DIMENSIONED_LAYER 0
#else
	#define DEBUG_DIMENSIONED_LAYER 0	// must always be set at 0
#endif

/// constructor
template<typename Kernel, typename Point, typename Vector>
Dimensioned_Layer<Kernel, Point, Vector>::Dimensioned_Layer(std::string pName, Layer* pParent, LayerType pType):
	Layer(std::move(pName), pParent, pType)
{
}

/// This function will limit hte inclusion of child layer to the same
/// template paramter
template<typename Kernel, typename Point, typename Vector>
bool Dimensioned_Layer<Kernel, Point, Vector>::addChild(Dimensioned_Layer* pLayer) {
	assert(pLayer);
	Layer::addChild(static_cast<Layer*>(pLayer));
	return true;
}

/// \brief remove a child layer from the layer.
/// This function will limit hte inclusion of child layer to the same
/// template paramter
template<typename Kernel, typename Point, typename Vector>
void Dimensioned_Layer<Kernel, Point, Vector>::removeChild(Dimensioned_Layer* pLayer) {
	assert(pLayer);
	if(_childLayers.find(pLayer->getName()) != _childLayers.end())
		_childLayers.erase(pLayer->getName());

	if(_childLayers.size()<1)
		_layerType = LEAF;

	Layer::removeChild(static_cast<Layer*>(pLayer));
}

/// Include all agents contained on this layer inside
/// spatial data structures owned by this layer.
/// If the layer is iterative include also all agents from sub layers.
/// Reference spatial data structure to the SpatialDataStructureAnnuary
template<typename Kernel, typename Point, typename Vector>
void Dimensioned_Layer<Kernel, Point, Vector>::initSpatialDataStructures() {
	/// clean spatial data structure
	for(auto itSDS = _spatialDataStructures.begin(); itSDS != _spatialDataStructures.end(); ++itSDS) {
		// include agent inside this layer
		for(auto itAgent = _agents.begin(); itAgent != _agents.end(); ++itAgent) {
			auto* lSpatAgt = dynamic_cast<SpatialableAgent<Kernel, Point, Vector>*>(*itAgent);
			(*itSDS)->add(lSpatAgt);
		}

		SpatialDataStructureManager::getInstance()->makeRegistration((*itSDS));
	}

	// init sub layers SDS
	for(auto itLayer = _childLayers.begin(); itLayer != _childLayers.end(); ++itLayer) {
		auto* dimLayer = dynamic_cast<Dimensioned_Layer*>(itLayer->second);
		if(dimLayer)
			dimLayer->initSpatialDataStructures();
	}
}

template<typename Kernel, typename Point, typename Vector>
void Dimensioned_Layer<Kernel, Point, Vector>::init() {
	Layer::init();
	if(DEBUG_DIMENSIONED_LAYER) {
		std::string mess = "start initalisation for dimensioned layer " + getName();
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess, "Dimensioned Layer");
	}

	initSpatialDataStructures();

	if(DEBUG_DIMENSIONED_LAYER) {
		std::string mess = "initalisation done for dimensioned layer " + getName();
		mess += ". Add init : " + std::to_string(_spatialDataStructures.size()) + " spatial data structures.";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess, "Layer");
	}
}

/// \param pSDSToAdd The spatial data structure to add to the layer
/// \return return true if sucess
template<typename Kernel, typename Point, typename Vector>
bool Dimensioned_Layer<Kernel, Point, Vector>::addSpatialDataStructure(SpatialDataStructureKPV* pSDSToAdd) {
	assert(pSDSToAdd);

	// if doesn't exist yet
	if(_spatialDataStructures.find(pSDSToAdd) == _spatialDataStructures.end()) {
		_spatialDataStructures.insert(pSDSToAdd);
		/// register the spatial data structure to the SDS maanger.
		SpatialDataStructureManager::getInstance()->makeRegistration(pSDSToAdd);
		return true;
	}

	return false;
}

/// \param pSDSToRemove The spatial data structure to remove from the layer
template<typename Kernel, typename Point, typename Vector>
void Dimensioned_Layer<Kernel, Point, Vector>::removeSpatialDataStruture(SpatialDataStructureKPV* pSDSToRemove) {
	assert(pSDSToRemove);

	/// register the spatial data structure to the SDS manager.
	SpatialDataStructureManager::getInstance()->makeUnregistration(pSDSToRemove);
	_spatialDataStructures.erase(pSDSToRemove);
}

/// \return The dimension of the Layer ( 2D, 3D ...)
template<typename Kernel, typename Point, typename Vector>
Dimension Dimensioned_Layer<Kernel, Point, Vector>::getDimension() const {
	return Unknow_D;
}

/// \cond
template<>
inline Dimension Dimensioned_Layer<double, Point_2, Vector_2>::getDimension() const {
	return _2D;
}

template<>
inline Dimension Dimensioned_Layer<double, Point_3, Vector_3>::getDimension() const {
	return _3D;
}
/// \endcond

#endif
