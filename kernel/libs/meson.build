kernel_sources += files(
    'stdio.cpp',
    'abort.cpp',
    'stacktrace.cpp',
    'assert.cpp',
    'cxa_at_exit.cpp'
)

if get_option('ubsan')
    kernel_sources += files('ubsan.cpp')
endif