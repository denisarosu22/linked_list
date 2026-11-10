#include "LinkedList.h"

#include <iostream>

int main()
{
    const containers::LinkedList<int> numbers;
    std::cout << std::boolalpha << "empty: " << numbers.empty() << ", size: " << numbers.size() << '\n';
    return 0;
}