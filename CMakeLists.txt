cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project("FoodRecommendationSystem")

message(STATUS "[${PROJECT_NAME}] Processing ${CMAKE_CURRENT_LIST_FILE}")

# Force the use of Clang if available
find_program(CLANG_CPP NAMES clang++ clang++-10 clang++-9 clang++-8 clang++-7 clang++-6 clang++-5)

if(CLANG_CPP)
    set(CMAKE_CXX_COMPILER ${CLANG_CPP})
    message(STATUS "Clang C++ compiler found: ${CLANG_CPP}")
else()
    message(STATUS "Clang C++ compiler not found, using default C++ compiler")
endif()

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/libs")

file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE INCLUDE_FILES "${INCLUDE_DIR}/*.hpp" "${INCLUDE_DIR}/*.h")
file(GLOB_RECURSE LIBS_INCLUDE_FILES "${LIB_DIR}/*.hpp" "${LIB_DIR}/*.h")

# Collect all subdirectories under INCLUDE_DIR and LIB_DIR
file(GLOB_RECURSE ALL_INCLUDE_DIRS LIST_DIRECTORIES true "${INCLUDE_DIR}/*")
file(GLOB_RECURSE ALL_LIBS_INCLUDE_DIRS LIST_DIRECTORIES true "${LIB_DIR}/*")

# Filter only directories
set(DIR_LIST "")
foreach(DIR ${ALL_INCLUDE_DIRS})
    if(IS_DIRECTORY ${DIR})
        list(APPEND DIR_LIST ${DIR})
    endif()
endforeach()

foreach(DIR ${ALL_LIBS_INCLUDE_DIRS})
    if(IS_DIRECTORY ${DIR})
        list(APPEND DIR_LIST ${DIR})
    endif()
endforeach()

include_directories(${DIR_LIST})

# Setup target executable with the same name as our project
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${DIR_LIST}
    ${INCLUDE_DIR}
    ${LIB_DIR}
)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Find packages
find_package(mysqlcppconn REQUIRED)
find_package(Threads REQUIRED)

# Link against math library
target_link_libraries(${PROJECT_NAME} PRIVATE m)

# Link against libraries using keywords
target_link_libraries(${PROJECT_NAME} PRIVATE mysqlcppconn::mysqlcppconn ${CMAKE_THREAD_LIBS_INIT})

# Set C++ standard to 14
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS NO
)

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Set debug flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Enable multiprocessing for MSVC
if (MSVC)
    target_compile_options(${PROJECT_NAME}
        PRIVATE "/MP"
    )
endif()
