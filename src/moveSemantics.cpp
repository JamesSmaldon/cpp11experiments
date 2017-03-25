#include "moveSemantics.h"

void moveSemantics()
{
	Str test("test");
	Str test2("test2");

	//move assignment operator
	Str test3 = test + test2;

	std::cout << test3 << std::endl;

	//this should call the copy constructor.
	Str test4(test3);

	//but this should call the move constructor.
	Str test5(std::move(test4));

	//and test4 should be null now.
	std::cout << test4 << std::endl;

	//should call lvalue g
	f(test3);

	//should call rvalue g
	f(test3.copyMe());
}
