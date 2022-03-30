#include <utility>
#include <functional>
#include <iostream>
#include <string>
#include "map.hpp"

int main(void)
{	
	ft::map<int, int> first;


	first.insert(ft::make_pair(30, 0));
	first.insert(ft::make_pair(15, 0));
	first.insert(ft::make_pair(17, 0));
	first.insert(ft::make_pair(10, 0));
	first.insert(ft::make_pair(18, 0));
	first.insert(ft::make_pair(16, 0));
	first.insert(ft::make_pair(35, 0));
	first.insert(ft::make_pair(42, 0));
	first.insert(ft::make_pair(28, 0));
	first.insert(ft::make_pair(20, 0));
	first.insert(ft::make_pair(43, 0));
	first.insert(ft::make_pair(7, 0));
	first.insert(ft::make_pair(8, 0));


	print2D(first.getBST());

}