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

bool SQLiteManager::createTablePerson() {
    const std::string sql = 
        "CREATE TABLE IF NOT EXISTS person ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "first_name TEXT NOT NULL, "
        "last_name TEXT NOT NULL, "
        "email TEXT NOT NULL, "
        "phone TEXT NOT NULL, "
        "driving_exp_years INTEGER NOT NULL);";
    return executeSQL(sql);
}

bool SQLiteManager::addUser(const std::string& first_name, const std::string& last_name,
                            const std::string& email, const std::string& phone,
                            int driving_exp_years) {
    try {
        std::string sql = "INSERT INTO person (first_name, last_name, email, phone, driving_exp_years) "
                          "VALUES ('" + first_name + "', '" + last_name + "', '" + email + 
                          "', '" + phone + "', " + std::to_string(driving_exp_years) + ");";

        return executeSQL(sql);
    } catch (const std::exception& e) {
        std::cerr << "Error in addUser: " << e.what() << std::endl;
        return false;
    }
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
        user_json["first_name"] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        user_json["last_name"] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        user_json["email"] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        user_json["phone"] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        user_json["driving_exp_years"] = sqlite3_column_int(stmt, 5);

        users_json[index] = std::move(user_json);  
        ++index;
    }

    sqlite3_finalize(stmt);

    return users_json;
}

bool SQLiteManager::deleteUser(int id) {
    try {
        std::string sql = "DELETE FROM person WHERE id = " + std::to_string(id) + ";";

        return executeSQL(sql);
    } catch (const std::exception& e) {
        std::cerr << "Error in deleteUser: " << e.what() << std::endl;
        return false;
    }
}
bool SQLiteManager::createTableVehicle() {
    const std::string sql = 
        "CREATE TABLE IF NOT EXISTS vehicle ("
        "vin TEXT PRIMARY KEY, "
        "model TEXT NOT NULL, "
        "brand TEXT NOT NULL, "
        "production_year INTEGER NOT NULL, "
        "mileage INTEGER NOT NULL, "
        "color TEXT NOT NULL, "
        "hp INTEGER NOT NULL, "
        "avg_fuel_usage REAL NOT NULL, "
        "seats_number INTEGER NOT NULL, "
        "doors_number INTEGER NOT NULL, "
        "is_available BOOLEAN NOT NULL, "
        "price_per_day INTEGER NOT NULL);";
    return executeSQL(sql);
}

// Function to add a vehicle
bool SQLiteManager::addVehicle(const std::string& vin, const std::string& model, const std::string& brand,
                               int production_year, int mileage, const std::string& color, int hp,
                               float avg_fuel_usage, int seats_number, int doors_number, bool is_available,
                               int price_per_day) {
    try {
        std::string sql = "INSERT INTO vehicle (vin, model, brand, production_year, mileage, color, hp, "
                          "avg_fuel_usage, seats_number, doors_number, is_available, price_per_day) "
                          "VALUES ('" + vin + "', '" + model + "', '" + brand + "', " + std::to_string(production_year) + 
                          ", " + std::to_string(mileage) + ", '" + color + "', " + std::to_string(hp) + ", " + std::to_string(avg_fuel_usage) + 
                          ", " + std::to_string(seats_number) + ", " + std::to_string(doors_number) + ", " + 
                          (is_available ? "1" : "0") + ", " + std::to_string(price_per_day) + ");";
        return executeSQL(sql);
    } catch (const std::exception& e) {
        std::cerr << "Error in addVehicle: " << e.what() << std::endl;
        return false;
    }
}

crow::json::wvalue SQLiteManager::getVehicles() {
    const std::string sql = "SELECT * FROM vehicle;";

    sqlite3_stmt* stmt;
    crow::json::wvalue vehicles_json = crow::json::wvalue();  

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << std::endl;
        return vehicles_json;
    }
    int index = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        crow::json::wvalue vehicle_json;

        vehicle_json["vin"] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        vehicle_json["model"] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        vehicle_json["brand"] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        vehicle_json["production_year"] = sqlite3_column_int(stmt, 3);
        vehicle_json["mileage"] = sqlite3_column_int(stmt, 4);
        vehicle_json["color"] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
        vehicle_json["hp"] = sqlite3_column_int(stmt, 6);
        vehicle_json["avg_fuel_usage"] = sqlite3_column_double(stmt, 7);
        vehicle_json["seats_number"] = sqlite3_column_int(stmt, 8);
        vehicle_json["doors_number"] = sqlite3_column_int(stmt, 9);
        vehicle_json["is_available"] = sqlite3_column_int(stmt, 10);
        vehicle_json["price_per_day"] = sqlite3_column_int(stmt, 11);

        vehicles_json[index] = std::move(vehicle_json);  
        ++index;
    }

    sqlite3_finalize(stmt);

    return vehicles_json;
}
bool SQLiteManager::deleteVehicle(const std::string& vin) {
    try {
        std::string sql = "DELETE FROM vehicle WHERE vin = " + vin + ";";

        return executeSQL(sql);
    } catch (const std::exception& e) {
        std::cerr << "Error in deleteVehicle: " << e.what() << std::endl;
        return false;
    }
}



