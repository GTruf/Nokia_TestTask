#include "csv_container.h"
#include <filesystem>
#include <unordered_set>

using namespace std;

CSVContainer::CSVContainer(const std::string& filePath) {
    ProcessFile(filePath);
}

const CSVCell& CSVContainer::GetCell(const string& columnHeader, size_t rowHeader) const {
    try {
        CSVCellHeaderStr csvCellHeaderStr = {rowHeader, columnHeader};
        auto [row, column] = _cellHeaderStrIndices.at(csvCellHeaderStr);
        return _cells.at(row).at(column);
    }
    catch (const exception& ex) {
        throw invalid_argument("Unknown " + columnHeader + to_string(rowHeader) + " cell");
    }
}

CSVCell& CSVContainer::RawCell(const std::string& columnHeader, size_t rowHeader) {
    try {
        CSVCellHeaderStr csvCellHeaderStr = {rowHeader, columnHeader};
        auto [row, column] = _cellHeaderStrIndices.at(csvCellHeaderStr);
        return _cells.at(row).at(column);
    }
    catch (const exception& ex) {
        throw invalid_argument("Unknown " + columnHeader + to_string(rowHeader) + " cell");
    }
}

void CSVContainer::ProcessFile(const std::string& filePath) {
    ifstream csvFile(filePath);

    CheckFileForErrors(csvFile, filePath);
    ExtractColumnHeaders(csvFile);
    FeelCells(csvFile);
}

void CSVContainer::FeelCells(ifstream& csvFile) {
    string tmp;
    unordered_set<string> rowHeadersTmp;

    for (size_t i = 1; getline(csvFile, tmp); i++) {
        _cells.emplace_back();
        istringstream iss(move(tmp));
        string rowHeader;

        getline(iss, rowHeader, ','); // Первая ячейка каждой строки (заголовок)
        CheckCellForFormatErrors(TrimLeft(rowHeader), CellType::RowHeader);

        if (rowHeadersTmp.contains(rowHeader))
            throw runtime_error("Error, row \"" + rowHeader + "\" already exists");

        rowHeadersTmp.insert(rowHeader);
        CSVCell rowHeaderCell;
        rowHeaderCell.expression = rowHeader;
        _cells[i].push_back(move(rowHeaderCell));

        for (size_t j = 1; getline(iss, tmp, ','); j++) {
            const auto tmpWithoutSpaces = WithoutSpaces(tmp);
            CheckCellForFormatErrors(tmpWithoutSpaces);

            // Идея в том, чтобы иметь возможность отображать строковые заголовки в физические индексы, т.е. к примеру ["B", 30] -> [3][2] (из примера в ТЗ)
            CSVCellHeaderStr csvCellHeaderStr = {stoul(rowHeader), _cells[0][j].expression}; // Сохраняем заголовки строки и столбца соответственно
            CSVCellHeaderIndex csvCellHeaderIndex = {i, j}; // Сохраняем "индексное" представление заголовков (физические индексы в векторе векторов ячеек)

            _cellHeaderStrIndices[move(csvCellHeaderStr)] = csvCellHeaderIndex;
            CSVCell cell;
            cell.columnHeader = _cells[0][j].expression;
            cell.rowHeader = stoul(rowHeader);

            if (IsInteger(tmpWithoutSpaces))
                cell.value = stoi(tmp);
            else
                cell.expression = move(tmp);

            _cells[i].push_back(move(cell));
        }
    }
}

void CSVContainer::ExtractColumnHeaders(ifstream& csvFile) {
    string tmp;
    unordered_set<string> columnHeadersTmp;

    getline(csvFile, tmp); // Первая строка в файле (заголовки столбцов)

    istringstream iss(move(tmp));
    _cells.emplace_back();

    while (getline(iss, tmp, ',')) {
        CheckCellForFormatErrors(TrimLeft(tmp), CellType::ColumnHeader);

        if (columnHeadersTmp.contains(tmp))
            throw runtime_error("Error, column \"" + tmp + "\" already exists");

        columnHeadersTmp.insert(tmp);
        CSVCell columnHeaderCell;
        columnHeaderCell.expression = move(tmp);
        _cells[0].push_back(move(columnHeaderCell));
    }
}

void CSVContainer::CheckCellForFormatErrors(const string& cellExpression, CellType cellType) {
    if (cellType == CellType::ColumnHeader) {
        if (_cells[0].empty()) {
            if (!cellExpression.empty())
                throw runtime_error("Error, first cell must be empty: " + cellExpression);
            return;
        }

        if (!IsJustWord(cellExpression))
            throw runtime_error("Error, column headers must be just a word: " + cellExpression);

    }
    else if (cellType == CellType::RowHeader) {
        if (!IsNonNegativeInteger(cellExpression))
            throw runtime_error("Error, row headers must be non-negative numbers: " + cellExpression);
    }
    else {
        const auto isNumber = IsInteger(cellExpression);

        if (cellExpression.empty())
            throw runtime_error("Error, some cell other than the first is empty");

        if (!isNumber && cellExpression[0] != '=')
            throw runtime_error("Error, formulas must start with \"=\": " + cellExpression);

        if (!isNumber)
            CheckForPatternMatch(cellExpression);
    }
}

void CSVContainer::CheckForPatternMatch(const std::string& cellExpression) {
    const auto opPos = cellExpression.find_first_of("+-*/");
    const auto left = cellExpression.substr(1, opPos - 1);
    const auto right = cellExpression.substr(opPos + 1);

    if (opPos != string::npos) {
        if (left.find_first_of("+-*/") != string::npos || right.find_first_of("+-*/") != string::npos ||
            left.empty() || right.empty() || !IsCellName(left) && !IsInteger(left) || !IsCellName(right) && !IsInteger(right))
        {
            throw runtime_error("Error, cell must match the pattern \"only number or ARG1 OP ARG2\": " + cellExpression);
        }
    }
    else {
        throw runtime_error("Error, cell must match the pattern \"only number or ARG1 OP ARG2\": " + cellExpression);
    }
}

void CSVContainer::CheckFileForErrors(ifstream& csvFile, const string& filePath) {
    if (!csvFile)
        throw runtime_error("Error opening file \"" + filePath + "\"");

    if (filesystem::path(filePath).extension() != ".csv")
        throw runtime_error("Error, \"" + filePath + "\" file extension does not match csv");

    if (filesystem::is_empty(filePath))
        throw runtime_error("Error, file \"" + filePath + "\" is empty");
}