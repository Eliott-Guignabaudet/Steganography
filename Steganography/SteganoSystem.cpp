#include <string>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;

#include "SteganoSystem.h"
#include "SteganoAlgoLSB.h";

SteganoSystem* SteganoSystem::s_instance = nullptr;

void SteganoSystem::Init()
{
	if (m_isInit)
	{
		return;
	}
	m_algo = new SteganoAlgoLSB();
}

SteganoSystem* SteganoSystem::GetInstance()
{
	if (s_instance == nullptr)
	{
		s_instance = new SteganoSystem();
		s_instance->Init();
	}
    return s_instance;
}

bool SteganoSystem::HideMessage(Bitmap& bmp, const std::string& message)
{
	m_algo->HideMessage(bmp, message);
	UINT x = bmp.GetWidth() - 20;
	UINT y = bmp.GetHeight() - 20;
	Gdiplus::Color color ;
	bmp.GetPixel(x, y, &color);
	BYTE r = color.GetR();
	BYTE g = color.GetG();
	BYTE b = color.GetB();
	
	return false;
}

std::string SteganoSystem::FindMessage(Gdiplus::Bitmap& bmp)
{
	std::string message;
	m_algo->FindMessage(bmp, message);
	return message;
}

