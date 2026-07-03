#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "log_sink.h"

class file_sink : public log_sink {
	std::string path;
	size_t max_size;
	std::ofstream file;
	size_t current_size = 0;

	void rotate();
public:
	file_sink(const std::string& p, size_t ms, std::unique_ptr<formater> fmt) : path(p), max_size(ms), log_sink(std::move(fmt)) {
		file.open(path, std::ios::out | std::ios::app);
		if (file.is_open()) {
			file.seekp(0, std::ios::end);
			current_size = file.tellp();
		}
		else {
			on_error("failed to open file!" + path);
		}
	}
	void on_error(const std::string& error) override;
	~file_sink() {
		if (file.is_open()) {
			file.close();
		}
	}
	bool open() override;
	void write_formated(const std::string& message)override;
	void flush() override;
};