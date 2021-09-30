#include "csv_printer.h"
#include <iostream>

using namespace std;

bool CSVPrinter::_isUseDebugText = false;

std::string CSVPrinter::Print(const CSVContainer& csvContainer, PrintType printType, const string& message) {
    if (!message.empty())
        cout << message << endl;

    string debugText;

    for (const auto& csvRow : csvContainer._cells) {
        bool isFirst = true;

        for (const auto& cell : csvRow) {
            if (!isFirst) {
                if (_isUseDebugText)
                    debugText.push_back(',');
                else
                    cout << ",";
            }
            isFirst = false;

            if (printType == Common) {
                if (_isUseDebugText)
                    debugText.append(cell.value.has_value() ? to_string(*cell.value) : cell.expression);
                else
                    cout << (cell.value.has_value() ? to_string(*cell.value) : cell.expression);
            }
            else {
                if (_isUseDebugText)
                    debugText.append(cell.expression);
                else
                    cout << cell.expression;
            }
        }

        if (_isUseDebugText)
            debugText.push_back('\n');
        else
            cout << "\n";
    }

    return debugText;
}

void CSVPrinter::SetUseOnlyDebugText(bool enabled) {
    _isUseDebugText = enabled;
}
