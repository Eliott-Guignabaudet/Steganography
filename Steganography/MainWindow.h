#pragma once
#include "UIEventSystem.h"

class ImageLoader;

class MainWindow
{
public:
	int Init(HINSTANCE hInstance, int nCmdShow);
	void RegisterForEvents();

	std::function<void(EventParams)> OnCreate;
	std::function<void(EventParams)> OnEventCommand;

	WCHAR m_szTitle[100];
	WCHAR m_szWindowClass[100];

private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	HWND m_imageZone;
	HWND m_messageInputField;
	HWND m_logWidowElement;
	HWND m_messageDisplayElement;

	ImageLoader* m_imgLoader;



	void HandleCreateEvent(EventParams params);
	LRESULT HandleCommandEvent(EventParams params);
	void AddButtons();
	ATOM MyRegisterClass(HINSTANCE hInstance);
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

	int OpenFile(HWND hwnd);
	void HideMessage(HWND hwnd);
	void ExtractMessage(HWND hwnd);

	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};