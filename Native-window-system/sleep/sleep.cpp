#include "sleep.h"

namespace SF {
    
    void SleepNano(uint64_t nanoseconds) {
        if (nanoseconds == 0) return;

        LARGE_INTEGER freq, start, end;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start);

        uint64_t target = start.QuadPart + (nanoseconds * freq.QuadPart) / 1000000000ULL;
        if (nanoseconds > 2000000) {
            ::Sleep(static_cast<DWORD>(nanoseconds / 1000000) - 1);
        }
        do {
            QueryPerformanceCounter(&end);
        } while (end.QuadPart < target);
    }

    void sleep(uint64_t millisecond) {
        SleepNano(millisecond * 1000000);
    }

} // namespace SF
