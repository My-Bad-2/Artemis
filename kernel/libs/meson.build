kernel_sources += files(
    'stdio.cpp',
    'abort.cpp',
    'stacktrace.cpp',
    'assert.cpp'
)

if get_option('ubsan')
    kernel_sources += files('ubsan.cpp')
endif