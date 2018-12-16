project('stroupo-hash-maps',
  'cpp',
  version: '0.1.0',
  default_options: [
    'cpp_std=c++17',
    'buildtype=release'
  ]
)

subdir('hash_map')
subdir('tests')

executable('main',
  'main.cc',
  dependencies: hash_map_dep
)