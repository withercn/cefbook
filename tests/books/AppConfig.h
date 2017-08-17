#include "SqliteHelper.h"
#include <list>

#ifndef __APP_SETTING__
#define __APP_SETTING__

using namespace std;

class AppConfig {
public:
	AppConfig();
	bool SaveSetting();
	bool SaveUserinfo();

	Setting setting;
	Userinfo user;
	typedef std::list<Recent> RecentList;
	RecentList recent_list;
private:
	SqliteHelper helper_;

	bool GetSetting();
	bool GetUserInfo();
	bool GetRecentList();
};


#endif // !__APP_SETTING__
