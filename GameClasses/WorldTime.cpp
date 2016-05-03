#include "pch.h"
#include "WorldTime.h"
#include <string>
#include <iostream>
#include <thread>

using namespace std;

void WorldTime::createThread()
{
	thread timerThread(time_loop);
}

void WorldTime::time_loop()
{
	int counter = 0;
	wstring s;
	std::chrono::seconds sec(1);
	while(1)
	{
		s =  L"counter: ";
		s += counter++;
		cout<<s.c_str()<<endl;
		this_thread::sleep_for(sec);
	}
}