#pragma once
#include <functional>
#include <windows.h>

struct EventParams;

enum class Theme 
{
	Light = 0,
	Dark
};

class ThemeSystem
{
public:
	void Init(HWND mainWnd);


private:
	HWND m_mainWnd;
	HBRUSH m_hBrush;
	HWND m_changeThemButton;
	Theme m_currentTheme;

	std::function<LRESULT(EventParams)> OnBkgColor;
	std::function<LRESULT(EventParams)> OnCtlColorBtn;
	std::function<LRESULT(EventParams)> OnCreate;
	std::function<LRESULT(EventParams)> OnCommand;

	void RegisterForEvents();
	LRESULT HandleBackgroundColorEvent(EventParams);
	LRESULT HandleCtlColorBtnEvent(EventParams);
	LRESULT HandleCreateEvent(EventParams);
	LRESULT HandleCommandEvent(EventParams);

};

