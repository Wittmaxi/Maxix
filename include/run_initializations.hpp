#pragma once
#include <vga_buffer.hpp>
#include <memory_management.hpp>

namespace INIT {
void initialize_all () {
    VGA::initialize();
    MEM::initialize();
}
}