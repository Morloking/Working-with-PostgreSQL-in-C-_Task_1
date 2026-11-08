cmake_minimum_required (VERSION 3.22)

project(homework_sql)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(homework_sql 
main.cpp
consts.h
my_bd.h
my_bd.cpp
)

set(PostgreSQL_ROOT "C:/Program Files/PostgreSQL/18")
add_subdirectory("libpqxx-master" libpqxx-build)

target_link_libraries(homework_sql pqxx)