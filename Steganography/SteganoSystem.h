#pragma once
#include <string>

class ASteganoAlgo;
namespace Gdiplus 
{
	class Bitmap;
}

class SteganoSystem
{
private:
	static SteganoSystem* s_instance;
	bool m_isInit;

	ASteganoAlgo* m_algo;
	void Init();

public:
	static SteganoSystem* GetInstance();

	bool HideMessage(Gdiplus::Bitmap& bmp, const std::string& message);
	std::string FindMessage(Gdiplus::Bitmap& bmp);

};

