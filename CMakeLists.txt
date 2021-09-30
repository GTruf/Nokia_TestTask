cmake_minimum_required(VERSION 3.18.4)
project(Nokia_TestCSV)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} src/main.cpp src/CSV_Module/csv_container.cpp src/CSV_Module/csv_container.h src/CSV_Module/csv_module.h src/CSV_Module/csv_calculator.cpp src/CSV_Module/csv_calculator.h src/CSV_Module/csv_printer.cpp src/CSV_Module/csv_printer.h src/Utilities/auxiliary_functionality.cpp src/Utilities/auxiliary_functionality.h)
