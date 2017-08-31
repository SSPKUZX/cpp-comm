#pragma once

#include <chrono>
#include <functional>

namespace utl 
{

class Scheduler
{
public:
    virtual ~Scheduler() {}

    /** 
     * The following functions need not to be thread-safe.
     * It is nonsense that schedule callback and submit request are in different threads.
     * Do it like this:
     * e.g.
     * 
     * // In this_loop thread.
     * 
     * Future<Buffer> ft(ReadFileInSeparateThread(very_big_file));
     * 
     * ft.Then(&this_loop, [](const Buffer& file_contents) {
     *      // SUCCESS : process file_content;
     * })
     * .OnTimeout(std::chrono::seconds(10), [=very_big_file]() {
     *      // FAILED OR TIMEOUT:
     *      printf("Read file %s failed\n", very_big_file);
     * },
     * &this_loop);
     */
    virtual void ScheduleOnceAfter(std::chrono::milliseconds const& duration, std::function<void()> f) = 0;
    virtual void ScheduleOnce(std::function<void()> f) = 0;
};

} // end namespace ananas


