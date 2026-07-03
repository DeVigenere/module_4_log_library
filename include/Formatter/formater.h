#pragma once
#include <string>
#include "log_record.h"

class formater {
public:
	virtual ~formater() = default;
	virtual std::string format(const log_record& lr) = 0;
};