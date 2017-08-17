#include <Windows.h>

#include "include/cef_sandbox_win.h"
#include "book.h"

#if defined(CEF_USE_SANDBOX)
#pragma comment(lib,"cef_sandbox.lib")
#endif

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	CefEnableHighDPISupport();//使用高清适配器 Win7以上

	void* sandbox_info = NULL;//沙盒

#if defined(CEF_USE_SANDBOX)
	CefScopedSandboxInfo scoped_sandbox;
	sandbox_info = scoped_sandbox.sandbox_info();
#endif

	CefMainArgs main_args(hInstance);
	int exit_code = CefExecuteProcess(main_args, NULL, sandbox_info);
	if (exit_code > 0)
		return exit_code;

	CefSettings settings;
#ifndef CEF_USE_SANDBOX
	settings.no_sandbox = true;
#endif
	
	CefRefPtr<bookApp> app(new bookApp);
	CefInitialize(main_args, settings, app.get(), sandbox_info);
	CefRunMessageLoop();
	CefShutdown();
	return 0;
}