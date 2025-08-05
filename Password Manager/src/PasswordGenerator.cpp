#include "PasswordGenerator.h"
#include <random>

QString PasswordGenerator::generate(int length, bool useSymbols) {
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    const QString symbols = "!@#$%^&*()-_=+[]{};:,.<>/?";
    QString candidate = chars + (useSymbols ? symbols : "");
    QString result;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, candidate.size() - 1);
    for (int i = 0; i < length; ++i)
        result += candidate[distrib(gen)];
    return result;
}
