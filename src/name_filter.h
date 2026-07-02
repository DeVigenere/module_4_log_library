#pragma once
#include <set>
#include <string>
#include "filter.h"

class name_filter : public filter {
	std::set<std::string> allowed;
public:
	name_filter(const std::set<std::string>& allow) : allowed(allow) {};
	bool should_log(const log_record& lr) override {
		return allowed.empty() || allowed.count(lr.logger_name) > 0;
	}
};