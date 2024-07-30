kernel_include = [include_directories('include')]

kernel_sources = files('main.cpp')

clangtidy_files += kernel_sources

kernel = executable(
    'kernel.elf',
    kernel_sources,
    build_by_default: true,
    include_directories: kernel_include,
    dependencies: dependencies,
    install: false,
    link_with: [llibc],
)