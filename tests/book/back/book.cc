#include "book.h"
#include <string>
#include "include/cef_browser.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "book_handler.h"

namespace {
	class bookWindowDelegate :public CefWindowDelegate {
	public:
		explicit bookWindowDelegate(CefRefPtr<CefBrowserView> browser_view) :browser_view_(browser_view) {}
		void OnWindowCreated(CefRefPtr<CefWindow> window) OVERRIDE {
			window->AddChildView(browser_view_);
			window->Show();
			window->Maximize();

			browser_view_->RequestFocus();
		}
		void OnWindowDestroyed(CefRefPtr<CefWindow> window) OVERRIDE { browser_view_ = NULL; }
		bool CanClose(CefRefPtr<CefWindow> window) OVERRIDE {
			CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
			if (browser)
				return browser->GetHost()->TryCloseBrowser();
			return true;
		}
	private:
		CefRefPtr<CefBrowserView> browser_view_;
		IMPLEMENT_REFCOUNTING(bookWindowDelegate);
		DISALLOW_COPY_AND_ASSIGN(bookWindowDelegate);
	};
	class bookWindow :public CefWindow {
	public:
		void Maximize() OVERRIDE {//没反应
			CefSize size;
			size.width = GetSystemMetrics(SM_CXFULLSCREEN) - 100;
			size.height = GetSystemMetrics(SM_CYFULLSCREEN) - 100;
			this->SetSize(size);
		}
	};
}
bookApp::bookApp() {}

void bookApp::OnContextInitialized() {
	CEF_REQUIRE_UI_THREAD();

	const bool use_views = false;

	CefRefPtr<bookHandler> handler(new bookHandler(use_views));
	CefBrowserSettings browser_settings;
	std::string url;
	//url = "web/index.html";
	url = "http://www.baidu.com";
	CefWindowInfo window_info;

#if defined(OS_WIN)
	//window_info.SetAsPopup(NULL, L"云小说");
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = GetSystemMetrics(SM_CXSCREEN);
	rect.bottom = GetSystemMetrics(SM_CYSCREEN)- GetSystemMetrics(SM_CYSIZE); //获得任务栏的高度
	window_info.SetNoBorder(NULL, L"云小说", rect);
	window_info.style = WS_VISIBLE | WS_POPUP;
	window_info.ex_style = WS_EX_DLGMODALFRAME;
	/*window_info.style &= ~WS_CAPTION;*/
	/*window_info.width = 500;
	window_info.height = 400;*/

#endif
	CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings, NULL);
}
