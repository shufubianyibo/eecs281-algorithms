// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C262F4E

#ifndef BINARYPQ_H
#define BINARYPQ_H


#include <algorithm>

#include "Eecs281PQ.hpp"

// A specialized version of the priority queue ADT implemented as a binary heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Description: Construct an empty PQ with an optional comparison functor.
    // Runtime: O(1)
    explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR())
        : BaseClass { comp } {
        // TODO: Implement this function, or verify that it is already done
    }  // BinaryPQ


    // Description: Construct a PQ out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR())
        : BaseClass { comp } {
        // TODO: Implement this function
        for (auto it = start; it != end; ++it) {
            this->data.push_back(*it);
        }
        if (!data.empty()) {
            for (long i = (long)data.size() / 2 - 1; i >= 0; --i) {
                fixDown(i);
            }
        }
    }  // BinaryPQ


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automatically.
    virtual ~BinaryPQ() = default;


    // Description: Copy constructors don't need any code, the data vector
    //              will be copied automatically.
    BinaryPQ(const BinaryPQ &) = default;
    BinaryPQ(BinaryPQ &&) noexcept = default;


    // Description: Copy assignment operators don't need any code, the data
    //              vector will be copied automatically.
    BinaryPQ &operator=(const BinaryPQ &) = default;
    BinaryPQ &operator=(BinaryPQ &&) noexcept = default;


    // Description: Assumes that all elements inside the heap are out of order and
    //              'rebuilds' the heap by fixing the heap invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        // TODO: Implement this function.
        if (!data.empty()) {
            for (long i = (long)data.size() / 2 - 1; i >= 0; --i) {
                fixDown(i);
            }
        }
    }  // updatePriorities()


    // Description: Add a new element to the PQ.
    // Runtime: O(log(n))
    virtual void push(const TYPE &val) {
        // TODO: Implement this function.
        this->data.push_back(val);
        fixUp(this->data.size() - 1);
    }  // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the PQ.
    // Note: We will not run tests on your code that would require it to pop
    // an element when the PQ is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: O(log(n))
    virtual void pop() {
        // TODO: Implement this function.
        std::swap(this->data.front(), this->data.back());
        this->data.pop_back();
        if (!this->data.empty()) {
            fixDown(0);
        }
    }  // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the PQ. This should be a reference for speed. It MUST
    //              be const because we cannot allow it to be modified, as
    //              that might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        // TODO: Implement this function.
        return this->data.front();
    }  // top()


    // Description: Get the number of elements in the PQ.
    // Runtime: O(1)
    [[nodiscard]] virtual std::size_t size() const {
        // TODO: Implement this function. Might be very simple,
        return this->data.size();
    }  // size()


    // Description: Return true if the PQ is empty.
    // Runtime: O(1)
    [[nodiscard]] virtual bool empty() const {
        // TODO: Implement this function. Might be very simple,
        return (this->data.size() == 0);
    }  // empty()


private:
    // Note: This vector *must* be used for your PQ implementation.
    std::vector<TYPE> data;
    // NOTE: You are not allowed to add any member variables. You don't need
    //       a "heapSize", since you can call your own size() member
    //       function, or check data.size().

    // TODO: Add any additional member functions you require here.
    //       For instance, you might add fixUp() and fixDown().
    void fixUp(size_t k) {
        while (k > 0) {
            size_t parent = (k - 1) / 2;
            if (!this->compare(this->data[parent], this->data[k])) {
                break;
            }
            std::swap(this->data[k], this->data[parent]);
            k = parent;
        }
    }
    void fixDown(size_t k) {
        size_t heapSize = this->data.size();
        while (2 * k + 1 < heapSize) {
            size_t j = 2 * k + 1;
            if (j < heapSize - 1 && this->compare(this->data[j], this->data[j + 1])) {
                j++;
            }
            if (!this->compare(this->data[k], this->data[j])) {
                break;
            }
            std::swap(this->data[k], this->data[j]);
            k = j;
        }
    }

};  // BinaryPQ


#endif  // BINARYPQ_H
