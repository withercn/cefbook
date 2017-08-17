#include "include/cef_scheme.h"
#include "include/wrapper/cef_helpers.h"

#ifndef __CLIENT_SCHEME_HANDLER__
#define __CLIENT_SCHEME_HANDLER__

class clientSchemeHandlerFactory :public CefSchemeHandlerFactory {
public:
	clientSchemeHandlerFactory();
	virtual CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& scheme_name, CefRefPtr<CefRequest> request) OVERRIDE;

	IMPLEMENT_REFCOUNTING(clientSchemeHandlerFactory);
};

//class clientSchemeHandler : public CefResourceHandler {
//public:
//	clientSchemeHandler();
//	virtual bool ProcessRequest(CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback) OVERRIDE;
//	virtual void GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl) OVERRIDE;
//	virtual bool ReadResponse(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefCallback> callback) OVERRIDE;
//	virtual void Cancel() OVERRIDE;
//
//private:
//	std::string data_;
//	std::string mime_type_;
//	size_t offset_;
//
//	IMPLEMENT_REFCOUNTING(clientSchemeHandler);
//};

#endif // !__CLIENT_SCHEME_HANDLER__