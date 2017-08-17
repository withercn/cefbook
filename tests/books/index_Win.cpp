#include <Windows.h>
#include "baseWindow.h"
#include "clientHandler.h"
#include "clientApp.h"
#include "utils.h"
#include "clientScheme.h"

#if defined(CEF_USE_SANDBOX)
#pragma comment(lib,"cef_sandbox.lib")
#endif

using namespace std;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	CefEnableHighDPISupport();//使用高清适配器 Win7以上

	CefMainArgs main_args(hInstance);
	CefRefPtr<clientApp> app(new clientApp);
	CefSettings settings;
	settings.command_line_args_disabled = true;
	settings.ignore_certificate_errors = true;
	settings.log_severity = cef_log_severity_t::LOGSEVERITY_DISABLE;
	//settings.multi_threaded_message_loop = true;
	//settings.single_process = true;
	settings.remote_debugging_port = 8089;
	std::string cache_path = Utils::GetApplicationDir() + "/Cache";
	std::string user_data_path = Utils::GetApplicationDir() + "/UserData";
	CefString(&settings.cache_path).FromString(cache_path);
	CefString(&settings.user_data_path).FromString(user_data_path);
	void* sandbox_info = NULL;//沙盒
#if defined(CEF_USE_SANDBOX)
	CefScopedSandboxInfo scoped_sandbox;
	sandbox_info = scoped_sandbox.sandbox_info();
	settings.no_sandbox = true;
#endif
	
	if (CefExecuteProcess(main_args, app.get(), sandbox_info) == -1) {
		CefInitialize(main_args, settings, app.get(), sandbox_info);
		CefRegisterSchemeHandlerFactory(SCHEME_NAME, HOST_NAME, new clientSchemeHandlerFactory());
		CefRefPtr<clientHandler> client(new clientHandler);
		baseWindow window(hInstance, client);
		RECT rect = { 0 };
		GetClientRect(window.GetHandle(), &rect);
		CefBrowserSettings browser_settings;
		CefWindowInfo info;
		info.SetAsChild(window.GetHandle(), rect);

		CefBrowserHost::CreateBrowser(info, client, DEFAULT_URL, browser_settings, NULL);

		//info.SetAsChild(hwnd, rect);
		//CefBrowserHost::CreateBrowser(info, client.get(), DEFAULT_URL, browser_settings, NULL);
		//if (!settings.multi_threaded_message_loop) {
		//	CefRunMessageLoop();
		//	//win.UnRegisterWindow(hInstance);
		//}
		//else
		//{
		//	BrowserWindow::CreateMessageWindow(hInstance);
		//}
		CefRunMessageLoop();
		CefShutdown();
	}
	return 0;
}