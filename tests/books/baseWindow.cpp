#include <dwmapi.h>
#include <shellapi.h>
#include "baseWindow.h"
#include "utils.h"
#include "resource.h"
#include "AppConfig.h"
#include <stdio.h>
#include<Shlobj.h>  
#pragma comment(lib,"Shell32.lib") 
#pragma comment(lib, "dwmapi.lib")

namespace {
	CefRefPtr<clientHandler> client_;
	HWND hwnd_root;
	WINDOWPLACEMENT wp;
	LONG dwStyle;
	static bool fullscreen_;
	HMENU menu_;
	NOTIFYICONDATA nId;
	AppConfig config;

	auto GetRect() {
		int scWidth = GetSystemMetrics(SM_CXSCREEN);
		int scHeight = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYSIZE); //去除任务栏的高度
		int width = scWidth * 3 / 4;
		int height = scHeight * 3 / 4;
		int x = (scWidth - width) / 2;
		int y = (scHeight - height) / 2;
		return RECT({ x,y,width,height });
	}
	auto GetBrowser() {
		return client_->GetMainFrameBrowser();
	}
	auto maximized(HWND hwnd) -> bool {
		WINDOWPLACEMENT placement;
		if (!::GetWindowPlacement(hwnd, &placement)) {
			return false;
		}

		return placement.showCmd == SW_MAXIMIZE;
	}
	auto adjust_maximized_client_rect(HWND window, RECT& rect) -> void {
		if (!maximized(window)) {
			return;
		}

		auto monitor = ::MonitorFromWindow(window, MONITOR_DEFAULTTONULL);
		if (!monitor) {
			return;
		}

		MONITORINFO monitor_info{};
		monitor_info.cbSize = sizeof(monitor_info);
		if (!::GetMonitorInfoW(monitor, &monitor_info)) {
			return;
		}
		// when maximized, make the client area fill just the monitor (without task bar) rect,
		// not the whole window rect which extends beyond the monitor.
		rect = monitor_info.rcWork;
	}
	auto ShowCefTray(HWND hwnd, POINT pt) {
		CefBrowserSettings browser_settings;
		CefWindowInfo info;
		info.style = WS_POPUP;
		info.x = pt.x - 75;
		info.y = pt.y - 400;
		info.width = 150;
		info.height = 400;
		info.parent_window = hwnd;
		CefString(&info.window_name).FromString("Clound_Book_Tray");
		CefBrowserHost::CreateBrowser(info, client_, TRAY_URL, browser_settings, NULL);
	}
}
baseWindow::baseWindow(HINSTANCE hInstance,  CefRefPtr<clientHandler> client) {
	instance_ = hInstance;
	client_ = client;
	CreateMainWindow();
}
baseWindow::~baseWindow() {
	instance_ = NULL;
	hwnd_ = NULL;
}
CefWindowHandle baseWindow::GetHandle() { return hwnd_; }

#pragma region MainWindow
void baseWindow::CreateMainWindow() {
	RegisterRootClass(instance_, MAIN_WINDOW_CLASS);
	RECT rect = GetRect();
	int nShowCmd = SW_SHOW;
	if (config.setting.maximize)
		nShowCmd = SW_MAXIMIZE;		
	hwnd_ = CreateWindowEx(
		WS_EX_STATICEDGE | WS_EX_APPWINDOW,
		MAIN_WINDOW_CLASS,
		MAIN_WINDOW_CLASS,
		WS_OVERLAPPEDWINDOW,
		rect.left,rect.top,rect.right,rect.bottom,
		NULL, NULL, instance_, NULL);
	hwnd_root = hwnd_;
	//HINSTANCE hUser32 = LoadLibrary(L"user32.dll");
	//if (hUser32)
	//{
	//	typedef BOOL(WINAPI* LPSetProcessDPIAware)(void);
	//	LPSetProcessDPIAware pSetProcessDPIAware = (LPSetProcessDPIAware)GetProcAddress(hUser32, "SetProcessDPIAware");
	//	if (pSetProcessDPIAware)
	//	{
	//		pSetProcessDPIAware();
	//	}
	//	FreeLibrary(hUser32);
	//}
	BOOL composition_enabled = FALSE;
	if (::DwmIsCompositionEnabled(&composition_enabled) == S_OK) {
		static const MARGINS shadow_state{ 1,1,1,1 };
		::DwmExtendFrameIntoClientArea(hwnd_, &shadow_state);
	}
	::SetWindowPos(hwnd_, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
	::ShowWindow(hwnd_, nShowCmd);
	::UpdateWindow(hwnd_);
	//InitTray(instance_, hwnd_);
	WindowsStart(config.setting.start);
}
void baseWindow::InitTray(HINSTANCE hInstance, HWND hwnd) {
	nId.cbSize = sizeof(NOTIFYICONDATA);
	nId.hWnd = hwnd;
	nId.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
	nId.uCallbackMessage = WM_TRAY;
	nId.hIcon = LoadIcon(hInstance, (LPCWSTR)IDI_ICON1);
	lstrcpy(nId.szTip, MAIN_WINDOW_CLASS);

	/*menu_ = CreatePopupMenu();
	AppendMenu(menu_, MF_STRING, ID_EXIT, TEXT("退出"));*/

	Shell_NotifyIcon(NIM_ADD, &nId);
}

void baseWindow::RegisterRootClass(HINSTANCE hInstance, const std::wstring& window_class) {
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;// | CS_DROPSHADOW;
	wcex.lpfnWndProc = WindowProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = WHITE_BRUSH;
	wcex.lpszClassName = window_class.c_str();
	wcex.hIcon = LoadIcon(hInstance, (LPCWSTR)IDI_ICON1);
	RegisterClassEx(&wcex);
}
LRESULT CALLBACK baseWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_NCCREATE) {
		auto userdata = reinterpret_cast<CREATESTRUCTW*>(lParam)->lpCreateParams;
		// store window instance pointer in window user data
		::SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(userdata));
	}
	//::SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	//::SetLayeredWindowAttributes(hwnd, 0, (255 * 20) / 150, LWA_ALPHA);//透明
	switch (msg) {
		case WM_NCCALCSIZE: {
			if (wParam == TRUE) {
				auto& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
				adjust_maximized_client_rect(hwnd, params.rgrc[0]);
				return 1;
			}
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		break;
		case WM_TRAY: {
			switch (lParam) {
				case WM_LBUTTONDBLCLK:
				{
					ShowWindow(hwnd, SW_RESTORE);
					SetForegroundWindow(hwnd);
				}
				break;
				case WM_RBUTTONDOWN:
				{
					POINT pt;
					GetCursorPos(&pt);
					SetForegroundWindow(hwnd);
					ShowCefTray(hwnd, pt);
					//int cmd = TrackPopupMenu(menu_, TPM_RETURNCMD, pt.x, pt.y, NULL, hwnd, NULL);
					//if (cmd == ID_EXIT)
					//	PostMessage(hwnd, WM_CLOSE, NULL, NULL);
				}
			}
			return 0;
		}
		break;
		case WM_ERASEBKGND:
		{
			if (client_.get()) {
				return 0;
			}
		}
		break;
		case WM_DESTROY:
		{
			WINDOWPLACEMENT winPlacment;
			::GetWindowPlacement(hwnd, &winPlacment);
			if (winPlacment.showCmd == SW_MAXIMIZE)
				config.setting.maximize = true;
			else
				config.setting.maximize = false;
			config.SaveSetting();
			Shell_NotifyIcon(NIM_DELETE, &nId);
			return 0;
		}
		break;
		case WM_SIZE:
		{
			if (wParam == SIZE_MAXIMIZED)
			{
				HMONITOR hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
				MONITORINFO moninfo;
				moninfo.cbSize = sizeof(moninfo);
				GetMonitorInfo(hmon, &moninfo);

				SetWindowPos(hwnd, 0, moninfo.rcWork.left, moninfo.rcWork.top,
					moninfo.rcWork.right, moninfo.rcWork.bottom, SWP_NOZORDER);
			}
			if (wParam != SIZE_MINIMIZED) {
				if (client_.get()) {
					RECT rect;
					GetClientRect(hwnd, &rect);
					client_->OnResize(rect);
				}
			}
			else {
				ShowWindow(hwnd, SW_SHOWMINIMIZED);
			}
		}
		break;
		case WM_NCHITTEST:
		{
			if (wParam == HTTOP)
				return HTTOP;
		}
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
#pragma endregion
#pragma region Functions
void baseWindow::Show(HWND hwnd, UINT nCmdShow) {
	HWND root_wnd = ::GetAncestor(hwnd, GA_ROOT);//获取顶级窗口
	WINDOWPLACEMENT placement;
	::GetWindowPlacement(root_wnd, &placement);

	if (placement.showCmd == nCmdShow)
	{
		if (fullscreen_) {
			wp.showCmd = SW_RESTORE;
			SetWindowPlacement(root_wnd, &wp);
			fullscreen_ = false;
		}
		else {
			::ShowWindow(root_wnd, SW_RESTORE);
		}
	}
	else
		::ShowWindow(root_wnd, nCmdShow);
}
void baseWindow::Close() {
	//HWND root_wnd = GetAncestor(hwnd, GA_ROOT);//获取顶级窗口
	PostMessage(hwnd_root, WM_CLOSE, NULL, NULL);
}
void baseWindow::StartDrag(HWND hwnd) {
	HWND root_wnd = ::GetAncestor(hwnd, GA_ROOT);
	WINDOWPLACEMENT placement;
	::GetWindowPlacement(root_wnd, &placement);
	if (placement.showCmd == SW_MAXIMIZE)
		return;
	::ReleaseCapture();
	::SendMessage(root_wnd, WM_NCLBUTTONDOWN, HTCAPTION, NULL);
	::SendMessage(root_wnd, WM_LBUTTONUP, NULL, NULL);
}
void baseWindow::ShowDevTools(HWND hwnd, CefRefPtr<CefBrowser> browser) {
	if (browser->GetHost()->HasDevTools())
	{
		browser->GetHost()->CloseDevTools();
	}
	else {
		CefWindowInfo info;
		info.SetAsPopup(hwnd, "DevTools");
		CefBrowserSettings settings;
		browser->GetHost()->ShowDevTools(info, client_, settings, CefPoint());
	}
}
void baseWindow::FullScreen(HWND hwnd) {
	HWND root_wnd = GetAncestor(hwnd, GA_ROOT);
	if (fullscreen_)
	{
		wp.showCmd = SW_RESTORE;
		SetWindowLong(root_wnd, GWL_STYLE, dwStyle);
		SetWindowPlacement(root_wnd, &wp);
		fullscreen_ = false;
	}
	else
	{
		GetWindowPlacement(root_wnd, &wp);
		dwStyle = GetWindowLong(root_wnd, GWL_STYLE);
		SetWindowLong(root_wnd, GWL_STYLE, WS_POPUP);
		Rectange rect(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
		SetWindowPos(root_wnd, 0, rect.x, rect.y, rect.width, rect.height, SWP_SHOWWINDOW);
		SetForegroundWindow(root_wnd);
		fullscreen_ = true;
	}
}
void baseWindow::ResizeTo(HWND hwnd, int width, int height) {
	// Retrieve current window placement information.
	POINT pt;
	GetCursorPos(&pt);
	pt.x = pt.x - (width / 2);
	pt.y = pt.y - height;
	::SetWindowPos(hwnd, NULL, pt.x, pt.y, width, height, SWP_NOZORDER);
}
void baseWindow::MoveTo(HWND hwnd, int x, int y) {
	::SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

}
bool baseWindow::WindowsStart(bool start) {
	BOOL isWow64;
	IsWow64Process(GetCurrentProcess(), &isWow64);
	REGSAM p;
	string path = Utils::GetApplicationPath();
	size_t size = path.length();
	wchar_t* buffer = new wchar_t[size + 1];
	MultiByteToWideChar(CP_ACP, 0, path.c_str(), size, buffer, size * sizeof(wchar_t));
	buffer[size] = 0;

	if (isWow64)
		p = KEY_WRITE | KEY_WOW64_64KEY;
	else
		p = KEY_WRITE;

	HKEY hKey;
	if (start) {
		if (RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, NULL, 0, p, NULL, &hKey, NULL) != ERROR_SUCCESS)
			return false;
		if (RegSetValueEx(hKey, MAIN_WINDOW_CLASS, 0, REG_SZ, (BYTE*)buffer, sizeof(wchar_t)*size) != ERROR_SUCCESS)
			return false;
	}
	else
	{
		if (RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, NULL, 0, p, NULL, &hKey, NULL) != ERROR_SUCCESS)
			return false;
		if (RegDeleteValue(hKey, MAIN_WINDOW_CLASS) != ERROR_SUCCESS)
			return false;
	}
	delete buffer;
	buffer = NULL;
	RegCloseKey(hKey);
	return true;
}
CefString baseWindow::BrowserFolder(HWND hwnd) {
	CefString folder;
	TCHAR path[MAX_PATH];
	wstring wpath = Utils::GetApplicationDirW();
	BROWSEINFO info = { 0 };
	info.hwndOwner = hwnd;
	info.pszDisplayName = path;
	info.lpszTitle = TEXT("请选择目录");
	info.lpfn = FolderCallbackProc;
	info.ulFlags = BIF_NEWDIALOGSTYLE;
	info.lParam = LPARAM(wpath.c_str());
	
	LPITEMIDLIST item = SHBrowseForFolder(&info);
	if (SHGetPathFromIDList(item,path))
		folder = CefString(path);
	return folder;
}
int CALLBACK baseWindow::FolderCallbackProc(HWND hwnd, UINT msg, LPARAM lParam, LPARAM lpData) {
	//string path = Utils::GetApplicationDir();
	switch (msg)
	{
	case BFFM_INITIALIZED:    //初始化消息  
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		break;
	case BFFM_SELCHANGED:    //选择路径变化，  
	{
		TCHAR curr[MAX_PATH];
		SHGetPathFromIDList((LPCITEMIDLIST)lParam, curr);
		::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)curr);
	}
	break;
	default:
		break;
	}
	return 0;
}
#pragma endregion