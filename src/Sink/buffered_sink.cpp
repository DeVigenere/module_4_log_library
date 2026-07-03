#include "buffered_sink.h"

bool buffered_sink::open() {
	return downstream->open();
}

void buffered_sink::write(const log_record& lr) {
	std::string formated = downstream->format->format(lr);
	write_formated(formated);
}

void buffered_sink::write_formated(const std::string& message) {
	buffer.push_back(message);
	if (buffer.size() >= batch_size) {
		flush();
	}
}

void buffered_sink::flush() {
	if (buffer.empty()) {
		return;
	}
	for (const auto& i : buffer) {
		downstream->write_formated(i);
	}
	buffer.clear();
	downstream->flush();
}

void buffered_sink::on_error(const std::string& error) {
	downstream->on_error(error);
}