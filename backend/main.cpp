#include "iostream"
#include "dependencies/Crow/include/crow.h"
#include "dependencies/Crow/include/crow/json.h"
#include "sqlite3.h"
#include "sqlite_manager.h"
#include "cors.h"
 
int main() {

    SQLiteManager sqlite("test.db");
    if (sqlite.createTablePerson()) {
        std::cout << "Database and table created successfully!" << std::endl;
    } else {
        std::cerr << "Failed to create the table!" << std::endl;
    }
    if (sqlite.createTableVehicle()) {
        std::cout << "Vehicle table created successfully!" << std::endl;
    } else {
        std::cerr << "Failed to create the vehicle table!" << std::endl;
    }

    crow::App<crow::CORSHandler> app;

    auto& cors = app.get_middleware<crow::CORSHandler>();
    cors.global()
        .origin("http://localhost:5173");  

    // ----------------------------- GET USERS -----------------------------------
    CROW_ROUTE(app, "/users")
    ([](const crow::request& req) {
        SQLiteManager sqlite("test.db");
        crow::json::wvalue users_json = sqlite.getUsers();
        return users_json;  
    });

    // ----------------------------- ADD USER -----------------------------------
    CROW_ROUTE(app, "/add_user").methods(crow::HTTPMethod::Post)([](const crow::request& req) {
    SQLiteManager sqlite("test.db");
    auto body = crow::json::load(req.body);
    if (!body) {
        return crow::response(400, "Invalid JSON"); 
    }

    try {
        // Extract fields from the JSON body
        std::string first_name = body["first_name"].s();
        std::string last_name = body["last_name"].s();
        std::string email = body["email"].s();
        std::string phone = body["phone"].s();
        int driving_exp_years = body["driving_exp_years"].i();

        // Prepare response JSON
        crow::json::wvalue res;
        res["message"] = "Data received successfully!";
        res["first_name"] = first_name;
        res["last_name"] = last_name;
        res["email"] = email;
        res["phone"] = phone;
        res["driving_exp_years"] = driving_exp_years;

        // Add user to database
        if (sqlite.addUser(first_name, last_name, email, phone, driving_exp_years)) {
            std::cout << "User added successfully!" << std::endl;
        } else {
            std::cerr << "Failed to add user!" << std::endl;
            return crow::response(500, "Failed to add user!");
        }

        return crow::response(200, res);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return crow::response(400, "Invalid or missing fields in JSON");
    }
    });

    // ----------------------------- DELETE USER -----------------------------------
    CROW_ROUTE(app, "/users/<int>").methods(crow::HTTPMethod::Delete)
    ([&sqlite](int id) {
        if (sqlite.deleteUser(id)) {
            return crow::response(200, "User deleted successfully");
        } else {
            return crow::response(400, "Failed to delete user");
        }
    });
     // ----------------------------- GET VEHICLES -----------------------------------
    CROW_ROUTE(app, "/vehicles")
    ([](const crow::request& req) {
        SQLiteManager sqlite("test.db");
        crow::json::wvalue vehicles_json = sqlite.getVehicles();
        return vehicles_json;  
    });

    // ----------------------------- ADD VEHICLE -----------------------------------
    CROW_ROUTE(app, "/add_vehicle").methods(crow::HTTPMethod::Post)([](const crow::request& req) {
        SQLiteManager sqlite("test.db");
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Invalid JSON"); 
        }

        try {
            // Extract fields from the JSON body
            std::string vin = body["vin"].s();
            std::string model = body["model"].s();
            std::string brand = body["brand"].s();
            int production_year = body["production_year"].i();
            int mileage = body["mileage"].i();
            std::string color = body["color"].s();
            int hp = body["hp"].i();
            float avg_fuel_usage = body["avg_fuel_usage"].d();
            int seats_number = body["seats_number"].i();
            int doors_number = body["doors_number"].i();
            bool is_available = body["is_available"].b();
            int price_per_day = body["price_per_day"].i();

            // Prepare response JSON
            crow::json::wvalue res;
            res["message"] = "Data received successfully!";
            res["vin"] = vin;
            res["model"] = model;
            res["brand"] = brand;
            res["production_year"] = production_year;
            res["mileage"] = mileage;
            res["color"] = color;
            res["hp"] = hp;
            res["avg_fuel_usage"] = avg_fuel_usage;
            res["seats_number"] = seats_number;
            res["doors_number"] = doors_number;
            res["is_available"] = is_available;
            res["price_per_day"] = price_per_day;

            // Add vehicle to database
            if (sqlite.addVehicle(vin, model, brand, production_year, mileage, color, hp,
                                  avg_fuel_usage, seats_number, doors_number, is_available, price_per_day)) {
                std::cout << "Vehicle added successfully!" << std::endl;
            } else {
                std::cerr << "Failed to add vehicle!" << std::endl;
                return crow::response(500, "Failed to add vehicle!");
            }

            return crow::response(200, res);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return crow::response(400, "Invalid or missing fields in JSON");
        }
    });
    // ----------------------------- DELETE VEHICLE -----------------------------------
    CROW_ROUTE(app, "/vehicle/<string>").methods(crow::HTTPMethod::Delete)
    ([&sqlite](const std::string& vin) {
        // Attempt to delete the vehicle using the provided VIN
        if (sqlite.deleteVehicle(vin)) {
            crow::response res(200, "Vehicle deleted successfully");
            res.add_header("Access-Control-Allow-Origin", "*"); // Allow all origins
            return res;
        } else {
            crow::response res(400, "Failed to delete vehicle");
            res.add_header("Access-Control-Allow-Origin", "*"); // Allow all origins
            return res;
        }
    });




    app.port(18080).multithreaded().run();

    return 0;
}