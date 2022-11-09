/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef DIMENSIONED_LAYER_HH
#define DIMENSIONED_LAYER_HH

#include "Layer.hh"
#include "SpatialableAgent.hh"
#include "SpatialDataStructureManager.hh"

/// \brief the Dimension recognized
/// \note we start at 0 to have the correspondance (index)(Dimension) == common denomination value <=> _3D == 3
enum Dimension
{
	_0D,
	_1D,
	_2D,
	_3D,
	Unknow_D
};


//////////////////////////////////////////////////////////////////////////////
/// \brief the Layer for a given dimension
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
class Dimensioned_Layer : public Layer
{
public:
	/// \brief constructor
	Dimensioned_Layer(QString, Layer* pParent = NULL, LayerType pType = LEAF);
	/// \brief destructor
	virtual ~Dimensioned_Layer();

	/// \brief return the spatial data structures link to the layer
	std::set<SpatialDataStructure<Kernel, Point, Vector>*> getSpatialDataStructures() const	{ return spatialDataStructures;};

	/// \brief add a spatial dta structure to the layer
	bool addSpatialDataStructure(SpatialDataStructure<Kernel, Point, Vector>*);
	/// \brief remove the spatial data structure
	void removeSpatialDataStruture(SpatialDataStructure<Kernel, Point, Vector>*);

	/// \brief add a layer of this dimensioned layer.
	virtual bool addChild(Dimensioned_Layer<Kernel, Point, Vector>*);
	/// \brief remove a layer of this dimensioned layer
	virtual void removeChild(Dimensioned_Layer<Kernel, Point, Vector>*);
	/// \brief init the dimensioned layer
	virtual void init();
	/// \brief return the dimension of the layer
	Dimension getDimension() const;

protected:
	/// \brief initialize all spatial data structures
	virtual void initSpatialDataStructures();

	/// \brief  the list of available spatial data structure
	std::set<SpatialDataStructure<Kernel, Point, Vector>*> spatialDataStructures;
};

////////////////// FUNCTION IMPLEMENTATION ///////////////////////////////////////

#ifndef NDEBUG
	#define DEBUG_DIMENSIONED_LAYER 0
#else
	#define DEBUG_DIMENSIONED_LAYER 0	// must always be set at 0
#endif

/////////////////////////////////////////////////////////////////////
/// constructor
/////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
Dimensioned_Layer<Kernel, Point, Vector>::Dimensioned_Layer(QString pName, Layer* pParent, LayerType pType):
	Layer(pName, pParent, pType)
{

}

/////////////////////////////////////////////////////////////////////
/// destructor
/////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
Dimensioned_Layer<Kernel, Point, Vector>::~Dimensioned_Layer()
{

}

/////////////////////////////////////////////////////////////////////
/// This function will limit hte inclusion of child layer to the same
/// template paramter
/////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
bool Dimensioned_Layer<Kernel, Point, Vector>::addChild(Dimensioned_Layer<Kernel, Point, Vector>* pLayer)
{
	assert(pLayer);
	Layer::addChild(static_cast<Layer*>(pLayer));
	return true;
}

/////////////////////////////////////////////////////////////////////
/// \brief remove a child layer from the layer.
/// This function will limit hte inclusion of child layer to the same
/// template paramter
/////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
void Dimensioned_Layer<Kernel, Point, Vector>::removeChild(Dimensioned_Layer<Kernel, Point, Vector>* pLayer)
{
	assert(pLayer);
	if(childLayers.find(pLayer->getName()) != childLayers.end())
	{
		childLayers.erase(pLayer->getName());
	}
	if(childLayers.size()<1)
	{
		layerType = LEAF;
	}
	Layer::removeChild(static_cast<Layer*>(pLayer));
}

/////////////////////////////////////////////////////////////////////
/// Include all agents contained on this layer inside 
/// spatial data structures owned by this layer.
/// If the layer is iterative include also all agents from sub layers.
/// Reference spatial data structure to the SpatialDataStructureAnnuary
/////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
void Dimensioned_Layer<Kernel, Point, Vector>::initSpatialDataStructures()
{
	std::set<Agent*>::const_iterator itAgent;
	/// clean spatial data structure
	typename std::set<SpatialDataStructure<Kernel, Point, Vector>*>::iterator itSDS;
	for(itSDS = spatialDataStructures.begin(); itSDS != spatialDataStructures.end(); ++itSDS)
	{
		// include agent inside this layer
		for(itAgent = agents.begin(); itAgent != agents.end(); ++itAgent)
		{
			SpatialableAgent<Kernel, Point, Vector>* lSpatAgt = dynamic_cast<SpatialableAgent<Kernel, Point, Vector>*>(*itAgent);
			(*itSDS)->add(lSpatAgt);
		}
		SpatialDataStructureManager::getInstance()->makeRegistration((*itSDS));
	}

	// init sub layers SDS
	typename std::map<QString, Layer*>::const_iterator itLayer;
	for(itLayer = childLayers.begin(); itLayer != childLayers.end(); ++itLayer)
	{
		Dimensioned_Layer<Kernel, Point, Vector>* dimLayer = dynamic_cast<Dimensioned_Layer<Kernel, Point, Vector>* >(itLayer->second);
		if(dimLayer)
		{
			dimLayer->initSpatialDataStructures();
		}
	}
}

/////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
void Dimensioned_Layer<Kernel, Point, Vector>::init()
{
	Layer::init();
	if(DEBUG_DIMENSIONED_LAYER)
	{
		QString mess = "start initalisation for dimensioned layer " + getName();
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "Dimensioned Layer");
	}
	initSpatialDataStructures();
	if(DEBUG_DIMENSIONED_LAYER)
	{
		QString mess = "initalisation done for dimensioned layer " + getName();
		mess += ". Add init : " + QString::number(spatialDataStructures.size()) + " spatial data structures.";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "Layer");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pSDSToAdd The spatial data structure to add to the layer
/// \return return true if sucess
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
bool Dimensioned_Layer<Kernel, Point, Vector>::addSpatialDataStructure(SpatialDataStructure<Kernel, Point, Vector>* pSDSToAdd)
{
	assert(pSDSToAdd);
	
	// if doesn't exist yet
	if(spatialDataStructures.find(pSDSToAdd) == spatialDataStructures.end())
	{
		spatialDataStructures.insert(pSDSToAdd);
		/// register the spatial data structure to the SDS maanger.
		SpatialDataStructureManager::getInstance()->makeRegistration(pSDSToAdd);
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pSDSToRemove The spatial data structure to remove from the layer
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
void Dimensioned_Layer<Kernel, Point, Vector>::removeSpatialDataStruture(SpatialDataStructure<Kernel, Point, Vector>* pSDSToRemove)
{
	assert(pSDSToRemove);
	
	/// register the spatial data structure to the SDS manager.
	SpatialDataStructureManager::getInstance()->makeUnregistration(pSDSToRemove);
	spatialDataStructures.erase(pSDSToRemove);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \return The dimension of the Layer ( 2D, 3D ...)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
Dimension Dimensioned_Layer<Kernel, Point, Vector>::getDimension() const
{
	return Unknow_D;
}

/// \cond
template<>
Dimension Dimensioned_Layer<double, Point_2, Vector_2>::getDimension() const;

template<>
Dimension Dimensioned_Layer<double, Point_3, Vector_3>::getDimension() const;
/// \endcond
#endif // DIMENSIONED_LAYER_HH