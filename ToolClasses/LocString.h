#pragma once

#include <string>
#include <map>

using namespace std;

class LocString: public string
{
public:
	LocString(string localeStringPath);
	~LocString();

	string LString(string strString) { return m_locStringTable[strString]; }

private:
	map<string,string> m_locStringTable; 
};