#include "Database.h"

Database::Database(const std::string& dbName) {
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    }
}

Database::~Database() {
    if (db) {
        sqlite3_close(db);
    }
}

bool Database::executeSQL(const std::string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

void Database::initialize(Database& db) {
    std::string createUsersTable = R"(
        CREATE TABLE IF NOT EXISTS Users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL UNIQUE,
            email TEXT NOT NULL
        );
    )";

    std::string createGamesTable = R"(
        CREATE TABLE IF NOT EXISTS Games (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL UNIQUE,
            genre TEXT NOT NULL
        );
    )";

    std::string createUsersGamesTable = R"(
        CREATE TABLE IF NOT EXISTS Users_Games (
            user_id INTEGER NOT NULL,
            game_id INTEGER NOT NULL,
            FOREIGN KEY(user_id) REFERENCES Users(id),
            FOREIGN KEY(game_id) REFERENCES Games(id),
            PRIMARY KEY (user_id, game_id)
        );
    )";

    db.executeSQL(createUsersTable);
    db.executeSQL(createGamesTable);
    db.executeSQL(createUsersGamesTable);
}

sqlite3* Database::getDB() const {
    return db;
}
