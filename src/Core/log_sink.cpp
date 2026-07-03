#include "log_sink.h"

void log_sink::write(const log_record& lr) {
	if (format) {
		std::string formated = format->format(lr);
		write_formated(formated);
	}
}


