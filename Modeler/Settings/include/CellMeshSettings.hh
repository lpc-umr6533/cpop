/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef CELL_MESH_SETTINGS_HH
#define CELL_MESH_SETTINGS_HH

#include <QString>
#include <QThread>

static const double MAX_RATIO_NUCLEUS_TO_CELL 	= 0.8;							///<\brief must be in [0..1]. Which ratio the nucleus can reach compare to the cell membrane radius
/// \brief the number of step on each axis the sphere must be composed of
static const unsigned int MIN_SPHERE_NB_POINT 	= 8;							///<\warning to prevent error this must be at least higher than NUCLEUS_SPHERE_NB_POINT
extern unsigned int NUCLEUS_SPHERE_NB_POINT; // = 8;// MIN_SPHERE_NB_POINT; 				///<\warning to prevent error this must be at least higher than NUCLEUS_SPHERE_NB_POINT
// extern unsigned int NUCLEUS_SPHERE_NB_POINT;
// TODO : reafaire un nombre de thread maximal ( genre 30 )
static const unsigned int MIN_NB_CELL_PER_THREAD= 600;							///<\brief number of cell each thread contains	
static const unsigned int MIN_DISC_POINT 		= 8;							///< \brief the number of points a cell mesh discribed by a disc must contained
static const bool REMOVE_SMALLEST_WEIGHT	 	= false;						///< \brief the polity of removal for conflict cells on Delaunay triangulation
static const QString cellNamePrefix				= "cell_";
static const QString nucleusNamePrefix			= "nucleus_";
static const bool USE_THREAD_FOR_MESH_SUBDVN 	= true;							/// \brief do we want to use thread for subdivision. To optimize must be set to true, but for some profiler must be set to false.

static const double overlapToleranceForG4 		= 0.00004;						// 40 nanometre. In G4

// \brief define the priority of the thread used for subdivision
static const QThread::Priority MESHING_THREAD_PRIORITY = QThread::LowPriority;

namespace Settings
{
	/// \brief set the number of degree to pick on each axis ( used for round nucleus ). The total number of points will be pNbPts*pNbPts
	/// \warning this function must be used carefully because this value must be inferior or equal of the cytoplasm shape to avoid potential overlaps of shapes.
	/// \param pNbPts the number of points to define
	void setNucleusNbDegreePerAxis(unsigned int pNbPts);
}

#endif // CELL_MESH_SETTINGS_HH