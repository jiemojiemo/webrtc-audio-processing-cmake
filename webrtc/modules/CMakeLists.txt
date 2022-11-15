file(GLOB_RECURSE AUDIO_PROCESSING_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")

if(NOT have_avx2)
    list(REMOVE_ITEM AUDIO_PROCESSING_SRC "${CMAKE_CURRENT_SOURCE_DIR}/audio_processing/aec3/fft_data_avx2.cc")
    list(REMOVE_ITEM AUDIO_PROCESSING_SRC "${CMAKE_CURRENT_SOURCE_DIR}/audio_processing/aec3/matched_filter_avx2.cc")
    list(REMOVE_ITEM AUDIO_PROCESSING_SRC "${CMAKE_CURRENT_SOURCE_DIR}/audio_processing/aec3/vector_math_avx2.cc")
    list(REMOVE_ITEM AUDIO_PROCESSING_SRC "${CMAKE_CURRENT_SOURCE_DIR}/audio_processing/aec3/adaptive_fir_filter_avx2.cc")
    list(REMOVE_ITEM AUDIO_PROCESSING_SRC "${CMAKE_CURRENT_SOURCE_DIR}/audio_processing/aec3/adaptive_fir_filter_erl_avx2.cc")
endif()

if(NOT have_mips64)
    list(REMOVE_ITEM AUDIO_PROCESSING_SRC "${CMAKE_CURRENT_SOURCE_DIR}/audio_processing/aecm/aecm_core_mips.cc")
endif()

list(REMOVE_ITEM AUDIO_PROCESSING_SRC "${CMAKE_CURRENT_SOURCE_DIR}/audio_processing/agc2/rnn_vad/test_utils.cc")
list(REMOVE_ITEM AUDIO_PROCESSING_SRC "${CMAKE_CURRENT_SOURCE_DIR}/audio_processing/agc2/fixed_gain_controller.cc")
list(REMOVE_ITEM AUDIO_PROCESSING_SRC "${CMAKE_CURRENT_SOURCE_DIR}/audio_processing/transient/click_annotate.cc")


set(AUDIO_PROCESSING_SRC ${AUDIO_PROCESSING_SRC} PARENT_SCOPE)