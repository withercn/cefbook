#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_

#include "include/cef_client.h"

class bookCefHandler :public CefClient,
	public CefContextMenuHandler,
	public CefDisplayHandler,
	//public CefDownloadHandler,
	public CefDragHandler,
	public CefGeolocationHandler,
	public CefKeyboardHandler,
	public CefLifeSpanHandler,
	public CefLoadHandler,
	public CefRequestHandler {

public:
	explicit bookCefHandler();
	virtual ~bookCefHandler();
	CefRefPtr<CefBrowser> GetBrowser();


	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE;
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE;
	//virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() OVERRIDE;
	virtual CefRefPtr<CefDragHandler> GetDragHandler() OVERRIDE;
	virtual CefRefPtr<CefGeolocationHandler> GetGeolocationHandler() OVERRIDE;
	virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE;
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE;
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;
	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE;


	//// �������Ϊ�麯�������Ǳ���ʵ��������������ʲôҲû�������������ļ��Ĳ������Ṥ��
	//virtual void OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback);
	//// ����һ��ָ��browser������
	//virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	//// �ͷ�browser����
	//virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual bool DoClose(CefRefPtr<CefBrowser> browser)OVERRIDE;

private:
	// the browser reference
	CefRefPtr<CefBrowser> browser;

	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(bookCefHandler);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_