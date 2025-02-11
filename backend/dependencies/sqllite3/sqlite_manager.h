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
    
    // Methods related to Person table
    bool createTablePerson();  
    bool addUser(const std::string& first_name, const std::string& last_name,
                 const std::string& email, const std::string& phone, int driving_exp_years);
    crow::json::wvalue getUsers();
    bool deleteUser(int id);

    // Methods related to Vehicle table
    bool createTableVehicle();
    bool addVehicle(const std::string& vin, const std::string& model, const std::string& brand,
                    int production_year, int mileage, const std::string& color, int hp,
                    float avg_fuel_usage, int seats_number, int doors_number, bool is_available,
                    int price_per_day);  
    crow::json::wvalue getVehicles();  
    bool deleteVehicle(const std::string& vin);  

    // Methods related to Rental table
    bool createTableRental();
    bool addRental(const std::string& rental_start, const std::string& rental_end, bool is_accepted, 
                   const std::string& car_vin, int user_id, double total_cost, bool if_paid, bool if_issued, bool if_returned);  // Added if_paid
    crow::json::wvalue getRentals();
    crow::json::wvalue getRentalById(int rentalId);
    bool deleteRental(int id);
    bool updateRental(int rental_id, const std::string& rental_start, const std::string& rental_end, 
                      bool is_accepted, const std::string& car_vin, int user_id, double total_cost, bool if_paid, bool if_issued, bool if_returned);  // Added if_paid
    
    // New method to get rentals by car VIN
    crow::json::wvalue getRentalsByCarVin(const std::string& carVin);

private:
    sqlite3* db;
    std::string dbName;
};

#endif  // SQLITE_MANAGER_H
