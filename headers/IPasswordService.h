#ifndef IPASSWORDSERVICE_H
#define IPASSWORDSERVICE_H

#include <string>

#include "../sources/passwordService.cpp"

std::string getMD5Password(std::string password);

#endif // IPASSWORDSERVICE_H