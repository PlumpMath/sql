#pragma once
#include <sql/types.h>
#include <optional>

namespace sql {

template <typename T>
struct traits<std::optional<T>> {
  static void set(sql::column& column, const std::optional<T>& value) {
    if (value) {
      column.set(value.value());
    } else {
      column.set();
    }
  }
  static std::optional<T> get(const sql::column& column) {
    if (column) {
      return column.get<T>();
    }
    return{};
  }
};

}  // namespace sql
