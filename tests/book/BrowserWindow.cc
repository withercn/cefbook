#include <Windows.h>
#include "BrowserWindow.h"
#include "rectange.h"
#include "clientHandler.h"
#include "include/cef_app.h"

#define BROWSER_WINDOW_CLASS TEXT("云小说")
#define INVALID_HWND (HWND)INVALID_HANDLE_VALUE
#define MESSAGE_WINDOW_CLASS TEXT("MessageWindowClass")
#define MESSAGE_QUIT 0xABAD1DEA

namespace {
	BrowserWindow* instance = NULL; 
	CefRefPtr<clientHandler> client;
}

#pragma region 窗口注册与回调定义
LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK msgWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BrowserWindow::BrowserWindow() { instance = this; }
BrowserWindow::~BrowserWindow() { 
	instance = NULL;
	client = NULL;
}
HWND BrowserWindow::CreateMessageWindow(HINSTANCE instance_handle)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(wcex);
	wcex.lpfnWndProc = msgWndProc;
	wcex.hInstance = instance_handle;
	wcex.lpszClassName = MESSAGE_WINDOW_CLASS;
	RegisterClassEx(&wcex);
	return CreateWindow(MESSAGE_WINDOW_CLASS, 0, 0, 0, 0, 0, 0, HWND_MESSAGE, 0, instance_handle, 0);
}
HWND BrowserWindow::CreateBrowserWindow(HINSTANCE hInstance) {
	return CreateBrowserWindow(hInstance, false, NULL, NULL);
}
HWND BrowserWindow::CreateBrowserWindow(HINSTANCE hInstance, bool sMaximize) {
	return CreateBrowserWindow(hInstance, sMaximize, NULL, NULL);
}
HWND BrowserWindow::CreateBrowserWindow(HINSTANCE hInstance, bool sMaximize, DWORD dwStyle) {
	return CreateBrowserWindow(hInstance, sMaximize, dwStyle, NULL);
}
HWND BrowserWindow::CreateBrowserWindow(HINSTANCE hInstance, bool sMaximize, DWORD dwStyle, DWORD dwExStyle) {
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = WHITE_BRUSH;
	wcex.lpszClassName = BROWSER_WINDOW_CLASS;
	RegisterClassEx(&wcex);

	if (dwStyle == NULL)
		dwStyle = WS_POPUP | WS_BORDER;// WS_OVERLAPPEDWINDOW;
	if (dwExStyle == NULL)
		dwExStyle = WS_EX_APPWINDOW;
	int scWidth = GetSystemMetrics(SM_CXSCREEN);
	int scHeight = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYSIZE); //去除任务栏的高度;;
	Rectange rect(0, 0, scWidth * 3 / 4, scHeight * 3 / 4);
	if (sMaximize) {
		rect.width = scWidth;
		rect.height = scHeight;
	}
	rect.x = (scWidth - rect.width) / 2;
	rect.y = (scHeight - rect.height) / 2;
	HWND hwnd = CreateWindowEx(dwExStyle, BROWSER_WINDOW_CLASS, BROWSER_WINDOW_CLASS, dwStyle, rect.x, rect.y, rect.width, rect.height, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, sMaximize ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);
	UpdateWindow(hwnd);
	return hwnd;
}
#pragma endregion
void BrowserWindow::SetClient(CefRefPtr<clientHandler> clients) {
	client = clients;
}
LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	LRESULT result(0);
	switch (msg) {
		case WM_SIZE:
		{
			if (wParam != SIZE_MINIMIZED) {
				if (client.get() && client->GetBrowser()) {
					HWND mHwnd = client->GetBrowser()->GetHost()->GetWindowHandle();
					RECT rect = { 0 };
					GetClientRect(hwnd, &rect);
					HDWP hdwp = BeginDeferWindowPos(1);
					hdwp = DeferWindowPos(hdwp, mHwnd, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
					EndDeferWindowPos(hdwp);
				}
				else if(wParam == SIZE_MAXIMIZED)
				{
					HMONITOR hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
					MONITORINFO moninfo;
					moninfo.cbSize = sizeof(moninfo);
					GetMonitorInfo(hmon, &moninfo);

					SetWindowPos(hwnd, 0, moninfo.rcWork.left, moninfo.rcWork.top,
						moninfo.rcWork.right, moninfo.rcWork.bottom, SWP_NOZORDER);
				}
			}
		}
		break;
		case WM_ERASEBKGND:
		{
			if (client.get()) {
				result = 0;
			}
		}
		break;
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			result = 0;
		}
		default:
		{
			result = DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}
	return result;
}
LRESULT CALLBACK msgWndProc(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param)
{
	return DefWindowProc(hwnd, message, w_param, l_param);
	//LRESULT result(0);
	//switch (message)
	//{
	//	case WM_COMMAND:
	//	{
	//		if (w_param == MESSAGE_QUIT)
	//		{
	//			DestroyWindow(window_handle);
	//		}
	//	}
	//	break;
	//	case WM_DESTROY: {
	//		PostQuitMessage(0);
	//	}
	//	default:
	//	{
	//		result = DefWindowProc(window_handle, message, w_param, l_param);
	//	}
	//	break;
	//}
	//return result;
}
int BrowserWindow::MessageLoop(HINSTANCE hInstance, bool multi_threaded_message_loop) {
	int result(0);
	if (!multi_threaded_message_loop) {
		CefRunMessageLoop();
	}
	else
	{
		HWND msgHwnd = CreateMessageWindow(hInstance);
		MSG  msg;

		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		UnregisterClass(MESSAGE_WINDOW_CLASS, hInstance);
		UnregisterClass(BROWSER_WINDOW_CLASS, hInstance);
		DestroyWindow(msgHwnd);
		msgHwnd = NULL;
		result = static_cast<int> (msg.wParam);
	}
	return result;
}