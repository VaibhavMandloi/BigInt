#include <iostream>
#include <string>
#include <cstring>
#include <vector>
using namespace std;

class BigInt {
private:
    string digits;

public:
    // Constructors
    BigInt(unsigned long long n = 0) {
        do {
            digits.push_back(n % 10);
            n /= 10;
        } while (n);
    }

    BigInt(const string &s) {
        digits = "";
        for (int i = s.size() - 1; i >= 0; i--) {
            if (!isdigit(s[i]))
                throw("ERROR");
            digits.push_back(s[i] - '0');
        }
    }

    BigInt(const char *s) {
        digits = "";
        for (int i = strlen(s) - 1; i >= 0; i--) {
            if (!isdigit(s[i]))
                throw("ERROR");
            digits.push_back(s[i] - '0');
        }
    }

    BigInt(const BigInt &a) {
        digits = a.digits;
    }

    // Assignment Operator
    BigInt &operator=(const BigInt &a) {
        digits = a.digits;
        return *this;
    }

    // Helper Functions
    bool isNull() const {
        return digits.size() == 1 && digits[0] == 0;
    }

    int length() const {
        return digits.size();
    }

    int operator[](const int index) const {
        if (index < 0 || index >= digits.size())
            throw("ERROR");
        return digits[index];
    }

    // Comparison Operators
    bool operator==(const BigInt &b) const {
        return digits == b.digits;
    }

    bool operator!=(const BigInt &b) const {
        return !(*this == b);
    }

    bool operator<(const BigInt &b) const {
        int n = length(), m = b.length();
        if (n != m)
            return n < m;
        for (int i = n - 1; i >= 0; i--)
            if (digits[i] != b.digits[i])
                return digits[i] < b.digits[i];
        return false;
    }

    bool operator>(const BigInt &b) const {
        return b < *this;
    }

    bool operator>=(const BigInt &b) const {
        return !(*this < b);
    }

    bool operator<=(const BigInt &b) const {
        return !(*this > b);
    }

    // Increment and Decrement Operators
    BigInt &operator++() {
        int i, n = digits.size();
        for (i = 0; i < n && digits[i] == 9; i++)
            digits[i] = 0;
        if (i == n)
            digits.push_back(1);
        else
            digits[i]++;
        return *this;
    }

    BigInt operator++(int) {
        BigInt temp = *this;
        ++(*this);
        return temp;
    }

    BigInt &operator--() {
        if (digits[0] == 0 && digits.size() == 1)
            throw("UNDERFLOW");
        int i, n = digits.size();
        for (i = 0; digits[i] == 0 && i < n; i++)
            digits[i] = 9;
        digits[i]--;
        if (n > 1 && digits[n - 1] == 0)
            digits.pop_back();
        return *this;
    }

    BigInt operator--(int) {
        BigInt temp = *this;
        --(*this);
        return temp;
    }

    
    BigInt &operator+=(const BigInt &b) {
        int t = 0, s, i;
        int n = length(), m = b.length();

        if (m > n)
            digits.append(m - n, 0);

        n = length(); // Update length after appending zeros

        for (i = 0; i < n; i++) {
            if (i < m)
                s = (digits[i] + b.digits[i]) + t;
            else
                s = digits[i] + t;

            t = s / 10;
            digits[i] = (s % 10);
        }

        if (t)
            digits.push_back(t);

        return *this;
    }

    BigInt operator+(const BigInt &b) const {
        BigInt temp = *this;
        temp += b;
        return temp;
    }

    BigInt &operator-=(const BigInt &b) {
        if (*this < b)
            throw("UNDERFLOW");

        int n = length(), m = b.length();
        int i, t = 0, s;

        for (i = 0; i < n; i++) {
            if (i < m)
                s = digits[i] - b.digits[i] + t;
            else
                s = digits[i] + t;

            if (s < 0)
                s += 10, t = -1;
            else
                t = 0;

            digits[i] = s;
        }

        while (n > 1 && digits[n - 1] == 0)
            digits.pop_back(), n--;

        return *this;
    }

    BigInt operator-(const BigInt &b) const {
        BigInt temp = *this;
        temp -= b;
        return temp;
    }

    // --- Multiplication Operators ---
    // In-place multiplication: this = this * b
    BigInt &operator*=(const BigInt &b) {
        // If either operand is 0, result is 0.
        if (this->isNull() || b.isNull()) {
            *this = BigInt();
            return *this;
        }

        int n = digits.size(), m = b.digits.size();
        // Temporary vector to store multiplication result; size n + m is sufficient.
        vector<int> v(n + m, 0);

        // Multiply each digit of 'this' with each digit of 'b'
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                v[i + j] += digits[i] * b.digits[j];
            }
        }

        // Process the vector to handle carry over
        int totalDigits = n + m;
        int carry = 0;
        // Resize our internal digits string to accommodate the maximum possible size.
        digits.resize(totalDigits);
        for (int i = 0; i < totalDigits; i++) {
            int sum = v[i] + carry;
            v[i] = sum % 10;
            carry = sum / 10;
            digits[i] = v[i];
        }

        // Remove any extra leading zeros (most-significant digits) if present.
        for (int i = totalDigits - 1; i >= 1 && digits[i] == 0; i--) {
            digits.pop_back();
        }
        return *this;
    }

    // Returns a new BigInt that is the product of this and b.
    BigInt operator*(const BigInt &b) const {
        BigInt temp(*this);
        temp *= b;
        return temp;
    }

    

};

int main(){
    
}