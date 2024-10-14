#pragma once
#include <string>
class ASteganoAlgo
{
public:
	virtual void HideMessage() = 0;
	virtual std::string FindMessage() = 0;
};

