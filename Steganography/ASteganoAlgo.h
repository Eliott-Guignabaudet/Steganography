#pragma once
#include <string>
namespace Gdiplus
{
	class Bitmap;
};

class ASteganoAlgo
{
public:
	virtual void HideMessage(Gdiplus::Bitmap& bmp, const std::string& message) = 0;
	virtual bool FindMessage(Gdiplus::Bitmap& bmp, std::string& result) = 0;
protected:
	virtual void ParseImage() = 0;
	virtual void UnparseImage() = 0;
};

