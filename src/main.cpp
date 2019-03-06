#include <run_initializations.hpp>
#include <terminal.h>

extern "C"
{
    void kernel_main () {
        INIT::initialize_all();
        Termutils::cout << "Hello world of kernels";
    }  
} 