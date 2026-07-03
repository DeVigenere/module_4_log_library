#include "time_stamp_enricher.h"
#include <iomanip>
#include <sstream>

log_record time_stamp_enricher::enrich(log_record& lr) {
	lr = next->enrich(std::move(lr));
	auto time = std::chrono::system_clock::to_time_t(lr.time_stamp);
	auto millisec = std::chrono::duration_cast<std::chrono::milliseconds>(lr.time_stamp.time_since_epoch()).count() % 1000;
	std::stringstream s_stream;
	s_stream << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
		<< "." << std::setfill('0') << std::setw(3) << millisec;
	lr.fields["timestamp"] = s_stream.str();
	return lr;
}