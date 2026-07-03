#include <gtest/gtest.h>
#include <memory>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <chrono>

#include "log_level.h"
#include "log_record.h"
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
    auto enricher = std::make_unique<time_stamp_enricher>(std::make_unique<base_enricher>());

    log_record record;
    record.time_stamp = std::chrono::system_clock::now();
    record.fields.clear();

    auto result = enricher->enrich(record);
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

TEST(SinkTest, ConsoleSink) {
    auto formatter = std::make_unique<plain_text_formater>();
    console_sink sink(std::move(formatter));
    EXPECT_TRUE(sink.open());
    EXPECT_NO_THROW(sink.write_formated("test message"));
    EXPECT_NO_THROW(sink.flush());
}

TEST(SinkTest, FileSink) {
    std::string test_file = "test_file.log";
    {
        auto formatter = std::make_unique<plain_text_formater>();
        file_sink sink(test_file, 1024, std::move(formatter));
        sink.write_formated("Message 1");
        sink.write_formated("Message 2");
        sink.flush();
    }

    std::ifstream file(test_file);
    EXPECT_TRUE(file.is_open());

    std::string line;
    int count = 0;
    while (std::getline(file, line)) {
        count++;
    }
    EXPECT_EQ(count, 2);
    file.close();
    fs::remove(test_file);
}

TEST(LoggerTest, BasicLogging) {
    class TestSink : public log_sink {
    public:
        std::string last_msg;

        TestSink() : log_sink(nullptr) {}
        bool open() override { return true; }
        void write(const log_record& lr) override {
            if (format) {
                last_msg = format->format(lr);
            }
        }
        void write_formated(const std::string& msg) override {
            last_msg = msg;
        }
        void flush() override {}
        void on_error(const std::string&) override {}
    };

    std::vector<std::unique_ptr<filter>> filters;
    filters.push_back(std::make_unique<level_filter>(log_level::INFO, log_level::INFO));
    filters.push_back(std::make_unique<name_filter>(std::set<std::string>{"http"}));

    auto enricher = std::make_unique<time_stamp_enricher>(std::make_unique<base_enricher>());

    std::vector<std::unique_ptr<log_sink>> sinks;
    auto sink = std::make_unique<TestSink>();
    sink->format = std::make_unique<plain_text_formater>();
    auto* sink_ptr = sink.get();
    sinks.push_back(std::move(sink));

    logger log("http", std::move(filters), std::move(enricher), std::move(sinks));
    log.log(log_level::INFO, "Test message", { {"key", "value"} });

    EXPECT_FALSE(sink_ptr->last_msg.empty());
    EXPECT_TRUE(sink_ptr->last_msg.find("Test message") != std::string::npos);
}

TEST(LogManagerTest, Singleton) {
    auto& manager1 = log_manager::instance();
    auto& manager2 = log_manager::instance();
    EXPECT_EQ(&manager1, &manager2);
}

TEST(LevelFilterTest, DifferentLevels) {
    level_filter filter(log_level::WARN, log_level::WARN);

    log_record record;
    record.logger_name = "test";

    record.level = log_level::TRACE;
    EXPECT_FALSE(filter.should_log(record));

    record.level = log_level::DEBUG;
    EXPECT_FALSE(filter.should_log(record));

    record.level = log_level::INFO;
    EXPECT_FALSE(filter.should_log(record));

    record.level = log_level::WARN;
    EXPECT_TRUE(filter.should_log(record));

    record.level = log_level::ERROR;
    EXPECT_TRUE(filter.should_log(record));

    record.level = log_level::FATAL;
    EXPECT_TRUE(filter.should_log(record));
}

TEST(NameFilterTest, EmptyAllowedList) {
    name_filter filter({});

    log_record record;
    record.logger_name = "any_name";
    EXPECT_TRUE(filter.should_log(record));

    record.logger_name = "another_name";
    EXPECT_TRUE(filter.should_log(record));
}
