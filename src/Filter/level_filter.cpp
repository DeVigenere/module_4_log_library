#include "level_filter.h"

bool level_filter::should_log(const log_record& lr) {
	return lr.level >= min_level || lr.level == log_level_filter;
}