#pragma once
#include <string>
#include <chrono>
#include <sstream>
#include "formater.h"

class plain_text_formater : public formater {
	std::string level_to_string(log_level lr) const;
public:
	std::string format(const log_record& lr) override;
};