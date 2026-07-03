#pragma once
#include "log_sink.h"
#include <iostream>

class console_sink : public log_sink {
public:
	console_sink(std::unique_ptr<formater> fmt) : log_sink(std::move(fmt)) {}
	bool open() override;
	void write_formated(const std::string& message) override;
	void flush() override;
	void on_error(const std::string& error) override;
};