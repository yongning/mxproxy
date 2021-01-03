#pragma once

#include <pthread.h>

#include <functional>

#include "envoy/thread/thread.h"

namespace MXiang {
namespace Thread {
class ThreadFactoryImplPosix : public ThreadFactory {
public:
    ThreadPtr createThread(std::function<void()> thread_routine, OptionsOptConstRef options) override;
    ThreadId currentThreadId() override;
};

}
}
