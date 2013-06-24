
/*
	Notes: Previously, constant expressions (such as "3+4") could not include function calls or object constructors

	c++11 introduces the "constexpr" keyword, which allows the user to guarantee that the function or constructor is a
	compile time constant.

	Compile with: g++ -g -std=c++0x -o constexpr constexpr.cpp
*/

#include <iostream>

constexpr int MeaningOfLife (int a, int b) { return a * b; }

const int meaningOfLife = MeaningOfLife(6,7);

int main(int argc, char **argv)
{
	std::cout << "The meaning of life is: " << meaningOfLife << std::endl;
	int lifeArr[MeaningOfLife(3,4)];

	return 0;
}
