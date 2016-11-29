#pragma once
#include <sql/database.h>
#include <mutex>

namespace sql {

class lock {
public:
  lock(database& db) : db_(db), lock_(db.mutex()) {}

  database& db() noexcept {
    return db_;
  }

private:
  database& db_;
  std::lock_guard<std::mutex> lock_;
};

}  // namespace sql
