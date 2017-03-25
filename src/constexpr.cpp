
/*
	Notes: Previously, constant expressions (such as "3+4") could not include function calls or object constructors

	c++11 introduces the "constexpr" keyword, which allows the user to guarantee that the function or constructor is a
	compile time constant.
*/

#include <iostream>
#include "constexpr.h"

void constExpr()
{
	std::cout << "The meaning of life is: " << meaningOfLife << std::endl;
	int lifeArr[MeaningOfLife(3,4)];
}
