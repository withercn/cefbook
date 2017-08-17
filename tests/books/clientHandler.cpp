#include "clientHandler.h"
#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include "baseWindow.h"
#include "clientCookie.h"

namespace { 
	clientHandler* g_instance = NULL; 

	auto CheckUIStatus(CefRefPtr<CefFrame> frame, bool canGoBack, bool canGoForward)
	{
		if (canGoBack)
			frame->ExecuteJavaScript(TEXT("$('#backward').addClass('enable');"), frame->GetURL(), 0);
		else
			frame->ExecuteJavaScript(TEXT("$('#backward').removeClass('enable');"), frame->GetURL(), 0);
		if (canGoForward)
			frame->ExecuteJavaScript(TEXT("$('#forward').addClass('enable');"), frame->GetURL(), 0);
		else
			frame->ExecuteJavaScript(TEXT("$('#forward').removeClass('enable');"), frame->GetURL(), 0);
	}
}

clientHandler::clientHandler() { g_instance = this; }
clientHandler::~clientHandler() { g_instance = NULL; }
clientHandler* clientHandler::GetInstance() { return g_instance; }
CefRefPtr<CefBrowser> clientHandler::GetMainFrameBrowser() {
	if (!browser_list_.empty())
		return browser_list_.back()->GetMainFrame()->GetBrowser();
	return NULL;
}
void clientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();
	HWND hwnd = browser->GetHost()->GetWindowHandle();
	TCHAR classname[256];
	
	if (::GetWindowText(hwnd, classname, 255) != 0) {
		std::wstring class1(classname);
		if (class1 == L"Clound_Book_Tray") {
			RECT rect = { 0 };
			GetWindowRect(hwnd, &rect);
			::SetWindowPos(hwnd, HWND_TOPMOST, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
			::SetForegroundWindow(hwnd);
		}
	}
	browser_list_.push_back(browser);
}
void clientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();
	BrowserList::iterator bit = browser_list_.begin();
	for (; bit != browser_list_.end(); ++bit) {
		if ((*bit)->IsSame(browser)) {
			browser_list_.erase(bit);
			break;
		}
	}
	if (browser_list_.empty())
		CefQuitMessageLoop();
}
bool clientHandler::DoClose(CefRefPtr<CefBrowser> browser) {
	if (browser_list_.size() == 1)
		is_closing_ = true;
	return false;
}
void clientHandler::OnResize(RECT rect) {
	BrowserList::iterator bit = browser_list_.begin();
	for (; bit != browser_list_.end(); ++bit) {
		HWND hwnd = (*bit)->GetHost()->GetWindowHandle();
		HDWP hdwp = BeginDeferWindowPos(1);
		hdwp = DeferWindowPos(hdwp, hwnd, NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
		EndDeferWindowPos(hdwp);
	}
}
bool clientHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) {
	std::string name = message->GetName();
	HWND hwnd = browser->GetHost()->GetWindowHandle();
	auto frame = browser->GetMainFrame();
	if (name == "min") {
		baseWindow::Show(hwnd, SW_SHOWMINIMIZED);
		return true;
	}
	else if (name == "max") {
		baseWindow::Show(hwnd, SW_SHOWMAXIMIZED);
		return true;
	}
	else if (name == "exit")
	{
		baseWindow::Close();
		return true;
	}
	else if (name == "fullscreen") {
		baseWindow::FullScreen(hwnd);
		return true;
	}
	else if (name == "startDrag") {
		baseWindow::StartDrag(hwnd);
		return true;
	}
	else if (name == "reload") {
		browser->ReloadIgnoreCache();
		return true;
	}
	else if (name == "showDevTools") {
		baseWindow::ShowDevTools(hwnd, browser);
		return true;
	}
	else if (name == "backward") {
		browser->GoBack();
		return true;
	}
	else if (name == "forward") {
		browser->GoForward();
		return true;
	}
	else if (name == "resizeTo") {
		baseWindow::ResizeTo(hwnd, message->GetArgumentList()->GetInt(0), message->GetArgumentList()->GetInt(1));
		return true;
	}
	else if (name == "moveTo") {
		baseWindow::MoveTo(hwnd, message->GetArgumentList()->GetInt(0), message->GetArgumentList()->GetInt(1));
		return true;
	}
	else if (name == "bookFolder") {
		std::wstring code = L"setBookPath('";
		CefString folder = baseWindow::BrowserFolder(hwnd);
		if (folder.empty())
		{
			frame->ExecuteJavaScript(L"msgbox({message:'请选择目录'});", frame->GetURL(), 0);
			return true;
		}
		folder = Utils::ReplaceW(folder, L"\\", L"\\\\");
		code.append(folder.c_str()).append(L"');");
		frame->ExecuteJavaScript(code, frame->GetURL(), 0);
		return true;
	}
	else if (name == "downloadFolder") {
		std::wstring code = L"setDownloadPath('";
		CefString folder = baseWindow::BrowserFolder(hwnd);
		if (folder.empty())
		{
			frame->ExecuteJavaScript(L"msgbox({message:'请选择目录'});", frame->GetURL(), 0);
			return true;
		}
		folder = Utils::ReplaceW(folder, L"\\", L"\\\\");
		code.append(folder.c_str()).append(L"');");
		frame->ExecuteJavaScript(code, frame->GetURL(), 0);
		return true;
	}
	return false;
}
//bool clientHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, const CefString& target_frame_name, WindowOpenDisposition target_disposition, bool user_gesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client, CefBrowserSettings& settings, bool* no_javascript_access) {
//	switch (target_disposition)
//	{
//	case WOD_NEW_FOREGROUND_TAB:
//	case WOD_NEW_BACKGROUND_TAB:
//	case WOD_NEW_POPUP:
//	case WOD_NEW_WINDOW:
//		browser->GetMainFrame()->LoadURL(target_url);
//		return true; //cancel create
//	}
//	return false;
//}
void clientHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) {
	CEF_REQUIRE_UI_THREAD();
	if ((params->GetTypeFlags() & (CM_TYPEFLAG_PAGE | CM_TYPEFLAG_FRAME)) != 0) {
		// Add a separator if the menu already has items.
		if (model->GetCount() > 0)
		{
			model->Clear();
			//model->AddSeparator();
		}
	}
}
bool clientHandler::OnDragEnter(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> dragData, DragOperationsMask mask) {
	return true;
}
void clientHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward) {
	auto frame = browser->GetMainFrame();
	CheckUIStatus(frame, canGoBack, canGoForward);
}
//void clientHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) {
//	bool canGoBack = browser->CanGoBack();
//	bool canGoForward = browser->CanGoForward();
//	CheckUIStatus(frame, canGoBack, canGoForward);
//}
//CefRefPtr<CefResourceHandler> clientHandler::GetResourceHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request) {
//
//}
void clientHandler::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transition_type) {
	//std::wstring member = clientCookie::GetCookie(_T("member"));
	//if (member.empty())
	//{
	//	clientCookie::SetCookie(_T("member"), _T("clal"));
	//	member = clientCookie::GetCookie(_T("member"));
	//}
}
void clientHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) {
	
}