#include "test_cases.h"

int main() {
    TestRunner testRunner;
    RUN_TEST(testRunner, TestAuxiliaryFunctionality);
    RUN_TEST(testRunner, TestFileProcess);
    RUN_TEST(testRunner, TestCellFormat);
    RUN_TEST(testRunner, TestCalculation);
    RUN_TEST(testRunner, TestPrint);

    return 0;
}
