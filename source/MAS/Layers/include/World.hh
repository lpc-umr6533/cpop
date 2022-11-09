/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef WORLD_HH
#define WORLD_HH

#include <QString>

#include "Dimensioned_Layer.hh"
#include "ViewerUpdater.hh"

/////////////////////////////////////////////////////////////////////////////////
/// \brief The world calss, defining the agent's world.
/// A world is composed of an infinite number of layer.
/// A layer include agent.
/// An agent can be included on 0..n Layer.
/// Layers are use to display world and to assure relationship between simulation actors.
///	Heritate from Layer
/// World will delete agent, no worldLayer, He will not display agent in the contrary of WorldLayer 
/// World's agents will be set from is initialisation.
/// WorldLayer's agents can be add on the fly before world intialisation
/// @author Henri Payno
/////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
class World : public ViewerUpdater, public Dimensioned_Layer<Kernel, Point, Vector>
{
public:
	/// \brief constructor
	World(QString pName);
	/// \brief destructor
	virtual ~World();

};

///////////////////////////// FUNCTIONS DEFINITION /////////////////////////////////////

#include <InformationSystemManager.hh>

#ifndef NDEBUG
	#define DEBUG_WORLD 0
#else
	#define DEBUG_WORLD 0	// must always be set to 0
#endif
/////////////////////////////////////////////////////////////////////
/// \brief constructor
/////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
World<Kernel, Point, Vector>::World(QString pName):
	ViewerUpdater(),
	Dimensioned_Layer<Kernel, Point, Vector>(pName, NULL, Layer::ROOT)
{

}

/////////////////////////////////////////////////////////////////////
/// \brief destructor
/////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
World<Kernel, Point, Vector>::~World()
{
	/// Delete all agents
	std::set<Agent*> lAllAgents = Layer::getUniqueAgentsAndSubAgents();
	for(std::set<Agent*>::iterator itA = lAllAgents.begin(); itA != lAllAgents.end(); ++itA)
	{
		assert(*itA);
		if(*itA)
		{
			Agent* toDelete = *itA;
			delete toDelete;
			toDelete = NULL;
			// delete (*itA);
			// (*itA) = NULL;
		}
	}
}

#endif // WORLD_HH
