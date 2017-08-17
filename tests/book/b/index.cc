#include "bookCefApp.h"
#include "bookCefHandler.h"
#include <windows.h>

#if defined(CEF_USE_SANDBOX)
#pragma comment(lib,"cef_sandbox.lib")
#endif

#define BROWSER_WINDOW_CLASS TEXT("BrowserWindowClass")
#define INVALID_HWND (HWND)INVALID_HANDLE_VALUE
#define MESSAGE_WINDOW_CLASS TEXT("MessageWindowClass")
#define QUIT_CEF_EXAMPLE 0xABAD1DEA

namespace
{
	CefRefPtr<bookCefHandler> book_cef_handler;
	HWND application_message_window_handle = INVALID_HWND;
}

LRESULT CALLBACK BrowserWindowWndProc(HWND, UINT, WPARAM, LPARAM);
void CreateBrowserWindow(HINSTANCE instance_handle, int show_minimize_or_maximize)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = BrowserWindowWndProc;
	wcex.hInstance = instance_handle;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = WHITE_BRUSH;
	wcex.lpszClassName = BROWSER_WINDOW_CLASS;
	RegisterClassEx(&wcex);
	HWND window_handle(CreateWindow(BROWSER_WINDOW_CLASS, BROWSER_WINDOW_CLASS,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0, NULL, NULL, instance_handle, NULL));
	ShowWindow(window_handle, show_minimize_or_maximize);
	UpdateWindow(window_handle);
}

LRESULT CALLBACK MessageWindowWndProc(HWND, UINT, WPARAM, LPARAM);
HWND CreateMessageWindow(HINSTANCE instance_handle)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(wcex);
	wcex.lpfnWndProc = MessageWindowWndProc;
	wcex.hInstance = instance_handle;
	wcex.lpszClassName = MESSAGE_WINDOW_CLASS;
	RegisterClassEx(&wcex);
	return CreateWindow(MESSAGE_WINDOW_CLASS, 0, 0, 0, 0, 0, 0, HWND_MESSAGE, 0, instance_handle, 0);
}

// Program entry point function.
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
	CefEnableHighDPISupport();

	void* sandbox_info = NULL;//É³ºÐ
#if defined(CEF_USE_SANDBOX)
	CefScopedSandboxInfo scoped_sandbox;
	sandbox_info = scoped_sandbox.sandbox_info();
#endif

	int result(0);
	CefMainArgs main_args(hInstance);
	CefRefPtr<bookCefApp> app(new bookCefApp);

	// CefExecuteProcess returns -1 for the host process
	if (CefExecuteProcess(main_args, app.get(), sandbox_info) == -1)
	{
		CefSettings settings;
#ifndef CEF_USE_SANDBOX
		settings.no_sandbox = true;
#endif
		settings.multi_threaded_message_loop = true;
		CefInitialize(main_args, settings, app.get(), sandbox_info);
		CreateBrowserWindow(hInstance, nCmdShow);
		application_message_window_handle = CreateMessageWindow(hInstance);

		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		result = static_cast<int>(msg.wParam);
		
		DestroyWindow(application_message_window_handle);
		application_message_window_handle = INVALID_HWND;
		
		// disabled due to https://code.google.com/p/chromiumembedded/issues/detail?id=755
		// CefShutdown ();

		UnregisterClass(BROWSER_WINDOW_CLASS, hInstance);
		UnregisterClass(MESSAGE_WINDOW_CLASS, hInstance);
	}
	return result;
}

LRESULT CALLBACK BrowserWindowWndProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param)
{
	LRESULT result(0);
	switch (message)
	{
	case WM_CREATE:
	{
		book_cef_handler = new bookCefHandler();

		RECT rect = { 0 };
		GetClientRect(window_handle, &rect);

		CefWindowInfo info;
		info.SetAsChild(window_handle, rect);

		CefBrowserSettings settings;
		CefBrowserHost::CreateBrowser(info, book_cef_handler.get(),
			CefString("http://www.baidu.com"), settings, NULL);
	}
	break;

	case WM_SIZE:
	{
		// from the cefclient example, do not allow the window to be resized to 0x0 or the layout will break;
		// also be aware that if the size gets too small, GPU acceleration disables
		if ((w_param != SIZE_MINIMIZED)
			&& (book_cef_handler.get())
			&& (book_cef_handler->GetBrowser()))
		{
			CefWindowHandle hwnd(book_cef_handler->GetBrowser()->GetHost()->GetWindowHandle());
			if (hwnd)
			{
				RECT rect = { 0 };
				GetClientRect(window_handle, &rect);
				HDWP hdwp = BeginDeferWindowPos(1);
				hdwp = DeferWindowPos(hdwp, hwnd, NULL, rect.left,
					rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
				EndDeferWindowPos(hdwp);
			}
		}
	}
	break;

	case WM_ERASEBKGND:
	{
		if ((book_cef_handler.get())
			&& (book_cef_handler->GetBrowser()))
		{
			CefWindowHandle hwnd(book_cef_handler->GetBrowser()->GetHost()->GetWindowHandle());
			// from the cefclient example, don't erase the background 
			// if the browser window has been loaded to avoid flashing
			result = hwnd ? 1 : DefWindowProc(window_handle, message, w_param, l_param);
		}
	}
	break;

	case WM_ENTERMENULOOP:
	{
		if (!w_param)
		{
			CefSetOSModalLoop(true);
		}
		result = DefWindowProc(window_handle, message, w_param, l_param);
	}
	break;

	case WM_EXITMENULOOP:
	{
		if (!w_param)
		{
			CefSetOSModalLoop(false);
		}
		result = DefWindowProc(window_handle, message, w_param, l_param);
	}
	break;
	case WM_CLOSE:
		book_cef_handler->GetBrowser()->GetHost()->CloseBrowser(false);
		result = 0;
		break;
	case WM_DESTROY:
		/*result = 0;*/
		break;

	default:
	{
		result = DefWindowProc(window_handle, message, w_param, l_param);
	}
	break;
	}
	return result;
}

LRESULT CALLBACK MessageWindowWndProc(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param)
{
	LRESULT result(0);
	switch (message)
	{
	case WM_COMMAND:
	{
		if (w_param == QUIT_CEF_EXAMPLE)
		{
			PostQuitMessage(0);
		}
	}
	break;

	default:
	{
		result = DefWindowProc(window_handle, message, w_param, l_param);
	}
	break;
	}
	return result;
}

void AppQuitMessageLoop()
{
	if (application_message_window_handle != INVALID_HWND)
	{
		PostMessage(application_message_window_handle, WM_COMMAND, QUIT_CEF_EXAMPLE, 0);
	}
}