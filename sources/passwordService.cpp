#include <iostream>
#include <string>

std::string getMD5Password(std::string password)
{
    return password.append("AAAAA");
}