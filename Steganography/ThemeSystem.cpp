#include "ThemeSystem.h"
#include "UIEventSystem.h"

using namespace std::placeholders;

void ThemeSystem::Init()
{
	hBrush = CreateSolidBrush(RGB(173, 216, 230));
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


}

LRESULT ThemeSystem::HandleBackgroundColorEvent(EventParams params)
{
	HDC hdc = (HDC)params.wParam;
	RECT rect;
	GetClientRect(params.hWnd, &rect);

	// Set the background color (e.g., light blue)
	HBRUSH brush = CreateSolidBrush(RGB(173, 216, 230)); // Light blue
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
	return (LONG_PTR)hBrush;
}
