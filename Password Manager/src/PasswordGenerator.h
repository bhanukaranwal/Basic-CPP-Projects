#pragma once
#include <QString>

class PasswordGenerator {
public:
    static QString generate(int length = 16, bool useSymbols = true);
};
