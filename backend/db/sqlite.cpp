#include "sqlite_manager.h"
#include <iostream>
#include <vector>
#include <crow/json.h>


SQLiteManager::SQLiteManager(const std::string& db_name) : dbName(db_name), db(nullptr) {
    if (sqlite3_open(dbName.c_str(), &db)) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    }
}

SQLiteManager::~SQLiteManager() {
    if (db) {
        sqlite3_close(db); 
    }
}

bool SQLiteManager::executeSQL(const std::string& sql) {
    char* errMessage = 0;
    if (sqlite3_exec(db, sql.c_str(), 0, 0, &errMessage) != SQLITE_OK) {
        std::cerr << "SQL error: " << errMessage << std::endl;
        sqlite3_free(errMessage);
        return false;
    }

    return true;
}

bool SQLiteManager::createTable() {
    const std::string sql = "CREATE TABLE IF NOT EXISTS person (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT);";
    return executeSQL(sql);
}

bool SQLiteManager::addUser(const std::string& name) {
    std::string sql = "INSERT INTO person (name) VALUES ('" + name + "');";
    return executeSQL(sql);
}

crow::json::wvalue SQLiteManager::getUsers() {
    const std::string sql = "SELECT * FROM person;";

    sqlite3_stmt* stmt;
    crow::json::wvalue users_json = crow::json::wvalue();  

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << std::endl;
        return users_json;
    }
    int index = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        crow::json::wvalue user_json;
        user_json["id"] = sqlite3_column_int(stmt, 0);  
        user_json["name"] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));

        users_json[index] = std::move(user_json);  
        ++index;
    }

    sqlite3_finalize(stmt);

    return users_json;
}





