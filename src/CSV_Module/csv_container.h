#ifndef NOKIA_TESTCSV_CSV_CONTAINER_H
#define NOKIA_TESTCSV_CSV_CONTAINER_H

#include "../Utilities/auxiliary_functionality.h"
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

struct CSVCell {
    std::string columnHeader;
    std::string expression;
    size_t rowHeader;
    std::optional<ssize_t> value;
    bool isVisited = false;

    std::string Name() const {
        return columnHeader + std::to_string(rowHeader);
    }
};
#include <iostream>
class CSVContainer {
public:
    explicit CSVContainer(const std::string& filePath);
    const CSVCell& GetCell(const std::string& columnHeader, size_t rowHeader) const;
    CSVCell& RawCell(const std::string& columnHeader, size_t rowHeader);

    friend class CSVCalculator;
    friend class CSVPrinter;

private:
    enum class CellType {
        ColumnHeader,
        RowHeader,
        Common
    };

    struct CSVCellHeaderStr {
        size_t rowHeader;
        std::string columnHeader;

        size_t operator()(const CSVCellHeaderStr& csvCellHeaderStr) const {
            return std::hash<size_t>()(csvCellHeaderStr.rowHeader) ^ std::hash<std::string>()(csvCellHeaderStr.columnHeader);
        }

        bool operator==(const CSVCellHeaderStr& other) const {
            return std::tie(rowHeader, columnHeader) == std::tie(other.rowHeader, other.columnHeader);
        }
    };

    using Content = std::vector<std::vector<CSVCell>>;
    using CSVCellHeaderIndex = std::pair<size_t, size_t>;
    using CellHeaderStrIndices = std::unordered_map<CSVCellHeaderStr, CSVCellHeaderIndex, CSVCellHeaderStr>;

    Content _cells;
    CellHeaderStrIndices _cellHeaderStrIndices;

    void ProcessFile(const std::string& filePath);
    void FeelCells(std::ifstream& csvFile);
    void ExtractColumnHeaders(std::ifstream& csvFile);
    void CheckCellForFormatErrors(const std::string& cellExpression, CellType cellType = CellType::Common);
    static void CheckForPatternMatch(const std::string& cellExpression);
    static void CheckFileForErrors(std::ifstream& csvFile, const std::string& filePath);
};

#endif //NOKIA_TESTCSV_CSV_CONTAINER_H
