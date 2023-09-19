Project: Assignment 1 - A Complex Number Calculator
Course: ENSE 452
Date: Sept 19, 2023
Name: Brandon Hillbom

Description: 
    This program computes the mathematical result of adding, 
    subtracting, multiplying, or dividing two complex numbers and outputs
    the result in the form of a + j b. All required features implemented. 
    No additional features added at this time.

Limitations: 
    - Can only compute arithmetic calculations for two complex numbers
    - Erroneous input will be caught by one of five error messages


Running the program:
    Prerequisite: must have c++ compiler, make extension, and 
    python 3 installed (to run tests).

    Build with g++: g++ -std=c++11 -o main main.cpp 
    or
    Build with make: make

    Run with user entered data: ./main

    Run with input data from mycommands.txt: ./main 0< mycommands.txt

    Run with input data from my commands.txt and output to output.txt 
    with any prompts outputted to junk.txt: 
    ./main < mycommands.txt 1> output.txt 2> junk.txt

    Run test file: ./runtests.py ./main

