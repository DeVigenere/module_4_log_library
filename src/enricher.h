#pragma once
#include "log_record.h"

class enricher {
public:
	virtual ~enricher() = default;
	virtual log_record enrich(log_record& lr) = 0;
};

class base_enricher : public enricher {
public:
	log_record enrich(log_record& lr) override {
		return lr;
	}
};