#ifndef NOKIA_TESTCSV_CSV_PRINTER_H
#define NOKIA_TESTCSV_CSV_PRINTER_H

#include "csv_container.h"

class CSVPrinter {
public:
    enum PrintType {
        Common, // Если есть значение в ячейке - выведет его, иначе исходное выражение ячейки
        Raw     // Выводит "сырые" (исходные) данные, которые были изначально в файле (формулы и т.д.)
    };

    static std::string Print(const CSVContainer& csvContainer, PrintType printType = Common, const std::string& message = "");
    static void SetUseOnlyDebugText(bool enabled);

private:
    static bool _isUseDebugText;
};

#endif //NOKIA_TESTCSV_CSV_PRINTER_H
