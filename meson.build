project('webrtc-audio-processing', 'c', 'cpp',
  version : '1.1',
  meson_version : '>= 0.54',
  default_options : [ 'warning_level=1',
                      'buildtype=debugoptimized',
                      'c_std=c11',
                      'cpp_std=c++14',
                    ]
)

version_split = meson.project_version().split('.')
# This will be incremented each time a breaking API change occurs
major_version = version_split[0]
# This will be incremented when there are backwards-compatible changes
minor_version = version_split[1]

# We maintain per-package versions to not have to break API for one if only the
# other has breaking changes

apm_major_version = major_version
apm_minor_version = minor_version
apm_version = apm_major_version + '.' + apm_minor_version
apm_project_name = 'webrtc-audio-processing-' + apm_major_version

ac_major_version = major_version
ac_minor_version = minor_version
ac_version = ac_major_version + '.' + ac_minor_version
ac_project_name = 'webrtc-audio-coding-' + ac_major_version

include_subdir = apm_project_name

cc = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')

host_system = host_machine.system()

platform_cflags = []
os_cflags = []
os_deps = []
have_posix = false
have_win = false

# Let's use pkg-config if available. This will also fallback to the subproject
# if pkg-config is not found, instead of CMake or manual library detection.
# This might be surprising, but is hopefully a temporary state until recent
# abseil versions become the norm.
absl_base_dep = dependency('absl_base', required : false,
  fallback: [ 'abseil-cpp', 'absl_base_dep' ]
)
absl_flags_dep = dependency('absl_flags_parse', required : false,
  fallback: [ 'abseil-cpp', 'absl_flags_dep' ]
)
absl_optional_dep = dependency('absl_optional', required : false,
  fallback: [ 'abseil-cpp', 'absl_types_dep' ]
)
absl_strings_dep = dependency('absl_strings', required : false,
  fallback: [ 'abseil-cpp', 'absl_strings_dep' ]
)
absl_synchronization_dep = dependency('absl_synchronization', required : false,
  fallback: [ 'abseil-cpp', 'absl_synchronization_dep' ]
)

# If we have the base dep, assume the rest should be present to
if absl_base_dep.found()
  absl_dep = [
    absl_base_dep,
    absl_flags_dep,
    absl_optional_dep,
    absl_strings_dep,
    absl_synchronization_dep,
  ]
else
  warning('Could not find abseil-cpp with pkg-config, trying CMake-based library detection.')
  absl_dep = dependency('absl', method : 'cmake',
    modules : [
      'absl::base',
      'absl::flags_parse',
      'absl::optional',
      'absl::strings',
      'absl::synchronization',
    ],
    required : false,
  )

  if not absl_dep.found()
    warning('Could not find abseil-cpp with CMake, using fallback library detection which may fail.')
    absl_libs = [
      'absl_base',
      'absl_bad_optional_access',
      'absl_city',
      'absl_flags_commandlineflag',
      'absl_flags_commandlineflag_internal',
      'absl_flags_config',
      'absl_flags_internal',
      'absl_flags_marshalling',
      'absl_flags_parse',
      'absl_flags_private_handle_accessor',
      'absl_flags_program_name',
      'absl_flags_reflection',
      'absl_flags_usage',
      'absl_flags_usage_internal',
      'absl_hash',
      'absl_int128',
      'absl_malloc_internal',
      'absl_raw_logging_internal',
      'absl_spinlock_wait',
      'absl_stacktrace',
      'absl_str_format_internal',
      'absl_strings',
      'absl_symbolize',
      'absl_synchronization',
      'absl_throw_delegate',
      'absl_time',
      'absl_time_zone',
    ]
    absl_dep = []
    foreach l : absl_libs
      absl_dep += cpp.find_library(l, required : false)
    endforeach
  endif
endif

if ['darwin', 'ios'].contains(host_system)
  os_cflags = ['-DWEBRTC_MAC']
  if host_system == 'ios'
    os_cflags += ['-DWEBRTC_IOS']
  endif
  have_posix = true
elif host_system == 'android'
  os_cflags += ['-DWEBRTC_ANDROID', '-DWEBRTC_LINUX']
  os_deps += [cc.find_library('log')]
  os_deps += [dependency('gnustl', required : get_option('gnustl'))]
  have_posix = true
elif host_system == 'linux'
  os_cflags += ['-DWEBRTC_LINUX', '-DWEBRTC_THREAD_RR']
  os_deps += [cc.find_library('rt', required : false)]
  os_deps += [dependency('threads')]
  have_posix = true
elif host_system == 'windows'
  platform_cflags += ['-DWEBRTC_WIN', '-D_WIN32', '-U__STRICT_ANSI__']
  # this one is for MinGW to get format specifiers from inttypes.h in C++
  platform_cflags += ['-D__STDC_FORMAT_MACROS=1']
  os_deps += [cc.find_library('winmm')]
  have_win = true
endif

if have_posix
  platform_cflags += ['-DWEBRTC_POSIX']
endif

arch_cflags = []
have_arm = false
have_armv7 = false
have_neon = false
have_mips = false
have_mips64 = false
have_x86 = false
have_avx2 = false
if host_machine.cpu_family() == 'arm'
  if cc.compiles('''#ifndef __ARM_ARCH_ISA_ARM
#error no arm arch
#endif''')
    have_arm = true
    arch_cflags += ['-DWEBRTC_ARCH_ARM']
  endif
  if cc.compiles('''#ifndef __ARM_ARCH_7A__
#error no armv7 arch
#endif''')
    have_armv7 = true
    arch_cflags += ['-DWEBRTC_ARCH_ARM_V7']
  endif
endif
if cc.compiles('''#ifndef __aarch64__
#error no aarch64 arch
#endif''')
  have_neon = true
  arch_cflags += ['-DWEBRTC_ARCH_ARM64', '-DWEBRTC_HAS_NEON']
endif
if ['mips', 'mips64'].contains(host_machine.cpu_family())
  have_mips = true
  arch_cflags += ['WEBRTC_ARCH_MIPS_FAMILY']
endif
if host_machine.cpu_family() == 'mips64'
  have_mips64 = true
endif
if ['x86', 'x86_64'].contains(host_machine.cpu_family())
  have_x86 = true
  # This is unconditionally enabled for now, actual usage is determined by
  # runtime CPU detection, so we're just assuming the compiler supports avx2
  have_avx2 = true
  arch_cflags += ['-DWEBRTC_ENABLE_AVX2']
endif

neon_opt = get_option('neon')
if neon_opt != 'no'
  if neon_opt != 'runtime'
    if cc.compiles('#include <arm_neon.h>', args : '-mfpu=neon')
      arch_cflags += ['-mfpu=neon', '-DWEBRTC_HAS_NEON']
      have_neon = true
    endif
  else
    arch_cflags += ['-DWEBRTC_DETECT_NEON', '-mfpu=neon']
    have_neon = true
  endif
endif

common_cflags = [
  '-DWEBRTC_LIBRARY_IMPL',
  '-DWEBRTC_ENABLE_SYMBOL_EXPORT',
  '-DNDEBUG'
  ] + platform_cflags + os_cflags + arch_cflags
common_cxxflags = common_cflags
common_deps = os_deps + [absl_dep]
webrtc_inc = include_directories('.')

subdir('webrtc')

pkgconfig = import('pkgconfig')

pkgconfig.generate(
    name: apm_project_name,
    description: 'WebRTC Audio Processing library',
    version: apm_major_version + '.' + apm_minor_version,
    filebase: apm_project_name,
    subdirs: include_subdir,
    extra_cflags: [
      '-DWEBRTC_LIBRARY_IMPL',
    ] + platform_cflags,
    libraries: libwebrtc_audio_processing,
)

audio_processing_dep = declare_dependency(link_with : libwebrtc_audio_processing,
    include_directories : [webrtc_inc])

meson.override_dependency(apm_project_name, audio_processing_dep)

pkgconfig.generate(
    name: ac_project_name,
    description: 'WebRTC Audio Coding library',
    version: ac_major_version + '.' + ac_minor_version,
    filebase: ac_project_name,
    subdirs: include_subdir,
    extra_cflags: [
      '-DWEBRTC_LIBRARY_IMPL',
    ] + platform_cflags,
    libraries: libwebrtc_audio_coding,
)

audio_coding_dep = declare_dependency(link_with : libwebrtc_audio_coding,
    include_directories : [webrtc_inc])

meson.override_dependency(ac_project_name, audio_coding_dep)
