#include "include/cef_app.h"

class bookCefApp :public CefApp {
public:
	bookCefApp() {}
	virtual ~bookCefApp(){}
private:
	IMPLEMENT_REFCOUNTING(bookCefApp);
};

