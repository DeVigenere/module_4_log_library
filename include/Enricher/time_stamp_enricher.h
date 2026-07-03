#pragma once
#include <chrono>
#include <sstream>
#include "enricher.h"

class time_stamp_enricher : public enricher {
	std::unique_ptr<enricher> next;
public:
	time_stamp_enricher(std::unique_ptr<enricher> n) : next(std::move(n)) {};
	log_record enrich(log_record& lr) override;
};