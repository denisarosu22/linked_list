#include "LinkedList.h"

#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>

struct Counter
{
    static int copies;

    explicit Counter(int v) : value(v) {}
    Counter(const Counter& other) : value(other.value) { ++copies; }
    Counter(Counter&&) = default;

    int value;
};
int Counter::copies = 0;

int main()
{
    static_assert(std::is_nothrow_move_constructible<containers::LinkedList<int>>::value, "move constructor must be noexcept");
    static_assert(std::is_nothrow_move_assignable<containers::LinkedList<int>>::value, "move assignment must be noexcept");

    containers::LinkedList<Counter> source;
    source.emplace_front(3);
    source.emplace_front(2);
    source.emplace_front(1);

    containers::LinkedList<Counter> moved(std::move(source));
    std::cout << "after move constructor -> moved size: " << moved.size()
        << ", source size: " << source.size()
        << ", source empty: " << std::boolalpha << source.empty()
        << ", element copies: " << Counter::copies << '\n';

    containers::LinkedList<Counter> target;
    target.emplace_front(99);
    target = std::move(moved);
    std::cout << "after move assignment -> target size: " << target.size()
        << ", front: " << target.front().value
        << ", moved size: " << moved.size()
        << ", element copies: " << Counter::copies << '\n';

    source.emplace_front(42);
    std::cout << "moved-from list is reusable -> source front: " << source.front().value << '\n';

    containers::LinkedList<std::unique_ptr<int>> owners;
    owners.push_front(std::make_unique<int>(7));
    containers::LinkedList<std::unique_ptr<int>> newOwners = std::move(owners);
    std::cout << "move-only elements -> value: " << *newOwners.front()
        << ", old list empty: " << owners.empty() << '\n';

    return 0;
}