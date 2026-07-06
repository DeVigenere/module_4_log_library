#include "name_filter.h"

bool name_filter::should_log(const log_record& lr) {
	return allowed.empty() || allowed.count(lr.logger_name) > 0;
}