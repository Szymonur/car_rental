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

    if (sqlite.createTableRental()) {
        std::cout << "Rental table created successfully!" << std::endl;
    } else {
        std::cerr << "Failed to create the rental table!" << std::endl;
    }
    // sqlite.executeSQL("ALTER TABLE rental ADD COLUMN if_issued BOOLEAN DEFAULT false;");
    // sqlite.executeSQL("ALTER TABLE rental ADD COLUMN if_returned BOOLEAN DEFAULT false;");


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
        if (sqlite.deleteVehicle(vin)) {
            crow::response res(200, "Vehicle deleted successfully");
            res.add_header("Access-Control-Allow-Origin", "*");
            return res;
        } else {
            crow::response res(400, "Failed to delete vehicle");
            res.add_header("Access-Control-Allow-Origin", "*");
            return res;
        }
    });

   // ----------------------------- ADD RENTAL -----------------------------------
    CROW_ROUTE(app, "/add_rental").methods(crow::HTTPMethod::Post)([](const crow::request& req) {
        SQLiteManager sqlite("test.db");
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Invalid JSON");
        }

        try {
            std::string rental_start = body["rental_start"].s();
            std::string rental_end = body["rental_end"].s();
            bool is_accepted = body["is_accepted"].b();
            std::string car_vin = body["car_vin"].s();
            int user_id = body["user_id"].i();
            double total_cost = body["total_cost"].d();
            bool if_paid = body["if_paid"].b();  
            bool if_issued =  body["if_issued"].b();  
            bool if_returned = body["if_returned"].b();  

            crow::json::wvalue res;
            res["message"] = "Data received successfully!";
            res["rental_start"] = rental_start;
            res["rental_end"] = rental_end;
            res["is_accepted"] = is_accepted;
            res["car_vin"] = car_vin;
            res["user_id"] = user_id;
            res["total_cost"] = total_cost;
            res["if_paid"] = if_paid;
            res["if_issued"] = if_issued;  // Include 'if_issued' in the response
            res["if_returned"] = if_returned;  // Include 'if_returned' in the response

            // Add rental to database
            if (sqlite.addRental(rental_start, rental_end, is_accepted, car_vin, user_id, total_cost, if_paid, if_issued, if_returned)) {
                std::cout << "Rental added successfully!" << std::endl;
            } else {
                std::cerr << "Failed to add rental!" << std::endl;
                return crow::response(500, "Failed to add rental!");
            }

            return crow::response(200, res);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return crow::response(400, "Invalid or missing fields in JSON");
        }
    });



    // ----------------------------- DELETE RENTAL -----------------------------------
    CROW_ROUTE(app, "/rental/<int>").methods(crow::HTTPMethod::Delete)
    ([&sqlite](int rental_id) {
        if (sqlite.deleteRental(rental_id)) {
            crow::response res(200, "Rental deleted successfully");
            res.add_header("Access-Control-Allow-Origin", "*");
            return res;
        } else {
            crow::response res(400, "Failed to delete rental");
            res.add_header("Access-Control-Allow-Origin", "*");
            return res;
        }
    });

    // ----------------------------- GET RENTALS -----------------------------------
    CROW_ROUTE(app, "/rentals")
    ([](const crow::request& req) {
        SQLiteManager sqlite("test.db");
        crow::json::wvalue rentals_json = sqlite.getRentals();
        return rentals_json;  
    });
    
    // ----------------------------- GET SINGLE RENTAL -----------------------------------
    CROW_ROUTE(app, "/rentals/<int>")
    ([](const crow::request& req, int rentalId) {
        SQLiteManager sqlite("test.db");
        crow::json::wvalue rental_json = sqlite.getRentalById(rentalId);

        if (rental_json.size() == 0) {
            return crow::response(404, "Rental not found");
        }

        return crow::response(rental_json);
    });

    // ----------------------------- MODIFY RENTAL -----------------------------------
    CROW_ROUTE(app, "/rentals/<int>").methods("PUT"_method)
    ([](const crow::request& req, int rental_id) {
        SQLiteManager sqlite("test.db");

        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Invalid JSON data");
        }

        std::string rental_start = body["rental_start"].s();
        std::string rental_end = body["rental_end"].s();
        bool is_accepted = body["is_accepted"].b();
        std::string car_vin = body["car_vin"].s();
        int user_id = body["user_id"].i();
        double total_cost = body["total_cost"].d();
        bool if_paid = body["if_paid"].b();
        bool if_issued = body["if_issued"].b();
        bool if_returned = body["if_returned"].b();

        bool success = sqlite.updateRental(rental_id, rental_start, rental_end, is_accepted, car_vin, user_id, total_cost, if_paid,if_issued ,if_returned );

        if (success) {
            return crow::response(200, "Rental updated successfully");
        } else {
            return crow::response(500, "Failed to update rental");
        }
    });


    // ----------------------------- GET ALL CAR RENTALS -----------------------------------
    CROW_ROUTE(app, "/rentals/car/<string>").methods("GET"_method)
    ([](const crow::request& req, const std::string& car_vin) {
        SQLiteManager sqlite("test.db");

        if (car_vin.size() == 0) {
            return crow::response(400, "Car VIN is required");
        }

        crow::json::wvalue rentals = sqlite.getRentalsByCarVin(car_vin);

        if (rentals.size() == 0) {
            return crow::response(404, "No rentals found for the specified car VIN");
        }

        return crow::response(200, rentals);
    });

    app.port(18080).multithreaded().run();

    return 0;
}
