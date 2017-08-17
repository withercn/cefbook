#include "sqlite3.h"
#include "utils.h"
#include "typeMime.h"
#include <list>

#ifndef __SQLITE_HELPER__
#define __SQLITE_HELPER__
#pragma comment(lib,"sqlite3.lib")

#define APPCONFIG Utils::GetCurrentDirectFilePath("/app.config").c_str()

using namespace std;

class SqliteHelper {
public:
	explicit SqliteHelper() {
		db_path_ = APPCONFIG;
	}
	explicit SqliteHelper(std::string dbPath) {
		db_path_ = dbPath;
	}
	~SqliteHelper() {
		error_ = NULL;
		db_ = NULL;
	}
	bool IsOpen() { return open_; }
	int GetID(string table, string query) {
		int id = -1;
		this->Open();
		if (!open_)
		{
			this->Close();
			return id;
		}
		sql_ = "";
		sql_.append("select id from").append(table);
		if (!query.empty())
			sql_.append(" where ").append(query);
		char** result;
		int row, col;
		if (sqlite3_get_table(db_,sql_.c_str(),&result,&row,&col,&error_)== SQLITE_OK) {
			if (row > 0 && col > 0) {
				id = atoi(result[1 * col]);
			}
		}
		sqlite3_free_table(result);
		this->Close();
		return id;
	}
	bool GetSetting(string table, Setting& setting) {
		bool success = false;
		this->Open();
		if (!open_) {
			this->Close();
			return success;
		}
		sql_ = "";
		sql_.append("select * from ").append(table);
		char** rows;
		int row, col;
		if (sqlite3_get_table(db_, sql_.c_str(), &rows, &row, &col, &error_) == SQLITE_OK)
		{
			if (row > 0 && col > 0) {
				success = true;
				for (int i = 0; i < col; i++) {
					string field = rows[i];
					string value = rows[col + i];
					if (field == "start")
						setting.start = atoi(value.c_str());
					if(field=="maximize")
						setting.maximize = atoi(value.c_str());
					if (field == "recent")
						setting.recent = atoi(value.c_str());
					if (field == "book_path")
						setting.book_path = value.c_str();
					if (field == "download_path")
						setting.download_path = value.c_str();
					if (field == "sync")
						setting.sync = atoi(value.c_str());
				}
			}
		}
		sqlite3_free_table(rows);
		this->Close();
		return success;
	}
	bool GetUserinfo(string table, Userinfo& userinfo) {
		bool success = false;
		this->Open();
		if (!open_) {
			this->Close();
			return success;
		}
		sql_ = "";
		sql_.append("select * from ").append(table);
		char** rows;
		int row, col;
		if (sqlite3_get_table(db_, sql_.c_str(), &rows, &row, &col, &error_) == SQLITE_OK)
		{
			if (row > 0 && col > 0) {
				success = true;
				for (int i = 0; i < col; i++) {
					string field = rows[i];
					if (field == "username")
						userinfo.username = rows[col + i];
					if (field == "server_id")
						userinfo.server_id = atoi(rows[col + i]);
				}
			}
		}
		sqlite3_free_table(rows);
		this->Close();
		return success;
	}
	typedef list<Recent> RecentList;
	bool GetRecentList(string table, RecentList& recentlist, int limit) {
		bool success = false;
		this->Open();
		if (!open_) {
			this->Close();
			return success;
		}
		sql_ = "";
		sql_.append("select * from ").append(table);
		if (limit > 0)
		{
			char limit_string[12];
			itoa(limit, limit_string, 10);
			sql_.append(" limit ").append(limit_string);
		}
		char** rows;
		int row, col;
		if (sqlite3_get_table(db_, sql_.c_str(), &rows, &row, &col, &error_) == SQLITE_OK)
		{
			if (row > 0 && col > 0) {
				success = true;
				for (int i = 0; i < row; i++) {
					Recent recent;
					for (int j = 0; j < col; j++) {
						string field = rows[j];
						string value = rows[row*i + col + j];
						if (field == "id")
							recent.id = atoi(value.c_str());
						if (field == "name")
							recent.name == value.c_str();
						if (field == "server_id")
							recent.server_id = atoi(value.c_str());
						if (field == "local")
							recent.local = value.c_str();
						if (field == "chapter")
							recent.chapter = value.c_str();
						if (field == "scroll_top")
							recent.scroll_top = atoi(value.c_str());
						if (field == "limit")
							recent.limit = atoi(value.c_str());
					}
					recentlist.push_back(recent);
				}
			}
		}
		sqlite3_free_table(rows);
		this->Close();
		return success;
	}
	bool SaveSetting(string table, Setting setting) {
		stringstream set_values;
		set_values << "start=" << setting.start;
		set_values << ",maximize=" << setting.maximize;
		set_values << ",recent=" << setting.recent;
		set_values << ",book_path='" << setting.book_path << "'";
		set_values << ",download_path='" << setting.download_path << "'";
		set_values << ",sync=" << setting.sync;
		string values;
		set_values >> values;
		return Update(table, values, "");
	}
	bool SaveUserinfo(string table, Userinfo userinfo) {
		stringstream set_values;
		set_values << "username='" << userinfo.username << "'";
		set_values << ",server_id=" << userinfo.server_id;
		string values;
		set_values >> values;
		return Update(table, values, "");
	}
	string GetValue(string table, string field_name, string query) {
		this->Open();
		string value;
		if (!open_)
		{
			this->Close();
			return value;
		}
		sql_ = "";
		sql_.append("select ").append(field_name).append(" from ").append(table);
		if(!query.empty())
			sql_.append(" where ").append(query);
		char** result;
		int row, col;
		if (sqlite3_get_table(db_, sql_.c_str(), &result, &row, &col, &error_) == SQLITE_OK) {
			if (row > 0 && col > 0) {
				value = result[1 * col];
			}
		}
		sqlite3_free_table(result);
		this->Close();
		return value;
	}
	bool Insert(string table, string fields,string values) {
		bool value = false;
		this->Open();
		if (!open_)
		{
			this->Close();
			return value;
		}
		sql_ = "";
		sql_.append("insert into ").append(table).append(" (");
		sql_.append(fields).append(") values (").append(values).append(")");
		if (sqlite3_exec(db_, sql_.c_str(), NULL, NULL, &error_) == SQLITE_OK)
			value = true;
		this->Close();
		return value;
	}
	bool Update(string table, string set_values, string query) {
		bool value = false;
		this->Open();
		if (!open_) {
			this->Close();
			return value;
		}
		sql_ = "";
		sql_.append("update ").append(table).append(" set ").append(set_values);
		if (!query.empty())
			sql_.append(" where ").append(query);
		if (sqlite3_exec(db_, sql_.c_str(), NULL, NULL, &error_) == SQLITE_OK)
			value = true;
		this->Close();
		return value;
	}
	bool Delete(string table, string query) {
		bool value = false;
		this->Open();
		if (!open_) {
			this->Close();
			return value;
		}
		sql_ = "";
		sql_.append("delete ").append(table);
		if (!query.empty())
			sql_.append(" where ").append(query);
		if (sqlite3_exec(db_, sql_.c_str(), NULL, NULL, &error_) == SQLITE_OK)
			value = true;
		this->Close();
		return value;
	}
private:
	void Open() {
		if (sqlite3_open(db_path_.c_str(), &db_) == SQLITE_OK)
			open_ = true;
		else
			open_ = false;
	}
	void Close() {
		if (open_)
			sqlite3_close(db_);
	}
	string db_path_;
	sqlite3* db_;
	char* error_;
	string sql_;
	bool open_;
};

#endif // !__SQLITE_HELPER__
