#include "log_manager.h"

log_manager& log_manager::instance() {
	static log_manager instance;
	return instance;
}

logger* log_manager::get_logger(const std::string& name) {
	auto it = loggers.find(name);
	if (it != loggers.end()) {
		return it->second.get();
	}
	return nullptr;
}

void log_manager::register_logger(const std::string& name, std::unique_ptr<logger> logger_ptr) {
	loggers[name] = std::move(logger_ptr);
}

void log_manager::flush_all() {
	for (auto& [name, logger_ptr] : loggers) {
		logger_ptr->flush();
	}
}