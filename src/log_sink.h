#pragma once
#include <string>
#include <iostream>

class log_sink {
public:
	virtual ~log_sink() = default;
	virtual bool open() = 0;
	virtual void write(const std::string& message) = 0;
	virtual void flush() = 0;
	virtual void on_error(const std::string& error) = 0;
};

class console_sink : public log_sink {
public:
	bool open() override {
		return true;
	}
	void write(const std::string& message) override {
		std::cout << message << std::endl;
	}
	void flush() override {
		std::cout.flush();
	}
	void on_error(const std::string& error) override {
		std::cerr << "console sink error!" << error << std::endl;
	}
};