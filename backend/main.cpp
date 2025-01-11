#include "iostream"
#include "dependencies/Crow/include/crow.h"
#include "dependencies/Crow/include/crow/json.h"
#include "sqlite3.h"
#include "sqlite_manager.h"
#include "cors.h"
 
int main() {

    SQLiteManager sqlite("test.db");
    if (sqlite.createTable()) {
        std::cout << "Database and table created successfully!" << std::endl;
    } else {
        std::cerr << "Failed to create the table!" << std::endl;
    }
    if (sqlite.addUser("John Doe")) {
        std::cout << "User added successfully!" << std::endl;
    } else {
        std::cerr << "Failed to add user!" << std::endl;
    }
    crow::App<crow::CORSHandler> app;

    auto& cors = app.get_middleware<crow::CORSHandler>();
    cors.global()
        .origin("http://localhost:5173");  

    // ----------------------------- TEST USER -----------------------------------
    CROW_ROUTE(app, "/json")([]() {
        crow::json::wvalue json_response;

        json_response["name"] = "John Doe";
        json_response["age"] = 30;
        json_response["is_student"] = false;
        json_response["courses"] = {"Math", "Science", "English"};

        return json_response;
    });
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
        std::string name = body["name"].s();
        crow::json::wvalue res;
        res["message"] = "Data received successfully!";
        res["name"] = name;
        
        if (sqlite.addUser(name)) {
            std::cout << "User added successfully!" << std::endl;
        } else {
            std::cerr << "Failed to add user!" << std::endl;
        }
        return crow::response(200, res);
    });




    app.port(18080).multithreaded().run();

    return 0;
}