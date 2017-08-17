#include "AppConfig.h"

using namespace std;

AppConfig::AppConfig() {
	this->GetSetting();
	this->GetUserInfo();
	this->GetRecentList();
}
bool AppConfig::GetSetting() {
	return helper_.GetSetting("setting", setting);
}
bool AppConfig::GetUserInfo() {
	return helper_.GetUserinfo("userinfo", user);
}
bool AppConfig::GetRecentList() {
	recent_list.clear();
	return helper_.GetRecentList("recent", recent_list, setting.recent);
}
bool AppConfig::SaveSetting() {
	return helper_.SaveSetting("setting", setting);
}
bool AppConfig::SaveUserinfo() {
	return helper_.SaveUserinfo("userinfo", user);
}