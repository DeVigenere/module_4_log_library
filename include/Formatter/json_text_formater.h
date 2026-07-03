#pragma once
#include <string>
#include <chrono>
#include "formater.h"

class json_text_formater : public formater {
	std::string escape_json(const std::string& str) const;
	std::string level_to_string(log_level lr) const;
public:
	std::string format(const log_record& lr) override;
};