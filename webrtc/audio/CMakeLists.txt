file(GLOB_RECURSE AUDIO_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")

set(AUDIO_SRC ${AUDIO_SRC} PARENT_SCOPE)