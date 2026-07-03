#include <iostream>
#include <memory>
#include <set>
#include <vector>
#include <map>
#include <string>

#include "log_level.h"
#include "filter.h"
#include "level_filter.h"
#include "name_filter.h"
#include "enricher.h"
#include "base_enricher.h"
#include "time_stamp_enricher.h"
#include "formater.h"
#include "plain_text_formater.h"
#include "json_text_formater.h"
#include "log_sink.h"
#include "console_sink.h"
#include "file_sink.h"
#include "buffered_sink.h"
#include "logger.h"
#include "log_manager.h"

int main() {
    std::vector<std::unique_ptr<filter>> filters;
    filters.push_back(std::make_unique<level_filter>(log_level::INFO, log_level::INFO));
    filters.push_back(std::make_unique<name_filter>(std::set<std::string>{"app"}));

    auto enricher = std::make_unique<time_stamp_enricher>(std::make_unique<base_enricher>());

    std::vector<std::unique_ptr<log_sink>> sinks;

    sinks.push_back(std::make_unique<console_sink>(std::make_unique<plain_text_formater>()));

    sinks.push_back(std::make_unique<file_sink>("app.log", 1024 * 1024,
        std::make_unique<json_text_formater>()));

    auto console = std::make_unique<console_sink>(std::make_unique<plain_text_formater>());
    sinks.push_back(std::make_unique<buffered_sink>(std::move(console), 5));

    auto logger_ptr = std::make_unique<logger>("app", std::move(filters),
        std::move(enricher), std::move(sinks));

    log_manager::instance().register_logger("app", std::move(logger_ptr));
    auto* log = log_manager::instance().get_logger("app");
    if (log) {
        log->log(log_level::INFO, "Application started", { {"version", "1.0.0"} });
        log->log(log_level::WARN, "High memory usage", { {"memory", "80%"} });
        log->log(log_level::ERROR, "Connection failed", { {"retry", "3"} });
        log->log(log_level::INFO, "Application stopped");
    }

    log_manager::instance().flush_all();
    return 0;
}