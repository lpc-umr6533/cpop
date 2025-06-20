#ifndef STATS_DATA_EMITTER_HH
#define STATS_DATA_EMITTER_HH

#include <string>

/// \brief The StatsDataEmitter interface
/// This is implemented by statistical "informer". That mean thaht the daughter
/// class will produce data able to be treated statistically
/// @author Henri Payno
class StatsDataEmitter {
public:
	virtual ~StatsDataEmitter()	= default;

	/// \brief define importante data to be treated
	[[nodiscard]] virtual std::string addStatsData() const = 0;
	/// \brief inform about the statistics exported by the meitter
	[[nodiscard]] virtual std::string writeStatsHeader() const = 0;
};

#endif
