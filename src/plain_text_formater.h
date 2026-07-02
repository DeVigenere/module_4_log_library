#pragma once
#include <string>
#include <chrono>
#include <sstream>
#include "formater.h"

class plain_text_formater : public formater {
	std::string level_to_string(log_level lr) const {
		switch (lr) {
		case log_level::TRACE: return "TRACE";
		case log_level::DEBUG: return "DEBUG";
		case log_level::INFO: return "INFO";
		case log_level::WARN: return "WARN";
		case log_level::ERROR: return "ERROR";
		case log_level::FATAL: return "FATAL";
		default: return "UNKNOWN";
		}
	}
public:
	std::string format(const log_record& lr) override {
		auto time = std::chrono::system_clock::to_time_t(lr.time_stamp);
		auto millisec = std::chrono::duration_cast<std::chrono::milliseconds>(lr.time_stamp.time_since_epoch()).count() % 1000;
		std::stringstream s_stream;
		s_stream << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S") << "."
			<< std::setfill('0') << std::setw(3) << millisec << "]";
		s_stream << "[" << level_to_string(lr.level) << "]";
		s_stream << "[" << lr.logger_name << "]";
		s_stream << lr.message;

		for (const auto& [key, value] : lr.fields) {
			s_stream << " " << key << "=" << value;
		}
		return s_stream.str();
	}
};