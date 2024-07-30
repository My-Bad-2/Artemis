#include <limine.h>

extern "C" void kernel_main() {
    for(;;) {
        asm volatile("cli");
        asm volatile("hlt");
    }
}