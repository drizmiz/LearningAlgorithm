
#pragma once

#ifndef TESTER_H
#define TESTER_H

#include <chrono>
#include <vector>

class ThreeSum
{
public:
	template<typename T>
	static int count(T a)
	{
		int length = a.end() - a.begin();
		int cnt = 0;
		for (int i = 0; i < length; i++)
			for (int j = i + 1; j < length; j++)
				for (int k = j + 1; k < length; k++)
					if (a[i] + a[j] + a[k] == 0)
						cnt++;
		return cnt;
	}
};

class Timer
{
private:
	std::chrono::steady_clock::time_point starttime;
	std::chrono::steady_clock::time_point endtime;
public:
	void start()
	{
		starttime = std::chrono::steady_clock::now();
	}
	void end()
	{
		endtime = std::chrono::steady_clock::now();
	}
	// ÒÔºÁÃë·µ»Ø
	enum class units
	{
		second,
		millisecond,
		microsecond
	};
	long long timespan( units unit = units::millisecond )
	{
		switch (unit)
		{
		case units::second:
			return std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count(); break;
		case units::millisecond:
			return std::chrono::duration_cast<std::chrono::milliseconds>(endtime - starttime).count(); break;
		case units::microsecond:
			return std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count();break;
		}
		return -1;
	}
};

class CComparer
{
private:
	static const int expandrate = 2;
public:
	using fptr = std::vector<int>(*)(int);
	static double cvalue(fptr func, int initialN)
	{
#pragma warning(push)
#pragma warning(disable:4244)
		double t1, t2;
		Timer t;
		t.start();
		std::vector<int> a = func(initialN);
		t.end();
		t1 = t.timespan();
		t.start();
		std::vector<int> b = func(initialN * expandrate);
		t.end();
		t2 = t.timespan();
		return t1 / t2;
#pragma warning(pop)
	}
};

#endif