#include <sql/database.h>
#include <sql/exception.h>
#include <sqlite3.h>
#include <limits>

namespace sql {
namespace {

std::string error(sqlite3* handle) {
  if (const auto str = sqlite3_errmsg(handle)) {
    return str;
  }
  return "unknown database error";
}

}  // namespace

database::database() : handle_(nullptr, sqlite3_close), mutex_(std::make_unique<std::mutex>()) {}

database::database(const std::filesystem::path& filename, flags flags) : database() {
  sqlite3* handle = nullptr;
  auto ec = sqlite3_open_v2(filename.u8string().data(), &handle, static_cast<int>(flags), nullptr);
  if (ec != SQLITE_OK) {
    sqlite3_close(handle);
    throw exception("open error: " + error(ec) + ": " + filename.u8string());
  }
  handle_.reset(handle);
  begin = prepare("BEGIN");
  commit = prepare("COMMIT");
  rollback = prepare("ROLLBACK");
}

statement database::prepare(std::string_view query) {
  sqlite3_stmt* handle = nullptr;
  const auto data = query.data();
  const auto size = query.size();
  if (size > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
    throw exception("query size error");
  }
  const auto isize = static_cast<int>(size);
  if (sqlite3_prepare_v2(handle_.get(), data, isize, &handle, nullptr) != SQLITE_OK) {
    sqlite3_finalize(handle);
    throw exception("prepare error: " + error(handle_.get()), query);
  }
  statement stmt;
  stmt.handle().reset(handle);
  return stmt;
}

std::size_t database::changes() const noexcept {
  const auto changes = sqlite3_changes(handle_.get());
  if (changes > 0) {
    return static_cast<std::size_t>(changes);
  }
  return 0;
}

}  // namespace sql
