/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef CPOP_LOADER_HH
#define CPOP_LOADER_HH

#include "CellProperties.hh"
#include "EnvironmentSettings.hh"
#include "GeometrySettings.hh"
#include "SimpleDiscoidalCell.hh"
#include "SimpleSpheroidalCell.hh"
#include "SpheresSDelimitation.hh"

#include <QDomElement>
#include <QFile>
#include <QString>

#include <QXmlStreamReader>

using namespace Settings::nEnvironment;
using namespace Settings::Geometry;

#ifndef NDEBUG
	#define DEBUG_CPOP_LOADER 0
#else
	#define DEBUG_CPOP_LOADER 0	// must always set to 0
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief the containing function used to load environment and cell population from an xml file.
/// @author Henri Payno
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CPOP_Loader
{

public:
	/// \brief constructor
	CPOP_Loader();
	/// \brief destructor
	~CPOP_Loader();

	/// \brief load 2D environment
	/// \warning for now this function is only able to load environment with only one "level" of similated sub environment
	t_Environment_2* load2DEnvironment(QString filePath, bool forceID = false)			{ return loadEnvironement<double, Point_2, Vector_2>(filePath, forceID); };
	/// \brief load 3D environment
	/// \warning for now this function is only able to load environment with only one "level" of similated sub environment
	t_Environment_3* load3DEnvironment(QString filePath, bool forceID = false)			{ return loadEnvironement<double, Point_3, Vector_3>(filePath, forceID); };

	/// \brief load all agent from the XML given and store them by the key "ID"
	std::map<unsigned long int, Agent*> getAgents(QXmlStreamReader* path, bool forceID = false);

private:
	/// \brief create all cell properties and store them with the key present on the XML file
	map<unsigned long int, CellProperties* > getCellPropertiesMap(QXmlStreamReader*);
	/// \brief return the cellProperties created from the map
	CellProperties* getCellProperties(QXmlStreamReader*, unsigned long int &);
	/// \brief return the CellProperties ID stored on the file
	// unsigned long int getCellPropIdFromNode(QXmlStreamReader*);


	/// \brief load an environment.
	template<typename Kernel, typename Point, typename Vector>
	Environment<Kernel, Point, Vector>* loadEnvironement(QString, bool);

	/// \brief load a sub environment from a QDomNode
	template<typename Kernel, typename Point, typename Vector>
	SimulatedSubEnv<Kernel, Point, Vector>* loadSimuSubEnvironment(Environment<Kernel, Point, Vector>* parent, const std::map<unsigned long int, Agent*>* pAgents, QXmlStreamReader* node);

	template<typename Kernel, typename Point, typename Vector>
	void loadSimuSubEnvironments(QXmlStreamReader* xmlReader, Environment<Kernel, Point, Vector>* parent, const std::map<unsigned long int, Agent*>* pAgents);
	/// \brief return the point stored on a QDomElement
	template<typename Point>
	void getPointFrmXMLStream(QXmlStreamReader*, Point*);
	/// \cond
	void getPointFrmXMLStream(QXmlStreamReader*, Point_2*);
	void getPointFrmXMLStream(QXmlStreamReader*, Point_3*);
	/// \endcond

	/// \brief return the delimitation stored on the QDomNode
	template<typename Kernel, typename Point, typename Vector>
	SpatialDelimitation<Kernel, Point, Vector> * getDelimitationFromDomNode(QXmlStreamReader*, SpatialDelimitation<Kernel, Point, Vector> *);
	/// \cond
	t_SpatialDelimitation_2* getDelimitationFromDomNode(QXmlStreamReader*, t_SpatialDelimitation_2*);
	t_SpatialDelimitation_3* getDelimitationFromDomNode(QXmlStreamReader*, t_SpatialDelimitation_3*);
	/// \endcond

	/// \brief return all agent stored on a QDomNode
	Agent* getAgentFromNode(QXmlStreamReader* node, bool forceID);
	/// \brief create a discoidal cell fro a QDomNode
	DiscoidalCell* createDiscoidalCell(QXmlStreamReader* node, bool forceID);
	/// \brief create a spheroidal cell fro a QDomNode
	SpheroidalCell* createSpheroidalCell(QXmlStreamReader* node, bool forceID);

	/// \brief return a map of variable attribute stored under the node of ID "nodeID"
	map<LifeCycles::LifeCycle, CellProperties::t_CellVarAtt_d > getCellProp_d_Value(QXmlStreamReader*, QString nodeID);
	/// \brief return a map of material attribute stored under the node of ID "nodeID"
	map<LifeCycles::LifeCycle, G4Material* > getCellProp_Mat_Value(QXmlStreamReader*, QString);

	bool foundEnvironmentName(QXmlStreamReader* xmlReader, QString& theName);


private:
	/// \brief last load cell proeprties
	map<unsigned long int, CellProperties* > cellPropertiesMap;
};

////////////////: template function implementation /////////////////////////////

///////////////////////////////////////////////////////////////////////
/// \param filePath The path to the data file
/// \param forceID if true, we will force the agent ID to the one referenced on the XML
/// \warning if agent already exists on the similation, forcing ID can create conflicting ID
/// must be used cautiously			
/// \return The set of file generated from the given file
///////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
Environment<Kernel, Point, Vector>* CPOP_Loader::loadEnvironement(QString filePath, bool forceID)
{
	typedef Environment<Kernel, Point, Vector> t_Environment;
	/// load data
	QFile xmlFile(filePath);
    if(!xmlFile.open(QIODevice::ReadOnly)) return nullptr;
	
	QXmlStreamReader xmlReader(&xmlFile);

	QXmlStreamReader::TokenType token = xmlReader.readNext();
    // If token is just StartDocument, we'll go to next.
    if(token != QXmlStreamReader::StartDocument) 
    {
        InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "Error parsing file. No node found", "FILE::XML::loadEnvironement");
		return NULL;
    }


    // go through xml informations
    while(!xmlReader.atEnd() &&  !xmlReader.hasError() && (token != QXmlStreamReader::StartElement) )
    {
    	token = xmlReader.readNext();
    }

    assert((token == QXmlStreamReader::StartElement));
    assert(xmlReader.name() == save_flag);

    // If token is StartElement, we'll see if we can read it.
    if( (token != QXmlStreamReader::StartElement) || (xmlReader.name() != save_flag) ) 
    {
       	InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "Error parsing file. Not a CPOP save", "FILE::XML::loadEnvironement");
		return NULL;
    }

    // create the environment
    QString environmentName;
    foundEnvironmentName(&xmlReader, environmentName);
   	t_Environment* env = new t_Environment( environmentName);

   	// get all stored cell properties
    cellPropertiesMap.clear();
    cellPropertiesMap = getCellPropertiesMap(&xmlReader);

    // close en reopen for now : hack because cellPropeties are needed here but write at the end
    /// \todo : store position of the xmlStreamReader or store cell Properties at the beginning !!!
    /// regarder qint64 	lineNumber() const ?
	xmlFile.close();
	QXmlStreamReader xmlReader2(&xmlFile);
	xmlFile.open(QIODevice::ReadOnly);

	/// get agents from the XMl
	std::map<unsigned long int, Agent*> agents = getAgents(&xmlReader2, forceID );
        //cout << "nb agent loaded = " << agents.size() << endl;

    // close en reopen for now : hack because cellPropeties are needed here but write at the end
    /// \todo : store position of the xmlStreamReader or store cell Properties at the beginning !!!
	xmlFile.close();
	QXmlStreamReader xmlReader3(&xmlFile);
	xmlFile.open(QIODevice::ReadOnly);

	/// create sub environnement and set them agents
	loadSimuSubEnvironments(&xmlReader3, env, &agents);

	/// \todo add subEnvironement of the subEnvironement ???
	xmlFile.close();
	return env;
}


///////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
void CPOP_Loader::loadSimuSubEnvironments(QXmlStreamReader* xmlReader, Environment<Kernel, Point, Vector>* parent, const std::map<unsigned long int, Agent*>* pAgents)
{	
	assert(xmlReader);
	assert(parent);
	// pass information until finding the openging node "ENVIRONMENT"
	while(!xmlReader->atEnd() &&  !xmlReader->hasError() && (xmlReader->name().toString() != environment_flag) )
    {
    	xmlReader->readNext();
    }

    if( xmlReader->atEnd() || xmlReader->hasError())
    {
       	InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "Error parsing file. Can't find sub environment informations", "FILE::XML::loadSimuSubEnvironments");
    	exit(0);
    }

    // parse all subEnvironment until moving out node "ENVIRONMENT"

    // enter inside the Environment node
    xmlReader->readNext();
	while(!xmlReader->atEnd() &&  !xmlReader->hasError() && (xmlReader->name().toString() != environment_flag) )
    {
    	if(xmlReader->name().toString() == simulated_sub_env_flag)
    	{
			loadSimuSubEnvironment<Kernel, Point, Vector>(parent, pAgents, xmlReader );
    	}
    	xmlReader->readNext();
    }
}

////////////////////////////////////// getPointFrmDomElm /////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
/// \param pXmlReader the location of the point definition
/// \param pointToSet The point where to stored the value
//////////////////////////////////////////////////////////////////////////////////////////
template<typename Point>
void CPOP_Loader::getPointFrmXMLStream(QXmlStreamReader* pXmlReader, Point* pointToSet)
{
	//*pointToSet = Point();	// TODO : redo
}

/////////////////////////////////// getDelimitationFromDomNode ///////////////////////////////
/// \param pXmlReader the location of the point definition
/// \param delimitation the spatial delimitation
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
SpatialDelimitation<Kernel, Point, Vector>* CPOP_Loader::getDelimitationFromDomNode(QXmlStreamReader* pXmlReader, SpatialDelimitation<Kernel, Point, Vector>* delimitation)
{
	delimitation = NULL;
	cout << " error, allocate null delimitation" << endl;
	return delimitation;
}

//////////////////////////////// loadSimuSubEnvi //////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param parent The parent of the sub environment
/// \param pAgents The agents map
/// \param node The node where is define the subEnvironment
/// \return the loaded SimulatedSubEnvironment, NULL if failed
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
SimulatedSubEnv<Kernel, Point, Vector>* CPOP_Loader::loadSimuSubEnvironment(Environment<Kernel, Point, Vector>* parent, const std::map<unsigned long int, Agent*>* pAgents, QXmlStreamReader* xmlReader)
{
	assert(parent);
	assert(xmlReader);
	assert(xmlReader->name().toString() == simulated_sub_env_flag);
	assert(xmlReader->attributes().hasAttribute(name_flag));

	QString lName = xmlReader->attributes().value(name_flag).toString();

	typedef SimulatedSubEnv<Kernel, Point, Vector> t_SimulatedSubEnv;
	typedef SpatialDelimitation<Kernel, Point, Vector> t_SpatialDelimitation;
	t_SimulatedSubEnv* subEnvironment;


	if(xmlReader->atEnd())
	{
		assert(false);
		exit(0);
	}

	t_SpatialDelimitation* delimitation = NULL; 
	delimitation = getDelimitationFromDomNode(xmlReader, delimitation);
	assert(delimitation);
	// if unable to create the spatial delimitation
	if( delimitation == NULL)
	{
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES,"Unable to generate the SimulatedSubEnvironment no delimitation recognize", "FILE::XML::loadSimuSubEnvironment");
		exit(0);
	}

	// create the subenvironment if able to create the delimitation
	subEnvironment = new t_SimulatedSubEnv(parent, lName, delimitation);
	assert(subEnvironment);

	// move to the contained agent node
	while(!xmlReader->atEnd() &&  !xmlReader->hasError() && (xmlReader->name().toString() != contained_agent_flag) )
	{
		xmlReader->readNext();
	}

	if(xmlReader->atEnd())
	{
		assert(false);
		exit(0);
	}

	if(xmlReader->hasError())
	{
		assert(false);
		exit(0);
	}

	// insert all contained agent / treat contained agent node
	// moving inside the contained agent node
	xmlReader->readNext();

	assert(xmlReader->name().toString() != contained_agent_flag);
	while(!xmlReader->atEnd() &&  !xmlReader->hasError() && (xmlReader->name().toString() != contained_agent_flag) )
	{
		if(xmlReader->name().toString() == agent_ID_flag)
		{
			// go in the node containing information
			xmlReader->readNext();

			unsigned long int agentID = xmlReader->text().toString().toLong();
			if(pAgents->find(agentID) == pAgents->end())
			{
				QString mess = "Unable to retrace to the agent of ID : " + QString::number(agentID);
				InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "FILE::XML::loadSimuSubEnvironment");
			}
			subEnvironment->addAgent( pAgents->find(agentID)->second );		
			// skip the node /ID
			xmlReader->readNext();

		}
		xmlReader->readNext();
	}

	// skipping the closing node of contained agent />
    xmlReader->readNext();

	/// \todo : add subEnvironement of the subEnvironement ???
	// QDomNodeList simulatedSubEnv = env_DOMElmt.toElement().elementsByTagName(simulated_sub_env_flag);
	// int iSubEnv = 0;
	// while(iSubEnv < simulatedSubEnv.size())
	// {
	// 	loadSimuSubEnvironment(env, agents, simulatedSubEnv.at(iSubEnv), dimension );
	// 	iSubEnv ++;
	// }
	// loadSubEnvironment(static_cast<Layer*> subEnvironment, agents, agentContained[iAgent], dimension );

	// skip the closing node of  SIMULATED_SUB_ENVIRONMENT />
    xmlReader->readNext();

	return subEnvironment;
}

#endif // CPOP_LOADER_HH
