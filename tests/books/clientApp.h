#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_base.h"

#ifndef _clientApp_
#define _clientApp_

class clientApp :public CefApp, CefRenderProcessHandler, public CefBrowserProcessHandler {
public:
	clientApp();

	virtual void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) OVERRIDE;
	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE { return this; }
	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;
	virtual void OnWebKitInitialized() OVERRIDE;
	virtual void OnContextInitialized() OVERRIDE;
private:
	IMPLEMENT_REFCOUNTING(clientApp);
};
//class clientApp : public CefApp, public CefRenderProcessHandler {
//public:
//	clientApp();
//	CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE { return this; }
//	void OnWebKitInitialized() OVERRIDE;
//	IMPLEMENT_REFCOUNTING(clientApp);
//};
#endif // !_clientApp_
