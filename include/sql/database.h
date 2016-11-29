#pragma once
#include <sql/statement.h>
#include <filesystem>
#include <mutex>

extern "C" {

struct sqlite3;

}  // extern "C"

namespace sql {

enum class flags {
  read = 0x00000001,
  read_write = 0x00000002,
  read_write_create = 0x00000002 | 0x00000004,
};

using database_handle = std::unique_ptr<sqlite3, int (*)(sqlite3*)>;

class database {
public:
  database();
  database(const std::filesystem::path& filename, flags flags = flags::read_write_create);

  statement begin;
  statement commit;
  statement rollback;

  template <typename... T>
  statement operator()(std::string_view query, T&&... args) {
    auto statement = prepare(query);
    statement(std::forward<T>(args)...);
    return statement;
  }

  statement prepare(std::string_view query);

  std::size_t changes() const noexcept;

  std::mutex& mutex() noexcept {
    return *mutex_;
  }

  database_handle& handle() noexcept {
    return handle_;
  }

private:
  database_handle handle_;
  std::unique_ptr<std::mutex> mutex_;
};

}  // namespace sql
