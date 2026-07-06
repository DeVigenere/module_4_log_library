#pragma once
#include "filter.h"

class level_filter : public filter {
	log_level log_level_filter;
	log_level min_level;
public:
	level_filter(const log_level ll, const log_level ml) : log_level_filter(ll), min_level(ml) {};
	bool should_log(const log_record& lr) override;
};