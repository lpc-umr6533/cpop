#include <QString>

#ifndef STATS_DATA_EMITTER_HH
#define STATS_DATA_EMITTER_HH

/// \brief The StatsDataEmitter interface
/// This is implemented by statistical "informer". That mean thaht the daughter
/// class will produce data able to be treated statistically
/// @author Henri Payno
class StatsDataEmitter {
public:
	virtual ~StatsDataEmitter()	= default;

	/// \brief define importante data to be treated
	[[nodiscard]] virtual QString addStatsData() const = 0;
	/// \brief inform about the statistics exported by the meitter
	[[nodiscard]] virtual QString writeStatsHeader() const = 0;
};

#endif
