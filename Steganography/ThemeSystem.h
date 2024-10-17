#pragma once
#include <functional>
#include <windows.h>

struct EventParams;

class ThemeSystem
{
public:
	void Init();
private:
	HBRUSH hBrush;

	std::function<LRESULT(EventParams)> OnBkgColor;
	std::function<LRESULT(EventParams)> OnCtlColorBtn;

	void RegisterForEvents();
	LRESULT HandleBackgroundColorEvent(EventParams);
	LRESULT HandleCtlColorBtnEvent(EventParams);

};

