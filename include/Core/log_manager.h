#pragma once
#include <map>
#include <memory>
#include <string>
#include "logger.h"

class log_manager {
	std::map<std::string, std::unique_ptr<logger>> loggers;
	log_manager() = default;
public:
	static log_manager& instance();
	log_manager(const log_manager&) = delete;
	log_manager& operator=(const log_manager&) = delete;
	logger* get_logger(const std::string& name);
	void register_logger(const std::string& name, std::unique_ptr<logger> logger_ptr);
	void flush_all();
};