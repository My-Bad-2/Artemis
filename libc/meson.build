libc_include = [include_directories('include')]

libc_sources = files('ctype.c')

subdir('abi')
subdir('string')
subdir('stdio')

clangtidy_files += libc_sources

llibc = static_library(
    'llibc',
    libc_sources,
    build_by_default: true,
    include_directories: libc_include,
    dependencies: dependencies,
    install: false,
)