#include "vertex.h"
#include "parse_input.h"
#include <iostream>
#include <string>

int main(){
    std::string inputstr;
    std::cout << "Enter reference determinant substitution level followed by operator sequence:" << '\n';
    std::getline(std::cin, inputstr);
    parse_input(inputstr);
   return 0;
}
