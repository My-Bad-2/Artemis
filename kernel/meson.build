kernel_include = [include_directories('include')]

if get_option('kernel_arch') == 'amd64'
    kernel_include += include_directories('include/arch/amd64')
endif

kernel_sources = files(
    'main.cpp',
    'logger.cpp',
    'artemis.cpp'
)

if get_option('kernel_arch') == 'amd64'
    subdir('arch/amd64')
endif

subdir('drivers')
subdir('libs')
subdir('memory')

clangtidy_files += kernel_sources

kernel = executable(
    'kernel.elf',
    kernel_sources,
    build_by_default: true,
    include_directories: [kernel_include, libc_include],
    dependencies: dependencies,
    install: false,
    link_with: [llibc],
)