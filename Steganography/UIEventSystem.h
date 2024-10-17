#pragma once
#include <windows.h>
#include <functional>
#include <map>
#include <vector>

struct EventParams 
{
	HWND hWnd;
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
};

class UIEventSystem
{
private:
	static UIEventSystem* s_instance;


	std::map<UINT,std::vector<std::function<void(EventParams)> >> m_registeredEvents;


public :
	static UIEventSystem* GetInstance();
	void RegisterFoMessage(UINT message, std::function<void(EventParams)> callback);
	void UnregisterForMessage(UINT message, std::function<void(EventParams)> callback);

	static LRESULT CALLBACK HandleMessages(HWND, UINT, WPARAM, LPARAM);
};

