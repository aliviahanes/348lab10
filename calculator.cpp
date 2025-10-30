#include "calculator.h"
#include <algorithm>
#include <cctype>
using namespace std;

// ----------------------------------------------------
// Validate whether string is a valid double number
// ----------------------------------------------------
bool isValidDouble(const string &s) {
    if (s.empty()) return false;

    int i = 0, n = s.size();
    if (s[i] == '+' || s[i] == '-') i++;
    if (i == n) return false;

    bool hasDot = false;
    bool hasDigitsBefore = false, hasDigitsAfter = false;

    for (; i < n; i++) {
        if (isdigit(s[i])) {
            if (hasDot) hasDigitsAfter = true;
            else hasDigitsBefore = true;
        } else if (s[i] == '.') {
            if (hasDot) return false; // multiple dots
            hasDot = true;
        } else {
            return false;
        }
    }

    if (hasDot) return (hasDigitsBefore && hasDigitsAfter);
    return hasDigitsBefore;
}

// ----------------------------------------------------
// Helper functions
// ----------------------------------------------------
string stripLeadingZeros(const string &s) {
    int i = 0;
    while (i + 1 < (int)s.size() && s[i] == '0') i++;
    return s.substr(i);
}

string stripTrailingZeros(const string &s) {
    if (s.find('.') == string::npos) return s;
    string res = s;
    while (!res.empty() && res.back() == '0') res.pop_back();
    if (!res.empty() && res.back() == '.') res.pop_back();
    return res.empty() ? "0" : res;
}

void splitNumber(const string &num, string &intPart, string &fracPart) {
    size_t dotPos = num.find('.');
    if (dotPos == string::npos) {
        intPart = num;
        fracPart = "";
    } else {
        intPart = num.substr(0, dotPos);
        fracPart = num.substr(dotPos + 1);
    }
}

string addPositiveIntegers(string a, string b) {
    string result = "";
    int carry = 0;

    if (a.size() < b.size()) swap(a, b);
    while (b.size() < a.size()) b = "0" + b;

    for (int i = (int)a.size() - 1; i >= 0; i--) {
        int sum = (a[i] - '0') + (b[i] - '0') + carry;
        carry = sum / 10;
        result.push_back((sum % 10) + '0');
    }
    if (carry) result.push_back(carry + '0');
    reverse(result.begin(), result.end());
    return result;
}

string addFractionalParts(string &fracA, string &fracB, int &carry) {
    string result = "";
    int len = max(fracA.size(), fracB.size());
    while ((int)fracA.size() < len) fracA += "0";
    while ((int)fracB.size() < len) fracB += "0";

    for (int i = len - 1; i >= 0; i--) {
        int sum = (fracA[i] - '0') + (fracB[i] - '0') + carry;
        carry = sum / 10;
        result.push_back((sum % 10) + '0');
    }
    reverse(result.begin(), result.end());
    return result;
}

// Compare two positive numbers (ignoring signs)
int comparePositive(const string &a, const string &b) {
    string intA, fracA, intB, fracB;
    splitNumber(a, intA, fracA);
    splitNumber(b, intB, fracB);

    intA = stripLeadingZeros(intA);
    intB = stripLeadingZeros(intB);

    if (intA.size() != intB.size())
        return intA.size() > intB.size() ? 1 : -1;

    if (intA != intB)
        return intA > intB ? 1 : -1;

    // Equal integer parts — compare fractional parts
    while (fracA.size() < fracB.size()) fracA += "0";
    while (fracB.size() < fracA.size()) fracB += "0";

    if (fracA == fracB) return 0;
    return fracA > fracB ? 1 : -1;
}

// Subtract positive numbers: assumes a >= b
string subtractPositive(string a, string b) {
    string intA, fracA, intB, fracB;
    splitNumber(a, intA, fracA);
    splitNumber(b, intB, fracB);

    // Pad fractional parts
    int maxFrac = max(fracA.size(), fracB.size());
    while ((int)fracA.size() < maxFrac) fracA += "0";
    while ((int)fracB.size() < maxFrac) fracB += "0";

    // Subtract fractional parts
    string fracResult = "";
    int borrow = 0;
    for (int i = maxFrac - 1; i >= 0; i--) {
        int diff = (fracA[i] - '0') - (fracB[i] - '0') - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else borrow = 0;
        fracResult.push_back(diff + '0');
    }
    reverse(fracResult.begin(), fracResult.end());

    // Equalize integer lengths
    while (intB.size() < intA.size()) intB = "0" + intB;

    // Subtract integer parts
    string intResult = "";
    for (int i = (int)intA.size() - 1; i >= 0; i--) {
        int diff = (intA[i] - '0') - (intB[i] - '0') - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else borrow = 0;
        intResult.push_back(diff + '0');
    }
    reverse(intResult.begin(), intResult.end());

    string result = stripLeadingZeros(intResult);
    result = result.empty() ? "0" : result;

    if (!fracResult.empty())
        result += "." + stripTrailingZeros(fracResult);

    return stripTrailingZeros(result);
}

// ----------------------------------------------------
// Add two valid double strings (supports + and -)
// ----------------------------------------------------
string addStrings(const string &a, const string &b) {
    bool negA = false, negB = false;
    string A = a, B = b;

    if (A[0] == '+' || A[0] == '-') {
        if (A[0] == '-') negA = true;
        A = A.substr(1);
    }
    if (B[0] == '+' || B[0] == '-') {
        if (B[0] == '-') negB = true;
        B = B.substr(1);
    }

    string result;

    if (negA == negB) {
        // Same sign: perform addition
        string intA, fracA, intB, fracB;
        splitNumber(A, intA, fracA);
        splitNumber(B, intB, fracB);

        int carry = 0;
        string fracSum = addFractionalParts(fracA, fracB, carry);
        string intSum = addPositiveIntegers(intA, intB);
        if (carry) intSum = addPositiveIntegers(intSum, "1");

        result = intSum;
        if (!fracSum.empty()) result += "." + fracSum;

        result = stripLeadingZeros(result);
        result = stripTrailingZeros(result);

        if (negA) result = "-" + result;
    } else {
        // Different signs: perform subtraction
        int cmp = comparePositive(A, B);

        if (cmp == 0) return "0"; // a == b

        bool resultNeg = false;
        string larger = A, smaller = B;
        if (cmp < 0) { // |A| < |B|
            swap(larger, smaller);
            resultNeg = negB; // result takes sign of larger
        } else {
            resultNeg = negA;
        }

        result = subtractPositive(larger, smaller);
        result = stripLeadingZeros(result);
        result = stripTrailingZeros(result);

        if (result == "0") return "0";
        if (resultNeg) result = "-" + result;
    }

    return result;
}
