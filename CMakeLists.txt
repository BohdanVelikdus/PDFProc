cmake_minimum_required(VERSION 3.28)
project(PRFproc)


set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig)
pkg_check_modules(POPPLER_CPP REQUIRED IMPORTED_TARGET poppler-cpp)

find_path(SQLITE_MODERN_CPP_INCLUDE_DIRS "sqlite_modern_cpp.h")


find_package(unofficial-sqlite3 CONFIG REQUIRED)

include_directories(${PROJECT_NAME} PRIVATE ${SQLITE_MODERN_CPP_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ./main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::POPPLER_CPP)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::sqlite3::sqlite3)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../Debug"
)