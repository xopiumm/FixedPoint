#pragma once

#include <cstdint>
#include <iostream>
#include <string>

struct FixedFormat {
    uint8_t integer;
    uint8_t fraction;

    FixedFormat();
};

FixedFormat::FixedFormat() : integer(0), fraction(0) {}

class FixedPoint {
private:
    int32_t number_;
    FixedFormat format_;

public:
    FixedPoint(int32_t number, FixedFormat format);
    ~FixedPoint() = default;

    int32_t GetNumber() const;
    FixedFormat GetFormat() const;
    bool IsNegative() const;
    FixedPoint operator+(const FixedPoint& other) const;
    FixedPoint operator-(const FixedPoint& other) const;
    FixedPoint operator*(const FixedPoint& other) const;
    FixedPoint operator/(const FixedPoint& other) const;
    friend std::ostream& operator<<(std::ostream& out, const FixedPoint& number);
};

FixedPoint::FixedPoint(int32_t number, FixedFormat format) : number_(number), format_(format) {}

int32_t FixedPoint::GetNumber() const {
    return number_;
}

FixedFormat FixedPoint::GetFormat() const  {
    return format_;
}

bool FixedPoint::IsNegative() const {
    uint32_t mask = static_cast<uint32_t>(1) << (format_.integer + format_.fraction - 1);
    if (format_.integer + format_.fraction != 32) {
        if (std::abs(number_) & mask) {
            return true;
        }
    } else {
        if (number_ & mask) {
            return true;
        }
    }

    return false;
}

FixedPoint FixedPoint::operator+(const FixedPoint& other) const {
    FixedPoint result(0, format_);
    result.number_ = static_cast<int32_t>(static_cast<int64_t>(number_) + static_cast<int64_t>(other.number_));

    return result;
}

FixedPoint FixedPoint::operator-(const FixedPoint& other) const {
    FixedPoint result(0, format_);
    result.number_ = static_cast<int32_t>(static_cast<int64_t>(number_) - static_cast<int64_t>(other.number_));

    return result;
}

FixedPoint FixedPoint::operator*(const FixedPoint& other) const {
    FixedPoint result(0, format_);

    int64_t temp_result = static_cast<int64_t>(number_) * static_cast<int64_t>(other.number_);
    result.number_ = static_cast<int32_t>(temp_result >> format_.fraction);

    return result;
}

FixedPoint FixedPoint::operator/(const FixedPoint& other) const {
    FixedPoint result(0, format_);

    if (other.number_ == 0) {
        std::cout << "division by zero" << std::endl;
        exit(0);
    }

    int64_t temp_result = (static_cast<int64_t>(number_) << format_.fraction) / static_cast<int64_t>(other.number_);
    result.number_ = static_cast<int32_t>(temp_result);

    return result;
}

std::ostream& operator<<(std::ostream& out, const FixedPoint& number) {
    int64_t output;
    int64_t data = static_cast<int64_t>(number.GetNumber());
    int64_t mask = (static_cast<int64_t>(1) << (number.GetFormat().integer + number.GetFormat().fraction)) - 1;

    if (number.IsNegative()) {
        int64_t temp_value = ~(data) + 1;
        temp_value &= mask;
        output = (temp_value * 1000) >> number.GetFormat().fraction;
        out << '-';
    } else {
        data &= mask;
        output = (data * 1000) >> number.GetFormat().fraction;
    }

    std::string str_numb = std::to_string(std::abs(output));

    while (str_numb.length() <= 3) {
        str_numb = '0' + str_numb;
    }
    out << str_numb.substr(0, str_numb.length() - 3) << '.' << str_numb.substr(str_numb.length() - 3, str_numb.length());

    return out;
}