#pragma once
#include "log_record.h"

class filter {
public:
	virtual ~filter() = default;
	virtual bool should_log(const log_record& lr) = 0;
};