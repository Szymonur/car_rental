#ifndef SQLITE_MANAGER_H
#define SQLITE_MANAGER_H

#include <string>
#include "sqlite3.h"
#include "crow/json.h" 


class SQLiteManager {
public:
    SQLiteManager(const std::string& db_name);
    ~SQLiteManager();

    bool executeSQL(const std::string& sql);
    bool createTable();  
    bool addUser(const std::string& name);
    crow::json::wvalue getUsers();
private:
    sqlite3* db;
    std::string dbName;
};

#endif 
