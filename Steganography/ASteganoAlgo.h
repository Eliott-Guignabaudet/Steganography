#pragma once
#include <string>
namespace Gdiplus
{
	class Bitmap;
};

class ASteganoAlgo
{
public:
	virtual void HideMessage(Gdiplus::Bitmap& bmp, std::string message) = 0;
	virtual std::string FindMessage() = 0;
protected:
	virtual void ParseImage() = 0;
	virtual void UnparseImage() = 0;
};

