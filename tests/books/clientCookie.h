#include "include/cef_cookie.h"
#include "include/cef_app.h"
#include "utils.h"
//未完成
typedef std::vector<CefCookie> CookieVector;

namespace {
	std::string scheme = SCHEME_NAME;
	CefString url = CefString(scheme.append("://").append(HOST_NAME).append("/"));
};

class visitorCookie :public CefCookieVisitor {
public:
	visitorCookie(CookieVector* cookies, bool deleteCookie) :cookies_(cookies), delete_cookie_(deleteCookie) {}
	~visitorCookie() {}

	virtual bool Visit(const CefCookie& cookie, int count, int total, bool& deleteCookie) OVERRIDE {
		cookies_->push_back(cookie);
		if (delete_cookie_)
			deleteCookie = true;
		return true;
	}

private:
	CookieVector* cookies_;
	bool delete_cookie_;

	IMPLEMENT_REFCOUNTING(visitorCookie);
};

class clientCookie {
public:
	clientCookie() {}
	~clientCookie() {}

	static std::wstring GetCookie(std::wstring name) {
		CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager(NULL);
		if (!manager.get())
		{
			CookieVector cookies;
			manager->VisitUrlCookies(url, false, new visitorCookie(&cookies, false));

			CookieVector::iterator it = cookies.begin();
			for (; it != cookies.end(); ++it) {
				if (it->name.str == name.c_str())
					return it->value.str;
			}
		}
		return NULL;
	}
	static bool HasCookie(std::wstring name) {
		CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager(NULL);
		if (!manager.get())
		{
			CookieVector cookies;
			manager->VisitUrlCookies(url, false, new visitorCookie(&cookies, false));

			CookieVector::iterator it = cookies.begin();
			for (; it != cookies.end(); ++it) {
				if (it->name.str == name.c_str())
					return true;
			}
		}
		return false;
	}

	static void SetCookie(std::wstring name, std::wstring value) {
		CefPostTask(TID_IO, base::Bind(&clientCookie::SetCookieA, name, value));
		//就是这段代码, 关键在于最后一句, 这里是开启了一个新的IO thread, 设置cookie一定要在io线程里面做.为什么呢? 我目前不能答复你, 随着我们对CEF内核的深入, 相信会水落石出.
		//CefPostTask(TID_IO, NewCefRunnableMethod(manager.get(), &clientCookie::SetCookie, CefString("http://venus.sogou-inc.com"), cookie));
	}
	static void SetCookieA(std::wstring name, std::wstring value) {
		CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager(NULL);
		if (!manager.get()) {
			CookieVector cookies;
			CefCookie cookie;
			CefString(&cookie.name).FromString16(name);
			CefString(&cookie.value).FromString16(value);
			CefString(&cookie.domain).FromString(HOST_NAME);
			cookie.has_expires = true;
			cookie.expires.year = 3000;
			cookie.expires.month = 1;
			cookie.expires.day_of_month = 1;
			cookie.expires.day_of_week = 1;

			cookies.push_back(cookie);
			manager->SetCookie(url, cookie, NULL);

		}
	}
};