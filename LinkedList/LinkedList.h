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
        ~LinkedList()
        {
            clear();
        }

        reference front()
        {
            ThrowIfEmpty("front");
            return head_->value;
        }

        const_reference front() const
        {
            ThrowIfEmpty("front");
            return head_->value;
        }

        bool empty() const noexcept
        {
            return head_ == nullptr;
        }

        size_type size() const noexcept
        {
            return size_;
        }

        void push_front(const T& value)
        {
            emplace_front(value);
        }

        void push_front(T&& value)
        {
            emplace_front(std::move(value));
        }

        template<typename... Args>
        reference emplace_front(Args&&... args)
        {
            auto newNode = std::make_unique<Node>(std::forward<Args>(args)...);
            newNode->next = std::move(head_);
            head_ = std::move(newNode);
            ++size_;
            return head_->value;
        }

        void pop_front()
        {
            ThrowIfEmpty("pop_front");
            const auto oldHead = std::move(head_);
            head_ = std::move(oldHead->next);
            --size_;
        }

        void clear() noexcept
        {
            while (head_)
            {
                auto next = std::move(head_->next);
                head_ = std::move(next);
            }
            size_ = 0;
        }

    private:
        void ThrowIfEmpty(const char* operation) const
        {
            if (empty())
            {
                throw std::out_of_range(std::string("LinkedList::") + operation + ": list is empty");
            }
        }
        std::unique_ptr<Node> head_;
        size_type size_ = 0;
    };

} // namespace containers