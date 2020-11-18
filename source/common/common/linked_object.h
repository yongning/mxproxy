#pragma once

#include <list>
#include <memory>

template <class T> class LinkedObject {
public:
    using ListType = std::list<std::unique_ptr<T>>;

    typename ListType::iterator entry() {
        ASSERT(inserted_);
        return entry_;
    }

    bool inserted() { return inserted_; }

    void moveBetweenLists(ListType& src, ListType& dst) {
        ASSERT(inserted_);
        ASSERT(std::find(src.begin(), src.end(), *entry_) != src.end());

        dst.splice(dst.begin(), src, entry_);
    }

        
