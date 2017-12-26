#pragma once

#include <util/asio.h>
#include <util/noncopyable.h>
#include <util/Logging.h>
#include <thread>

namespace vixal {

namespace detail {
typedef asio::basic_waitable_timer<std::chrono::system_clock> timer;

template<typename T>
struct task_wrapped {
private:
    T task_unwrapped_;
public:
    explicit task_wrapped(const T &f)
            : task_unwrapped_(f) {}

    void operator()() const {
        try {
            task_unwrapped_();
        } catch (const std::exception &e) {
            LOG(DEBUG) << "exception in task:" << e.what();
        } catch (...) {
            LOG(DEBUG) << "exception in task";
        }
    }
};


template<typename Functor>
struct timer_task {
private:
    std::unique_ptr<detail::timer> timer_;
    task_wrapped<Functor> task_;

public:
    explicit timer_task(std::unique_ptr<detail::timer> timer,
                        const Functor &task_unwrapped)
            : timer_(std::move(timer)), task_(task_unwrapped) {
    }

    void operator()(const asio::error_code &error) const {
        if (!error) {
            task_();
        } else {
            LOG(DEBUG) << error;
        }
    }
};


} // namespace detail

namespace detail {

template<typename T>
task_wrapped<T> make_task_wrapped(const T &task_unwrapped) {
    return task_wrapped<T>(task_unwrapped);
}

} // namespace detail

class tasks_processor : private noncopyable {
protected:
    static asio::io_context &get_ioc() {
        static asio::io_context ioc;
        static auto work = asio::make_work_guard(ioc);

        return ioc;
    }

public:
    template<typename T>
    static void push_task(const T &task_unwrapped) {
        asio::post(get_ioc(), detail::make_task_wrapped(task_unwrapped));
    }

    static void start() {
        get_ioc().run();
    }

    static void stop() {
        get_ioc().stop();
    }

    template<typename Time, typename Func>
    static void run_delayed(Time duration_or_time, const Func &f) {

        auto time_ptr = std::make_unique<detail::timer>(get_ioc(), duration_or_time);
        auto &timer_ref = *time_ptr;
        timer_ref.async_wait(detail::timer_task<Func>(std::move(time_ptr), f));
    }

};
}
