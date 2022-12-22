#ifndef DLRING_LABS_DLRING_H
#define DLRING_LABS_DLRING_H

#include <iostream>
#include <stdexcept>


template <typename T>
class DoublyLinkedRing {
private:
    struct Node {
        T data{};
        Node* prev = this;
        Node* next = this;
    };

    Node* insertNode(Node* curr, const T& data) {
        Node* new_node = new Node{data, curr, curr->next};
        curr->next->prev = new_node;
        curr->next = new_node;
        ++number_of_nodes;
        return new_node;
    }

    void deleteNode(Node* curr) {
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
        delete curr;
        --number_of_nodes;
    }

public:
    template <typename NodeType>
    struct Iter {
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = typename std::conditional<std::is_const<NodeType>::value, const T, T>::type;
        using reference = value_type&;
        using pointer = value_type*;

        using owner_type = typename std::conditional<std::is_const<NodeType>::value, const DoublyLinkedRing, DoublyLinkedRing>::type;

        explicit Iter(NodeType* n, owner_type * ring) : curr(n), owner(ring) {}
        Iter(const Iter &other) : curr(other.curr), owner(other.owner) {};
        Iter& operator=(const Iter& other) { curr = other.curr; owner = other.owner; };

        // Prefix increment operator
        Iter& operator++() {
            curr = curr->next;
            return *this;
        }

        // Postfix increment operator
        Iter operator++(int) {
            Iter it = *this;
            ++(*this);
            return it;
        }

        // Prefix decrement operator
        Iter& operator--() {
            curr = curr->prev;
            return *this;
        }

        // Postfix decrement operator
        Iter operator--(int) {
            Iter it = *this;
            --(*this);
            return it;
        }

        reference operator*() { return curr->data; }
        pointer operator->() { return &(curr->data); }

        template<typename U>
        bool operator==(const Iter<U>& other) const { return curr == other.curr; }

        template<typename U>
        bool operator!=(const Iter<U>& other) const { return curr != other.curr; }

        template<typename U>
        difference_type operator-(const Iter<U>& other) const {
            // need to result of `this-other`, so a distance between two elements
            // eg. ptr+7 - (ptr+5) = 2, but ptr+5 - (ptr+7) = -2

            // we can optimise this if we calculate the distance od begin and end as this just owner number_of_nodes!
            if (*this == owner->end() && other == owner->begin()) return owner->number_of_nodes;
            if (*this == owner->begin() && other == owner->end()) return -owner->number_of_nodes;

            // else we need to traverse the owner until we find match
            // first check if they are of the same owner
            if (this->owner != other.owner) throw std::invalid_argument("Iterators don't have the same owner!");

            // now just iterate till you find what you look for

            auto ret = other;
            difference_type result = 0;
            while (ret!=*this) {
                ++ret; ++result;
            }

            return result;
        }

        Iter operator-(difference_type shift) const {
            auto ret = *this;
            ret -= shift;
            return ret;
        }
        Iter& operator-=(difference_type shift) {
            while (shift < 0){ ++*this; shift++; }
            while (shift > 0){ --*this; shift--; }
            return *this;
        }

        Iter operator+(difference_type shift) const {
            auto ret = *this;
            ret += shift;
            return ret;
        }
        Iter& operator+=(difference_type shift) {
            while (shift < 0){ --*this; shift++; }
            while (shift > 0){ ++*this; shift--; }
            return *this;
        }

        Iter operator[](difference_type shift) const { return (*this) + shift; }

        NodeType* curr;
        owner_type* owner;
    };

    using Iterator = Iter<Node>;
    using ConstIterator = Iter<const Node>;

    DoublyLinkedRing() = default;
    DoublyLinkedRing(std::initializer_list<T> list) { for (T elem : list) insertBefore(end(), elem); }
    DoublyLinkedRing(const DoublyLinkedRing<T>& other) { *this = other; }

    DoublyLinkedRing& operator=(const DoublyLinkedRing& other) {
        if (this == &other) return *this;
        clear();
        for (T elem : other) insertBefore(end(), elem);
        return *this;
    }

    ~DoublyLinkedRing() { clear(); }
    void clear() { while (!empty()) erase(begin()); }

    Iterator insertAfter(Iterator it, const T& data) { return Iterator(insertNode(it.curr, data), this); }
    Iterator insertAfter(Iterator it, T&& data) { return Iterator(insertNode(it.curr, data), this); }

    Iterator insertBefore(Iterator it, const T& data) { return Iterator(insertNode(it.curr->prev, data), this); }
    Iterator insertBefore(Iterator it, T&& data) { return Iterator(insertNode(it.curr->prev, data), this); }

    Iterator erase(Iterator it) {
        Node* curr = it.curr;
        Iterator next = ++it;
        deleteNode(curr);
        return next;
    }

    T& operator[](size_t index) { return *(this->begin()[index]); }
    const T& operator[](size_t index) const { return *(this->begin()[index]); }
    T& at(size_t index) { return *(this->begin()[index]); }
    const T& at(size_t index) const { return *(this->begin()[index]); }

    constexpr Iterator begin()              { return Iterator(sentinel.next, this); }
    constexpr Iterator end()                { return Iterator(&sentinel, this); }
    constexpr ConstIterator begin() const   { return ConstIterator(sentinel.next, this); }
    constexpr ConstIterator end() const     { return ConstIterator(&sentinel, this); }

    [[nodiscard]] bool empty() const { return number_of_nodes == 0; }
    [[nodiscard]] size_t size() const { return number_of_nodes; }

    /// Assumes data has operator<< defined!
    /// Will result in compile error in this function is used for any other type!
    friend std::ostream& operator<<(std::ostream& stream, const DoublyLinkedRing& ring){
        stream << "{ ";
        for (auto node : ring){
            stream << node << ",";
        }
        stream << " }";
        return stream;
    }

private:
    // sentinel does not need to be dynamically allocated! It can just as well be a member variable
    Node sentinel = Node();
    size_t number_of_nodes{};
};


#endif //DLRING_LABS_DLRING_H
