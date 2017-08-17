#include <Windows.h>
#include <list>
#include "include/views/cef_window.h"
#include "include/cef_browser.h"
#include "clientHandler.h"

#ifndef __BASE_WINDOW__
#define __BASE_WINDOW__
using namespace std;
class baseWindow {
public:
	baseWindow(HINSTANCE hInstance, CefRefPtr<clientHandler> client);
	~baseWindow();

	void CreateMainWindow();
	//void CreateBrowserWindow(CefWindowHandle hwnd, const CefRect &rect, const CefBrowserSettings& settings, CefRefPtr<CefRequestContext> request_context);
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static int CALLBACK FolderCallbackProc(HWND hwnd, UINT msg, LPARAM lParam, LPARAM lpData);
	CefWindowHandle GetHandle();
	//Window Functions
	static void Show(HWND hwnd, UINT nCmdShow);
	static void Close();
	static void StartDrag(HWND hwnd);
	static void ShowDevTools(HWND hwnd, CefRefPtr<CefBrowser> browser);
	static void FullScreen(HWND hwnd);
	static void ResizeTo(HWND hwnd, int width, int height);
	static void MoveTo(HWND hwnd, int x, int y);
	static bool WindowsStart(bool start);
	static CefString BrowserFolder(HWND hwnd);
private:
	void RegisterRootClass(HINSTANCE hInstance, const std::wstring& window_class);
	void InitTray(HINSTANCE hInstance, HWND hwnd);
	HINSTANCE instance_;
	CefWindowHandle hwnd_;
};
#endif // !__BASE_WINDOW__