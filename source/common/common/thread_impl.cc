#include "common/common/thread_impl.h"

#include "absl/strings/str_cat.h"

#include <sys/syscall.h>

namespace MXiang {
namespace Thread {

namespace {

int64_t getCurrentThreadId() {
    return static_cast<int64_t>(syscall(SYS_gettid));
}

}

#define PTHREAD_MAX_THREADNAME_LEN_INCLUDING_NULL_BYTE 16

class ThreadImplPosix : public Thread {
public:
    ThreadImplPosix(std::function<void()> thread_routine, OptionsOptConsRef options)
        : thread_routine_(std::move(thread_routine))
    {
        if (options) {
            name_ = options->name_.substr(0, PTHREAD_MAX_THREADNAME_LEN_INCLUDING_NULL_BYTE - 1);
        }
        
        const int rc = pthread_create(&thread_handle_, nullptr,
            [](void* arg) -> void * {
                static_cast<ThreadImplPosix*>(arg)->thread_routine_();
                return nullptr;
            },
            this);
        
#if SUPPORTS_PTHREAD_NAMING
        if (name_.empty()) {
            getNameFromOS(name_);
        } else {
            const int set_name_rc = pthread_setname_np(thread_handle_, name_.c_str());
            if (set_name_rc != 0) {
                //LOG_MSG??
            } else {
            }
        }
#endif
    }

    ~ThreadImplPosix() override { };

    std::string name() const override { return name_; }

    void join() override
    {
        joined_ = true;
        const int rc = pthread_join(thread_handle_, nullptr);

    }

private:
#if SUPPORTS_PTHREAD_NAMING
    bool getNameFromOS(std::string& name)
    {
        char buf[PTHREAD_MAX_THREADNAME_LEN_INCLUDING_NULL_BYTE];
        const int get_name_rc = pthread_getname_np(thread_handle_, buf, sizeof(buf));
        if (get_name_rc != 0) {
            // log ??
            return false;
        }
        name = buf;
        return true;
    }
#endif

    std::function<void()> thread_routine_;
    pthread_t thread_handle_;
    std::string name_;
    bool joined_{false};
};

ThreadPtr ThreadFactoryImplPosix::createThread(std::function<void()> thread_routine,
                                               OptionOptConstRef options)
{
    return std::make_unique<ThreadImplPosix>(thread_routine, options);
}

ThreadId ThreadFactoryImplPosix::currentThreadId()
{
    return ThreadId(getCurrentThreadId());
}

}  // namespace Thread
} // namespace MXiang
 

