#include <gtest/gtest.h>
#include "level_filter.h"
#include "name_filter.h"
#include "log_level.h"
#include "time_stamp_enricher.h"
#include "plain_text_formater.h"
#include "json_text_formater.h"

using namespace std::chrono_literals;
namespace fs = std::filesystem;

TEST(FilterTest, LevelAndNameFilters) {
    level_filter level_filter(log_level::INFO, log_level::INFO);
    name_filter name_filter({ "http", "db" });

    log_record record;
    record.logger_name = "http";
    record.level = log_level::DEBUG;

    EXPECT_FALSE(level_filter.should_log(record));
    record.level = log_level::INFO;
    EXPECT_TRUE(level_filter.should_log(record));

    EXPECT_TRUE(name_filter.should_log(record));
    record.logger_name = "unknown";
    EXPECT_FALSE(name_filter.should_log(record));
}

TEST(EnricherTest, TimestampEnricher) {
    time_stamp_enricher enricher;

    log_record record;
    record.time_stamp = std::chrono::system_clock::now();
    record.fields.clear();

    auto result = enricher.enrich(record);
    EXPECT_TRUE(result.fields.count("timestamp") > 0);
    EXPECT_FALSE(result.fields["timestamp"].empty());
}

TEST(FormatterTest, PlainAndJson) {
    log_record record;
    record.level = log_level::INFO;
    record.message = "Test";
    record.logger_name = "http";
    record.time_stamp = std::chrono::system_clock::now();
    record.fields["method"] = "GET";
    record.fields["status"] = "200";

    plain_text_formater plain;
    std::string plain_result = plain.format(record);
    EXPECT_TRUE(plain_result.find("INFO") != std::string::npos);
    EXPECT_TRUE(plain_result.find("Test") != std::string::npos);
    EXPECT_TRUE(plain_result.find("http") != std::string::npos);
    EXPECT_TRUE(plain_result.find("method=GET") != std::string::npos);

    json_text_formater json;
    std::string json_result = json.format(record);
    EXPECT_TRUE(json_result.front() == '{' && json_result.back() == '}');
    EXPECT_TRUE(json_result.find("\"level\":\"INFO\"") != std::string::npos);
    EXPECT_TRUE(json_result.find("\"logger\":\"http\"") != std::string::npos);
    EXPECT_TRUE(json_result.find("\"method\":\"GET\"") != std::string::npos);

    record.message = "Test \"quotes\"";
    json_result = json.format(record);
    EXPECT_TRUE(json_result.find("\\\"quotes\\\"") != std::string::npos);
}