#pragma once

#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace containers
{

    template<typename T>
    class LinkedList
    {
    private:
        struct Node
        {
            template<typename... Args>
            explicit Node(Args&&... args)
                : value(std::forward<Args>(args)...)
            {
            }

            T value;
            std::unique_ptr<Node> next;
        };

    public:
        using value_type = T;
        using size_type = std::size_t;
        using reference = T&;
        using const_reference = const T&;

        LinkedList() = default;

        bool empty() const noexcept
        {
            return head_ == nullptr;
        }

        size_type size() const noexcept
        {
            return size_;
        }

    private:
        std::unique_ptr<Node> head_;
        size_type size_ = 0;
    };

} // namespace containers