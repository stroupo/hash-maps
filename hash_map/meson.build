hash_map_lib = library('hash_map',
    include_directories: include_directories('.'),
    install: true
)

install_headers('hash_map.h',
  subdir: 'hash_map'
)

hash_map_dep = declare_dependency(
  link_with: hash_map_lib,
)

pkg = import('pkgconfig')
pkg.generate(hash_map_lib)