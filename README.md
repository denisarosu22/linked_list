# LinkedList

Homework for the Modern & Effective C++ lecture.

A templated singly linked list, similar to std::forward_list.

Implemented:
- constructors (default, initializer_list, copy, move)
- push_front, emplace_front, pop_front, front
- iterators (begin/end), size, empty, clear
- remove, remove_if
- copy and move assignment
- bonus: split_when

Memory is managed with std::unique_ptr.

Build: open LinkedList.sln in Visual Studio (C++17) and run. The tests are in main.cpp.
