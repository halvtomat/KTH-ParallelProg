#include <iostream>
#include "common.h"

int main(int argc, char const *argv[]){
    
    int arg_i = read_int_argument(argc, argv, "-n", 10);
    bool arg_b = read_bool_argument(argc, argv, "-h", false);
    std::string arg_s = read_string_argument(argc, argv, "-a", "");

    std::cout << "int argument = " << arg_i << "\nbool argument = " << arg_b << "\nstring argument = " << arg_s << std::endl;
    
    return 0;
}
