#include "vertex.h"
#include "parseinput.h"
#include "creategraph.h"
#include <iostream>
#include <string>
#include <deque>

int main(){

    // Test inputparser function
    std::string input;
    std::deque<std::string> inputlist;
    std::cout << "Please Enter Substitution level followed by operator sequence:" << '\n';
    std::getline(std::cin, input);
    create_graph( parse_input(input) );
    return 0;
}
