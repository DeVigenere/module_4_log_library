#include "file_sink.h"

void file_sink::rotate() {
	file.close();
	std::string old_path = path + ".3";
	std::remove(old_path.c_str());

	for (int i = 2; i >= 1; --i) {
		std::string from = path + "." + std::to_string(i);
		std::string to = path + "." + std::to_string(i + 1);
		std::rename(from.c_str(), to.c_str());
	}
	std::string from = path;
	std::string to = path + ".1";
	std::rename(from.c_str(), to.c_str());

	file.open(path, std::ios::out | std::ios::trunc);
	current_size = 0;
}

void file_sink::on_error(const std::string& error) {
	std::cerr << error << std::endl;
}

bool file_sink::open() {
	if (!file.is_open()) {
		file.open(path, std::ios::out | std::ios::app);
		if (file.is_open()) {
			file.seekp(0, std::ios::end);
			current_size = file.tellp();
			return true;
		}
		return false;
	}
	return true;
}

void file_sink::write_formated(const std::string& message) {
	if (!file.is_open()) {
		if (!open()) {
			on_error("failed to open file" + path);
			return;
		}
	}
	size_t message_size = message.size() + 1;
	if (current_size + message_size > max_size) {
		rotate();
	}
	file << message << std::endl;
	current_size += message_size;
}

void file_sink::flush() {
	if (file.is_open()) {
		file.flush();
	}
}