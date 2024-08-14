#include <logger.hpp>

extern "C" void abort() {
    LogPanic("abort() called!");
}