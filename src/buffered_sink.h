#pragma once
#include <memory>
#include <vector>
#include <string>
#include "log_sink.h"

class buffered_sink : public log_sink {
	std::unique_ptr<log_sink> downstream;
	size_t batch_size;
	std::vector<std::string> buffer;
public:
	buffered_sink(std::unique_ptr<log_sink> ds, size_t bs) : downstream(std::move(ds)), batch_size(bs) {
		buffer.reserve(batch_size);
	}
	bool open() override {
		return downstream->open();
	}

	void write(const std::string& message) override {
		buffer.push_back(message);
		if (buffer.size() >= batch_size) {
			flush();
		}
	}
	void flush() override {
		if (buffer.empty()) {
			return;
		}
		for (const auto& i : buffer) {
			downstream->write(i);
		}
		buffer.clear();
		downstream->flush();
	}
	void on_error(const std::string& error) override {
		downstream->on_error(error);
	}
};