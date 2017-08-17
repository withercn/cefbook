#include "utils.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

string Utils::GetApplicationDir()
{
	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR   wpath[MAX_PATH];

	GetModuleFileNameW(hModule, wpath, MAX_PATH);
	wstring wide(wpath);

	string path = CefString(wide);
	path = path.substr(0, path.find_last_of("\\/"));
	return path;
}
wstring Utils::GetApplicationDirW() {
	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR   wpath[MAX_PATH];
	GetModuleFileNameW(hModule, wpath, MAX_PATH);
	wstring wide(wpath);
	wide = wide.substr(0, wide.find_last_of(L"\\/"));
	return wide;
}
string Utils::GetApplicationPath() {
	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR   wpath[MAX_PATH];

	GetModuleFileNameW(hModule, wpath, MAX_PATH);
	wstring wide(wpath);
	string path = CefString(wide);
	return path;
}
string Utils::GetCurrentDirectFilePath(string filepath) {
	string path = GetApplicationDir();
	return path.append(filepath);
}
string Utils::Replace(string text, string source, string desc) {
	int pos = text.find(source, 0);
	while (pos != -1) {
		text = text.replace(pos, source.length(), desc);
		pos = pos + (desc.length() - source.length()) + 1;
		pos = text.find(source, pos);
	}
	return text;
}
wstring Utils::ReplaceW(wstring text, wstring source, wstring desc) {
	int pos = text.find(source, 0);
	while (pos != -1) {
		text = text.replace(pos, source.length(), desc);
		pos = pos + (desc.length() - source.length()) + 1;
		pos = text.find(source, pos);
	}
	return text;
}
string Utils::GetFile(string path) {
	path = Replace(path, "/", "\\");
	int pos = path.find('?');
	if (pos != -1)
		return path.substr(0, pos++);
	pos = path.find('#');
	if (pos != -1)
		return path.substr(0, pos++);
	return path;
}
string Utils::GetMimeType(std::string url) {
	std::string mime = "text/html";
	int pos = url.find_last_of('/');
	if (pos == -1)
		return mime;
	pos++;
	std::string filename = url.substr(pos, url.length() - pos);
	pos = filename.find('?');
	if (pos != -1)
		filename = filename.substr(0, pos - 1);
	pos = filename.find_last_of('.');
	if (pos == -1)
		return mime;
	std::string extension = filename.substr(pos, filename.length() - pos);
	for (int i = 0; i < sizeof(mime_type); i++) {
		if (mime_type[i].ext == extension.substr(0, sizeof(mime_type[i].ext)))
		{
			mime = mime_type[i].mimeType;
			return mime;
		}
	}
	return mime;
}
string Utils::CreateUrl(string url) {
	string url_ = SCHEME_NAME;
	url_.append("://").append(HOST_NAME);
	url_.append(url);
	return url_;
}
string Utils::UrlDecode(std::string &source)
{
	std::string ret;
	char ch;
	int ii;
	for (size_t i = 0; i<source.length(); i++) {
		if (int(source[i]) == 37) {
			sscanf(source.substr(i + 1, 2).c_str(), "%x", &ii);
			ch = static_cast<char>(ii);
			ret += ch;
			i = i + 2;
		}
		else {
			ret += source[i];
		}
	}
	return (ret);
}
//std::string Utils::GetRegisterJavascript(std::string path)
//{
//	std::string js = "";
//	char buffer[256];
//	ifstream in(path);
//	if (!in.is_open())
//		return NULL;
//	while (!in.eof()) {
//		in.getline(buffer, strlen(buffer));
//		js += buffer;
//	}
//	return js;
//}