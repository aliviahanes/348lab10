# Makefile for String-Based Calculator

CXX = g++
CXXFLAGS = -std=c++17 -Wall

all: string_calculator

string_calculator: main.o calculator.o
	$(CXX) $(CXXFLAGS) -o string_calculator main.o calculator.o

main.o: main.cpp calculator.h
	$(CXX) $(CXXFLAGS) -c main.cpp

calculator.o: calculator.cpp calculator.h
	$(CXX) $(CXXFLAGS) -c calculator.cpp

clean:
	rm -f *.o string_calculator
