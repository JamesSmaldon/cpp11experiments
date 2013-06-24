#include <cstring>
#include <algorithm>
#include <iostream>

class Str
{
	public:
		Str(const char * const p)
			: data(nullptr)
		{
			size = strlen(p);
			data = new char[size];
			memcpy(data, p, size);	
			std::cout << "Constructed: " << this << " " << "(" << data << ")" << std::endl;
		}

		Str(const Str &other)
			: data(nullptr)
		{
			size = other.size;
			data = new char[size];
			memcpy(data, other.data, size);
			std::cout << "Copied: " << this << " " << "(" << data << ")" << std::endl;
		}

		Str(Str &&other)
			: data(nullptr)
		{
			std::swap(data, other.data);
			std::swap(size, other.size);
			std::cout << "Moved: " << &other << " " << "(" << data << ")" << " to " << this << " (" << data << ")" << std::endl;
		}

		Str &operator=(Str other)
		{
			std::swap(data, other.data);
			std::swap(size, other.size);
			return *this;
		}

		virtual ~Str()
		{
			if (data != nullptr)
			{
				std::cout << "Deleted: " << this << " (" << data << ")" << std::endl;
				delete [] data;
			}
			else
			{
				std::cout << "Deleted: " << this << " null" << std::endl;
			}
		}

		Str copyMe()
		{
			return Str(*this);
		}

	
		friend std::ostream &operator<<(std::ostream &str, const Str &s);
		friend Str operator+(const Str &lhs, const Str &rhs);
	private:
		char *data;		
		size_t size;
};

Str operator+(const Str &lhs, const Str &rhs)
{
	char *newData = new char[lhs.size + rhs.size - 1];
	strcat(newData, lhs.data);
	strcat(newData, rhs.data);

	Str tmp(newData);
	delete [] newData;
	return tmp;
}

std::ostream &operator<<(std::ostream &str, const Str &s)
{
	if (s.data == nullptr)
	{
		str << "null";
	}
	else
	{
		str << s.data;
	}

	return str; 
}

void g(Str &s)
{
	std::cout << "lvalue g" << std::endl;
}

void g(Str &&s)
{
	std::cout << "rvalue g" << std::endl;
}

template <typename T>
void f(T &&t)
{
	g(std::forward<T>(t));	
}


int main(int argc, char **argv)
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
