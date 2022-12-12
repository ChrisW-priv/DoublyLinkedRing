#include <iostream>
#include "dlring.h"


//Insert an element into the container and verify that the number_of_nodes of the container increases by one.
//Erase an element from the container and verify that the number_of_nodes of the container decreases by one.
//Insert an element into the container, then use an iterator to access the element and verify that it is the correct element.
//Use the begin() and end() methods to create a range of iterators, then use the std::distance() function to verify that the range covers the entire container.
//Use the std::next() function to move an iterator forward by a specific number of elements, then verify that the iterator is at the correct position.
//To test iterator properties specifically, you could try the following:
//
//Use the == and != operators to compare iterators and verify that they return the correct results.
//Use the * operator to dereference an iterator and verify that it returns the correct element.
//Use the ++ operator to increment an iterator and verify that it points to the correct element after the increment.
//Use the -- operator to decrement an iterator and verify that it points to the correct element after the decrement.
//Use the -> operator to access a member of the element pointed to by an iterator and verify that it returns the correct value.


template<typename T>
void compare_with_expected(T value_expected, T value_returned, const std::string& description="", std::ostream& stream = std::cout){
    stream << ((value_expected == value_returned) ? "PASS" : "FAIL");
    stream << (description.empty() ? description : ": " + description);
    stream << "\n";
}


int main() {
    {
        // test empty
        DoublyLinkedRing<int> ring;
        compare_with_expected(true, ring.empty(), "test if new ring is empty");
        compare_with_expected((size_t)0, ring.size(), "test if new ring size==0");
        compare_with_expected(ring.begin(), ring.end(), "test iterators equal each other in empty container");
    }
    {
        // test insertions and accessing through index operator as well as at method
        // also we will test if iterator shifts work as expected as well iterator dereferencing

        DoublyLinkedRing<int> ring;
        auto inserted_at = ring.insertBefore(ring.end(), 1);

        compare_with_expected(false, ring.empty(), "test if empty when entered one element");
        compare_with_expected((size_t)1, ring.size(), "test if new ring size==1");
        compare_with_expected(true, ring.begin()!=ring.end(), "test iterators equal each other in non-empty container");
        compare_with_expected(1, *(ring.begin()), "test dereference operator of iterator");
        compare_with_expected(1, ring.at(0), "test at method");
        compare_with_expected(1, ring[0], "test index operator");

        ring.insertAfter(inserted_at, 2);
        compare_with_expected((size_t)2, ring.size(), "test if new ring size==1");
        compare_with_expected(2, *(ring.begin()+1), "test dereferencing shifted iterator");
        compare_with_expected(2, ring.at(1), "test at method");
        compare_with_expected(2, ring[1], "test index operator");

        int x = 3, y=4;
        auto pos2 = ring.insertBefore(ring.end(), x);
        ring.insertAfter(pos2, y);
        compare_with_expected(3, ring[2], "test index operator");
        compare_with_expected(4, ring[3], "test index operator");
        compare_with_expected(ring.end(), (ring.begin()+4), "test if we get 'end' when over indexing");
    }
    {
        DoublyLinkedRing<int> ring{1,2,3,4,5};
        int& ref = ring[0];
        ref = 0;
        compare_with_expected(0, ring[0], "test if overwriting data in container works as expected");
    }
    {
        // std::find works as we want it to
        DoublyLinkedRing<int> ring{1,2,3,4,5};
        auto a = std::find(ring.begin(), ring.end(), 1);
        auto b = std::find(ring.begin(), ring.end(), 6);
        compare_with_expected(ring.begin(), a, "test if std::find finds elements correctly");
        compare_with_expected(ring.end(), b, "test if std::find finds elements correctly");
    }
    {
        // distance between nodes
        DoublyLinkedRing<int> ring{1,2,3,4,5};
        auto a = std::find(ring.begin(), ring.end(), 1);
        auto b = std::find(ring.begin(), ring.end(), 2);
        compare_with_expected(ring.size(), (size_t) (ring.end() - ring.begin()), "test if distance between nodes is calculated correctly");
        compare_with_expected(0, (int) (a-ring.begin()), "test if distance between nodes is calculated correctly");

        compare_with_expected(1, (int) (b-ring.begin()), "test if distance between nodes is calculated correctly");
        compare_with_expected(ring.size(), (size_t) (ring.begin()-b), "test if distance between nodes is calculated correctly");

        // now using std::distance()
        auto dist1 = std::distance(ring.begin(), ring.end());
        compare_with_expected(ring.size(), (size_t) (dist1), "test if distance between nodes is calculated correctly");

        auto dist2 = std::distance(ring.end(), ring.begin());
        compare_with_expected(-ring.size(), (size_t) (dist2), "test if distance between nodes is calculated correctly");

        auto dist3 = std::distance(ring.begin(), a);
        compare_with_expected(0, (int) (dist3), "test if distance between nodes is calculated correctly");

        auto dist4 = std::distance(ring.begin(), b);
        compare_with_expected(1, (int) (dist4), "test if distance between nodes is calculated correctly");
    }
    {
        DoublyLinkedRing<int> ring{1,2,3,4,5};
        DoublyLinkedRing<int> ring2{ring};
        auto ring3 = ring;
        auto result = std::equal(ring.begin(), ring.end(), ring2.begin());
        auto result2 = std::equal(ring.begin(), ring.end(), ring3.begin());
        compare_with_expected(true, result, "test of copy constructor");
        compare_with_expected(true, result2, "test of operator=");
    }

    return 0;
}
