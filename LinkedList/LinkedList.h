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
        template<bool IsConst>
        class Iterator
        {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = std::conditional_t<IsConst, const T*, T*>;
            using reference = std::conditional_t<IsConst, const T&, T&>;

            Iterator() = default;

            explicit Iterator(Node* node) noexcept
                : node_(node)
            {
            }

            template<bool OtherIsConst>
            Iterator(const Iterator<OtherIsConst>& other) noexcept
                : node_(other.node_)
            {
                static_assert(IsConst || !OtherIsConst, "Cannot convert const_iterator to iterator");
            }

            reference operator*() const
            {
                return node_->value;
            }

            pointer operator->() const
            {
                return &node_->value;
            }

            Iterator& operator++()
            {
                node_ = node_->next.get();
                return *this;
            }

            Iterator operator++(int)
            {
                Iterator previous = *this;
                ++(*this);
                return previous;
            }

            friend bool operator==(const Iterator& lhs, const Iterator& rhs) noexcept
            {
                return lhs.node_ == rhs.node_;
            }

            friend bool operator!=(const Iterator& lhs, const Iterator& rhs) noexcept
            {
                return !(lhs == rhs);
            }

        private:
            template<bool>
            friend class Iterator;

            Node* node_ = nullptr;
        };

    public:
        using value_type = T;
        using size_type = std::size_t;
        using reference = T&;
        using const_reference = const T&;
        using iterator = Iterator<false>;
        using const_iterator = Iterator<true>;

        LinkedList() = default;

        // Delegating to the default constructor guarantees the destructor runs if an element constructor throws
        template<typename InputIt>
        LinkedList(InputIt first, InputIt last)
            : LinkedList()
        {
            std::unique_ptr<Node>* tail = &head_;
            for (; first != last; ++first)
            {
                *tail = std::make_unique<Node>(*first);
                tail = &(*tail)->next;
                ++size_;
            }
        }

        LinkedList(std::initializer_list<T> init)
            : LinkedList(init.begin(), init.end())
        {
        }

        LinkedList(const LinkedList& other)
            : LinkedList(other.begin(), other.end())
        {
        }

        LinkedList(LinkedList&& other) noexcept
            : head_(std::move(other.head_))
            , size_(std::exchange(other.size_, 0))
        {
        }

        ~LinkedList()
        {
            clear();
        }

        LinkedList& operator=(const LinkedList& other)
        {
            if (this != &other)
            {
                LinkedList copy(other);
                swap(copy);
            }
            return *this;
        }

        LinkedList& operator=(LinkedList&& other) noexcept
        {
            if (this != &other)
            {
                clear();
                head_ = std::move(other.head_);
                size_ = std::exchange(other.size_, 0);
            }
            return *this;
        }

        LinkedList& operator=(std::initializer_list<T> init)
        {
            LinkedList copy(init);
            swap(copy);
            return *this;
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
        iterator begin() noexcept
        {
            return iterator(head_.get());
        }

        const_iterator begin() const noexcept
        {
            return const_iterator(head_.get());
        }

        const_iterator cbegin() const noexcept
        {
            return begin();
        }

        iterator end() noexcept
        {
            return iterator();
        }

        const_iterator end() const noexcept
        {
            return const_iterator();
        }

        const_iterator cend() const noexcept
        {
            return end();
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

        // Removed nodes are kept alive until the end, so a predicate/value referring to a list element stays valid
        template<typename Predicate>
        size_type remove_if(Predicate predicate)
        {
            LinkedList removed;
            std::unique_ptr<Node>* current = &head_;
            while (*current)
            {
                if (predicate((*current)->value))
                {
                    auto node = std::move(*current);
                    *current = std::move(node->next);
                    --size_;
                    node->next = std::move(removed.head_);
                    removed.head_ = std::move(node);
                    ++removed.size_;
                }
                else
                {
                    current = &(*current)->next;
                }
            }
            return removed.size_;
        }

        size_type remove(const T& value)
        {
            return remove_if([&value](const T& element) { return element == value; });
        }

        void swap(LinkedList& other) noexcept
        {
            using std::swap;
            swap(head_, other.head_);
            swap(size_, other.size_);
        }

        friend void swap(LinkedList& lhs, LinkedList& rhs) noexcept
        {
            lhs.swap(rhs);
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