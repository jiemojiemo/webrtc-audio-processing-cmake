set(SYSTEM_WRAPPERS_SRC
        "source/cpu_features.cc"
        "source/field_trial.cc"
        "source/metrics.cc"
        "source/sleep.cc"
        )

list(TRANSFORM SYSTEM_WRAPPERS_SRC PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(SYSTEM_WRAPPERS_SRC ${SYSTEM_WRAPPERS_SRC} PARENT_SCOPE)