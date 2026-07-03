#pragma once
#include <string>
#include <chrono>
#include <map>
#include "log_level.h"

struct log_record {
	log_level level;
	std::string message;
	std::string logger_name;
	std::chrono::system_clock::time_point time_stamp;
	std::map<std::string, std::string> fields;
};