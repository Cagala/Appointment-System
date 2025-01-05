#include <iostream>
#include <string>

#include "../headers/IDoctorSystem.h"

#include "../headers/IClearConsole.h"
#include "../headers/IJsonHandler.h"
#include "../headers/IPasswordService.h"


int doctorLoginHandler()
{

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while(1)
    {
        std::string username, password;
        
        std::cout << "DOKTOR GİRİŞ SİSTEMİNE HOŞGELDİNİZ!" << std::endl;

        do {
                std::cout << "(Çıkış yapmak için isime 'exit' yazınız.)" << std::endl;
                std::cout << "Kullanıcı Adını Giriniz: "; 
                std::getline(std::cin, username);

                std::cout << "Şifre Giriniz: "; 
                std::getline(std::cin, password);

                if (username == "exit")
                {
                    clearConsole();
                    return 0;
                }

                if (username.empty()) 
                {
                    clearConsole();
                    std::cout << "Kullanıcı Adı boş olamaz. Lütfen tekrar girin.\n";
                }

                if (password.length() < 6)
                {
                    clearConsole();
                    std::cout << "Şifre en az 6 karakter olmalıdır. Lütfen tekrar girin.\n";
                }

                if (password.empty()) 
                {
                    clearConsole();
                    std::cout << "Şifre boş olamaz. Lütfen tekrar girin.\n";
                }

                if (isdigit(username[0]))
                {
                    clearConsole();
                    std::cout << "Kullanıcı Adı sayı olamaz. Lütfen tekrar girin.\n";
                    username = "";
                }
            
            } while (username.empty() || password.empty());

        json json_data;
        json_data = readJsonFile("doctors");
        
        if(json_data.empty() || json_data.is_null())
        {
            std::cout << "doctors.json dosyası arızalı. Tekrar oluşturuluyor..." << "\n" << std::endl;
            int result = checkJsonAndCreate("doctors");
            
            if(result){ continue; }
            else { std::cout << "doctors.json dosyası oluşturulamadı. Program kapatılıyor, daha sonra tekrar deneyiniz..." << "\n" << std::endl; return 0; }

        }

        if (!json_data.contains("doctors"))
        {
            clearConsole();
            std::cout << "doctors.json dosyası arızalı. Tekrar oluşturuluyor..." << "\n" << std::endl;
            int result = createJsonFile("doctors");
            
            if(result){ continue; }
            else { std::cout << "doctors.json dosyası oluşturulamadı. Program kapatılıyor, daha sonra tekrar deneyiniz..." << "\n" << std::endl; return 0; }

        }

        if(!json_data["doctors"].contains(username))
        {
            clearConsole();
            std::cout << "Kullanıcı adı bulunamadı! Tekrar deneyin." << "\n" << std::endl;
            continue;
        }

        std::string ePass = getMD5Password(password);

        if(json_data["doctors"][username]["password"] != ePass)
        {
            clearConsole();
            std::cout << "Şifre hatalı! Tekrar deneyin." << "\n" << std::endl;
            continue;
        }

        clearConsole();
        
        int result = doctorSystemHandler(username);

        if(result == 2 || result == 0)
        {
            return 0;
        }   

    }

    return 1;
}