
#include <condition_variable>
#include <functional>
#include <string>
#include <thread>
#include <iostream>
using namespace std;

class Timer
{
public:
    Timer() = default;

    ~Timer() {
        delete s;
        s = nullptr;
    }

private:
    static bool _updateStatus(Timer* timer) {
        if (timer->status == TIMER_NOT_INIT || timer->status == TIMER_START) {
            timer->status = TIMER_START;
            std::cout << "start timer. update status:" << timer->status << std::endl;
            return false;
        }
        else {
            std::cout << "stop timer. status:" << timer->status << std::endl;
            return true;//stop
        }
    }

    static void _do_task_func(int64_t sec, Timer* timer, std::function<void()> callback) {
        std::unique_lock<std::mutex> lk(timer->cv_m);
        auto now = std::chrono::system_clock::now();
        std::cout << "start task, delay:" << sec << std::endl;
        if (timer->cv.wait_until(lk, now + sec * 1000ms, std::bind(_updateStatus, timer))) {
            std::cerr << "Thread finished waiting. \n";
        }
        else {
            std::cerr << "Thread timed out.\n";
            callback();
        }
    }

public:
    void start(int64_t sec, std::function<void()> callback) {
        s = new std::thread(_do_task_func, sec, this, callback);

    }

    void stop() {
        //新增判断条件，防止多次调用cv.notify_one导致deadlock
        if (status == TIMER_START) {
            status = TIMER_STOP;
            std::cout << "Notifying...\n";
            cv.notify_one();
            s->join();
        }
    }

    enum TimerStatus {
        TIMER_NOT_INIT = 0,
        TIMER_START = 1,
        TIMER_STOP = 2
    };
    std::mutex cv_m;
    std::condition_variable cv;
    TimerStatus status = TIMER_NOT_INIT;
    std::thread* s = nullptr;

};