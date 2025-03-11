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

     // --- Division Operators ---
    // In-place division: this = this / b (quotient)
    BigInt &operator/=(const BigInt &b) {
        if (b.isNull())
            throw invalid_argument("Arithmetic Error: Division By 0");

        if (*this < b) {
            *this = BigInt();
            return *this;
        }

        if (*this == b) {
            *this = BigInt(1);
            return *this;
        }

        int n = digits.size(), m = b.length();
        vector<int> quotient(n, 0);  // Temporary storage for quotient digits
        BigInt t;  // Temporary BigInt for holding parts of the dividend

        int pos; // 'pos' will be used as our digit index
        // Find the first position where t * 10 + next_digit >= b.
        for (pos = n - 1; pos >= 0 && (t * 10 + digits[pos]) < b; pos--) {
            t *= 10;
            t += BigInt((unsigned long long)digits[pos]);  // Add the current digit
        }

        int qIndex = 0;  // To count the number of quotient digits
        // Now process the remaining digits
        for (; pos >= 0; pos--) {
            t = t * 10 + BigInt((unsigned long long)digits[pos]);
            int count;
            // Find the largest count such that (count * b) <= t
            for (count = 9; (BigInt((unsigned long long)count) * b) > t; count--);
            t -= BigInt((unsigned long long)count) * b;
            quotient[qIndex++] = count;
        }
        // The quotient digits are currently in reverse order.
        digits.resize(quotient.size());
        for (int i = 0; i < qIndex; i++) {
            digits[i] = quotient[qIndex - i - 1];
        }
        digits.resize(qIndex);
        return *this;
    }

    // Returns a new BigInt that is the quotient of this / b.
    BigInt operator/(const BigInt &b) const {
        BigInt temp(*this);
        temp /= b;
        return temp;
    }

    // --- Modulus Operators ---
    // In-place modulus: this = this % b (remainder)
    BigInt &operator%=(const BigInt &b) {
        if (b.isNull())
            throw invalid_argument("Arithmetic Error: Division By 0");

        if (*this < b)
            return *this;  // Remainder is 'this' if 'this' is smaller than b

        if (*this == b) {
            *this = BigInt();
            return *this;
        }

        int n = digits.size(), m = b.length();
        vector<int> dummy(n, 0);  // Unused here except for size; same as in division
        BigInt t;  // Temporary BigInt to hold division steps

        int pos;
        for (pos = n - 1; pos >= 0 && (t * 10 + digits[pos]) < b; pos--) {
            t *= 10;
            t += BigInt((unsigned long long)digits[pos]);
        }
        for (; pos >= 0; pos--) {
            t = t * 10 + BigInt((unsigned long long)digits[pos]);
            int count;
            for (count = 9; (BigInt((unsigned long long)count) * b) > t; count--);
            t -= BigInt((unsigned long long)count) * b;
            // The actual quotient is not used here since we only want the remainder.
        }
        *this = t;
        return *this;
    }

    // Returns a new BigInt that is the remainder when this is divided by b.
    BigInt operator%(const BigInt &b) const {
        BigInt temp(*this);
        temp %= b;
        return temp;
    }

    // --- Exponentiation Operators ---
    // In-place exponentiation: this = this ^ b
    // Uses exponentiation by squaring.
    BigInt &operator^=(const BigInt &b) {
        BigInt Exponent(b);    // Copy the exponent
        BigInt Base(*this);    // Copy the base
        *this = BigInt(1);     // Initialize result as 1

        // While the exponent is not 0
        while (!Exponent.isNull()) {
            // If the least significant digit is odd, multiply result by Base.
            if (Exponent[0] & 1)
                *this *= Base;
            Base *= Base;  // Square the base
            divide_by_2(Exponent);  // Divide the exponent by 2 (see helper function below)
        }
        return *this;
    }

    // Returns a new BigInt that is the result of this raised to the power of b.
    BigInt operator^(const BigInt &b) const {
        BigInt temp(*this);
        temp ^= b;
        return temp;
    }

    // --- Helper: Divide a BigInt by 2 (used in exponentiation) ---
    // This function divides a BigInt by 2, modifying it in-place.
    void divide_by_2(BigInt &num) {
        int carry = 0;
        for (int i = num.digits.size() - 1; i >= 0; i--) {
            int current = num.digits[i] + carry * 10;
            num.digits[i] = current / 2;
            carry = current % 2;
        }
        // Remove any leading zero if present
        while (num.digits.size() > 1 && num.digits.back() == 0)
            num.digits.pop_back();
    }

    // --- Input/Output Operators (as friend functions) ---
    // We still define these as friend functions since ostream and istream are not part of BigInt.
    friend ostream &operator<<(ostream &out, const BigInt &a) {
        // Print digits in reverse order to display the number correctly.
        for (int i = a.digits.size() - 1; i >= 0; i--)
            out << char(a.digits[i] + '0');
        return out;
    }

    friend istream &operator>>(istream &in, BigInt &a) {
        string s;
        in >> s;
        a = BigInt(s);
        return in;
    }

};

int main(){
    // Test case 1: Basic addition
    BigInt num1;
    BigInt num2;
    cout << "Enter first number: ";
    cin>>num1;
    
    cout << "Enter Second number: ";
    cin>>num2;

    BigInt sum = num1 + num2;
    
    cout << "Sum: "<<sum<<endl;
    

    // Test case 2: Basic subtraction
    BigInt diff = num1 - num2;
    cout << "Difference: "<<diff<<endl;

    // Test case 3: Multiplication
    BigInt product = num1 * num2;
    std::cout << "Product: "<<product<<endl;
    

    // Test case 4: Division
    BigInt quotient = num1 / num2;
    std::cout << "Quotient: "<<quotient<<endl;
    

    // Test case 5: Modulus (Remainder)
    BigInt remainder = num1 % num2;
    std::cout << "Remainder: "<<remainder<<endl;
    
}