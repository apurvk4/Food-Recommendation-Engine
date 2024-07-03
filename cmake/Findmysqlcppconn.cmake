# Findmysqlcppconn.cmake
# This module looks for the mysqlcppconn library and its includes.

find_path(MYSQLCPPCONN_INCLUDE_DIR NAMES mysql_driver.h
  HINTS $ENV{MYSQLCPPCONN_INCLUDE_DIR}
  PATH_SUFFIXES mysqlcppconn
)

find_library(MYSQLCPPCONN_LIBRARY NAMES mysqlcppconn
  HINTS $ENV{MYSQLCPPCONN_LIBRARY}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(mysqlcppconn DEFAULT_MSG MYSQLCPPCONN_LIBRARY MYSQLCPPCONN_INCLUDE_DIR)

if(MYSQLCPPCONN_INCLUDE_DIR AND MYSQLCPPCONN_LIBRARY)
  if(NOT TARGET mysqlcppconn::mysqlcppconn)
    add_library(mysqlcppconn::mysqlcppconn UNKNOWN IMPORTED)
    set_target_properties(mysqlcppconn::mysqlcppconn PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${MYSQLCPPCONN_INCLUDE_DIR}"
      IMPORTED_LOCATION "${MYSQLCPPCONN_LIBRARY}"
    )
  endif()
else()
  message(FATAL_ERROR "Could not find mysqlcppconn library")
endif()
