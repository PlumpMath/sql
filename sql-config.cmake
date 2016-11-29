set(SQL_FOUND true)
set(SQL_VERSION "3.15.1")

if(NOT TARGET sql)
  find_package(gsl REQUIRED PATHS "${CMAKE_CURRENT_LIST_DIR}/../gsl")
  add_library(sql STATIC IMPORTED)
  set_target_properties(sql PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/include"
    IMPORTED_LINK_INTERFACE_LANGUAGES "C;CXX")
  if(MSVC)
    set_target_properties(sql PROPERTIES
      IMPORTED_LOCATION_DEBUG "${CMAKE_CURRENT_LIST_DIR}/lib/debug/sql.lib"
      IMPORTED_LOCATION_RELEASE "${CMAKE_CURRENT_LIST_DIR}/lib/release/sql.lib"
      IMPORTED_CONFIGURATIONS "DEBUG;RELEASE"
      INTERFACE_LINK_LIBRARIES "gsl")
  else()
    set_target_properties(sql PROPERTIES
      IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/lib/libsql.a"
      IMPORTED_CONFIGURATIONS "RELEASE"
      INTERFACE_LINK_LIBRARIES "gsl;dl")
  endif()
endif()
