#include <iostream>
#include <fstream>
#include <string>
#include "calculator.h"
using namespace std;

int main() {
    string filename;
    cout << "Enter file name: ";
    cin >> filename;

    ifstream file(filename);
    if (!file) {
        cout << "Error: Cannot open file.\n";
        return 1;
    }

    string num1, num2;
    while (file >> num1 >> num2) {
        if (!isValidDouble(num1)) {
            cout << "Invalid number: " << num1 << endl;
            continue;
        }
        if (!isValidDouble(num2)) {
            cout << "Invalid number: " << num2 << endl;
            continue;
        }

        string result = addStrings(num1, num2);
        cout << num1 << " + " << num2 << " = " << result << endl;
    }

    file.close();
    return 0;
}
