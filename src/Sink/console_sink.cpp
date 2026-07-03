#include "console_sink.h"

bool console_sink::open() {
	return true;
}

void console_sink::write_formated(const std::string& message) {
	std::cout << message << std::endl;
}

void console_sink::flush() {
	std::cout.flush();
}

void console_sink::on_error(const std::string& error) {
	std::cerr << "console sink error!" << error << std::endl;
}