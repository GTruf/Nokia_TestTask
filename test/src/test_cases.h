
#ifndef NOKIA_TESTCSV_TEST_TEST_CASES_H
#define NOKIA_TESTCSV_TEST_TEST_CASES_H
#include "test_runner.h"
#include "../../src/Utilities/auxiliary_functionality.h"
#include "../../src/CSV_Module/csv_module.h"

void TestAuxiliaryFunctionality() {
    ASSERT(!IsInteger(""))
    ASSERT(IsInteger("-100"))
    ASSERT(IsInteger("0"))
    ASSERT(IsInteger("100"))
    ASSERT(!IsInteger("100+"))
    ASSERT(!IsInteger("--100"))
    ASSERT(!IsInteger("nonNumber"))
    ASSERT(!IsInteger("15.25"))
    ASSERT(!IsInteger("+97"))

    ASSERT(!IsNonNegativeInteger(""))
    ASSERT(!IsNonNegativeInteger("-100"))
    ASSERT(IsNonNegativeInteger("0"))
    ASSERT(IsNonNegativeInteger("100"))
    ASSERT(!IsNonNegativeInteger("100+"))
    ASSERT(!IsNonNegativeInteger("--100"))
    ASSERT(!IsNonNegativeInteger("nonNumber"))
    ASSERT(!IsNonNegativeInteger("15.25"))
    ASSERT(!IsNonNegativeInteger("+97"))

    ASSERT(!IsJustWord(""))
    ASSERT(!IsJustWord(" "))
    ASSERT(IsJustWord("word"))
    ASSERT(!IsJustWord("0word"))
    ASSERT(!IsJustWord("word0"))
    ASSERT(!IsJustWord("w0rd"))
    ASSERT(!IsJustWord("0"))

    ASSERT(!IsCellName(""))
    ASSERT(!IsCellName(" "))
    ASSERT(!IsCellName("w o r d"))
    ASSERT(!IsCellName("1B"))
    ASSERT(!IsCellName("155"))
    ASSERT(!IsCellName("A20Cell"))
    ASSERT(IsCellName("Cell30"))

    ASSERT(WithoutSpaces("").empty())
    ASSERT(WithoutSpaces(" ").empty())
    ASSERT(WithoutSpaces("w o r d") == "word")
    ASSERT(WithoutSpaces("0 word") == "0word")
    ASSERT(WithoutSpaces("word 0 ") == "word0")
    ASSERT(WithoutSpaces("w 0 rd") == "w0rd")
    ASSERT(WithoutSpaces(" % 0 , ") == "%0,")
    ASSERT(WithoutSpaces("  b   1   TABULATION") == "b1TABULATION")

    ASSERT(TrimLeft("").empty())
    ASSERT(TrimLeft("    ").empty())
    ASSERT(TrimLeft("     w o r d") == "w o r d")
    ASSERT(TrimLeft(" 0 word") == "0 word")
    ASSERT(TrimLeft("        twoTabulationsAndSpace") == "twoTabulationsAndSpace")
}

void TestFileProcess() {
    try {
        CSVContainer emptyFile("../resource/TestFileProcess/empty.csv");
    }
    catch (const runtime_error& ex) {
        ASSERT(ex.what() == string("Error, file \"../resource/TestFileProcess/empty.csv\" is empty"))
    }

    try {
        CSVContainer incorrectExtension("../resource/TestFileProcess/incorrectExtension.txt");
    }
    catch (const runtime_error& ex) {
        ASSERT(ex.what() == string("Error, \"../resource/TestFileProcess/incorrectExtension.txt\" file extension does not match csv"))
    }

    try {
        CSVContainer nonExistent("abcdefg.abc");
    }
    catch (const runtime_error& ex) {
        ASSERT(ex.what() == string("Error opening file \"abcdefg.abc\""))
    }
}

void TestCellFormat() {
    try {
        CSVContainer csv("../resource/TestCellFormat/nonEmptyFirstCell.csv");
    }
    catch (const runtime_error& ex) {
        ASSERT(ex.what() == string("Error, first cell must be empty: 4"))
    }

    try {
        CSVContainer csv("../resource/TestCellFormat/strangeColumnHeaders.csv");
    }
    catch (const runtime_error& ex) {
        ASSERT(ex.what() == string("Error, column headers must be just a word: 1Cell  4strange"))
    }

    try {
        CSVContainer csv("../resource/TestCellFormat/strangeRowHeaders.csv");
    }
    catch (const runtime_error& ex) {
        ASSERT(ex.what() == string("Error, row headers must be non-negative numbers: 1Row_"))
    }

    try {
        CSVContainer csv("../resource/TestCellFormat/emptyCell.csv");
    }
    catch (const runtime_error& ex) {
        ASSERT(ex.what() == string("Error, some cell other than the first is empty"))
    }

    try {
        CSVContainer csv("../resource/TestCellFormat/formulaWithoutEquals.csv");
    }
    catch (const runtime_error& ex) {
        ASSERT(ex.what() == string("Error, formulas must start with \"=\": A1/Cell30"))
    }

    try {
        CSVContainer csv("../resource/TestCellFormat/columnAlreadyExists.csv");
    }
    catch (const runtime_error& ex) {
        ASSERT(ex.what() == string("Error, column \"A\" already exists"))
    }

    try {
        CSVContainer csv("../resource/TestCellFormat/rowAlreadyExists.csv");
    }
    catch (const runtime_error& ex) {
        ASSERT(ex.what() == string("Error, row \"1\" already exists"))
    }

    try {
        CSVContainer csv("../resource/TestCellFormat/cellPattern1.csv");
    }
    catch (const runtime_error& ex) {
        ASSERT(ex.what() == string("Error, cell must match the pattern \"only number or ARG1 OP ARG2\": =B1-"))
    }

    try {
        CSVContainer csv("../resource/TestCellFormat/cellPattern2.csv");
    }
    catch (const runtime_error& ex) {
        ASSERT(ex.what() == string("Error, cell must match the pattern \"only number or ARG1 OP ARG2\": =*B4"))
    }

    try {
        CSVContainer csv("../resource/TestCellFormat/cellPattern3.csv");
    }
    catch (const runtime_error& ex) {
        ASSERT(ex.what() == string("Error, cell must match the pattern \"only number or ARG1 OP ARG2\": =5**C4"))
    }

    try {
        CSVContainer csv("../resource/TestCellFormat/cellPattern4.csv");
    }
    catch (const runtime_error& ex) {
        ASSERT(ex.what() == string("Error, cell must match the pattern \"only number or ARG1 OP ARG2\": =B1-5K"))
    }
}

void TestCalculation() {
    try {
        CSVContainer csv("../resource/TestCalculation/unknownCell.csv");
        CSVCalculator::Calculate(csv);
    }
    catch (const invalid_argument& ex) {
        ASSERT(ex.what() == string("Unknown UnknownCell50 cell"))
    }

    try {
        CSVContainer csv("../resource/TestCalculation/divisionByZero.csv");
        CSVCalculator::Calculate(csv);
    }
    catch (const invalid_argument& ex) {
        ASSERT(ex.what() == string("Division by zero error: \"0/0\""))
    }

    try {
        CSVContainer csv("../resource/TestCalculation/recursiveCalculation.csv");
        CSVCalculator::Calculate(csv);
    }
    catch (const invalid_argument& ex) {
        ASSERT(ex.what() == string("Cell A1 has already been used but has not yet been counted"))
    }

    CSVContainer csv("../resource/TestCalculation/goodFile.csv");
    CSVCalculator::Calculate(csv);

    ASSERT(csv.GetCell("B", 2).value == 6)
    ASSERT(csv.GetCell("B", 30).value == 1)
    ASSERT(csv.GetCell("B", 1).value == 0)
    ASSERT(csv.GetCell("Cell", 30).value == 5)

    CSVContainer csvHeavy("../resource/TestCalculation/heavyCsv.csv");
    CSVCalculator::Calculate(csvHeavy);

    ASSERT(csvHeavy.GetCell("B", 1).value == -1)
    ASSERT(csvHeavy.GetCell("B", 3).value == 1)
    ASSERT(csvHeavy.GetCell("C", 3).value == 0)
    ASSERT(csvHeavy.GetCell("D", 1).value == 0)
    ASSERT(csvHeavy.GetCell("D", 2).value == 0)
    ASSERT(csvHeavy.GetCell("D", 3).value == 6)
}

void TestPrint() {
    CSVContainer csvPrint("../resource/TestPrint/testPrint.csv");
    CSVPrinter::SetUseOnlyDebugText(true);

    ASSERT(CSVPrinter::Print(csvPrint) == ",A,B,Cell\n"
                                          "1,1,0,-1\n"
                                          "2,2,=A1+Cell30,0\n"
                                          "30,0,=B1+A1,5\n")

    CSVCalculator::Calculate(csvPrint);

    ASSERT(CSVPrinter::Print(csvPrint) == ",A,B,Cell\n"
                                          "1,1,0,-1\n"
                                          "2,2,6,0\n"
                                          "30,0,1,5\n")

    CSVPrinter::SetUseOnlyDebugText(false);
}

#endif //NOKIA_TESTCSV_TEST_TEST_CASES_H
