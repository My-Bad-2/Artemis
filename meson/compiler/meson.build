common_warning_flags = [
    '-Wno-unknown-pragmas',
    '-Wno-c++98-compat',
    '-Wno-c++98-compat-pedantic',

    '-Wno-padded',
    '-Wno-exit-time-destructors',
    '-Wno-global-constructors',
    '-Wno-covered-switch-default',

    '-Wfloat-equal',
    '-Wconversion',
    '-Wlogical-op',
    '-Wundef',
    '-Wredundant-decls',
    '-Wshadow',
    '-Wstrict-overflow=2',
    '-Wwrite-strings',
    '-Wpointer-arith',
    '-Wcast-qual',
    '-Wformat=2',
    '-Wformat-truncation',
    '-Wmissing-include-dirs',
    '-Wcast-align',
    '-Wswitch-enum',
    '-Wsign-conversion',
    '-Wdisabled-optimization',

    '-Winvalid-pch',
    '-Wmissing-declarations',
    '-Wdouble-promotion',
    '-Wshadow',
    '-Wtrampolines',
    '-Wvector-operation-performance',
    '-Wshift-overflow=2',
    '-Wnull-dereference',
    '-Wduplicated-cond',
    '-Wshift-overflow=2',
    '-Wnull-dereference',
    '-Wduplicated-cond',
    '-Wcast-align=strict',
]

common_compile_flags = [
    '-fdiagnostics-show-option',
    '-fcolor-diagnostics',
    '-fdevirtualize',
    '-fno-stack-protector',
    '-ffreestanding',
    '-fno-pic',
    '-fno-pie',
    '-fno-omit-frame-pointer',
]

if get_option('ubsan')
    common_compile_flags += ['-fsanitize=undefined']
endif

common_link_flags = ['-Wl,-dead_strip', '-Wl,--gc-sections']