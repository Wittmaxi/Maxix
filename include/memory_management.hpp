#include <stddef.h>
#include <util/memory_landmark.hpp>

namespace MEM {
    void initialize();

    void* allocate(size_t size);

    extern landmark* entry;
}

