#pragma once

#include "GameObject.h"

class WorldTime{
public:
	WorldTime(){ createThread();}
	~WorldTime();

protected:
	void createThread();
	static void time_loop();

	UINT m_hThread;
};