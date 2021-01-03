#pragma once

#include <functional>
#include <limits>
#include <memory>
#include <string>

#include "absl/strings/string_view.h"
#include "absl/types/optional.h"

namespace MXiang {
namespace Thread {

class ThreadId {
public:
    ThreadId() : id_(std::numeric_limits<int64_t>::min()) {}
    explicit ThreadId(int64_t id) : id_(id) {}

    int64_t getId() const { return id_ ; };
    std::string debugString() const { return std::to_string(id_); }
    bool isEmpty() const { return *this == ThreadId(); }
    friend bool operator==(ThreadId lhs, Thread rhs) { return lhs.id_ == rhs.id_; }
    friend bool operator!=(ThreadId lhs, Thread rhs) { return lhs.id_ != rhs.id_; }
    template <typename H> friend H AbslHashValue(H h, ThreadId id)
    {
        return H::combine(std::move(h), id.id_);
    }

private:
    int64_t id_;
};

class Thread {
public:
    virtual ~Thread() = default;

    virtual std::string name() const = 0;

    virtual void join() = 0;
};

using ThreadPtr = std::unique_ptr<Thread>;

struct Options {
    std::string name_;
};

using OptionsOptConstRef = const absl::optional<Options>&;

class ThreadFactory {
public:
    virtual ~ThreadFactory() = default;

    virtual ThreadPtr createThread(std::function<void()> thread_routine,
                                   OptionsOptConstRef options = absl::nullptr) = 0;

    virtual ThreadId currentThreadId() = 0;
};

using ThreadFactoryPtr = std::unique_ptr<ThreadFactory>;

class ABSL_LOCKABLE BasicLockable {
public:
    virtual ~BasicLockable() = default;

    virtual void lock() = 0;
    virtual bool tryLock() = 0;
    virtual void unlock() = 0;
};

} //namespace Thread
}  //namespace MXiang
