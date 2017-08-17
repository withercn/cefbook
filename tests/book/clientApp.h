#include "include/cef_app.h"
#include "ClientV8ExtensionHandler.h"

class clientApp :public CefApp, CefRenderProcessHandler {
public:
	clientApp();
	~clientApp();

	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;
	virtual void OnWebKitInitialized() OVERRIDE;
private:
	IMPLEMENT_REFCOUNTING(clientApp);
};