#include "LinkedList.h"

#include <iostream>
#include <string>

struct Resource
{
    static int alive;

    explicit Resource(int v) : value(v) { ++alive; }
    Resource(const Resource& other) : value(other.value) { ++alive; }
    ~Resource() { --alive; }

    bool operator==(const Resource& other) const { return value == other.value; }

    int value;
};
int Resource::alive = 0;

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
    containers::LinkedList<int> numbers = { 2, 1, 2, 3, 2, 4, 5, 2 };
    Print("numbers", numbers);

    const auto removedTwos = numbers.remove(2);
    std::cout << "removed " << removedTwos << " elements equal to 2\n";
    Print("numbers", numbers);

    const auto removedOdd = numbers.remove_if([](int value) { return value % 2 != 0; });
    std::cout << "removed " << removedOdd << " odd elements\n";
    Print("numbers", numbers);

    std::cout << "removed missing value: " << numbers.remove(100) << '\n';

    containers::LinkedList<int> same = { 7, 7, 7 };
    std::cout << "remove(front()) removed: " << same.remove(same.front())
        << ", empty: " << std::boolalpha << same.empty() << '\n';

    {
        containers::LinkedList<Resource> resources;
        for (int i = 0; i < 5; ++i)
        {
            resources.emplace_front(i);
        }
        std::cout << "alive resources: " << Resource::alive << '\n';

        resources.remove_if([](const Resource& resource) { return resource.value < 3; });
        std::cout << "alive after remove_if: " << Resource::alive << ", list size: " << resources.size() << '\n';
    }
    std::cout << "alive after list destroyed: " << Resource::alive << '\n';

    return 0;
}