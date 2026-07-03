#include "logger.h"

void logger::log(log_level level, const std::string& message, const std::map<std::string, std::string>& fields) {
	log_record record{
		level, message, name, std::chrono::system_clock::now(), fields
	};
	for (const auto& filt_ptr : filters) {
		if (!filt_ptr->should_log(record)) {
			return;
		}
	}
	if (enrich) {
		record = enrich->enrich(record);
	}

	for (const auto& sink_ptr : sinks) {
		sink_ptr->write(record);
	}

}
void logger::flush() {
	for (auto& sink_ptr : sinks) {
		sink_ptr->flush();
	}
}
