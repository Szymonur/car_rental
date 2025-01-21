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
    bool createTablePerson();  
    bool addUser(const std::string& first_name, const std::string& last_name,
             const std::string& email, const std::string& phone, int driving_exp_years);
    crow::json::wvalue getUsers();
    bool deleteUser(int id);

    bool createTableVehicle();
    bool addVehicle(const std::string& vin, const std::string& model, const std::string& brand,
                    int production_year, int mileage, const std::string& color, int hp,
                    float avg_fuel_usage, int seats_number, int doors_number, bool is_available,
                    int price_per_day);  
    crow::json::wvalue getVehicles();  
    bool deleteVehicle(const std::string& vin);  

    bool createTableRental();
    bool addRental(const std::string& rental_start, const std::string& rental_end, bool is_accepted, const std::string& car_vin, int user_id);
    crow::json::wvalue getRentals();
    bool deleteRental(int id);

private:
    sqlite3* db;
    std::string dbName;
};

#endif 
