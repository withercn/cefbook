#include "include/cef_app.h"
#include "typeMime.h"
#include <tchar.h>

#ifndef __Utils__
#define __Utils__

using namespace std;

class Rectange {
public:
	int x;
	int y;
	int width;
	int height;

	Rectange(int x, int y, int width, int height) {
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}
	~Rectange() {}
};
class Utils {
public:
	Utils() {};

	static wstring GetApplicationDirW();
	static string GetApplicationDir();
	static string GetApplicationPath();
	static string GetCurrentDirectFilePath(string filepath);
	static string Replace(string text, string source, string desc);
	static wstring ReplaceW(wstring text, wstring source, wstring desc);
	static string GetFile(string path);
	static string GetMimeType(string url);
	static string CreateUrl(string url);
	static string UrlDecode(std::string &source);
};
#endif // !__Utils__