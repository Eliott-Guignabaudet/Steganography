#include "UIEventSystem.h"

UIEventSystem* UIEventSystem::s_instance = nullptr;

UIEventSystem* UIEventSystem::GetInstance()
{
	if (s_instance == nullptr)
	{
		s_instance = new UIEventSystem();
	}
    return s_instance;
}

void UIEventSystem::RegisterFoMessage(
   UINT message,  std::function<void(EventParams)> callback)
{
	m_registeredEvents[message].push_back(callback);
}

void UIEventSystem::UnregisterForMessage(
    UINT message,  std::function<void(EventParams)> callback)
{

}

LRESULT UIEventSystem::HandleMessages(
HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    char buffer[250];
	auto& registeredEvents = s_instance->m_registeredEvents;
	if (registeredEvents.find(message) == registeredEvents.end())
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	for (int i = 0; i < s_instance->m_registeredEvents[message].size(); i++)
	{
		auto function = s_instance->m_registeredEvents[message][i];
		if (function) 
		{
			EventParams params;
			params.hWnd = hWnd;
			params.message = message;
			params.wParam = wParam;
			params.lParam = lParam;
			function(params);
		}
	}
	return 0;
}
