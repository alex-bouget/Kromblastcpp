#ifndef KB_OS_HPP
#define KB_OS_HPP

/**
 * @brief OS family
 * @note 0: Unix, 1: Windows, 2: Mac
 */

/**
 * @brief OS Name
 * @note Linux, Windows, Mac
 */

#ifdef _WIN32
    #define KB_OS_FAMILY 1
    #define KB_OS_NAME "Windows"
    #ifdef _WIN64                   // 64 bits Windows
    #else                           // 32 bits Windows
    #endif
#elif __APPLE__                     // Apple OS
    #define KB_OS_FAMILY 2
    #define KB_OS_NAME "Mac"
    #include "TargetConditionals.h"
    #ifdef TARGET_IPHONE_SIMULATOR     // iOS Simulator
    #elif TARGET_OS_IPHONE          // iOS device
    #elif TARGET_OS_MAC             // Other kinds of Mac OS
    #else
        #error "Unknown Apple platform"
    #endif
#elif __unix__                     // Unix
    #define KB_OS_FAMILY 0
    #define KB_OS_NAME "Unix"
    #ifdef __linux__               // Linux
        #define KB_OS_NAME "Linux"
        #ifdef __ANDROID__         // Android
            #define KB_OS_NAME "Android"
        #endif
    #endif
#endif

#endif