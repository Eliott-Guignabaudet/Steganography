#pragma once
#include <functional>
#include <windows.h>

namespace Gdiplus 
{
	class Bitmap;
}
using namespace Gdiplus;

class ImageLoader;
struct EventParams;


enum class Filter 
{
	None = 0,
	BlanckAndWhite,
	Reverse
};


class FilterSystem
{
public:
	void Init(HWND hwnd, ImageLoader* imgLoader);
	Bitmap* GetFilteredBitmap();


	Filter GetFilter();
	void SetFilter(Filter);
private:
	HWND m_mainWnd;
	ImageLoader* m_imgLoader;
	Bitmap* m_filteredBitmap;
	Filter m_currentFilter;

	std::function<LRESULT(EventParams)> OnCommand;

	void RegisterForEvents();

	void SetBlackAndWhiteFilter();
	void SetReverseFilter();

	LRESULT HandleCommandEvent(EventParams);
};

