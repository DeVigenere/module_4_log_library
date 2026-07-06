#include "log_sink.h"

void log_sink::write(const log_record& lr) {
	if (formatter) {
		std::string formated = formatter->format(lr);
		write_formated(formated);
	}
}


