#include "book_handler.h"

#include <sstream>
#include <string>
#include <windows.h>

#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

namespace { bookHandler* g_instance = NULL; }

bookHandler::bookHandler(bool use_views)
	:use_views_(use_views), is_closing_(false) {
	DCHECK(!g_instance);
	g_instance = this;
}

bookHandler::~bookHandler() { g_instance = NULL; }
bookHandler* bookHandler::GetInstance() { return g_instance; }

void bookHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) {
	CEF_REQUIRE_UI_THREAD();

	if (use_views_) {
		CefRefPtr<CefBrowserView> browser_view = CefBrowserView::GetForBrowser(browser);
		if (browser_view) {
			CefRefPtr<CefWindow> window = browser_view->GetWindow();
			if (window)
				window->SetTitle(title);
		}
	} else {
		PlatformTitleChange(browser, title);
	}
}
void bookHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();
	browser_list_.push_back(browser);
}
bool bookHandler::DoClose(CefRefPtr<CefBrowser>) {
	CEF_REQUIRE_UI_THREAD();
	if (browser_list_.size() == 1)
		is_closing_ = true;
	return false;
}
void bookHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();
	BrowserList::iterator bit = browser_list_.begin();
	for (; bit != browser_list_.end(); ++bit) {
		if ((*bit)->IsSame(browser))
		{
			browser_list_.erase(bit);
			break;
		}
	}
	if (browser_list_.empty())
		CefQuitMessageLoop();
}
void bookHandler::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) {
	CEF_REQUIRE_UI_THREAD();
	if (errorCode == ERR_ABORTED)
		return;
	std::stringstream ss;
	ss << "<html><body bgcolor=\"white\">"
		"<h2>Failed to load URL "
		<< std::string(failedUrl) << " with error " << std::string(errorText)
		<< " (" << errorCode << ").</h2></body></html>";
	frame->LoadString(ss.str(), failedUrl);
}
void bookHandler::CloseAllBrowsers(bool force_close) {
	if (!CefCurrentlyOn(TID_UI))
	{
		CefPostTask(TID_UI, base::Bind(&bookHandler::CloseAllBrowsers, this, force_close));
		return;
	}
	if (browser_list_.empty())
		return;
	BrowserList::const_iterator it = browser_list_.begin();
	for (; it != browser_list_.end(); ++it)
		(*it)->GetHost()->CloseBrowser(force_close);
}
void bookHandler::PlatformTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) {
	/*CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
	SetWindowText(hwnd, std::wstring(title).c_str());*/
}