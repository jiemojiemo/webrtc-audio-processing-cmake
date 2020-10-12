project('webrtc-audio-processing', 'c', 'cpp',
  version : '0.4.0',
  meson_version : '>= 0.52',
  default_options : [ 'warning_level=1',
                      'buildtype=debugoptimized',
                      'c_std=c11',
                      'cpp_std=c++14',
                    ]
)

soversion = 0

cc = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')

host_system = host_machine.system()

platform_cflags = []
os_cflags = []
os_deps = []
have_posix = false
have_win = false

absl_dep = [
  cpp.find_library('absl_base'),
  cpp.find_library('absl_bad_optional_access'),
  cpp.find_library('absl_flags_internal'),
  cpp.find_library('absl_flags_marshalling'),
  cpp.find_library('absl_flags_parse'),
  cpp.find_library('absl_flags_registry'),
  cpp.find_library('absl_flags_usage_internal'),
  cpp.find_library('absl_raw_logging_internal'),
  cpp.find_library('absl_strings'),
  cpp.find_library('absl_synchronization'),
  cpp.find_library('absl_throw_delegate'),
]

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
if ['arm', 'armv7'].contains(host_machine.cpu_family())
  if cc.compiles('''#ifdef __ARM_ARCH_ISA_ARM
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
    neon_opt += ['-DWEBRTC_DETECT_NEON', '-mfpu=neon']
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
    name: 'webrtc-audio-processing',
    description: 'WebRTC Audio Processing library',
    version: meson.project_version(),
    filebase: 'webrtc-audio-processing',
    subdirs: 'webrtc_audio_processing',
    extra_cflags: [
      '-DWEBRTC_LIBRARY_IMPL',
    ] + platform_cflags,
    libraries: libwebrtc_audio_processing,
)

pkgconfig.generate(
    name: 'webrtc-audio-coding',
    description: 'WebRTC Audio Coding library',
    version: meson.project_version(),
    filebase: 'webrtc-audio-coding',
    subdirs: 'webrtc_audio_processing',
    extra_cflags: [
      '-DWEBRTC_LIBRARY_IMPL',
    ] + platform_cflags,
    libraries: libwebrtc_audio_coding,
)
