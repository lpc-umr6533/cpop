/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef SUB_ENVIRONMENT_HH
#define SUB_ENVIRONMENT_HH

#include "ActiveDelimitation.hh"
#include "Environment.hh"
#include "WorldLayer.hh"
#include "Writable.hh"

#include <QString>

//////////////////////////////////////////////////////////////////////////////////////
/// \brief enumeration of option for the getAgent() function
//////////////////////////////////////////////////////////////////////////////////////
enum GET_AGENT_OPTIONS
{
	WITH_DELIMITATION,		/// all agent type
	WITHOUT_DELIMITATION	/// all agent exec
};

template<typename Kernel, typename Point, typename Vector>
class Environment;

/////////////////////////////////////////////////////////////////////////////////////
/// \brief Sub environment are object containing cells. This object is spatially limited by their SpatialDelimitation.
/// \details on the future this object should be able to contain object such as portal to curl in itself
/// @author Henri Payno
/////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
class SubEnvironment : public WorldLayer<Kernel, Point, Vector>, public Writable
{
	using Layer::getAgents; // overload

public:
	/// \brief constructor
	SubEnvironment(Environment<Kernel, Point, Vector>* pParent, QString name);
	/// \brief destructor
	virtual ~SubEnvironment();
	/// \brief the parent getter, return the environement he is part of.
	Environment<Kernel, Point, Vector>* getParent() const	{ return parent;};
	/// \brief return the list of agent contained on the sub environment
	virtual std::set<Agent*> getAgents(GET_AGENT_OPTIONS o = WITHOUT_DELIMITATION) const;
	/// \brief draw the sub environement
	virtual void draw() const;
	/// \brief  export all information needed to sumarize the Writable
	virtual	void write(QXmlStreamWriter& ) const = 0;
protected:
	Environment<Kernel, Point, Vector>* parent;				///< \brief the environment he si part of.
};

//////////////////////// FUNCTION DEFINITION ////////////////

//////////////////////////////////////////////////////////////////////////////////////
/// \param pParent the parent of this subEnvironment
/// \param pName The name of the creted SubEnvironment
//////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
SubEnvironment<Kernel, Point, Vector>::SubEnvironment(Environment<Kernel, Point, Vector>* pParent, QString pName):
	WorldLayer<Kernel, Point, Vector>(pName, pParent), 
	Writable(),
	parent(pParent)
{
	
}

//////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>	
SubEnvironment<Kernel, Point, Vector>::~SubEnvironment()
{

}

//////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
void SubEnvironment<Kernel, Point, Vector>::draw() const
{
	WorldLayer<Kernel, Point, Vector>::draw();	
}

//////////////////////////////////////////////////////////////////////////////////////
/// \param option which kind of Agent we want to get
//////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
std::set<Agent*> SubEnvironment<Kernel, Point, Vector>::getAgents(GET_AGENT_OPTIONS option) const
{
	switch(option)
	{
		case WITH_DELIMITATION:
		{
			return Layer::getAgents();
		}
		case WITHOUT_DELIMITATION:
		{
			std::set<Agent*> agentLessDelimitations;
			std::set<Agent*>::iterator itAgt;
			for(itAgt = Layer::agents.begin(); itAgt != Layer::agents.end(); ++itAgt)
			{
				assert(*itAgt);
				if(!dynamic_cast<ActiveDelimitation<Kernel, Point, Vector>*>(*itAgt))
				{
					agentLessDelimitations.insert(*itAgt);
				}
			}
			return agentLessDelimitations;
		}
		default:
			return std::set<Agent*>();
	}
}


#endif // SUB_ENVIRONMENT_HH