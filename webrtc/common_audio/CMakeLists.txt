set(COMMON_AUDIO_SRC
        "audio_converter.cc"
        "audio_converter.h"
        "audio_util.cc"
        "channel_buffer.cc"
        "channel_buffer.h"
        "include/audio_util.h"
        "real_fourier.cc"
        "real_fourier.h"
        "real_fourier_ooura.cc"
        "real_fourier_ooura.h"
        "resampler/include/push_resampler.h"
        "resampler/include/resampler.h"
        "resampler/push_resampler.cc"
        "resampler/push_sinc_resampler.cc"
        "resampler/push_sinc_resampler.h"
        "resampler/resampler.cc"
        "resampler/sinc_resampler.cc"
        "smoothing_filter.cc"
        "smoothing_filter.h"
        "vad/include/vad.h"
        "vad/vad.cc"
        "wav_file.cc"
        "wav_file.h"
        "wav_header.cc"
        "wav_header.h"
        "window_generator.cc"
        "window_generator.h"

        "fir_filter_c.cc"
        "fir_filter_c.h"
        "fir_filter_factory.cc"
        "fir_filter_factory.h"

        "ring_buffer.c"
        "ring_buffer.h"
        "signal_processing/auto_corr_to_refl_coef.c"
        "signal_processing/auto_correlation.c"
        "signal_processing/complex_fft_tables.h"
        "signal_processing/copy_set_operations.c"
        "signal_processing/cross_correlation.c"
        "signal_processing/division_operations.c"
        "signal_processing/downsample_fast.c"
        "signal_processing/energy.c"
        "signal_processing/filter_ar.c"
        "signal_processing/filter_ma_fast_q12.c"
        "signal_processing/get_hanning_window.c"
        "signal_processing/get_scaling_square.c"
        "signal_processing/ilbc_specific_functions.c"
        "signal_processing/include/real_fft.h"
        "signal_processing/include/signal_processing_library.h"
        "signal_processing/include/spl_inl.h"
        "signal_processing/include/spl_inl_armv7.h"
        "signal_processing/levinson_durbin.c"
        "signal_processing/lpc_to_refl_coef.c"
        "signal_processing/min_max_operations.c"
        "signal_processing/randomization_functions.c"
        "signal_processing/real_fft.c"
        "signal_processing/refl_coef_to_lpc.c"
        "signal_processing/resample.c"
        "signal_processing/resample_48khz.c"
        "signal_processing/resample_by_2.c"
        "signal_processing/resample_by_2_internal.c"
        "signal_processing/resample_by_2_internal.h"
        "signal_processing/resample_fractional.c"
        "signal_processing/spl_init.c"
        "signal_processing/spl_inl.c"
        "signal_processing/spl_sqrt.c"
        "signal_processing/splitting_filter.c"
        "signal_processing/sqrt_of_one_minus_x_squared.c"
        "signal_processing/vector_scaling_operations.c"
        "signal_processing/complex_fft.c"
        "signal_processing/dot_product_with_scale.cc"
        "signal_processing/dot_product_with_scale.h"

        "vad/include/webrtc_vad.h"
        "vad/vad_core.c"
        "vad/vad_core.h"
        "vad/vad_filterbank.c"
        "vad/vad_filterbank.h"
        "vad/vad_gmm.c"
        "vad/vad_gmm.h"
        "vad/vad_sp.c"
        "vad/vad_sp.h"
        "vad/webrtc_vad.c"

        "third_party/spl_sqrt_floor/spl_sqrt_floor.c"

        "third_party/ooura/fft_size_128/ooura_fft.cc"
        "third_party/ooura/fft_size_128/ooura_fft.h"
        "third_party/ooura/fft_size_128/ooura_fft_tables_common.h"
        "third_party/ooura/fft_size_256/fft4g.cc"
        "third_party/ooura/fft_size_256/fft4g.h"
        )

if (have_neon)
    LIST(APPEND COMMON_AUDIO_SRC
            "fir_filter_neon.cc"
            "fir_filter_neon.h"
            "resampler/sinc_resampler_neon.cc"
            "signal_processing/cross_correlation_neon.c"
            "signal_processing/downsample_fast_neon.c"
            "signal_processing/min_max_operations_neon.c"
            "third_party/ooura/fft_size_128/ooura_fft_neon.cc"
            "third_party/ooura/fft_size_128/ooura_fft_tables_neon_sse2.h"
            )
endif ()

if (have_arm)
    LIST(APPEND COMMON_AUDIO_SRC
            "signal_processing/complex_bit_reverse_arm.S"
            )
    if (have_armv7 OR have_arm64)
        LIST(APPEND COMMON_AUDIO_SRC
                "signal_processing/filter_ar_fast_q12_armv7.S"
                )
    else ()
        LIST(APPEND COMMON_AUDIO_SRC
                "signal_processing/filter_ar_fast_q12.c"
                )
    endif ()
endif ()

if (NOT have_arm)
    LIST(APPEND COMMON_AUDIO_SRC
            "signal_processing/complex_bit_reverse.c"
            "signal_processing/filter_ar_fast_q12.c"
            )
endif ()


if (CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(x86_64)|(i686)")
    LIST(APPEND COMMON_AUDIO_SRC
            "third_party/ooura/fft_size_128/ooura_fft_sse2.cc"
            "fir_filter_sse.cc"
            "fir_filter_sse.h"
            "resampler/sinc_resampler_sse.cc"
            "resampler/sinc_resampler_avx2.cc"
            )
endif ()
list(TRANSFORM COMMON_AUDIO_SRC PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(COMMON_AUDIO_SRC ${COMMON_AUDIO_SRC} PARENT_SCOPE)