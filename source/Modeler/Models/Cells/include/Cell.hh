#ifndef CELL_HH
#define CELL_HH

#include "DynamicAgent.hh"
#include "ECellComposition.hh"
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

/// \brief The cell basic class. Heritate from DynamicAgent wich is the spatial templated entity
/// and from ICell which is the "interface" of cell.
/// Define cell composition and behavior.
/// @author Henri Payno
template <typename Kernel, typename Point, typename Vector>
class Cell : public DynamicAgent<Kernel, Point, Vector>, public ICell, public Writable, public StatsDataEmitter {
	using Force_t = Force<Kernel, Point, Vector>;		///< \brief force definition used by Cell

public:
	Cell(
		const CellProperties*,
		Body* pBody = nullptr,
		Kernel pMass = 1. * CLHEP::nanogram,
		std::vector<Force<Kernel, Point, Vector>*> pForcesToApply = {}
	);
	~Cell() override;

	int exec() override;
	int init() override;
	int start() override;
	int stop() override;
	/// \brief return true if the cell own a mesh
	[[nodiscard]] virtual bool hasMesh() const = 0;

	/// \brief set the life state of the cell
	void setLifeCycle(LifeCycle pState) { _state = pState; }
	/// \brief life state getter
	[[nodiscard]] LifeCycle getLifeCycle() const { return _state; }
	/// \brief age setter
	void setAge(Kernel pAge) { _age = pAge; }
	/// \brief age getter
	[[nodiscard]] Kernel getAge() const	{ return _age; }
	/// \brief mass setter
	void setMass(Kernel pMass) { _mass = pMass; }
	/// \brief mass getter
	[[nodiscard]] Kernel getMass() const { return _mass; }
	/// \brief return the cell dimension
	[[nodiscard]] Dimension getDimension() const override;
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
	[[nodiscard]] std::vector<Nucleus<Kernel, Point, Vector>*> getNuclei() const { return _nuclei; }
	/// \brief return true if nuclei radius are coherent
	[[nodiscard]] virtual bool checkNucleiRadius() const = 0;
	/// \brief max ratio setter
	void setMaxRatioNucleusToCell(double r)	{ _maxRatioToCellRadius = r; }

	/// \brief forces to apply setter
	void setForces(std::vector< Force_t*> pForces) { _forces = pForces; }
	/// \brief add a force to be applied
	void addForce( Force_t* pForce) { assert(pForce); _forces.push_back(pForce); }
	/// \brief remove and delete all forces registred
	void removeAndDeleteForces();

	/// \brief reset the mesh
	virtual void resetMesh() = 0;
	/// \brief return the cell description
	[[nodiscard]] virtual QString getDescription() const = 0;
	/// \brief return the volume defined by the mesh
	virtual Kernel getMeshVolume(MeshOutFormats::outputFormat meshType) const = 0;
	/// \brief will return cell mesh specificities for R.
	/// By default : no specifities.
	/// \warning must be in correlation with the R treatment.
	[[nodiscard]] QString addStatsData() const override;
	/// \brief inform about the statistics exported by the meitter
	[[nodiscard]] QString writeStatsHeader() const override;
	/// \brief print cell information (used also to save the cell on a .txt file)
	void write(QXmlStreamWriter&) const override;
	/// \brief write the cell properties
	virtual void writeAttributes(QXmlStreamWriter&) const;
	/// \brief write the nuclei properties
	void writeNuclei(QXmlStreamWriter&) const;
	/// \brief return the position to be saved on the stats files
	[[nodiscard]] QString getPositionForStates() const;
	/// \brief return the flag used for each position
	[[nodiscard]] QString getPositionFlag() const;
	/// \brief return true id the point is in one of the nucleoplasm
	bool hasInNucleoplasm(Point) const;

protected:
	Kernel _age;                      ///< \brief The cell age, in day ?
	LifeCycle _state{LifeCycles::NA}; ///< \brief The state of the cell (G0, G1...)
	Kernel _mass;                     ///< \brief the mass of the cell

	std::vector<Nucleus<Kernel, Point, Vector> *> _nuclei;  ///< \brief the list of nuclei owned by the cell
	double _maxRatioToCellRadius;                           ///< \brief the radius nuclei max % of the radius cell

	std::vector< Force_t* > _forces;  ///< \brief forces applied by the cell
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

/// \brief  constructor
/// \param pCellProperties The properties of the cell line
/// \param pAnimat The animat of the cell
/// \param pBody The body of the cell
/// \param pMass The mass of the cell
/// \param pForcesToApply The forces the cell is applying
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
	_age((Kernel)0),
	_mass(pMass),
	_forces(pForcesToApply)
{
	Agent::setColor(IO::getRandomColor());
}

/// \brief  constructor
template<typename Kernel, typename Point, typename Vector>
Cell<Kernel, Point, Vector>::~Cell() {
	/// delete nuclei
	for(auto* nucleus: _nuclei)
		delete nucleus;

	/// delete forces
	while(_forces.size() > 0) {
		auto* toDelete = _forces.back();
		delete (toDelete);
		_forces.pop_back();
	}
}

#include "File_CPOP_Utils.hh"
/// \param writer where to redirect writing of information
template<typename Kernel, typename Point, typename Vector>
void Cell<Kernel, Point, Vector>::write(QXmlStreamWriter& writer) const {
	writer.writeStartElement(cell_flag);
	/// write the cell dimension
	writer.writeAttribute(dimension_flag, QString::number(getDimension()) );
	/// write cell attributes
	writeAttributes(writer);
	/// write nuclei attributes
	writeNuclei(writer);
	writer.writeEndElement();
}

/// \param writer where to redirect writing of information
template<typename Kernel, typename Point, typename Vector>
void Cell<Kernel, Point, Vector>::writeAttributes(QXmlStreamWriter& writer) const {
	writer.writeAttribute(agent_ID_flag, QString::number(Agent::getID()));
	writer.writeAttribute(mass_flag, QString::number(getMass()));
	writer.writeAttribute(cell_properties_ID_flag, QString::number(this->getCellProperties()->getID()));
	writer.writeAttribute(life_cycle_flag, QString::number(getLifeCycle()));

	IO::CPOP::writePoint(position_flag, writer, Spatialable<Kernel, Point, Vector>::getPosition());
}

template<typename Kernel, typename Point, typename Vector>
void Cell<Kernel, Point, Vector>::writeNuclei(QXmlStreamWriter& writer) const {
	writer.writeStartElement(nuclei_flag);

 	auto const& lNuclei = getNuclei();
 	for(auto const* nucleus : _nuclei)
		nucleus->write(writer);

	writer.writeEndElement();
}

/// \brief  the running process called at each step by
/// the simulation manager.
template<typename Kernel, typename Point, typename Vector>
int Cell<Kernel, Point, Vector>::exec() {
	/// first : apply forces
	Vector stepForce;
	for(auto const* force : _forces)
		Movable<Kernel, Point, Vector>::addForce(force->computeForce());

	/// then call dynamic agent exec (update position)
	DynamicAgent<Kernel, Point, Vector>::exec();
	if(DEBUG_CELL) {
		QString mess = "agent " + QString::number( Agent::getID()) + " running at t = " + QString::number(Scheduler::getInstance()->getRunningTime());
		InformationSystemManager::getInstance()->Message(InformationSystemManager::INFORMATION_MES, mess.toStdString(), "Cell");
	}

	return 0;
}

/// \brief  Function called just before launching an agent on the simulation
template<typename Kernel, typename Point, typename Vector>
int Cell<Kernel, Point, Vector>::init() {
	if(DEBUG_CELL) {
		QString mess = " Initialization of agent" + QString::number(Agent::getID());
		InformationSystemManager::getInstance()->Message(InformationSystemManager::INFORMATION_MES, mess.toStdString(), "Cell");
	}

	return 0;
}

/// \brief  Function called just before launching an agent on the simulation
template<typename Kernel, typename Point, typename Vector>
int Cell<Kernel, Point, Vector>::start() {
	DynamicAgent<Kernel, Point, Vector>::start();
	if(DEBUG_CELL) {
		QString mess = " start agent " + QString::number( Agent::getID());
		InformationSystemManager::getInstance()->Message(InformationSystemManager::INFORMATION_MES, mess.toStdString(), "Cell");
	}

	return 0;
}

/// \brief  Function called if we want to stop this agent from the simulation
template<typename Kernel, typename Point, typename Vector>
int Cell<Kernel, Point, Vector>::stop() {
	DynamicAgent<Kernel, Point, Vector>::stop();
	if(DEBUG_CELL) {
		QString mess = " stop agent " + QString::number( Agent::getID());
		InformationSystemManager::getInstance()->Message(InformationSystemManager::INFORMATION_MES, mess.toStdString(), "Cell");
	}

	return 0;
}

template<typename Kernel, typename Point, typename Vector>
void Cell<Kernel, Point, Vector>::removeAndDeleteForces() {
	for(auto const* force : _forces) {
		assert(force);
		delete force;
	}

	_forces.clear();
}

/// \param Organelle The targetted Organelle
/// \return The requested spot
template<typename Kernel, typename Point, typename Vector>
Point Cell<Kernel, Point, Vector>::getSpotOnOrganelle(Organelle Organelle) const {
	switch(Organelle) {
		// will return a point inside the mesh. G4 use a tesselated solid so works to
		case _CYTOPLASM:        return getSpotOnCytoplasm();
		// will return a point in the theorical nuclei - a sphere. Because used in G4 and representation is a sphere (G4Orb)
		case _NUCLEOPLASM:      return getSpotOnNuclei();
		// will return a point in the theorical nuclei - a sphere. Because used in G4 and representation is a sphere (G4Orb)
		case _NUCLEAR_MEMBRANE: return getSpotInNuclei();
		// will return a point on a facet of the mesh. G4 use a tesselated solid so works to
		case _CELL_MEMBRANE:    return getSpotOnCellMembrane();
		default:
		{
			QString mess = " unknow Organelle requested for a spot. Returning spot on O";
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "Cell");
			return Point();
		}
	}
}

template<typename Kernel, typename Point, typename Vector>
Dimension Cell<Kernel, Point, Vector>::getDimension() const {
	return Unknow_D;
}

template<>
Dimension Cell<double, Point_2, Vector_2>::getDimension() const;

template<>
Dimension Cell<double, Point_3, Vector_3>::getDimension() const;

template<typename Kernel, typename Point, typename Vector>
QString Cell<Kernel, Point, Vector>::addStatsData() const {
	return (
		QString::number(Agent::getID()) + "\t" +
		QString::number(_cellProperties->getCellType()) + "\t" +
		QString::number(_state) + "\t" +
		QString::number(_mass) + "\t" +
		QString::number(_age) + "\t" +
		getPositionForStates()
	);
}

template<typename Kernel, typename Point, typename Vector>
QString Cell<Kernel, Point, Vector>::writeStatsHeader() const {
	return QString(
		Settings::Statistics::Cell_ID_flag + "\t" +
		Settings::Statistics::Cell_Type_flag + "\t" +
		Settings::Statistics::Cell_State_flag + "\t" +
		Settings::Statistics::Cell_Mass_flag + "\t" +
		Settings::Statistics::Cell_Age_flag	+ "\t" +
		getPositionFlag()
	);
}

/// \param pt the point we want to check position
/// \return true if the point is in one of the nucleus
template<typename Kernel, typename Point, typename Vector>
bool Cell<Kernel, Point, Vector>::hasInNucleoplasm(Point pt) const {
	auto const& lNuclei = getNuclei();
 	for(auto const* nucleus: lNuclei) {
		if(nucleus->hasIn(pt))
			return true;
	}

	return false;
}

template<typename Kernel, typename Point, typename Vector>
QString Cell<Kernel, Point, Vector>::getPositionForStates() const {
	return {};
}

template<>
QString Cell<double, Point_2, Vector_2>::getPositionForStates() const;

template<>
QString Cell<double, Point_3, Vector_3>::getPositionForStates() const;

template<typename Kernel, typename Point, typename Vector>
QString Cell<Kernel, Point, Vector>::getPositionFlag() const {
	return {};
}

template<>
QString Cell<double, Point_2, Vector_2>::getPositionFlag() const;

template<>
QString Cell<double, Point_3, Vector_3>::getPositionFlag() const;

#endif
