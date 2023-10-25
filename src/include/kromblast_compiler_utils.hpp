#ifndef KRBLST_COMPILER_UTILS_HPP
#define KRBLST_COMPILER_UTILS_HPP

/**
 * @brief OS family
 * @note 0: Linux, 1: Windows, 2: Mac
 */

#define KROMBLAST_OS_LINUX 0
#define KROMBLAST_OS_WINDOWS 1
#define KROMBLAST_OS_MAC 2

#ifdef _WIN32
#define KROMBLAST_OS_FAMILY KROMBLAST_OS_WINDOWS
#elif __APPLE__ // Apple OS
#define KROMBLAST_OS_FAMILY KROMBLAST_OS_MAC
#elif __linux__
#define KROMBLAST_OS_FAMILY KROMBLAST_OS_LINUX
#endif

#ifdef DEBUG // Debug mode
#define KROMBLAST_DEBUG
#else
#define KROMBLAST_RELEASE
#endif

#endif