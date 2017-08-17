#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_

#include "include/cef_client.h"

class clientHandler : public CefClient,
	public CefContextMenuHandler,
	public CefDisplayHandler,
	//public CefDownloadHandler,
	public CefDragHandler,
	public CefGeolocationHandler,
	public CefKeyboardHandler,
	public CefLifeSpanHandler,
	public CefLoadHandler,
	public CefRequestHandler 
{
public:
	clientHandler();
	~clientHandler();

	static clientHandler* GetInstance();
	CefRefPtr<CefBrowser> GetBrowser();
	//typedef cef_window_open_disposition_t WindowOpenDisposition;

	//// CefClient methods. Important to return |this| for the handler callbacks.
	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE { return this; }
	//virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefDragHandler> GetDragHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefGeolocationHandler> GetGeolocationHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE { return this; }
	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE { return this; }

	//// Handle IPC messages from the render process...
	//virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)OVERRIDE;
	//// CefContextMenuHandler methods
	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) OVERRIDE;
	//virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, EventFlags event_flags) OVERRIDE;
	//// CefDisplayHandler methods
	//virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward) OVERRIDE;
	//virtual void OnAddressChange(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url) OVERRIDE;
	//virtual void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) OVERRIDE;
	//virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser, const CefString& message, const CefString& source, int line) OVERRIDE;
	//// CefDownloadHandler methods
	//virtual void OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback) OVERRIDE;
	//virtual void OnDownloadUpdated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, CefRefPtr<CefDownloadItemCallback> callback) OVERRIDE;
	//// CefDragHandler methods
	//virtual bool OnDragEnter(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> dragData, DragOperationsMask mask) OVERRIDE;
	//// CefGeolocationHandler methods
	//virtual bool OnRequestGeolocationPermission(CefRefPtr<CefBrowser> browser, const CefString& requesting_url, int request_id, CefRefPtr<CefGeolocationCallback> callback) OVERRIDE;
	//// CefKeyboardHandler methods
	//virtual bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& event, CefEventHandle os_event, bool* is_keyboard_shortcut) OVERRIDE;
	//// CefLifeSpanHandler methods
	//virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, const CefString& target_frame_name, WindowOpenDisposition target_disposition, bool user_gesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client, CefBrowserSettings& settings, bool* no_javascript_access)OVERRIDE;
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual bool DoClose(CefRefPtr<CefBrowser> browser)OVERRIDE;
	//// CefLoadHandler methods
	//virtual void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transition_type) OVERRIDE;
	//virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) OVERRIDE;
	//virtual void OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl) OVERRIDE;
	//virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser, TerminationStatus status) OVERRIDE;
	//// CefRequestHandler methods
	//virtual CefRefPtr<CefResourceHandler> GetResourceHandler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request) OVERRIDE;
	//virtual bool OnQuotaRequest(CefRefPtr<CefBrowser> browser, const CefString& origin_url, int64 new_size, CefRefPtr<CefRequestCallback> callback) OVERRIDE;
	//virtual void OnProtocolExecution(CefRefPtr<CefBrowser> browser, const CefString& url, bool& allow_os_execution) OVERRIDE;
	//void ShowDevTools(CefRefPtr<CefBrowser> browser, const CefPoint& inspect_element_at);
private:
	CefRefPtr<CefBrowser> browser_;
	IMPLEMENT_REFCOUNTING(clientHandler);
};
#endif