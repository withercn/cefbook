#include "clientHandler.h"
#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

#define CLIENT_ID_SHOW_DEVTOOLS 26500
#define MESSAGE_QUIT 0xABAD1DEA

namespace { clientHandler* g_instance = NULL; }

clientHandler::clientHandler() { g_instance = this; }
clientHandler::~clientHandler() { g_instance = NULL; }
clientHandler* clientHandler::GetInstance() { return g_instance; }
CefRefPtr<CefBrowser> clientHandler::GetBrowser() { return browser_; }
void clientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();
	if (!browser_.get())
		browser_ = browser;
}
void clientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
	CEF_REQUIRE_UI_THREAD();
	browser_ = NULL;
	CefQuitMessageLoop();
}
bool clientHandler::DoClose(CefRefPtr<CefBrowser> browser) {
	return false;
}
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
	//model->AddItem(CLIENT_ID_SHOW_DEVTOOLS, CefString(L"开发工具"));
}
//bool clientHandler::OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, EventFlags event_flags) {
//	CEF_REQUIRE_UI_THREAD();
//	switch (command_id) {
//	case CLIENT_ID_SHOW_DEVTOOLS:
//		ShowDevTools(browser, CefPoint());
//		return true;
//	}
//}

//void clientHandler::ShowDevTools(CefRefPtr<CefBrowser> browser,
//	const CefPoint& inspect_element_at) {
//	if (!CefCurrentlyOn(TID_UI)) {
//		// Execute this method on the UI thread.
//		CefPostTask(TID_UI, base::Bind(&clientHandler::ShowDevTools, this, browser,
//			inspect_element_at));
//		return;
//	}
//
//	CefWindowInfo windowInfo;
//	CefRefPtr<CefClient> client;
//	CefBrowserSettings settings;
//
//	CefRefPtr<CefBrowserHost> host = browser->GetHost();
//
//	// Test if the DevTools browser already exists.
//	bool has_devtools = host->HasDevTools();
//	if (!has_devtools) {
//		// Create a new RootWindow for the DevTools browser that will be created
//		// by ShowDevTools().
//		has_devtools = CreatePopupWindow(browser, true, CefPopupFeatures(),
//			windowInfo, client, settings);
//	}
//
//	if (has_devtools) {
//		// Create the DevTools browser if it doesn't already exist.
//		// Otherwise, focus the existing DevTools browser and inspect the element
//		// at |inspect_element_at| if non-empty.
//		host->ShowDevTools(windowInfo, client, settings, inspect_element_at);
//	}
//}

//bool clientHandler::CreatePopupWindow(CefRefPtr<CefBrowser> browser,
//	bool is_devtools,
//	const CefPopupFeatures& popupFeatures,
//	CefWindowInfo& windowInfo,
//	CefRefPtr<CefClient>& client,
//	CefBrowserSettings& settings) {
//	// Note: This method will be called on multiple threads.
//
//	// The popup browser will be parented to a new native window.
//	// Don't show URL bar and navigation buttons on DevTools windows.
//	MainContext::Get()->GetRootWindowManager()->CreateRootWindowAsPopup(
//		!is_devtools, is_osr(), popupFeatures, windowInfo, client, settings);
//
//	return true;
//}