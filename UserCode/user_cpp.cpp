#include "user_cpp.hpp"
// #include <iostream>
#include <string>

using namespace std;

string str;

const char* hello_cpp()
{
    str += "Hello Cpp\n";
    str += "Are U OK\n";
    return str.c_str();
}
