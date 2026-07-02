#pragma once
#include <string>
#include <chrono>
#include "formater.h"

class json_text_formater : public formater {
	std::string escape_json(const std::string& str) const {
		std::string result;
		for (char c : str) {
			switch (c) {
			case '"': result += "\\\""; break;
			case '\\': result += "\\\\"; break;
			case '\b': result += "\\b"; break;
			case '\f': result += "\\f"; break;
			case '\n': result += "\\n"; break;
			case '\r': result += "\\r"; break;
			case '\t': result += "\\t"; break;
			default: result += c; break;
			}
		}
		return result;
	}
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
		std::stringstream s_stream;
		s_stream << "{";
		auto time = std::chrono::system_clock::to_time_t(lr.time_stamp);
		std::stringstream time_string;
		time_string << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");

		auto add_field = [&](const std::string& key, const std::string& value) {
			s_stream << "\"" << escape_json(key) << "\":\"" << escape_json(value) << "\",";
			};
		add_field("level", level_to_string(lr.level));
		add_field("logger", lr.logger_name);
		add_field("message", lr.message);
		add_field("timestamp", time_string.str());
		for (const auto& [key, value] : lr.fields) {
			if (key != "timestamp") {
				add_field(key, value);
			}
		}
		std::string result = s_stream.str();
		if (result.back() == ',') {
			result.pop_back();
		}
		result += "}";
		return result;
	}
};