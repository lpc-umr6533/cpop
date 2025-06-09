#ifndef REFINEMENT_THREAD_HH
#define REFINEMENT_THREAD_HH

#include <QThread>

/// \brief The thrad definition to process to a refinement.
/// @author Henri Payno
class RefinementThread : public QThread {
	Q_OBJECT
public:
	RefinementThread(unsigned int pID, unsigned int pNumberOfUnitaryEntityPerCell=10, double pDeltaWin = 0., double pMaxRatio = 0.8);
	
	/// \brief run of the thread.
	void run() override = 0;

protected:
	/// \brief ID getter
	unsigned int getID() { return _id; }
	/// \brief number of unitary entiries (facets, segments...) setter
	void setNumberOfUnitaryEntities(unsigned int nb) { _numberOfUnitaryEntityPerCell = nb; }
	/// \brief number of unitary entiries (facets, segments...) getter
	[[nodiscard]] unsigned int getNumberOfUnitaryEntities() const { return _numberOfUnitaryEntityPerCell; }
	/// \brief delta reffinement setter
	void setDeltaReffinement(double delta) { _deltaReffinement = delta; }
	/// \brief delta reffinement getter
	[[nodiscard]] double getDeltaReffinement() const { return _deltaReffinement; }
	/// \brief max ratio to cell radius setter
	void setMaxRatioNucleus(double r) { _maxRatioToCellRadius = r; }
	/// \brief max ratio to cell radius getter
	double getMaxRatioNucleus() { return _maxRatioToCellRadius; }

protected:
	unsigned int _id;                           ///< \brief the ID of the thread
	unsigned int _numberOfUnitaryEntityPerCell; ///< \brief a number of information. Is the number of segment in 2D, the number of facet in 3D. When we reach this value we must stop		
	double _deltaReffinement;                   ///< \brief the step gain value until which it is not interesting to reffine.
	double _maxRatioToCellRadius;               ///< \brief The maximal ration the nucleus radius must not overtake. in [0..1]
};

#endif
