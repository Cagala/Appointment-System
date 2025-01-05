#ifndef IJSONHANDLER_H
#define IJSONHANDLER_H

#include <string>

#include "../sources/jsonHandler.cpp"
#include "../include/json.hpp"

using json = nlohmann::json;

json readJsonFile(std::string jsonFileName);
int createDoctor(std::string doctorName, std::string doctorPassword);
int checkAndCreateOffices();
void test(std::string doctorName);

#endif // IJSONHANDLER_H