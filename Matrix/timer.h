#include <stdio.h>
#include <windows.h>

class timer
{
private:
	bool flag;
	LARGE_INTEGER lint;
	long long frequence;
    long long cnt1, cnt2;
public:
	timer() throw()
	{
		QueryPerformanceFrequency(&lint);
		frequence = lint.QuadPart;
		QueryPerformanceCounter(&lint);
		cnt1 = lint.QuadPart;
		flag = false;
	}
	void clock() throw()
	{
		QueryPerformanceCounter(&lint); 
		if(flag == true)
			cnt1 = lint.QuadPart;
		else
		{
			cnt2 = lint.QuadPart;
			printf("Time Interval: %lf ms\n", (cnt2-cnt1)*1000.0/frequence);
		}
		flag = !flag;
	}
};

class Timer
{
private:
	LARGE_INTEGER lint;  
    long long frequence;
	long long start, end;
public:
	Timer() throw()
	{
		QueryPerformanceFrequency(&lint);
		frequence = lint.QuadPart;
		QueryPerformanceCounter(&lint);
		start = lint.QuadPart;
	}
	~Timer() throw()
	{
		QueryPerformanceCounter(&lint);
		end = lint.QuadPart;
		printf("Time Interval: %lf ms\n", (end-start)*1000.0/frequence);
	}
	double time() throw()
	{
		QueryPerformanceCounter(&lint);
		return (lint.QuadPart-start)*1000.0/frequence;
	}
};


