#include "window.h"

#if defined(SF_PLATEFORM_WINDOWS)
    #include "windows/windowWin32.h"

#elif defined (SF_PLATEFORM_LINUX)
    #include "linux/XCBwindow.h"
#endif

namespace SF {
    Window* Window::Create(const char* Title, uint32_t width, uint32_t height) {
        #if defined(SF_PLATEFORM_WINDOWS)
            return new windowWin32(Title, width, height);
        #elif defined(SF_PLATEFORM_LINUX)
            return
        #endif
    }
};