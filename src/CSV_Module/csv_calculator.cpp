#include "csv_calculator.h"
#include "../Utilities/auxiliary_functionality.h"

using namespace std;

void CSVCalculator::Calculate(CSVContainer& csvContainer) {
    for (size_t i = 1; i < csvContainer._cells.size(); i++) {
        for (size_t j = 1; j < csvContainer._cells[i].size(); j++) {
            auto& cell = csvContainer._cells[i][j];

            if (cell.value.has_value())
                continue;

            CalculateCellValue(cell, csvContainer);
        }
    }
}

void CSVCalculator::CalculateCellValue(CSVCell& cell, CSVContainer& csvContainer) {
    if (cell.isVisited)
        throw invalid_argument("Cell " + cell.columnHeader + to_string(cell.rowHeader) + " has already been used but has not yet been counted");

    cell.isVisited = true;

    const auto cellExprWithoutSpaces = WithoutSpaces(cell.expression);

    const auto opPos = cellExprWithoutSpaces.find_first_of("+-*/");
    const auto leftStr = cellExprWithoutSpaces.substr(1, opPos - 1); // С 1 индекса, чтобы не брать "=" из формулы
    const auto rightStr = cellExprWithoutSpaces.substr(opPos + 1);

    const auto leftValue = CalculateOperand(leftStr, csvContainer);
    const auto rightValue = CalculateOperand(rightStr, csvContainer);

    cell.value = ApplyOperator(leftValue, rightValue, cellExprWithoutSpaces[opPos]);
    cell.isVisited = false;
}

ssize_t CSVCalculator::CalculateOperand(const string& operand, CSVContainer& csvContainer) {
    if (IsInteger(operand)) {
        return stoi(operand);
    }
    else {
        auto [columnHeader, lineHeader] = ParseCellReference(operand);
        auto& cell = csvContainer.RawCell(columnHeader, lineHeader);

        if (cell.value.has_value())
            return *cell.value;

        CalculateCellValue(cell, csvContainer);
        return *cell.value;
    }
}

ssize_t CSVCalculator::ApplyOperator(ssize_t left, ssize_t right, char op) {
    switch (op) {
        case '+': return left + right;
        case '-': return left - right;
        case '*': return left * right;
        case '/':
            if (right == 0)
                throw invalid_argument("Division by zero error: \"" + to_string(left) + op + to_string(right) + "\"");
            return left / right;
        default: throw invalid_argument("ApplyOperator error. Expression is \"" + to_string(left) + op + to_string(right) + "\"");
    }
}

pair<string, ssize_t> CSVCalculator::ParseCellReference(const string& cellReference) {
    auto numPos = cellReference.find_first_of("0123456789");
    return {cellReference.substr(0, numPos), stoi(cellReference.substr(numPos))};
}
