/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include <QString>

#ifndef STATS_DATA_EMITTER_HH
#define STATS_DATA_EMITTER_HH

//////////////////////////////////////////////////////////////////////////////
/// \brief The StatsDataEmitter interface
/// This is implemented by statistical "informer". That mean thaht the daughter 
/// class will produce data able to be treated statistically
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class StatsDataEmitter
{
public:
	/// \brief constructor
	StatsDataEmitter()			{};
	/// \brief destructor
	virtual ~StatsDataEmitter()			{};

	/// \brief define importante data to be treated
	virtual QString addStatsData() const = 0;
	/// \brief inform about the statistics exported by the meitter
	virtual QString writeStatsHeader() const = 0;
};

#endif // STATS_DATA_EMITTER_HH