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
	buffered_sink(std::unique_ptr<log_sink> ds, size_t bs) : log_sink(nullptr), downstream(std::move(ds)), batch_size(bs)  {
		buffer.reserve(batch_size);
	}
	bool open() override;
	void write(const log_record& lr) override;
	void write_formated(const std::string& message) override;
	void flush() override;
	void on_error(const std::string& error) override;
};