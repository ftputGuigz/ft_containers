#include <utility>
#include <functional>
#include <iostream>
#include <string>
#include "map.hpp"
#include <map>

# define ERR_NO_NUM -1
# define ERR_NO_MEM -2
# define PREFIX ft

int myRandom (int size) {
    int i, n;
    static int numNums = 0;
    static int *numArr = NULL;

    // Initialize with a specific size.

    if (size >= 0) {
        if (numArr != NULL)
            free (numArr);
        if ((numArr = (int *)malloc (sizeof(int) * size)) == NULL)
            return ERR_NO_MEM;
        for (i = 0; i  < size; i++)
            numArr[i] = i;
        numNums = size;
    }

    // Error if no numbers left in pool.

    if (numNums == 0)
       return ERR_NO_NUM;

    // Get random number from pool and remove it (rnd in this
    //   case returns a number between 0 and numNums-1 inclusive).

    n = rand() % numNums;
    i = numArr[n];
    numArr[n] = numArr[numNums-1];
    numNums--;
    if (numNums == 0) {
        free (numArr);
        numArr = 0;
    }

    return i;
}

int main(void)
{	
	PREFIX::map<int, int> first;


 	// srand (time (NULL));
    // int i = myRandom (10);
    // while (i >= 0) {
    //     i = myRandom (-1);
	// 	first.insert(PREFIX::make_pair(i, 0));
    // }


	first.insert(PREFIX::make_pair(30, 0));
	first.insert(PREFIX::make_pair(15, 0));
	first.insert(PREFIX::make_pair(17, 0));
	first.insert(PREFIX::make_pair(10, 0));
	first.insert(PREFIX::make_pair(18, 0));
	first.insert(PREFIX::make_pair(16, 0));
	first.insert(PREFIX::make_pair(35, 0));
	first.insert(PREFIX::make_pair(42, 0));
	first.insert(PREFIX::make_pair(40, 0));
	first.insert(PREFIX::make_pair(28, 0));
	first.insert(PREFIX::make_pair(20, 0));
	first.insert(PREFIX::make_pair(43, 0));
	first.insert(PREFIX::make_pair(7, 0));
	first.insert(PREFIX::make_pair(8, 0));

	PREFIX::map<int, int>::const_reverse_iterator it(first.rbegin());
	PREFIX::map<int, int>::const_reverse_iterator it2(first.rend());

	for (; it != it2; it++)
		std::cout << "it->first " << it->first << std::endl;




	// print2D(first.getBST());
	// std::cout << "first.size() IS :" << first.size()  << std::endl;
	// first.erase(30);
	// first.erase(10);
	// first.erase(16);
	// first.erase(40);
	// first.erase(15);
	// first.erase(35);
	// first.erase(43);
	// first.erase(8);
	// first.erase(28);
	// first.erase(17);
	// first.erase(18);
	// first.erase(20);
	// first.erase(7);
	// first[1000] = 0;
	// first[1000] = 50;
	// first[1000] = 50;
	// first[100000] = 0;
	// std::cout << "first.size() IS :" << first.size()  << std::endl;
}