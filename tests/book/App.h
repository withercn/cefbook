#include "include/cef_app.h"
#include "ClientV8ExtensionHandler.h"

class bookApp :public CefApp {
public:
	bookApp() {}
	virtual ~bookApp() {}
	virtual void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)OVERRIDE {
		command_line->AppendSwitch("process-per-site");
	}
	//virtual void OnWebKitInitialized() OVERRIDE;
private:
	IMPLEMENT_REFCOUNTING(bookApp);
};