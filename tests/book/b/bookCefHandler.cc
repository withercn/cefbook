#include "bookCefHandler.h"
#include "include/wrapper/cef_helpers.h"
#include "include/cef_app.h"

namespace { bookCefHandler* g_instance = NULL; }

extern void AppQuitMessageLoop();

bookCefHandler::bookCefHandler() { g_instance = this; };
bookCefHandler::~bookCefHandler() {};

CefRefPtr<CefBrowser> bookCefHandler::GetBrowser() {
	return browser;
}

CefRefPtr<CefContextMenuHandler> bookCefHandler::GetContextMenuHandler() { return this; }
CefRefPtr<CefDisplayHandler> bookCefHandler::GetDisplayHandler() { return this; }
//CefRefPtr<CefDownloadHandler> bookCefHandler::GetDownloadHandler() { return this; }
CefRefPtr<CefDragHandler> bookCefHandler::GetDragHandler() { return this; }
CefRefPtr<CefGeolocationHandler> bookCefHandler::GetGeolocationHandler() { return this; }
CefRefPtr<CefKeyboardHandler> bookCefHandler::GetKeyboardHandler() { return this; }
CefRefPtr<CefLifeSpanHandler> bookCefHandler::GetLifeSpanHandler() { return this; }
CefRefPtr<CefLoadHandler> bookCefHandler::GetLoadHandler() { return this; }
CefRefPtr<CefRequestHandler> bookCefHandler::GetRequestHandler() { return this; }

//void bookCefHandler::OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback) {
//	UNREFERENCED_PARAMETER(browser);
//	UNREFERENCED_PARAMETER(download_item);
//	callback->Continue(suggested_name, true);
//}

void bookCefHandler::OnAfterCreated(CefRefPtr<CefBrowser> browsers) {
	CEF_REQUIRE_UI_THREAD();
	this->browser = browsers;

	CefLifeSpanHandler::OnAfterCreated(browser);
}

void bookCefHandler::OnBeforeClose(CefRefPtr<CefBrowser> browsers)
{
	CEF_REQUIRE_UI_THREAD();

	browsers = NULL;
	CefShutdown();
	CefQuitMessageLoop();
	AppQuitMessageLoop();

	CefLifeSpanHandler::OnBeforeClose(browser);
}
bool bookCefHandler::DoClose(CefRefPtr<CefBrowser> browsers) {
	CEF_REQUIRE_UI_THREAD();
	return false;
}