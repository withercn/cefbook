#include "clientApp.h"
#include "clientHandler.h"
#include "ClientV8ExtensionHandler.h"
#include "utils.h"
#include "include/wrapper/cef_helpers.h"
#include "include/cef_cookie.h"


clientApp::clientApp() {

}
void clientApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) {
	if (!frame->IsMain())
		return;
	CefRefPtr<CefV8Value> object = context->GetGlobal();

	CefRefPtr<CefV8Handler> handler(new ClientV8ExtensionHandler);
	object->SetValue("startDrag", CefV8Value::CreateFunction("startDrag", handler), V8_PROPERTY_ATTRIBUTE_READONLY);
	object->SetValue("moveTo", CefV8Value::CreateFunction("moveTo", handler), V8_PROPERTY_ATTRIBUTE_READONLY);
	object->SetValue("moveBy", CefV8Value::CreateFunction("moveBy", handler), V8_PROPERTY_ATTRIBUTE_READONLY);
	object->SetValue("resizeTo", CefV8Value::CreateFunction("resizeTo", handler), V8_PROPERTY_ATTRIBUTE_READONLY);
	object->SetValue("resizeBy", CefV8Value::CreateFunction("resizeBy", handler), V8_PROPERTY_ATTRIBUTE_READONLY);
	object->SetValue("min", CefV8Value::CreateFunction("min", handler), V8_PROPERTY_ATTRIBUTE_READONLY);
	object->SetValue("max", CefV8Value::CreateFunction("max", handler), V8_PROPERTY_ATTRIBUTE_READONLY);
	object->SetValue("exit", CefV8Value::CreateFunction("exit", handler), V8_PROPERTY_ATTRIBUTE_READONLY);
	object->SetValue("fullscreen", CefV8Value::CreateFunction("fullscreen", handler), V8_PROPERTY_ATTRIBUTE_READONLY);
	object->SetValue("reload", CefV8Value::CreateFunction("reload", handler), V8_PROPERTY_ATTRIBUTE_READONLY);
	object->SetValue("showDevTools", CefV8Value::CreateFunction("showDevTools", handler), V8_PROPERTY_ATTRIBUTE_READONLY);
	object->SetValue("backward", CefV8Value::CreateFunction("backward", handler), V8_PROPERTY_ATTRIBUTE_READONLY);
	object->SetValue("forward", CefV8Value::CreateFunction("forward", handler), V8_PROPERTY_ATTRIBUTE_READONLY); 
	object->SetValue("bookFolder", CefV8Value::CreateFunction("bookFolder", handler), V8_PROPERTY_ATTRIBUTE_READONLY);
	object->SetValue("downloadFolder", CefV8Value::CreateFunction("downloadFolder", handler), V8_PROPERTY_ATTRIBUTE_READONLY);
	//std::string window_code =
	//	"(function() {"
	//	"	window.addEventListener('load', function(){"
	//	"		document.getElementById('header').addEventListener('mousedown',function(event){"
	//	"			if(event.target != event.currentTarget)"
	//	"				return;"
	//	"			window.starDrag();"
	//	"		}, false);"
	//	"	}, false);"
	//	"})();";
	//frame->ExecuteJavaScript(window_code, frame->GetURL(), 0);
	//frame->ExecuteJavaScript(_T("(function(){window.addEventListener('load', function(){msgbox({ message: '脚本测试成功' });});})();"), frame->GetURL(), 0);
}

void clientApp::OnWebKitInitialized() {
	std::string code =
		"var book;"
		"if (!book)"
		"    book = {};"
		"(function() {"
		"   book.test=function(){native function test();return test();};"
		"})();";
	CefRegisterExtension("v8/book", code, new ClientV8ExtensionHandler(this));
}

void clientApp::OnContextInitialized() {
	CEF_REQUIRE_UI_THREAD();
	
	/*CefRefPtr<clientHandler> client(new clientHandler);
	
	CefBrowserSettings browser_settings;
	CefWindowInfo info;
	info.SetAsPopup(NULL, "");
	baseWindow window(NULL, info, client);
	CefBrowserHost::CreateBrowser(info, client, DEFAULT_URL, browser_settings, NULL);*/
	//CefBrowserHost::CreateBrowser(info, client, DEFAULT_URL, browser_settings,NULL);
}
void clientApp::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) {
	registrar->AddCustomScheme(SCHEME_NAME, true, false, false, false, true, false);
}

//class bookSchemeHandlerFactory :public CefSchemeHandlerFactory {
//public:
//	virtual CefRefPtr<CefResourceHandler> Create(
//		CefRefPtr<CefBrowser> browser,
//		CefRefPtr<CefFrame> frame,
//		const CefString& scheme_name,
//		CefRefPtr<CefRequest> request) OVERRIDE {
//		CEF_REQUIRE_IO_THREAD();
//		return new bookSchemeHandler();
//	}
//	IMPLEMENT_REFCOUNTING(bookSchemeHandlerFactory);
//};
//class bookSchemeHandler : public CefResourceHandler {
//public:
//	bookSchemeHandler() : offset_(0) {}
//
//	virtual bool ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback) OVERRIDE {
//		CEF_REQUIRE_IO_THREAD();
//
//		return true;
//	}
//	virtual void GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl) OVERRIDE {
//		CEF_REQUIRE_IO_THREAD();
//
//		DCHECK(!data_.empty());
//
//		response->SetMimeType(mime_type_);
//		response->SetStatus(200);
//
//		// Set the resulting response length
//		response_length = data_.length();
//	}
//	virtual void Cancel() OVERRIDE { CEF_REQUIRE_IO_THREAD(); }
//	virtual bool ReadResponse(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefCallback> callback) OVERRIDE {
//		CEF_REQUIRE_IO_THREAD();
//
//		bool has_data = false;
//		bytes_read = 0;
//
//		if (offset_ < data_.length()) {
//			// Copy the next block of data into the buffer.
//			int transfer_size =
//				std::min(bytes_to_read, static_cast<int>(data_.length() - offset_));
//			memcpy(data_out, data_.c_str() + offset_, transfer_size);
//			offset_ += transfer_size;
//
//			bytes_read = transfer_size;
//			has_data = true;
//		}
//
//		return has_data;
//	}
//private:
//	std::string data_;
//	std::string mime_type_;
//	size_t offset_;
//
//	IMPLEMENT_REFCOUNTING(bookSchemeHandler);
//};