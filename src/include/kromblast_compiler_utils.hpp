#ifndef KRBLST_COMPILER_UTILS_HPP
#define KRBLST_COMPILER_UTILS_HPP

/**
 * @brief OS family
 * @note 0: Linux, 1: Windows, 2: Mac
 */

#define KB_OS_LINUX 0
#define KB_OS_WINDOWS 1
#define KB_OS_MAC 2

#ifdef _WIN32
#define KB_OS_FAMILY KB_OS_WINDOWS
#elif __APPLE__ // Apple OS
#define KB_OS_FAMILY KB_OS_MAC
#elif __linux__
#define KB_OS_FAMILY KB_OS_LINUX
#endif

#ifdef DEBUG
#define KB_DEBUG
#else
#define KB_RELEASE
#endif

#endif