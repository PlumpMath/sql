# SQL
C++17 SQLite3 library for Visual Studio 2015, Visual Studio 2015 Clang/C2 and Clang 4.0.

```cpp
#include <sql.h>
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>

int main() {
  // Initialize the database.
  sql::database db("test.db");
  db << "DROP TABLE IF EXISTS users";
  db << "CREATE TABLE users (id NUMBER PRIMARY KEY, name TEXT)";

  // Create prepared statements.
  sql::statement get = db << "SELECT id, name FROM users";
  sql::statement set = db << "INSERT INTO users VALUES (?, ?)";

  // Run tests on multiple threads.
  std::vector<std::thread> threads;
  for (std::uint64_t i = 0; i < 4; i++) {
    threads.emplace_back([&db, &get, &set, i]() {
      // Start a transaction that will automatically rollback if an exception is thrown.
      sql::transaction lock(db);

      // Insert 10 new rows.
      for (std::uint64_t id = i * 10; id < i * 10 + 10; id++) {
        set(id, std::to_string(id));
      }

      // Retrieve all rows.
      for (auto& row : get()) {
        const auto id = row.get<std::uint64_t>(0);
        const auto name = row.get<std::string>(1);
        std::cout << id << ' ' << name << '\n';
      }
    });
  }

  for (auto& thread : threads) {
    if (thread.joinable()) {
      thread.join();
    }
  }

  return 0;
}
```
