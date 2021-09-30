#include "CSV_Module/csv_module.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    setlocale(LC_ALL, "RUS");

    if (argc < 2) {
        cerr << "Error, the file name must be passed to the command line" << endl;
        getchar();
        return 1;
    }

    try {
        CSVContainer csv(argv[1]);
        CSVCalculator::Calculate(csv);
        CSVPrinter::Print(csv);
    }
    catch (const exception& errorMessage) {
        cerr << errorMessage.what() << endl;
        getchar();
        return 1;
    }

    getchar();
    return 0;
}
