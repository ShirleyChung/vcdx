#include "pch.h"
#include "WorldTime.h"
#include <string>

using namespace std;

void WorldTime::createThread()
{
}

void WorldTime::time_loop(void* p)
{
	int counter = 0;
	wstring msg = L"current time is:";

	OutputDebugString(msg.c_str());
}