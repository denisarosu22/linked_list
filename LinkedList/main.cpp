#include "LinkedList.h"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#if defined(_MSC_VER) && defined(_DEBUG)
#include <crtdbg.h>
#endif

using containers::LinkedList;

namespace
{

    int g_failedChecks = 0;

    void Check(bool condition, const std::string& description)
    {
        std::cout << (condition ? "[PASS] " : "[FAIL] ") << description << '\n';
        if (!condition)
        {
            ++g_failedChecks;
        }
    }

    template<typename T>
    std::string ToString(const LinkedList<T>& list)
    {
        std::string result;
        for (const auto& value : list)
        {
            result += std::to_string(value) + ' ';
        }
        return result;
    }

    struct Tracker
    {
        static int alive;
        static int copies;

        explicit Tracker(int v) : value(v) { ++alive; }
        Tracker(const Tracker& other) : value(other.value) { ++alive; ++copies; }
        Tracker(Tracker&& other) noexcept : value(other.value) { ++alive; }
        Tracker& operator=(const Tracker&) = default;
        Tracker& operator=(Tracker&&) noexcept = default;
        ~Tracker() { --alive; }

        int value;
    };
    int Tracker::alive = 0;
    int Tracker::copies = 0;

} // namespace

int main()
{
#if defined(_MSC_VER) && defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    LinkedList<int> numbers = { 1, 2, 3 };
    numbers.push_front(0);
    Check(ToString(numbers) == "0 1 2 3 " && numbers.size() == 4, "initializer_list, push_front, iteration, size");

    numbers.pop_front();
    numbers.remove(2);
    numbers.remove_if([](int value) { return value > 2; });
    Check(ToString(numbers) == "1 " && numbers.size() == 1, "pop_front, remove, remove_if");

    LinkedList<int> copied(numbers);
    copied.front() = 100;
    Check(ToString(copied) == "100 " && ToString(numbers) == "1 ", "copy constructor makes an independent copy");

    LinkedList<int> source = { 5, 6 };
    LinkedList<int> moved(std::move(source));
    LinkedList<int> assigned;
    assigned = std::move(moved);
    Check(ToString(assigned) == "5 6 ", "move constructor and move assignment");

    LinkedList<int> original = { 1, 42, 3 };
    const auto tail = original.split_when([](const int& value) { return value == 42; });
    Check(ToString(original) == "1 " && ToString(tail) == "42 3 ", "split_when example from the assignment");

    {
        LinkedList<Tracker> objects;
        const Tracker lvalue(1);
        objects.push_front(lvalue);
        objects.push_front(Tracker(2));
        Check(Tracker::copies == 1, "push_front copies lvalues and moves rvalues");

        LinkedList<Tracker> movedObjects = std::move(objects);
        Check(Tracker::copies == 1 && movedObjects.size() == 2, "moving a list does not copy its elements");
    }
    Check(Tracker::alive == 0, "all objects are released");

    LinkedList<std::unique_ptr<int>> owners;
    owners.push_front(std::make_unique<int>(7));
    Check(*owners.front() == 7, "move-only types are supported");

    constexpr int kLongListSize = 1000000;
    LinkedList<int> longList;
    for (int i = 0; i < kLongListSize; ++i)
    {
        longList.push_front(i);
    }
    Check(longList.size() == static_cast<std::size_t>(kLongListSize), "long list is destroyed without stack overflow");

    std::cout << '\n' << (g_failedChecks == 0 ? "All checks passed" : "Some checks failed") << '\n';
    return g_failedChecks == 0 ? 0 : 1;
}
