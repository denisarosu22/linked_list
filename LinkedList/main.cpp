#include "LinkedList.h"

#include <iostream>
#include <stdexcept>
#include <string>

template<typename T>
void Print(const containers::LinkedList<T>& list)
{
    for (const auto& value : list)
    {
        std::cout << value << ' ';
    }
    std::cout << "(size: " << list.size() << ")\n";
}

int main()
{
    containers::LinkedList<int> numbers;

    for (int i = 5; i >= 1; --i)
    {
        numbers.push_front(i);
    }
    Print(numbers);

    for (auto& value : numbers)
    {
        value *= 10;
    }
    Print(numbers);

    const auto found = std::find(numbers.cbegin(), numbers.cend(), 30);
    std::cout << "found 30: " << std::boolalpha << (found != numbers.cend()) << '\n';

    containers::LinkedList<int>::const_iterator constIt = numbers.begin();
    auto it = numbers.begin();
    const auto previous = it++;
    std::cout << "first: " << *constIt << ", previous: " << *previous << ", current: " << *it << '\n';

    containers::LinkedList<std::string> words;
    words.push_front("world");
    words.push_front("hello");
    std::cout << "length of first word: " << words.begin()->size() << '\n';
    Print(words);

    const containers::LinkedList<int> empty;
    std::cout << "empty list begin == end: " << (empty.begin() == empty.end()) << '\n';

    return 0;
}