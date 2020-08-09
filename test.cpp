#include "vertex.h"
#include "inputparser.h"
#include <iostream>
#include <string>
#include <deque>

int main(){

    // Test inputparser function
    std::string input;
    std::deque<std::string> inputlist;
    std::cout << "Please Enter Substitution level followed by operator sequence:" << '\n';
    std::getline(std::cin, input);
    inputparser(input);
    return 0;
}
