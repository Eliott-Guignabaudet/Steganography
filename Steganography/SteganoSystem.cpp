#include "SteganoSystem.h"
#include "ASteganoAlgo.h";

SteganoSystem* SteganoSystem::s_instance = nullptr;

void SteganoSystem::LoadImage()
{
}

SteganoSystem* SteganoSystem::GetInstance()
{
	if (s_instance == nullptr)
	{
		s_instance = new SteganoSystem();
	}
    return s_instance;
}

bool SteganoSystem::HideMessage(std::string a_message)
{
	return false;
}

std::string SteganoSystem::FindMessage()
{
	return std::string();
}

void SteganoSystem::ParseImage()
{
}

void SteganoSystem::UnparseImage()
{
}
