#include "LinkedList.h"

#include <iostream>
#include <stdexcept>
#include <string>

int main()
{
    containers::LinkedList<std::string> words;

    const std::string first = "first";
    words.push_front(first);

    std::string second = "second";
    words.push_front(std::move(second));

    words.emplace_front("third");

    std::cout << "size: " << words.size() << ", front: " << words.front() << '\n';
    std::cout << "first after copy: \"" << first << "\", second after move: \"" << second << "\"\n";

    words.pop_front();
    std::cout << "after pop_front -> size: " << words.size() << ", front: " << words.front() << '\n';

    words.clear();
    std::cout << std::boolalpha << "after clear -> empty: " << words.empty() << ", size: " << words.size() << '\n';

    try
    {
        words.pop_front();
    }
    catch (const std::out_of_range& error)
    {
        std::cout << "exception: " << error.what() << '\n';
    }

    containers::LinkedList<int> big;
    for (int i = 0; i < 1000000; ++i)
    {
        big.push_front(i);
    }
    std::cout << "big list size: " << big.size() << '\n';

    return 0;
}