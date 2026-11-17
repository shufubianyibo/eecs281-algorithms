#ifndef HASHTABLE_H
#define HASHTABLE_H

// Project Identifier: 2C4A3C53CD5AD45A7BEA3AE5356A3B1622C9D04B

// INSTRUCTIONS:
// fill out the methods in the class below.

// You may assume that the key and value types can be copied and have default
// constructors.

// You can also assume that the key type has (==) and (!=) methods.

// You may assume that Hasher is a functor type with a
// size_t operator()(const Key&) overload.

// The key/value types aren't guaranteed to support any other operations.

// Do not add, remove, or change any of the class's member variables.
// The num_deleted counter is *optional*. You might find it helpful, but it
// is not required to pass the lab assignment.

// Do not change the Bucket type.

// SUBMISSION INSTRUCTIONS:
// Submit this file, by itself, in a .tar.gz.
// Other files will be ignored.

#include <cstdint>
#include <functional> // where std::hash lives
#include <vector>
#include <cassert> // useful for debugging!
using namespace std;

// A bucket's status tells you whether it's empty, occupied, 
// or contains a deleted element.
enum class Status : uint8_t {
    Empty,
    Occupied,
    Deleted
};

template<typename K, typename V, typename Hasher = std::hash<K>>
class HashTable {
    // used by the autograder; do not change/remove.
    friend class Verifier;
public:
    // A bucket has a status, a key, and a value.
    struct Bucket {
        // Do not modify Bucket.
        Status status = Status::Empty;
        K key;
        V val;
    };

    HashTable() {
        // TODO: a default constructor (possibly empty).
        buckets.resize(20);
        // You can use the following to avoid implementing rehash_and_grow().
        // However, you will only pass the AG test cases ending in _C.
        // To pass the rest, start with at most 20 buckets and implement rehash_and_grow().
        //    buckets.resize(10000);
    }

    size_t size() const {
        return num_elements;
    }

    // returns a reference to the value in the bucket with the key, if it
    // already exists. Otherwise, insert it with a default value, and return
    // a reference to the resulting bucket.
    V& operator[](const K& key) {
        // TODO
        if (num_elements + num_deleted >= buckets.size() / 2) {
            rehash_and_grow();
        }

        size_t hash_val = Hasher{}(key);
        size_t first_deleted = static_cast<size_t>(-1); 

        for (size_t i = 0; i < buckets.size(); i++) {
            size_t idx = (hash_val + i) % buckets.size();

            if (buckets[idx].status == Status::Empty) {
                size_t target_idx = (first_deleted != static_cast<size_t>(-1)) ? first_deleted : idx;
                
                buckets[target_idx].status = Status::Occupied;
                buckets[target_idx].key = key;
                buckets[target_idx].val = V{}; 
                
                num_elements++;
                if (target_idx == first_deleted) num_deleted--; 
                
                return buckets[target_idx].val;

            } else if (buckets[idx].status == Status::Occupied && buckets[idx].key == key) {
                return buckets[idx].val; 

            } else if (buckets[idx].status == Status::Deleted && first_deleted == static_cast<size_t>(-1)) {
                first_deleted = idx; 
            }
        }
        
        return buckets[0].val;
    }

    // insert returns whether inserted successfully
    // (if the key already exists in the table, do nothing and return false).
    bool insert(const K& key, const V& val) {
        // TODO
        if (num_elements + num_deleted >= buckets.size() / 2) {
            rehash_and_grow();
        }

        size_t hash_val = Hasher{}(key);
        size_t first_deleted = static_cast<size_t>(-1);

        for (size_t i = 0; i < buckets.size(); i++) {
            size_t idx = (hash_val + i) % buckets.size();

            if (buckets[idx].status == Status::Empty) {
                size_t target_idx = (first_deleted != static_cast<size_t>(-1)) ? first_deleted : idx;
                
                buckets[target_idx].status = Status::Occupied;
                buckets[target_idx].key = key;
                buckets[target_idx].val = val;
                
                num_elements++;
                if (target_idx == first_deleted) num_deleted--; 
                
                return true;

            } else if (buckets[idx].status == Status::Occupied && buckets[idx].key == key) {
                return false; 

            } else if (buckets[idx].status == Status::Deleted && first_deleted == static_cast<size_t>(-1)) {
                first_deleted = idx; 
            }
        }
        return false;
    }
    // erase returns the number of items remove (0 or 1)
    size_t erase(const K& key) {
        // TODO
        size_t hash_val = Hasher{}(key); 

        for (size_t i = 0; i < buckets.size(); i++) {
            size_t idx = (hash_val + i) % buckets.size();
            
            if (buckets[idx].status == Status::Empty) {
                return 0; 
            } else if (buckets[idx].status == Status::Occupied && buckets[idx].key == key) {
                buckets[idx].status = Status::Deleted;
                num_elements--;
                num_deleted++;
                return 1;
            }
        }
        return 0;
    }

private:
    size_t num_elements = 0;
    size_t num_deleted = 0; // OPTIONAL: you don't need to use num_deleted to pass
    std::vector<Bucket> buckets;

    void rehash_and_grow() {
        // You can avoid implementing rehash_and_grow() by calling
        //    buckets.resize(10000);
        // in the constructor. However, you will only pass the AG test cases ending in _C.
        // To pass the rest, start with at most 20 buckets and implement rehash_and_grow().
        vector<Bucket> old_buckets = buckets;
        
        buckets.clear();
        buckets.resize(old_buckets.size() * 2);
        num_elements = 0;
        num_deleted = 0;
        
        for (const auto& bucket : old_buckets) {
            if (bucket.status == Status::Occupied) {
                insert(bucket.key, bucket.val); 
            }
        }

    }

    // You can add methods here if you like.
    // TODO
};

#endif // HASHTABLE_H