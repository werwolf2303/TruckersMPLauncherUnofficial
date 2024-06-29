//
// Created by werwolf2303 on 6/28/24.
//

#ifndef OSDETECT_H
#define OSDETECT_H

/**
 * Determination a platform of an operation system
 * Fully supported supported only GNU GCC/G++, partially on Clang/LLVM
 */

#if defined(_WIN32)
    #define PLATFORM_NAME "windows" // Windows
#elif defined(_WIN64)
    #define PLATFORM_NAME "windows" // Windows
#elif defined(__CYGWIN__) && !defined(_WIN32)
    #define PLATFORM_NAME "windows" // Windows (Cygwin POSIX under Microsoft Window)
#elif defined(__linux__)
    #define PLATFORM_NAME "linux" // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and other
#elif defined(__unix__) || !defined(__APPLE__) && defined(__MACH__)
    #include <sys/param.h>
    #if defined(BSD)
        #define PLATFORM_NAME "bsd" // FreeBSD, NetBSD, OpenBSD, DragonFly BSD
    #endif
#elif defined(__APPLE__) && defined(__MACH__) // Apple OSX and iOS (Darwin)
    #include <TargetConditionals.h>
    #elif TARGET_OS_MAC == 1
        #define PLATFORM_NAME "osx" // Apple OSX
    #endif
#else
    #define PLATFORM_NAME NULL
#endif

enum OSTypes {
    Windows,
    Linux,
    MacOS,
    Unix,
    Unknown
};

// Return a name of platform, if determined, otherwise - an empty string
OSTypes getPlatform() {
    std::string platformName = (PLATFORM_NAME == NULL) ? "Unknown" : PLATFORM_NAME;
    if(platformName == "windows") {
        return Windows;
    }

    if(platformName == "linux") {
        return Linux;
    }

    if(platformName == "bsd") {
        return Unix;
    }

    if(platformName == "osx") {
        return MacOS;
    }
    return Unknown;
}


