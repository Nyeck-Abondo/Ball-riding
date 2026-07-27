#include <windows.h>
#include <cstdint>

namespace sf {
    
    void SleepNano(uint64_t nanoseconds);
    void sleep(uint64_t millisecond);

} // namespace sf
