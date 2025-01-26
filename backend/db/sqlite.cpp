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

bool SQLiteManager::createTableRental() {
    const std::string sql = 
        "CREATE TABLE IF NOT EXISTS rental ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "rental_start DATE NOT NULL, "
        "rental_end DATE NOT NULL, "
        "is_accepted BOOLEAN NOT NULL, "
        "car_vin TEXT NOT NULL, "
        "user_id INTEGER NOT NULL, "
        "total_cost REAL NOT NULL, "
        "if_paid BOOLEAN NOT NULL DEFAULT 0, "
        "if_issued BOOLEAN NOT NULL DEFAULT 0, "  // New column for issued status
        "if_returned BOOLEAN NOT NULL DEFAULT 0, " // New column for returned status
        "FOREIGN KEY(car_vin) REFERENCES vehicle(vin), "
        "FOREIGN KEY(user_id) REFERENCES person(id));";
    return executeSQL(sql);
}

bool SQLiteManager::addRental(const std::string& rental_start, const std::string& rental_end, 
                              bool is_accepted, const std::string& car_vin, 
                              int user_id, double total_cost, bool if_paid, bool if_issued, bool if_returned) {
    try {
std::string sql = "INSERT INTO rental (rental_start, rental_end, is_accepted, car_vin, user_id, total_cost, if_paid, if_issued, if_returned) "
                  "VALUES ('" + rental_start + "', '" + rental_end + "', " + 
                  (is_accepted ? "1" : "0") + ", '" + car_vin + "', " + 
                  std::to_string(user_id) + ", " + std::to_string(total_cost) + ", " + 
                  (if_paid ? "1" : "0") + ", " +  (if_issued? "1" : "0") + ", " + (if_returned ? "1" : "0") + ");";
        return executeSQL(sql);
    } catch (const std::exception& e) {
        std::cerr << "Error in addRental: " << e.what() << std::endl;
        return false;
    }
}

crow::json::wvalue SQLiteManager::getRentals() {
    const std::string sql = "SELECT * FROM rental;";

    sqlite3_stmt* stmt;
    crow::json::wvalue rentals_json = crow::json::wvalue();

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Failed to fetch data: " << sqlite3_errmsg(db) << std::endl;
        return rentals_json;
    }
    int index = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        crow::json::wvalue rental_json;

        rental_json["id"] = sqlite3_column_int(stmt, 0);
        rental_json["rental_start"] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        rental_json["rental_end"] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        rental_json["is_accepted"] = sqlite3_column_int(stmt, 3);
        rental_json["car_vin"] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        rental_json["user_id"] = sqlite3_column_int(stmt, 5);
        rental_json["total_cost"] = sqlite3_column_double(stmt, 6);
        rental_json["if_paid"] = sqlite3_column_int(stmt, 7);  // Include if_paid
        rental_json["if_issued"] = sqlite3_column_int(stmt, 8); // Include if_issued
        rental_json["if_returned"] = sqlite3_column_int(stmt, 9); // Include if_returned

        rentals_json[index] = std::move(rental_json);
        ++index;
    }

    sqlite3_finalize(stmt);

    return rentals_json;
}

crow::json::wvalue SQLiteManager::getRentalById(int rentalId) {
    const std::string sql = "SELECT * FROM rental WHERE id = ?;";
    sqlite3_stmt* stmt;
    crow::json::wvalue rental_json;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Failed to fetch rental: " << sqlite3_errmsg(db) << std::endl;
        return rental_json;
    }

    // Bind the rental ID to the query
    if (sqlite3_bind_int(stmt, 1, rentalId) != SQLITE_OK) {
        std::cerr << "Failed to bind rental ID: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return rental_json;
    }

    // Execute the query
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        rental_json["id"] = sqlite3_column_int(stmt, 0);
        rental_json["rental_start"] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        rental_json["rental_end"] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        rental_json["is_accepted"] = sqlite3_column_int(stmt, 3);
        rental_json["car_vin"] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        rental_json["user_id"] = sqlite3_column_int(stmt, 5);
        rental_json["total_cost"] = sqlite3_column_double(stmt, 6);
        rental_json["if_paid"] = sqlite3_column_int(stmt, 7);  // Include if_paid
        rental_json["if_issued"] = sqlite3_column_int(stmt, 8); // Include if_issued
        rental_json["if_returned"] = sqlite3_column_int(stmt, 9); // Include if_returned
    } else {
        std::cerr << "No rental found with ID: " << rentalId << std::endl;
    }

    sqlite3_finalize(stmt);

    return rental_json;
}




bool SQLiteManager::deleteRental(int id) {
    try {
        std::string sql = "DELETE FROM rental WHERE id = " + std::to_string(id) + ";";

        return executeSQL(sql);
    } catch (const std::exception& e) {
        std::cerr << "Error in deleteRental: " << e.what() << std::endl;
        return false;
    }
}
bool SQLiteManager::updateRental(int rental_id, const std::string& rental_start, const std::string& rental_end, 
                                 bool is_accepted, const std::string& car_vin, 
                                 int user_id, double total_cost, bool if_paid, bool if_issued, bool if_returned) {
    const std::string sql = "UPDATE rental SET rental_start = ?, rental_end = ?, is_accepted = ?, car_vin = ?, user_id = ?, total_cost = ?, if_paid = ?, if_issued = ?, if_returned = ?  WHERE id = ?;";
    sqlite3_stmt* stmt;

    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Failed to prepare update statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind the parameters to the statement
    sqlite3_bind_text(stmt, 1, rental_start.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, rental_end.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, is_accepted);
    sqlite3_bind_text(stmt, 4, car_vin.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, user_id);
    sqlite3_bind_double(stmt, 6, total_cost);  // Bind total_cost
    sqlite3_bind_int(stmt, 7, if_paid);  // Bind if_paid status
    sqlite3_bind_int(stmt, 8, if_issued);  // Bind the rental ID for update
    sqlite3_bind_int(stmt, 9, if_returned);  // Bind the rental ID for update
    sqlite3_bind_int(stmt, 10, rental_id);  // Bind the rental ID for update

    
    

    // Execute the statement
    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    if (!success) {
        std::cerr << "Failed to update rental: " << sqlite3_errmsg(db) << std::endl;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);
    return success;
}


crow::json::wvalue SQLiteManager::getRentalsByCarVin(const std::string& carVin) {
    const std::string sql = "SELECT * FROM rental WHERE car_vin = ?;";
    sqlite3_stmt* stmt;
    crow::json::wvalue rentals_json = crow::json::wvalue(); // Initialize as a container for the results

    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Failed to prepare query: " << sqlite3_errmsg(db) << std::endl;
        return rentals_json;
    }

    // Bind the car VIN to the query
    if (sqlite3_bind_text(stmt, 1, carVin.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        std::cerr << "Failed to bind car VIN: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return rentals_json;
    }

    int index = 0;

    // Execute the query and process each row
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        crow::json::wvalue rental_json;

        rental_json["id"] = sqlite3_column_int(stmt, 0);
        rental_json["rental_start"] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        rental_json["rental_end"] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        rental_json["is_accepted"] = sqlite3_column_int(stmt, 3);
        rental_json["car_vin"] = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        rental_json["user_id"] = sqlite3_column_int(stmt, 5);
        rental_json["total_cost"] = sqlite3_column_double(stmt, 6); // Include total_cost if needed
        rental_json["if_paid"] = sqlite3_column_int(stmt, 7);  // Retrieve if_paid status
        rental_json["if_issued"] = sqlite3_column_int(stmt, 8); // Include if_issued
        rental_json["if_returned"] = sqlite3_column_int(stmt, 9); // Include if_returned

        // Add rental to the JSON array
        rentals_json[index] = std::move(rental_json);
        ++index;
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    return rentals_json;
}










