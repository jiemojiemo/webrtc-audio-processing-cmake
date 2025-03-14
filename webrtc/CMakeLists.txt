add_subdirectory(api)
add_subdirectory(rtc_base)
add_subdirectory(audio)
add_subdirectory(common_audio)
add_subdirectory(modules)
add_subdirectory(system_wrappers)
add_subdirectory(third_party)

if (have_neon)
    add_compile_options(-mfloat-abi=soft)
    add_link_options(-mfloat-abi=soft)
endif()

if (have_avx2)
    add_compile_options(-mavx -mfma)
endif()

set(WBRTC_APM_SRC
        ${API_SRC}
        ${AUDIO_SRC}
        ${COMMON_AUDIO_SRC}
        ${AUDIO_PROCESSING_SRC}
        ${SYSTEM_WRAPPERS_SRC}
        ${RTC_BASE_SRC}
        ${THIRD_PARTY_SRC}
)

add_library(webrtc_apm_wrapper STATIC
        ${WBRTC_APM_SRC}
        wrapper/webrtc_apm_wrapper.cpp
        )

target_compile_definitions(webrtc_apm_wrapper PRIVATE WEBRTC_APM_DEBUG_DUMP=0)
target_link_libraries(webrtc_apm_wrapper PRIVATE absl::optional absl::throw_delegate absl::strings)
if(is_mac OR is_ios)
    target_link_libraries(webrtc_apm_wrapper PRIVATE "-framework Foundation")
endif()

target_include_directories(webrtc_apm_wrapper PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
        )