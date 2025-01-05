#include <iostream>
#include <string>
#include<map>

#include "../headers/IJsonHandler.h"
#include "../headers/IClearConsole.h"

#include "../include/json.hpp"

using json = nlohmann::json;

int changeOffice(std::string doctorName)
{

    checkAndCreateOffices();

    json json_data;
    std::map<int, std::string> dict;

    /* :errorcodes:
        0 : Hata yok
        1 : Hatalı ofis seçimi
        2 : Seçilen ofis halihazırda kullanıcı tarafından kullanılıyor
        3 : Seçilen ofis başka bir doktor tarafından kullanılıyor
        4 : Input hatası. int yerine string girilmiş */
    int errorCode = 0;

    int tempChoice = 0;

    json_data = readJsonFile("offices");

    while(1)
    {


        json doctor_json = readJsonFile("doctors");

        std::cout << "Mevcut Ofis: " << doctor_json["doctors"][doctorName]["office"] << std::endl;
        std::cout << "1-) Genel Poliklinik" << std::endl;
        dict[0] = "Genel Poliklinik";

        int counter = 1;
        for(auto& [key, val] : json_data["offices"].items())
        {
            counter++;
            
            std::string doctor = val["doctor"];

            if(doctor == "") { doctor = "Boşta"; }
            if(doctor == doctorName) {doctor = doctor + " (Siz)";}

            std::cout << counter << "-) " << key << " : " << doctor << std::endl;
            dict[counter-1] = key;

        }

        switch (errorCode)
        {
            case 1:
                std::cout << "Hatalı ofis seçimi yaptınız! Yapılan seçim: " << tempChoice <<std::endl;
                errorCode = 0;
                break;

            case 2:
                std::cout << "Seçilen ofis halihazırda size kayıtlı! Seçilen ofis: " << dict[tempChoice-1] << std::endl;
                errorCode = 0;
                break;

            case 3:
                std::cout << "Seçilen ofis başka bir doktor tarafından kullanılıyor! Seçilen ofis: " << dict[tempChoice-1] << std::endl;
                errorCode = 0;
                break;

            case 4:
                std::cout << "Geçersiz bir giriş yaptınız. Lütfen sadece sayı girişi yapınız!" << std::endl;
                errorCode = 0;
                break;

            default:
                break;
        }


        int choice;

        std::cout << "Değiştirmek istediğiniz polikliniği seçiniz: ";
        std::cin >> choice; std::cout << std::endl;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearConsole();
            errorCode = 4;
            continue;
        }

        if (choice < 1 || choice > counter)
        {
            clearConsole();
            errorCode = 1;
            tempChoice = choice;
            continue;
        }


        if (choice == 1)
        {
            json doc_json = readJsonFile("doctors");
            json office_json = readJsonFile("offices");

            std::string docCurrentOffice = doc_json["doctors"][doctorName]["office"];

            if (docCurrentOffice == "Genel Poliklinik")
            {
                clearConsole();
                tempChoice = choice;
                errorCode = 2;
                continue;
            }

            doc_json["doctors"][doctorName]["office"] = "Genel Poliklinik";
            office_json["offices"][docCurrentOffice]["doctor"] = "";
            
            if (!writeJson("doctors", doc_json)) { return 0; }
            if (!writeJson("offices", office_json)) { return 0; }

            std::cout << "Eski Ofis: " << docCurrentOffice << "\nYeni Ofis: Genel Poliklinik"<< std::endl;
            
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ofis kayıt sisteminden çıkmak için entera basınız..."; std::cin.get();
            clearConsole();
            break;
        }
        else 
        {
            std::string new_office = dict[choice-1];

            json office_json = json_data["offices"][new_office];
            json doc_json = readJsonFile("doctors");

            if(office_json["doctor"] == doctorName)
            {
                clearConsole();
                tempChoice = choice;
                errorCode = 2;
                continue;
            }
            
            if(office_json["doctor"] != "")
            {
                clearConsole();
                tempChoice = choice;
                errorCode = 3;
                continue;
            }

            std::string docCurrentOffice = doc_json["doctors"][doctorName]["office"];

            doc_json["doctors"][doctorName]["office"] = new_office;
            json_data["offices"][dict[choice-1]]["doctor"] = doctorName;

            if (docCurrentOffice != "Genel Poliklinik")
            {
                json_data["offices"][docCurrentOffice]["doctor"] = "";
            }
            
            
            if (!writeJson("doctors", doc_json)) { return 0; }
            if (!writeJson("offices", json_data)) { return 0; }
            
            std::cout << "Eski Ofis: " << docCurrentOffice << "\nYeni Ofis: " << new_office << std::endl;
            
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ofis kayıt sisteminden çıkmak için entera basınız..."; std::cin.get();
            clearConsole();
            break;
        }

    }

    return 1;
}