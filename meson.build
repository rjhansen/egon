project(
  'egon',
  ['c', 'cpp'],
  meson_version : '>= 1.3.0',
  version : '0.1',
  default_options : ['warning_level=3', 'c_std=c2x', 'cpp_std=c++23',],
)

executable(
  'egon',
  ['src/egon.cpp', 'src/fetch.cpp',],
  dependencies : [dependency('gpgmepp'), dependency('libssl'), dependency('libcrypto'),],
  install : true,
)
