#pragma once
#include <string>

class ASteganoAlgo;

class SteganoSystem
{
private:
	static SteganoSystem* s_instance;
	ASteganoAlgo* m_usedAlgo;
	void LoadImage();
public:
	static SteganoSystem* GetInstance();

	bool HideMessage(std::string a_message);
	std::string FindMessage();
	void ParseImage();
	void UnparseImage();

};

