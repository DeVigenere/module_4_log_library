#pragma once
#include "enricher.h"

class base_enricher : public enricher {
public:
	log_record enrich(log_record& lr) override;
};