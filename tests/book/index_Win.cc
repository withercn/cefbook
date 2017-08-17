#include <Windows.h>
#include "BrowserWindow.h"
#include "clientHandler.h"
#include "clientApp.h"

#if defined(CEF_USE_SANDBOX)
#pragma comment(lib,"cef_sandbox.lib")
#endif

//#define DEFAULT_URL CefString("http://www.baidu.com")
#define DEFAULT_URL CefString("file://" + GetApplicationDir() + "/html/index.html")

std::string GetApplicationDir()
{
	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR   wpath[MAX_PATH];

	GetModuleFileNameW(hModule, wpath, MAX_PATH);
	std::wstring wide(wpath);

	std::string path = CefString(wide);
	path = path.substr(0, path.find_last_of("\\/"));
	return path;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	int result(0);
	

	CefEnableHighDPISupport();//使用高清适配器 Win7以上

	CefMainArgs main_args(hInstance);
	CefRefPtr<clientApp> app(new clientApp);
	CefSettings settings;
	CefWindowInfo info;
	CefBrowserSettings browser_settings;
	CefRefPtr<clientHandler> client(new clientHandler);
	settings.multi_threaded_message_loop = false;
	//settings.single_process = true;

	void* sandbox_info = NULL;//沙盒
#if defined(CEF_USE_SANDBOX)
	CefScopedSandboxInfo scoped_sandbox;
	sandbox_info = scoped_sandbox.sandbox_info();
	settings.no_sandbox = true;
#endif
	
	if (CefExecuteProcess(main_args, app.get(), sandbox_info) == -1) {
		CefInitialize(main_args, settings, app.get(), sandbox_info);
		BrowserWindow win;
		HWND hwnd = win.CreateBrowserWindow(hInstance, true);
		RECT rect = { 0 };
		GetClientRect(hwnd, &rect);
		info.SetAsChild(hwnd, rect);
		CefBrowserHost::CreateBrowser(info, client.get(), DEFAULT_URL, browser_settings, NULL);
		result = win.MessageLoop(hInstance, settings.multi_threaded_message_loop);
		CefShutdown();
	}
	return result;
}
