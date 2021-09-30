#ifndef NOKIA_TESTCSV_CSV_CALCULATOR_H
#define NOKIA_TESTCSV_CSV_CALCULATOR_H

#include "csv_container.h"

class CSVCalculator {
public:
    static void Calculate(CSVContainer& csvContainer);

private:
    static void CalculateCellValue(CSVCell& cell, CSVContainer& csvContainer);
    static ssize_t CalculateOperand(const std::string& operand, CSVContainer& csvContainer);
    static ssize_t ApplyOperator(ssize_t left, ssize_t right, char op);
    static std::pair<std::string, ssize_t> ParseCellReference(const std::string& cellReference);
};

#endif //NOKIA_TESTCSV_CSV_CALCULATOR_H
