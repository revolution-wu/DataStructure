#pragma once
#include <iostream>
#include <chrono>
using std::cout;
using std::endl;
class Timer
{
public:
	Timer(bool microSeconds = false)
		: microSeconds_(microSeconds),
		start_(std::chrono::steady_clock::now())
	{
	}

	~Timer()
	{
		auto end = std::chrono::steady_clock::now();
		//auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
		if (microSeconds_)
		{
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start_);
			cout << duration.count() << "μs" << endl;
		}
		else
		{
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start_);
			cout << duration.count() << "ms" << endl;
		}
	}
private:
	bool microSeconds_;
	std::chrono::steady_clock::time_point start_;
};
