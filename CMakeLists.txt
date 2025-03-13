cmake_minimum_required(VERSION 3.15)

project(webrtc-audio-processing)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    add_definitions(-DWEBRTC_MAC)
    set(have_posix TRUE)
    set(is_mac TRUE)
elseif(CMAKE_SYSTEM_NAME MATCHES "iOS")
    add_definitions(-DWEBRTC_IOS)
    set(have_posix TRUE)
    set(is_ios TRUE)
elseif(CMAKE_SYSTEM_NAME MATCHES "Android")
    add_definitions(-DWEBRTC_ANDROID -DWEBRTC_LINUX)
    set(have_posix TRUE)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DWEBRTC_LINUX -DWEBRTC_THREAD_RR")
    add_definitions(-DWEBRTC_LINUX -DWEBRTC_THREAD_RR)
    set(have_posix TRUE)
endif()

if(have_posix)
    message(STATUS "have_posix ${have_posix}")
    add_definitions(-DWEBRTC_POSIX)
endif()


include(CheckCXXSourceCompiles)

check_cxx_source_compiles("
#ifndef __ARM_ARCH_ISA_ARM
#error no arm arch
#endif
int main(){return 0;}
" have_arm)

check_cxx_source_compiles("
#ifndef __ARM_ARCH_7A__
#error no armv7 arch
#endif
int main(){return 0;}
" have_armv7)

check_cxx_source_compiles("
#ifndef __aarch64__
#error no aarch64 arch
#endif
int main(){return 0;}
" have_arm64)

message(STATUS "have_arm: ${have_arm}")
message(STATUS "have_armv7: ${have_armv7}")
message(STATUS "have_arm64: ${have_arm64}")

if (have_arm)
    add_definitions(-DWEBRTC_ARCH_ARM)
endif ()
if(have_armv7)
    add_definitions(-DWEBRTC_ARCH_ARM_V7)
endif()
if(have_arm64)
    add_definitions(-DWEBRTC_ARCH_ARM64 -DWEBRTC_HAS_NEON)
    set(have_neon TRUE)
endif()
message(STATUS "${CMAKE_SYSTEM_PROCESSOR}")
if (CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(x86_64)|(i686)")
    add_definitions(-DWEBRTC_ENABLE_AVX2)
    set(have_avx2 TRUE)
endif()

if (CMAKE_SYSTEM_PROCESSOR MATCHES "(mips)|(mips64)")
    add_definitions(-DWEBRTC_ARCH_MIPS_FAMILY)
    set(have_mips TRUE)
endif()

if (CMAKE_SYSTEM_PROCESSOR MATCHES "(mips64)")
    set(have_mips64 TRUE)
endif()

if (have_neon)
    add_definitions(-DWEBRTC_HAS_NEON)
endif()

add_definitions(-DWEBRTC_LIBRARY_IMPL -DNDEBUG)

add_subdirectory(abseil-cpp EXCLUDE_FROM_ALL)
add_subdirectory(webrtc)

add_subdirectory(example)