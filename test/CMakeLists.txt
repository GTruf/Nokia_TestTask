cmake_minimum_required(VERSION 3.18.4)
project(Nokia_TestCSV_Test)

set(CMAKE_CXX_STANDARD 20)


add_executable(${PROJECT_NAME} src/main_test.cpp src/test_cases.h src/test_runner.h ../../src/Utilities/auxiliary_functionality.cpp ../../src/CSV_Module/csv_calculator.cpp ../../src/CSV_Module/csv_container.cpp ../../src/CSV_Module/csv_printer.cpp)