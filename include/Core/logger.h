#pragma once
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <chrono>
#include "filter.h"
#include "enricher.h"
#include "log_sink.h"
#include "plain_text_formater.h"

class logger {
	std::string name;
	std::vector<std::unique_ptr<filter>> filters;
	std::unique_ptr<enricher> enrich;
	std::vector<std::unique_ptr<log_sink>> sinks;
public:
	logger(const std::string& n,
		std::vector<std::unique_ptr<filter>> f,
		std::unique_ptr<enricher> e,
		std::vector<std::unique_ptr<log_sink>> s) :
		name(n),
		filters(std::move(f)),
		enrich(std::move(e)),
		sinks(std::move(s)) {
	}
	void log(log_level level, const std::string& message, const std::map<std::string, std::string>& fields = {});
	void flush();
};