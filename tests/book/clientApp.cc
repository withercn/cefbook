#include "clientApp.h"

clientApp::clientApp() {

}
clientApp::~clientApp() {

}
void clientApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) {
	MessageBox(browser->GetHost()->GetWindowHandle(), context.get() ? L"context" : L"NULL", L"title", NULL);
	frame->ExecuteJavaScript("alert('ExecuteJavaScript succeed!');", frame->GetURL(), 0);
	// Retrieve the context's window object.
	CefRefPtr<CefV8Value> object = context->GetGlobal();

	// Create a new V8 string value. See the "Basic JS Types" section below.
	CefRefPtr<CefV8Value> str = CefV8Value::CreateString("My Value!");

	// Add the string to the window object as "window.myval". See the "JS Objects" section below.
	object->SetValue("myval", str, V8_PROPERTY_ATTRIBUTE_NONE);
}

void clientApp::OnWebKitInitialized() {
		std::string app_code =
			"var app;"
			"if (!app)"
			"    app = {};"
			"(function() {"
			"    app.ChangeTextInJS = function(text) {"
			"        native function ChangeTextInJS();"
			"        return ChangeTextInJS(text);"
			"    };"
			"})();";
		CefRegisterExtension("v8/app", app_code, new ClientV8ExtensionHandler(this));
}