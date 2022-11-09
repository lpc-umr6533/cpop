/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef CELL_HH
#define CELL_HH

#include "DynamicAgent.hh"
#include "ECellComposition.hh"
#include "ECellTypes.hh"
#include "ENucleusPosType.hh"
#include "ELifeCycles.hh"
#include "File_Utils.hh"
#include "Force.hh"
#include "ICell.hh"
#include "Nucleus.hh"
#include "StatsSettings.hh"
#include "StatsDataEmitter.hh"
#include "UnitSystemManager.hh"
#include "Writable.hh"

template<typename Kernel, typename Point, typename Vector> 
class Force;

using namespace LifeCycles;
using namespace CellComposition;
using namespace XML::CPOP_Flag;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief The cell basic class. Heritate from DynamicAgent wich is the spatial templated entity 
/// and from ICell which is the "interface" of cell.
/// Define cell composition and behavior.
/// @author Henri Payno
////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point, typename Vector>
class Cell : public DynamicAgent<Kernel, Point, Vector>, public ICell, public Writable, public StatsDataEmitter
{
	typedef Force<Kernel, Point, Vector> Force_t;		///< \brief force definition used by Cell
public:
	/// \brief constructor
	Cell(const CellProperties*, Body* pBody = NULL, Kernel pMass = 1. * CLHEP::nanogram, 
		std::vector< Force<Kernel, Point, Vector>* > pForcesToApply = std::vector<Force<Kernel, Point, Vector>* >() ) ;
	/// \brief destructor
	virtual ~Cell();

	virtual int exec();
	virtual int init();
	virtual int start();
	virtual int stop();
	/// \brief return true if the cell own a mesh
	virtual bool hasMesh() const = 0;

	/// \brief set the life state of the cell
	void setLifeCycle(LifeCycle pState)			{state = pState;};
	/// \brief life state getter
	LifeCycle getLifeCycle() const				{return state;};
	/// \brief age setter
	void setAge(Kernel pAge)					{age = pAge;};
	/// \brief age getter
	Kernel getAge() const						{return age;};
	/// \brief mass setter
	void setMass(Kernel pMass)					{ mass = pMass;};
	/// \brief mass getter
	Kernel getMass()	const 					{return mass;};
	/// \brief return the cell dimension
	Dimension getDimension() const ;
	/// \brief return a random point set on the requested Organelle
	virtual Point getSpotOnOrganelle(Organelle) const;
	/// \brief return a random point on/inside the membrane
	virtual Point getSpotOnCellMembrane() const = 0;
	/// \brief return a random point inside the cytoplasm, avoiding membrane and nucleus
	virtual Point getSpotOnCytoplasm() const = 0;
	/// \brief return a random point on the nuclear membrane
	virtual Point getSpotOnNuclei() const = 0;
	/// \brief return a random point in the Nucleoplasm
	virtual Point getSpotInNuclei() const = 0;
	/// \brief return true if the point is inside the cell
	virtual bool hasIn(Point) const = 0;
	/// \brief nuclei getter
	std::vector<Nucleus<Kernel, Point, Vector>* > getNuclei() const 	{return nuclei;};
	/// \brief return true if nuclei radius are coherent
	virtual bool checkNucleiRadius() const = 0;
	/// \brief max ratio setter
	void setMaxRatioNucleusToCell(double r)	{maxRatioToCellRadius = r;}

	/// \brief forces to apply setter
	void setForces(	std::vector< Force_t* > pForces)	{forces = pForces;};
	/// \brief add a force to be applied
	void addForce( Force_t* pForce)						{assert(pForce); forces.push_back(pForce);};
	/// \brief remove and delete all forces registred
	void removeAndDeleteForces();

	/// \brief reset the mesh
	virtual void resetMesh() = 0;
	/// \brief return the cell description
	virtual QString getDescription() const = 0;
	/// \brief return the volume defined by the mesh
	virtual Kernel getMeshVolume(MeshOutFormats::outputFormat meshType) const = 0;
	/// \brief will return cell mesh specificities for R. 
	/// By default : no specifities.
	/// \warning must be in correlation with the R treatment.
	virtual QString addStatsData() const	;
	/// \brief inform about the statistics exported by the meitter
	virtual QString writeStatsHeader() const;	
	/// \brief print cell information (used also to save the cell on a .txt file)
	void write(QXmlStreamWriter&) const;
	/// \brief write the cell properties
	virtual void writeAttributes(QXmlStreamWriter&) const;
	/// \brief write the nuclei properties
	void writeNuclei(QXmlStreamWriter&) const;
	/// \brief return the position to be saved on the stats files
	QString getPositionForStates() const;
	/// \brief return the flag used for each position
	QString getPositionFlag() const;
	/// \brief return true id the point is in one of the nucleoplasm
	bool hasInNucleoplasm(Point) const;

protected:
	Kernel age;								///< \brief The cell age, in day ?
	LifeCycle state;						///< \brief The state of the cell (G0, G1...)
	Kernel mass;							///< \brief the mass of the cell

	std::vector<Nucleus<Kernel, Point, Vector> *> nuclei;	///< \brief the list of nuclei owned by the cell
	double maxRatioToCellRadius;							///< \brief the radius nuclei max % of the radius cell

	std::vector< Force_t* > forces;							///< \brief forces applied by the cell
};


/////////////////////////// FUNCTION DEFINITIONS ///////////////////////////
#ifndef NDEBUG
	#define DEBUG_CELL 0
#else
	#define DEBUG_CELL 0	// must always stay at 0
#endif

#include <InformationSystemManager.hh>
#include <Scheduler.hh>

#include <QString>

////////////////////////////////////////////////////////////////////////////////
/// \brief  constructor
/// \param pCellProperties The properties of the cell line
/// \param pAnimat The animat of the cell
/// \param pBody The body of the cell
/// \param pMass The mass of the cell
/// \param pForcesToApply The forces the cell is applying
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
Cell<Kernel, Point, Vector>::Cell(
	const CellProperties* pCellProperties, 
	Body* pBody, 
	Kernel pMass, 
	std::vector<Force<Kernel, Point, Vector>* > pForcesToApply):
	DynamicAgent<Kernel, Point, Vector>(pBody),
	ICell(pCellProperties),
	Writable(),
	StatsDataEmitter(),
	age((Kernel)0),
	state(LifeCycles::NA),
	mass(pMass),
	forces(pForcesToApply)
{
	Agent::setColor( IO::getRandomColor());
}

////////////////////////////////////////////////////////////////////////////////
/// \brief  constructor
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
Cell<Kernel, Point, Vector>::~Cell()
{
	/// delete nuclei
	typename std::vector<Nucleus<Kernel, Point, Vector> *>::iterator itNucleus;
	for(itNucleus = nuclei.begin(); itNucleus != nuclei.end(); ++itNucleus)
	{
		delete (*itNucleus);
		*itNucleus = NULL;
	}

	/// delete forces
	while(forces.size() > 0)
	{
		Force<Kernel, Point, Vector>* toDelete = forces.back();
		delete (toDelete);
		toDelete = NULL;
		forces.pop_back();
	}
}

#include "File_CPOP_Utils.hh"
////////////////////////////////////////////////////////////////////////////////
/// \param writer where to redirect writing of information
/// 
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
void Cell<Kernel, Point, Vector>::write(QXmlStreamWriter& writer) const
{ 
	writer.writeStartElement(cell_flag);
	/// write the cell dimension
	writer.writeAttribute(dimension_flag, QString::number(getDimension()) );
	/// write cell attributes
	writeAttributes(writer);
	/// write nuclei attributes
	writeNuclei(writer);
	writer.writeEndElement();
}

////////////////////////////////////////////////////////////////////////////////
/// \param writer where to redirect writing of information
/// 
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
void Cell<Kernel, Point, Vector>::writeAttributes(QXmlStreamWriter& writer) const
{
	writer.writeAttribute(agent_ID_flag, 			QString::number(Agent::getID()));
	writer.writeAttribute(mass_flag, 				QString::number(getMass()));
	writer.writeAttribute(cell_properties_ID_flag, 	QString::number(this->getCellProperties()->getID()));
	writer.writeAttribute(life_cycle_flag,			QString::number(getLifeCycle()));
	
	IO::CPOP::writePoint(position_flag, writer, Spatialable<Kernel, Point, Vector>::getPosition());
}

////////////////////////////////////////////////////////////////////////////////
/// 
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
void Cell<Kernel, Point, Vector>::writeNuclei(QXmlStreamWriter& writer) const
{
	writer.writeStartElement(nuclei_flag);

 	typename std::vector<Nucleus<Kernel, Point, Vector>* >::iterator itNucleus;
 	std::vector<Nucleus<Kernel, Point, Vector>* > lNuclei = getNuclei();
 	for(itNucleus = lNuclei.begin(); itNucleus != lNuclei.end(); ++itNucleus)
	{
		(*itNucleus)->write(writer);
	}

	writer.writeEndElement(); 
}

////////////////////////////////////////////////////////////////////////////////
/// \brief  the running process called at each step by
/// the simulation manager.
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
int Cell<Kernel, Point, Vector>::exec()
{
	/// first : apply forces
	Vector stepForce;
	typename std::vector<Force<Kernel, Point, Vector>* >::const_iterator itForce;
	for(itForce = forces.begin(); itForce != forces.end(); ++itForce)
	{
		Movable<Kernel, Point, Vector>::addForce((*itForce)->computeForce());
	}

	/// then call dynamic agent exec (update position)
	DynamicAgent<Kernel, Point, Vector>::exec();
	if(DEBUG_CELL)
	{
		QString mess = "agent " + QString::number( Agent::getID()) + " running at t = " + QString::number(Scheduler::getInstance()->getRunningTime());
		InformationSystemManager::getInstance()->Message(InformationSystemManager::INFORMATION_MES, mess.toStdString(), "Cell");
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief  Function called just before launching an agent on the simulation
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
int Cell<Kernel, Point, Vector>::init()
{
	if(DEBUG_CELL)
	{
		QString mess = " Initialization of agent" + QString::number( Agent::getID());
		InformationSystemManager::getInstance()->Message(InformationSystemManager::INFORMATION_MES, mess.toStdString(), "Cell");
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief  Function called just before launching an agent on the simulation
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
int Cell<Kernel, Point, Vector>::start()
{
	DynamicAgent<Kernel, Point, Vector>::start();
	if(DEBUG_CELL)
	{
		QString mess = " start agent " + QString::number( Agent::getID());
		InformationSystemManager::getInstance()->Message(InformationSystemManager::INFORMATION_MES, mess.toStdString(), "Cell");
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief  Function called if we want to stop this agent from the simulation
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
int Cell<Kernel, Point, Vector>::stop()
{
	DynamicAgent<Kernel, Point, Vector>::stop();
	if(DEBUG_CELL)
	{
		QString mess = " stop agent " + QString::number( Agent::getID());
		InformationSystemManager::getInstance()->Message(InformationSystemManager::INFORMATION_MES, mess.toStdString(), "Cell");
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// 
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
void Cell<Kernel, Point, Vector>::removeAndDeleteForces()
{
	typename std::vector< Force_t* >::iterator itForce;
	for(itForce = forces.begin(); itForce != forces.end(); ++itForce)
	{
		assert(*itForce);
		delete *itForce;
		*itForce = NULL;
	}
	forces.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param Organelle The targetted Organelle
/// \return The requested spot
//////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
Point Cell<Kernel, Point, Vector>::getSpotOnOrganelle(Organelle Organelle) const
{
	switch(Organelle)
	{
		case _CYTOPLASM:
		{
			// will return a point inside the mesh. G4 use a tesselated solid so works to
			return getSpotOnCytoplasm();
		}
		case _NUCLEOPLASM:
		{
			// will return a point in the theorical nuclei - a sphere. Because used in G4 and representation is a sphere (G4Orb)
			return getSpotOnNuclei();
		}
		case _NUCLEAR_MEMBRANE:
		{
			// will return a point in the theorical nuclei - a sphere. Because used in G4 and representation is a sphere (G4Orb)
			return getSpotInNuclei();
		}
		case _CELL_MEMBRANE:
		{
			// will return a point on a facet of the mesh. G4 use a tesselated solid so works to
			return getSpotOnCellMembrane();
		}
		default:
		{
			QString mess = " unknow Organelle requested for a spot. Returning spot on O";
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "Cell");
			return Point();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//					GetDimension
//////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
Dimension Cell<Kernel, Point, Vector>::getDimension() const
{
	return Unknow_D;
}

template<>
Dimension Cell<double, Point_2, Vector_2>::getDimension() const;

template<>
Dimension Cell<double, Point_3, Vector_3>::getDimension() const;

////////////////////////////////////////////////////////////////////////////////
// 	addStatsData
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
QString Cell<Kernel, Point, Vector>::addStatsData() const
{	
	return (
		QString::number(Agent::getID()) 				+ "\t" +
		QString::number(cellProperties->getCellType() ) + "\t" +
		QString::number(state ) 						+ "\t" +
		QString::number(mass ) 							+ "\t" +
		QString::number(age )							+ "\t" +
		getPositionForStates()
		);
}

////////////////////////////////////////////////////////////////////////////////
// 	writeStatsHeader
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
QString Cell<Kernel, Point, Vector>::writeStatsHeader() const
{
	return QString(
		Settings::Statistics::Cell_ID_flag 		+ "\t" +
		Settings::Statistics::Cell_Type_flag 	+ "\t" +
		Settings::Statistics::Cell_State_flag 	+ "\t" +
		Settings::Statistics::Cell_Mass_flag 	+ "\t" +
		Settings::Statistics::Cell_Age_flag 	+ "\t" +
		getPositionFlag()
		);
}

////////////////////////////////////////////////////////////////////////////////
/// \param pt the point we want to check position
/// \return true if the point is in one of the nucleus
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
bool Cell<Kernel, Point, Vector>::hasInNucleoplasm(Point pt) const
{
 	typename std::vector<Nucleus<Kernel, Point, Vector>* >::iterator itNucleus;
	std::vector<Nucleus<Kernel, Point, Vector>* > lNuclei = getNuclei();
 	for(itNucleus = lNuclei.begin(); itNucleus != lNuclei.end(); ++itNucleus)
	{
		if((*itNucleus)->hasIn(pt))
		{
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
/// 	getPositionForStates
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
QString Cell<Kernel, Point, Vector>::getPositionForStates() const
{	
	return QString();
}

template<>
QString Cell<double, Point_2, Vector_2>::getPositionForStates() const;

template<>
QString Cell<double, Point_3, Vector_3>::getPositionForStates() const;

////////////////////////////////////////////////////////////////////////////////
/// 	getPositionFlag
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
QString Cell<Kernel, Point, Vector>::getPositionFlag() const
{	
	return QString();
}

template<>
QString Cell<double, Point_2, Vector_2>::getPositionFlag() const;

template<>
QString Cell<double, Point_3, Vector_3>::getPositionFlag() const;

#endif // CELL_HH
