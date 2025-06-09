#ifndef ENVIRONMENT_HH
#define ENVIRONMENT_HH

#include "Cell.hh"
#include "Cell_Utils.hh"
#include "SubEnvironment.hh"
#include "World.hh"
#include "WorldLayer.hh"
#include "Writable.hh"
#include "XMLSettings.hh"

#include <set>
#include <map>

using namespace XML::CPOP_Flag;

template<typename Kernel, typename Point, typename Vector>
class SubEnvironment;

/// \brief Environment is an object able to contain many SubEnvironment. Environment are unbounded entities
/// Sub environment are object containing cells. This object is spatially limited by their SpatialDelimitation.
/// @author Henri Payno
template<typename Kernel, typename Point, typename Vector>
class Environment : public World<Kernel, Point, Vector>, public Writable {
public:
	Environment(QString pName, std::set<SubEnvironment<Kernel, Point, Vector>*> pSubEnvs = std::set<SubEnvironment<Kernel, Point, Vector>*>());

	/// \brief add a subEnvrionement to the environment.
	void add(SubEnvironment<Kernel, Point, Vector>*);
	/// \brief  export all information needed to sumarize the Writable
	void write(QXmlStreamWriter&) const override;

private:
	std::set<SubEnvironment<Kernel, Point, Vector>*> subEnvs;		///< The set of environments to mirror
};

//////////////////////// FUNCTION DEFINITIONS //////////////////////////////////////////
 
template<typename Kernel, typename Point, typename Vector>
Environment<Kernel, Point, Vector>::Environment(QString pName, std::set<SubEnvironment<Kernel, Point, Vector>*> pSubEnvs):
	World<Kernel, Point, Vector>(pName),
	Writable()
{
	for(auto itSE = pSubEnvs.begin(); itSE != pSubEnvs.end(); ++itSE)
		add(*itSE);
}

/// \param pSubEnv The sub environment to add to the environment
template<typename Kernel, typename Point, typename Vector>
void Environment<Kernel, Point, Vector>::add(SubEnvironment<Kernel, Point, Vector>* pSubEnv) {
	assert(pSubEnv);
	Layer::addChild(static_cast<WorldLayer<Kernel, Point, Vector>*>(pSubEnv));
	
	/// for mirrored environments !!!
	if(subEnvs.find(pSubEnv) == subEnvs.end())
		subEnvs.insert(pSubEnv);
}

/// \param writer The writer to redirect information
template<typename Kernel, typename Point, typename Vector>
void Environment<Kernel, Point, Vector>::write(QXmlStreamWriter& writer) const {
	// export environnement architectures
	// write imself
	{
		writer.writeStartElement(environment_flag);	// start env
		writer.writeAttribute(dimension_flag, QString::number( Dimensioned_Layer<Kernel, Point, Vector>::getDimension()));
		writer.writeAttribute(name_flag, Layer::getName());

		// ask his child to write themself
		std::map<QString, Layer*> childs = Layer::getChilds();
		std::map<QString, Layer*>::iterator itChild;
		for(itChild = childs.begin(); itChild != childs.end(); ++itChild) {
			if(dynamic_cast<Writable*>(itChild->second))
				dynamic_cast<Writable*>(itChild->second)->write(writer);	
		}
		writer.writeEndElement(); // "Environment"
	}

	// export cells
	std::set<const Cell<Kernel, Point, Vector>*> lCells;	
	// request to all unique agent to print themself ( to make sure no doublon )
	std::set<Agent*> lAllAgents = Layer::getUniqueAgentsAndSubAgents();
	{
		writer.writeStartElement(cells_flag);	// start cell

		/// export only Cells
		for(auto const& lAllAgent : lAllAgents) {
			auto* lCell = dynamic_cast<Cell<Kernel, Point, Vector>*>(lAllAgent);
			if(lCell) {
				static_cast<Writable*>(lCell)->write(writer);
				lCells.insert( const_cast<const Cell<Kernel, Point, Vector>* > (lCell) );
			}
		}
		writer.writeEndElement(); // "Cells"
	}

	// export cell properties
	{
		writer.writeStartElement(all_cell_properties_flag); // start cell properties
		// get all unique cell properties
		auto const& cellProperties = Utils::getAllCellsProperties(std::vector<const ICell*>(lCells.begin(), lCells.end()));
		// export them all
		for(auto const& cellPropertie : cellProperties)
			cellPropertie->write(writer);

		writer.writeEndElement(); // end cell properties
	}
}

#endif
