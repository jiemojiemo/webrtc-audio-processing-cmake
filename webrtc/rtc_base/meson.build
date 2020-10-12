base_sources = [
  'checks.cc',
  'event.cc',
  'event_tracer.cc',
  'experiments/field_trial_parser.cc',
  'logging.cc',
  'memory/aligned_malloc.cc',
  'platform_thread.cc',
  'platform_thread_types.cc',
  'race_checker.cc',
  'string_encode.cc',
  'string_to_number.cc',
  'string_utils.cc',
  'strings/string_builder.cc',
  'synchronization/mutex.cc',
  'synchronization/rw_lock_wrapper.cc',
  'synchronization/yield.cc',
  'synchronization/yield_policy.cc',
  'system/file_wrapper.cc',
  'time_utils.cc',
  'zero_memory.cc',
]

base_headers = [
  [ '', 'arraysize.h' ],
  [ '', 'checks.h' ],
  [ '', 'constructor_magic.h' ],
  [ '', 'deprecation.h' ],
  [ '', 'ref_count.h' ],
  [ '', 'type_traits.h' ],
  [ 'numerics', 'safe_compare.h' ],
  [ 'system', 'file_wrapper.h' ],
  [ 'system', 'inline.h' ],
  [ 'system', 'rtc_export.h' ],
]

if have_posix
  base_sources += [
    'synchronization/rw_lock_posix.cc',
  ]
elif have_win
  base_sources += [
    'synchronization/rw_lock_win.cc',
  ]
endif

foreach h : base_headers
  install_headers(
    join_paths(h[0], h[1]),
    subdir: join_paths('webrtc_audio_processing', 'rtc_base', h[0])
  )
endforeach

libbase = static_library('libbase',
    base_sources,
    dependencies: common_deps,
    include_directories: webrtc_inc,
    cpp_args : common_cxxflags
)

base_dep = declare_dependency(
    link_with: libbase
)

