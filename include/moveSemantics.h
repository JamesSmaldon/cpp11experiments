#pragma once

#include <cstring>
#include <algorithm>
#include <iostream>

/*
	James Smaldon 24/06/2013

	Experiments with move semantics.

	Compile with: g++ -g -std=c++0x -fno-elide-constructors -o move moveSemantics.cpp
*/


class Str
{
	public:
		Str(const char * const p)
			: data(nullptr)
		{
			size = strlen(p);
			data = new char[size + 1];
			data[size] = '\0';
			memcpy(data, p, size);	
			std::cout << "Constructed: " << this << " " << "(" << data << ")" << std::endl;
		}

		Str(const Str &other)
			: data(nullptr)
		{
			size = other.size;
			data = new char[size+1];
			data[size] = '\0';
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

inline Str operator+(const Str &lhs, const Str &rhs)
{
	int new_size = lhs.size + rhs.size;
	char *newData = new char[new_size + 1];
	memset(newData, '\0', new_size + 1);
	strcat(newData, lhs.data);
	strcat(newData, rhs.data);

	Str tmp(newData);
	delete [] newData;
	return tmp;
}

inline std::ostream &operator<<(std::ostream &str, const Str &s)
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

inline void g(Str &s)
{
	std::cout << "lvalue g" << std::endl;
}

inline void g(Str &&s)
{
	std::cout << "rvalue g" << std::endl;
}

template <typename T>
void f(T &&t)
{
	g(std::forward<T>(t));	
}

void moveSemantics();
