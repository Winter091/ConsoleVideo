#pragma once

#include <chrono>
#include <fstream>

class Timer {
	std::chrono::time_point<std::chrono::steady_clock> start, end;

public:
	Timer()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		std::ofstream out("time.txt");
		out << duration << std::endl;
		out.close();
	}
};
