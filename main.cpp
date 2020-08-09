#include "vertex.h"
#include "inputparser.h"
#include <iostream>
#include <string>

int main(){
    std::string inputstr;
    std::cout << "Enter reference determinant substitution level followed by operator sequence:" << '\n';
    std::getline(std::cin, inputstr);
    inputparser(inputstr);
   return 0;
}
