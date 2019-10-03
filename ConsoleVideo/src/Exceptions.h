#pragma once

struct ConsoleVideoException {
	const char* str_what;

	ConsoleVideoException() { str_what = nullptr; }
	ConsoleVideoException(const char* info) { str_what = info; }
};
