/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef REFINEMENT_THREAD_HH
#define REFINEMENT_THREAD_HH

#include "CellMeshSettings.hh"

#include <QThread>

//////////////////////////////////////////////////////////////////////////////
/// \brief The thrad definition to process to a refinement.
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class RefinementThread : public QThread
{
	Q_OBJECT
public:
	RefinementThread(unsigned int pID, unsigned int pNumberOfUnitaryEntityPerCell=10, double pDeltaWin = 0., double pMaxRatio = 0.8);
	virtual ~RefinementThread();
	
	/// \brief run of the thread.
	virtual void run() = 0;

protected:
	/// \brief ID getter
	unsigned int getID()	{return ID;};
	/// \brief number of unitary entiries (facets, segments...) setter
	void setNumberOfUnitaryEntities(unsigned int nb)	{ numberOfUnitaryEntityPerCell = nb;};
	/// \brief number of unitary entiries (facets, segments...) getter
	unsigned int getNumberOfUnitaryEntities() const		{ return numberOfUnitaryEntityPerCell;};
	/// \brief delta reffinement setter
	void setDeltaReffinement(double delta)				{ deltaReffinement = delta;};
	/// \brief delta reffinement getter
	double getDeltaReffinement() const					{ return deltaReffinement;};
	/// \brief max ratio to cell radius setter
	void setMaxRatioNucleus(double r)					{ maxRatioToCellRadius = r;};
	/// \brief max ratio to cell radius getter
	double getMaxRatioNucleus()							{ return maxRatioToCellRadius;};

protected:
	unsigned int ID;								///< \brief the ID of the thread
	unsigned int numberOfUnitaryEntityPerCell;		///< \brief a number of information. Is the number of segment in 2D, the number of facet in 3D. When we reach this value we must stop		
	double deltaReffinement;						///< \brief the step gain value until which it is not interesting to reffine.
	double maxRatioToCellRadius;					///< \brief The maximal ration the nucleus radius must not overtake. in [0..1]

};

#endif // REFINEMENT_THREAD_HH