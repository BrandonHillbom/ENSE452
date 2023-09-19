/*

Project: Assignment 1 - A Complex Number Calculator
Course: ENSE 452
Date: Sept 19, 2023
Name: Brandon Hillbom

Description: This program computes the mathematical result of adding, 
subtracting, multiplying, or dividing two complex numbers and outputs
the result in the form of a + j b.

*/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

struct Complex {
    double real; //real part of a complex number
    double imag; //imaginary part of a complex number
};

bool isWhitespaceOrEmpty(const string str);
int countArguments(istringstream &args);
Complex add(const Complex z1, const Complex z2);
Complex subtract(const Complex z1, const Complex z2);
Complex multiply(const Complex z1, const Complex z2);
Complex divide(const Complex z1, const Complex z2);
string formatNumber(const double number);
void print(const Complex z);

/*
    Main function: user is prompted to enter an arithmetic operator (A, S, M, D)
    followed by 2 sets of numbers where the first number is the first complex 
    real number, the second is the first complex imaginary number, the third 
    is the second complex real number and the fourth is the second complex 
    imaginary number. The output will be the calculated result. 
*/
int main () {
    string input;
    bool exit = false;
    bool error = false;

    cerr << "Type a letter to specify the arithmetic operator (A, S, M, D)" << endl;
    cerr << "followed by two complex numbers expressed as pairs of doubles." << endl;
    cerr << "Type q to quit." << endl;

    while (!exit) {
        // Go to next line if there was an error. Prof given test file doesn't 
        //account for end lines in the error output
        if (error) {
            cerr << endl;
            error = false;
        }
        cerr << "Enter exp: ";
        getline(cin, input);

         // Ignore empty or whitespace only lines
        if (isWhitespaceOrEmpty(input)) {
            continue;
        }

        istringstream stream(input);
        istringstream tempStream(input);

        char command;
        stream >> command;
        command = tolower(command);
        if (command == 'q') {
            //exit the program
            exit = true;
            continue;
        } else if (command != 'a' && command != 's' && command != 'm' 
                   && command != 'd') {
            cout << "error code: 1: illegal command";
            error = true;
            continue;
        }
        Complex z1, z2, result;
        int argumentCount = countArguments(tempStream);
        
        if ((argumentCount - 1) < 4) {
            cout << "error code: 2: missing arguments";
            error = true;
            continue;
        }
        if ((argumentCount - 1) > 4) {
            cout << "error code: 3: extra arguments";
            error = true;
            continue;
        }

        //write the string stream to new string variables
        string z1Real, z2Real, z1Imag, z2Imag;
        stream >> z1Real >> z1Imag >> z2Real >> z2Imag;
        
        try {
            //convert strings to doubles
            z1.real = stod(z1Real);
            z1.imag = stod(z1Imag);
            z2.real = stod(z2Real);
            z2.imag = stod(z2Imag);
        } catch (const exception e) {
            cout << "error code: 5: failed to convert string to double";
            continue;
        }

        switch (command) {
            case 'a':
                result = add(z1, z2);
                break;
            case 's':
                result = subtract(z1, z2);
                break;
            case 'm': 
                result = multiply(z1, z2);
                break;
            case 'd': 
               if (z2.real == 0.0 && z2.imag == 0.0) {
                    cout << "error code: 4: divide by zero";
                    error = true;
                    continue;
                }
                result = divide(z1, z2);
                break;
            default:
                cout << "error code: 1: illegal command";
                error = true;
        }
        print(result);
    }
    return 0;
}

/*
    Return the number of separate input values to determine 
    if there are missing or extra arguments
*/
int countArguments(istringstream &arguments) {
    int count = 0;
    string value;
    while (arguments >> value) {
        count++;
    }
    
    return count;
}

/*
    Add two complex numbers
*/
Complex add(const Complex z1, const Complex z2) {
    Complex result;
    result.real = z1.real + z2.real;
    result.imag = z1.imag + z2.imag;
    return result;
}

/*
    Subtract two complex numbers
*/
Complex subtract(const Complex z1, const Complex z2) {
    Complex result;
    result.real = z1.real - z2.real;
    result.imag = z1.imag - z2.imag;
    return result;
}

/*
    Multiply two complex numbers
*/
Complex multiply(const Complex z1, const Complex z2) {
    Complex result;
    result.real = (z1.real * z2.real) - (z1.imag * z2.imag);
    result.imag = (z1.real * z2.imag) + (z1.imag * z2.real);
    return result;
}

/*
    Divide two complex numbers
*/
Complex divide(const Complex z1, const Complex z2) {
    Complex result;
    double denominator = (z2.real * z2.real) + (z2.imag * z2.imag);
    result.real = ((z1.real * z2.real) + (z1.imag * z2.imag)) / denominator;
    result.imag = ((z1.imag * z2.real) - (z1.real * z2.imag)) / denominator;
    return result;
}

/*
    Return true if the input is null data (empty line)
*/
bool isWhitespaceOrEmpty(const string str) {
    for (int i = 0; i < str.length(); i++) {
        if (!isspace(str[i])) {
            return false;
        }
    }
    return true;
}

/*
    Format the number to display without trailing zeroes
*/
string formatNumber(const double number) {
    ostringstream ss;
    ss << fixed << setprecision(6) << number; // arbitrary initial precision
    string formatted = ss.str(); //convert to string

    // Check if the number is a whole number
    if (abs(number) - abs(number) == 0.0) {
        // Position of the decimal point
        size_t decimalPos = formatted.find('.');
        if (decimalPos != string::npos) {
            // Remove decimal point following sig fig rules
            formatted.erase(formatted.find_last_not_of('0') + 1, std::string::npos);
            if (formatted.back() == '.') {
                formatted.pop_back();
            }
        }
    }

    return formatted;
}

/*
    Print the calculated result in the form a + b j
*/
void print(const Complex z) {
    const string sign = z.imag >= 0 ? "+" : "-";
  string realStr = formatNumber(z.real);
    string imagStr = formatNumber(abs(z.imag));
    cout << fixed << realStr << " " << sign << " j " << imagStr << endl;
}
