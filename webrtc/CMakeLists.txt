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

add_library(libwebrtc_audio_processing STATIC
        ${API_SRC}
        ${AUDIO_SRC}
        ${COMMON_AUDIO_SRC}
        ${AUDIO_PROCESSING_SRC}
        ${SYSTEM_WRAPPERS_SRC}
        ${RTC_BASE_SRC}
        ${THIRD_PARTY_SRC}
        wrapper/webrtc_apm_wrapper.cpp
        )
target_compile_definitions(libwebrtc_audio_processing PRIVATE WEBRTC_APM_DEBUG_DUMP=0)

target_include_directories(libwebrtc_audio_processing PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
        )

target_link_libraries(libwebrtc_audio_processing PUBLIC absl::optional absl::throw_delegate absl::strings)
if(is_mac OR is_ios)
    target_link_libraries(libwebrtc_audio_processing PRIVATE "-framework Foundation")
endif()