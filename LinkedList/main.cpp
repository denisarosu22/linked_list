#include "LinkedList.h"

#include <iostream>
#include <string>
#include <vector>

template<typename T>
void Print(const std::string& name, const containers::LinkedList<T>& list)
{
    std::cout << name << ": ";
    for (const auto& value : list)
    {
        std::cout << value << ' ';
    }
    std::cout << "(size: " << list.size() << ")\n";
}

int main()
{
    containers::LinkedList<int> numbers = { 1, 2, 3 };
    Print("numbers", numbers);

    const std::vector<std::string> source = { "a", "b", "c" };
    const containers::LinkedList<std::string> fromVector(source.begin(), source.end());
    Print("fromVector", fromVector);

    containers::LinkedList<int> copy(numbers);
    copy.push_front(0);
    Print("copy", copy);
    Print("numbers after changing copy", numbers);

    containers::LinkedList<int> assigned = { 9, 9 };
    assigned = numbers;
    assigned.front() = 100;
    Print("assigned", assigned);
    Print("numbers after changing assigned", numbers);

    assigned = { 7, 8 };
    Print("assigned from initializer_list", assigned);

    swap(numbers, assigned);
    Print("numbers after swap", numbers);
    Print("assigned after swap", assigned);

    return 0;
}