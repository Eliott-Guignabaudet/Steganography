#include "ThemeSystem.h"
#include "UIEventSystem.h"
#include "Resource.h"

using namespace std::placeholders;

void ThemeSystem::Init(HWND mainWnd)
{
	m_mainWnd = mainWnd;
	m_currentTheme = Theme::Light;
	m_hBrush = CreateSolidBrush(RGB(173, 216, 230));
	RegisterForEvents();
}

void ThemeSystem::RegisterForEvents()
{
	auto bind = std::bind(
		&ThemeSystem::HandleBackgroundColorEvent, this, _1);
	OnBkgColor = bind;
	UIEventSystem::GetInstance()->RegisterFoMessage(WM_ERASEBKGND, OnBkgColor);

	bind = std::bind(
		&ThemeSystem::HandleCtlColorBtnEvent, this, _1);
	OnCtlColorBtn = bind;
	UIEventSystem::GetInstance()->RegisterFoMessage(WM_CTLCOLORBTN, OnCtlColorBtn);

	bind = std::bind(
		&ThemeSystem::HandleCreateEvent, this, _1);
	OnCreate = bind;
	UIEventSystem::GetInstance()->RegisterFoMessage(WM_CREATE, OnCreate);

	bind = std::bind(
		&ThemeSystem::HandleCommandEvent, this, _1);
	OnCommand = bind;
	UIEventSystem::GetInstance()->RegisterFoMessage(WM_COMMAND, OnCommand);
}

LRESULT ThemeSystem::HandleBackgroundColorEvent(EventParams params)
{
	//return 1;
	HDC hdc = (HDC)params.wParam;
	RECT rect;
	GetClientRect(params.hWnd, &rect);
	HBRUSH brush;
	// Set the background color (e.g., light blue)
	switch (m_currentTheme) 
	{
	case Theme::Light:
		brush = CreateSolidBrush(RGB(255, 255, 255)); // Light
		break;
	case Theme::Dark:
		brush = CreateSolidBrush(RGB(80, 80, 80));
		break;
	default:
		brush = CreateSolidBrush(RGB(255, 255, 255)); // Light
	}
	 // Dark
	FillRect(hdc, &rect, brush);
	DeleteObject(brush); // Clean up the brush


	//GetStockObject(DKGRAY_BRUSH);
	return 1;
}

LRESULT ThemeSystem::HandleCtlColorBtnEvent(EventParams params)
{
	HDC hdcButton = (HDC)params.wParam;
	HWND hButton = (HWND)params.lParam;
	SetBkColor(hdcButton, RGB(173, 216, 230));
	UpdateWindow(hButton);
		// Set background color
	return (LONG_PTR)m_hBrush;
}

LRESULT ThemeSystem::HandleCreateEvent(EventParams params)
{
	int xPos = 1050;
	int yPos = 70;
	int width = 150;
	int height = 40;
	const char* title = "Changer Theme";
	DWORD dwStyle = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON;
	HMENU buttonID = (HMENU)CHANGE_THEME_BUTTON;

	m_changeThemButton = CreateWindowA("Button", title, dwStyle, xPos, yPos, width, height, params.hWnd, buttonID, NULL, NULL);
	return DefWindowProc(params.hWnd, params.message, params.wParam, params.lParam);

}

LRESULT ThemeSystem::HandleCommandEvent(EventParams params)
{
	int wmId = LOWORD(params.wParam);
	if (wmId == CHANGE_THEME_BUTTON)
	{
		if (m_currentTheme == Theme::Light)
		{
			m_currentTheme = Theme::Dark; 
			
		}
		else
		{
			m_currentTheme = Theme::Light;
		}
		InvalidateRect(m_mainWnd, NULL, NULL);
	}
	return DefWindowProc(params.hWnd, params.message, params.wParam, params.lParam);
}
