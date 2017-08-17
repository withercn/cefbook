﻿#include "include/cef_app.h"

struct ClientV8ExtensionHandler : public CefV8Handler {
	ClientV8ExtensionHandler();
	ClientV8ExtensionHandler(CefRefPtr<CefApp> app);

	virtual bool Execute(const CefString &name, CefRefPtr<CefV8Value> object, const CefV8ValueList &arguments, CefRefPtr<CefV8Value> &retval, CefString &exception) OVERRIDE;

private:
	CefRefPtr<CefApp> app;

	IMPLEMENT_REFCOUNTING(ClientV8ExtensionHandler);
};