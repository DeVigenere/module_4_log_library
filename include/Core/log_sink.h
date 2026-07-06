#pragma once
#include <string>
#include <iostream>
#include "formater.h"

class log_sink {
public:
	std::unique_ptr<formater> formatter;
	log_sink(std::unique_ptr<formater> fmt) : formatter(std::move(fmt)) {};
	virtual ~log_sink() = default;
	virtual bool open() = 0;
	virtual void write(const log_record& lr);
	virtual void write_formated(const std::string& message) = 0;
	virtual void flush() = 0;
	virtual void on_error(const std::string& error) = 0;
};

