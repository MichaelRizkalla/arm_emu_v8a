#if !defined(API_H_INCLUDED_A35EE2E1_3652_495E_9291_553590DD3FCA)
    #define API_H_INCLUDED_A35EE2E1_3652_495E_9291_553590DD3FCA

    #if defined(_WIN32) && defined(_WIN64)
        #define ARMEMU_OS_WINDOWS_X64
    #elif defined(_WIN32)
        #define ARMEMU_OS_WINDOWS_X86
    #elif defined(__linux__)
        #define ARMEMU_OS_LINUX
    #else
        #error "Undefined-unsupported platform"
    #endif

    #if defined(ARMEMU_OS_WINDOWS_X64) || defined(ARMEMU_OS_WINDOWS_X86)
        #define ARMEMU_OS_WINDOWS
    #endif

    #if defined(ARMEMU_OS_WINDOWS)
        #ifndef NOMINMAX
            #define NOMINMAX
        #endif
        #ifndef WIN32_LEAN_AND_MEAN
            #define WIN32_LEAN_AND_MEAN
        #endif
        #ifndef NOMCX
            #define NOMCX
        #endif
        #ifndef NOIME
            #define NOIME
        #endif
    #endif

    #if defined(_WIN32)
        #if defined(_MSC_VER) && (_MSC_VER < 1910)
            #error This is a MSVC17 or later compiler header
        #elif defined(_MSC_VER)
            #define ARMEMU_COMPILER_MSVC
            #define ARMEMU_INTERFACE_ONLY __declspec(novtable)
            #define ARMEMU_DLL(cond)      __declspec(cond)
        #elif defined(__GNUC__)
            #define ARMEMU_COMPILER_GNUC
            #define ARMEMU_INTERFACE_ONLY
            #define ARMEMU_DLL(cond) __attribute__((cond))
        #elif defined(__clang__)
            #define ARMEMU_COMPILER_CLANG
            #define ARMEMU_INTERFACE_ONLY
            #define ARMEMU_DLL(cond) __attribute__((cond))
        #else
            #error "Unsupported compiler"
        #endif
    #elif defined(__linux__)
        #if defined(_MSC_VER)
            #error "MSVC compiler is not supported on Linux"
        #elif defined(__GNUC__)
            #define ARMEMU_COMPILER_GNUC
            #define ARMEMU_INTERFACE_ONLY
            #ifdef ARMEMU_BUILD_DLL
                #define ARMEMU_DLL(cond) [[gnu::visibility("default")]]
            #else
                #define ARMEMU_DLL(cond)
            #endif
        #elif defined(__clang__)
            #define ARMEMU_COMPILER_CLANG
            #define ARMEMU_INTERFACE_ONLY
            #ifdef ARMEMU_BUILD_DLL
                #define ARMEMU_DLL(cond) __attribute__ ((visibility ("default")))
            #else
                #define ARMEMU_DLL(cond)
            #endif
        #else
            #error "Unsupported compiler"
        #endif
    #else
        #error "Unsupport platform"
    #endif

    #define ARMEMU_EXPORT ARMEMU_DLL(dllexport)
    #define ARMEMU_IMPORT ARMEMU_DLL(dllimport)

    #define ARMEMU_LOG_NONE  0
    #define ARMEMU_LOG_ALL   1
    #define ARMEMU_LOG_OTHER 2

    #ifndef ARMEMU_LOG_TYPE
        #define ARMEMU_LOG_TYPE ARMEMU_LOG_OTHER
    #endif // ARMEMU_LOG_TYPE

    #if defined(ARMEMU_BUILD_DLL)
        #define ARMEMU_API        ARMEMU_EXPORT
        #define ARMEMU_EXPORT_API 1
        #include <API/HiddenAPI.h>
    #elif defined(ARMEMU_BUILD_TEST)
        #define ARMEMU_API
        #define ARMEMU_EXPORT_API
        #include <API/HiddenAPI.h>
    #else
        #define ARMEMU_API        ARMEMU_IMPORT
        #define ARMEMU_IMPORT_API 1
    #endif

#endif // !defined(API_H_INCLUDED_A35EE2E1_3652_495E_9291_553590DD3FCA)
