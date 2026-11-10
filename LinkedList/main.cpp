#include "LinkedList.h"

#include <iostream>
#include <string>

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
    containers::LinkedList<int> original = { 1, 42, 3 };
    auto tail = original.split_when([](const int& value) { return value == 42; });
    Print("original", original);
    Print("tail", tail);

    containers::LinkedList<int> noMatch = { 1, 2, 3 };
    const auto noMatchTail = noMatch.split_when([](const int& value) { return value > 100; });
    Print("noMatch", noMatch);
    Print("noMatchTail", noMatchTail);

    containers::LinkedList<int> firstMatches = { 5, 6, 7 };
    const auto firstMatchesTail = firstMatches.split_when([](const int& value) { return value == 5; });
    Print("firstMatches", firstMatches);
    Print("firstMatchesTail", firstMatchesTail);

    containers::LinkedList<std::string> words = { "keep", "this", "STOP", "and", "the", "rest" };
    const containers::SplitPredicate<std::string> isStop = [](const std::string& word) { return word == "STOP"; };
    const auto rest = words.split_when(isStop);
    Print("words", words);
    Print("rest", rest);

    return 0;
}