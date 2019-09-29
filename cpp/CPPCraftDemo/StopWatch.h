#pragma once
#include <chrono>

class StopWatch {
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
    using Duration = Clock::duration;

    TimePoint d_start;
    TimePoint d_end;

public:
    StopWatch() : d_start{ Clock::now() } {}
    
    template <typename DUR>
    DUR duration() const {
        auto end = d_end.time_since_epoch() != Duration::zero() ? d_end : Clock::now();
        return std::chrono::duration_cast<DUR>(end - d_start);
    }

    double seconds() const {
        return double(duration<std::chrono::nanoseconds>().count()) / 1'000'000'000.0;
    }
    
    inline void stop() {
        d_end = Clock::now();
    }

    inline void start() {
        d_end = TimePoint{};
    }
};
