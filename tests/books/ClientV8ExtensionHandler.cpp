#include "ClientV8ExtensionHandler.h"
#include <tchar.h>

ClientV8ExtensionHandler::ClientV8ExtensionHandler(){}
ClientV8ExtensionHandler::ClientV8ExtensionHandler(CefRefPtr<CefApp> app)
{
	this->app = app;
}

bool ClientV8ExtensionHandler::Execute(const CefString &name, CefRefPtr<CefV8Value> object, const CefV8ValueList &arguments, CefRefPtr<CefV8Value> &retval, CefString &exception)
{
	bool handled = false;
	CefRefPtr<CefFrame> frame = CefV8Context::GetCurrentContext()->GetBrowser()->GetMainFrame();
	CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
	CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(name);

	if (name == "min" ||
		name == "max" ||
		name == "exit" ||
		name == "fullscreen"||
		name == "startDrag" ||
		name == "reload" ||
		name == "showDevTools" ||
		name == "backward" ||
		name == "forward" ||
		name == "bookFolder" ||
		name == "downloadFolder") {
		browser->SendProcessMessage(PID_BROWSER, msg);
		handled = true;
	}
	if (name == "hitTest" ||
		name == "resizeTo" ||
		name == "moveTo") {
		if (arguments.size() == 2 && arguments[0]->IsInt() && arguments[1]->IsInt()) {
			msg->GetArgumentList()->SetInt(0, arguments[0]->GetIntValue());
			msg->GetArgumentList()->SetInt(1, arguments[1]->GetIntValue());
			browser->SendProcessMessage(PID_BROWSER, msg);
			handled = true;
		}
	}
	if (!handled)
		exception == _T("无效的数据。");
	return true;
}