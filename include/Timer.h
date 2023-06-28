#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <ostream>

using high_resolution_clock = std::chrono::high_resolution_clock;
using milliseconds = std::chrono::milliseconds;


    class Timer {
    private:
        high_resolution_clock::time_point _start;
        high_resolution_clock::time_point _end;
        milliseconds time;
    public:
        explicit Timer(bool run = false) : time(0)
        {
            if (run)
                Reset();
        }

        void Reset()
        {
            _start = high_resolution_clock::now();
        }

        void start() {
            Reset();
        }

        void end() {
            time = std::chrono::duration_cast<milliseconds>(high_resolution_clock::now() - _start);
            Reset();
        }

        milliseconds getTime() const {
            return time;
        }

        uint32_t Elapsed() const
        {
            return std::chrono::duration_cast<milliseconds>(high_resolution_clock::now() - _start).count();
        }

    };

#endif