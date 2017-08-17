#include <Windows.h>
#include "clientHandler.h"

class BrowserWindow {
public:
	BrowserWindow();
	~BrowserWindow();
	HWND CreateMessageWindow(HINSTANCE hInstance);
	HWND CreateBrowserWindow(HINSTANCE hInstance);
	HWND CreateBrowserWindow(HINSTANCE hInstance, bool sMaximize);
	HWND CreateBrowserWindow(HINSTANCE hInstance, bool sMaximize, DWORD dwStyle);
	HWND CreateBrowserWindow(HINSTANCE hInstance, bool sMaximize, DWORD dwStyle, DWORD dwExStyle);
	void SetClient(CefRefPtr<clientHandler> client);
	int MessageLoop(HINSTANCE hInstance, bool multi_threaded_message_loop);
};