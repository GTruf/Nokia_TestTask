#include "auxiliary_functionality.h"

using namespace std;

bool IsInteger(const std::string& expression) {
    if (!expression.empty()) {
        size_t i = 0;

        auto first = expression[i];
        if (first == '-')
            i++;

        for (; i < expression.size(); i++) {
            if (!isdigit(expression[i]))
                return false;
        }

        return true;
    }

    return false;
}

bool IsNonNegativeInteger(const string& expression) {
    if (!expression.empty()) {
        if (expression[0] == '-')
            return false;

        for (const auto& ch : expression) {
            if (!isdigit(ch))
                return false;
        }

        return true;
    }

    return false;
}

bool IsJustWord(const string& expression) {
    if (!expression.empty()) {
        for (const auto& ch : expression) {
            if (!isalpha(ch))
                return false;
        }

        return true;
    }

    return false;
}

bool IsCellName(const string& expression) {
    if (!expression.empty()) {
        if (!isalpha(expression[0]))
            return false;

        bool isCheckForAlpha = true;

        for (const auto& ch : expression) {
            if (isCheckForAlpha) {
                if (!isalpha(ch)) {
                    if (!isdigit(ch)) {
                        return false;
                    }
                    isCheckForAlpha = false;
                }
            }
            else {
                if (!isdigit(ch)) {
                    return false;
                }
            }
        }

        if (isCheckForAlpha)
            return false;

        return true;
    }

    return false;
}

std::string WithoutSpaces(std::string someString) {
    someString.erase(remove(someString.begin(), someString.end(), ' '), someString.end());
    return someString;
}

std::string TrimLeft(const std::string& someString) {
    size_t start = someString.find_first_not_of(" \t");
    return (start == string::npos) ? "" : someString.substr(start);
}
