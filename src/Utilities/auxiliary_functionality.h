#ifndef NOKIA_TESTCSV_AUXILIARY_FUNCTIONALITY_H
#define NOKIA_TESTCSV_AUXILIARY_FUNCTIONALITY_H

#include <string>

bool IsInteger(const std::string& expression);
bool IsNonNegativeInteger(const std::string& expression);
bool IsJustWord(const std::string& expression);
bool IsCellName(const std::string& expression);
std::string WithoutSpaces(std::string someString);
std::string TrimLeft(const std::string& someString);

#endif //NOKIA_TESTCSV_AUXILIARY_FUNCTIONALITY_H
