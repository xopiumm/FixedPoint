#include "FixedPoint.h"

FixedFormat ParseFixedFormat(const char* form) {
    FixedFormat FixedFormat;

    while (*form != '.') {
        FixedFormat.integer = FixedFormat.integer * 10 + (*form - '0');
        ++form;
    }

    ++form;

    while (*form != '\0') {
        FixedFormat.fraction = FixedFormat.fraction * 10 + (*form - '0');
        ++form;
    }

    return FixedFormat;
}

int32_t ParseNumberFix(const char* numb, FixedFormat format) {
    int64_t result = 0;
    int32_t addition = 0;

    if (numb[0] != '0' || numb[1] != 'x') {
        std::cerr << "invalid number format" << std::endl;
        exit(1);
    }

    while (*numb != '\0') {
        if ((*numb < '0' || *numb > '9') && (*numb < 'a' || *numb > 'f') && (*numb < 'A' || *numb > 'F')) {
            std::cerr << "invalid number format" << std::endl;
            exit(1);
        }

        if (*numb == '0' && *(numb + 1) && *(numb + 1) == 'x') {
            numb += 2;
        } else if (*numb >= '0' && *numb <= '9') {
            addition = *numb - '0';
            result = result * 16 + addition;
            ++numb;
        } else if ((*numb >= 'a' && *numb <= 'f') || (*numb >= 'A' && *numb <= 'F')) {
            addition = ((*numb >= 'a') ? (*numb - 'a' + 10) : (*numb - 'A' + 10));
            result = result * 16 + addition;
            ++numb;
        }
    }

    uint64_t mask = (static_cast<int64_t>(1) << (format.integer + format.fraction)) - 1;
    result &= mask;

    return static_cast<int32_t>(result);
}


int main(int argc, char* argv[]) {
    if (argc != 4 && argc != 6) {
        std::cerr << "invalid arguments" << std::endl;
        exit(1);
    }

    if (*argv[2] != '0') {
        std::cerr << "this rounding is not supported" << std::endl;
        exit(1);
    }

    FixedFormat form = ParseFixedFormat(argv[1]);
    int32_t numb_first = ParseNumberFix(argv[3], form);
    FixedPoint first(numb_first, form);
    char operation = *argv[4];
    int32_t numb_second = ParseNumberFix(argv[5], form);
    FixedPoint second(numb_second, form);

    switch (operation) {
        case '+' :
            std::cout << first + second << std::endl;
            exit(0);
        case '-' :
            std::cout << first - second << std::endl;
            exit(0);
        case '*':
            std::cout << first * second << std::endl;
            exit(0);
        case '/' :
            std::cout << first / second << std::endl;
            exit(0);
    }

    exit(0);
}
