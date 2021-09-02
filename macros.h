#ifndef MACROS_H
#define MACROS_H

// For cross-platform functionality.
#if defined(_WIN32) || defined(WIN32)
    #include <windows.h> // for Sleep
    #define CLEAR_WINDOW system("CLS")
    #define SLEEP(x) Sleep(x)
#elif defined(__unix__)
    #include <unistd.h> // for usleep
    #define CLEAR_WINDOW system("clear")
    #define SLEEP(x) usleep(x * 1000)
#endif

#endif