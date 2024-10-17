#pragma once
#include "UIEventSystem.h"

class ImageLoader;

class MainWindow
{
public:
	int Run(HINSTANCE hInstance, int nCmdShow);
	void RegisterForEvents();

#pragma region Events
	std::function<LRESULT(EventParams)> OnCreate;
	std::function<LRESULT(EventParams)> OnEventCommand;
	std::function<LRESULT(EventParams)> OnEventPaint;
	std::function<LRESULT(EventParams)> OnEventDestroy;
#pragma endregion

	WCHAR m_szTitle[100];
	WCHAR m_szWindowClass[100];

private:

#pragma region UIElement
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	HWND m_imageZone;
	HWND m_messageInputField;
	HWND m_logWidowElement;
	HWND m_messageDisplayElement;
#pragma endregion

	ImageLoader* m_imgLoader;
	HBITMAP m_hbitmap = NULL;
	WCHAR* m_imagePath;

#pragma region Event Handlers
	LRESULT HandleCreateEvent(EventParams params);
	LRESULT HandleCommandEvent(EventParams params);
	LRESULT HandlePaintEvent(EventParams params);
	LRESULT HandleDestroyEvent(EventParams params);
#pragma endregion

	void AddButtons(HWND hwnd);
	ATOM MyRegisterClass(HINSTANCE hInstance);
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

	int OpenFile(HWND hwnd);
	void HideMessage(HWND hwnd);
	void ExtractMessage(HWND hwnd);

	static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};