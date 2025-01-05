#include <iostream>
#include <string>

#include "../headers/IJsonHandler.h"
#include "../headers/IClearConsole.h"

void clearConsole();

int registrationHandler()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while(1)
    {
        std::string fullName = u8"";
        std::string password = u8"";

        std::cout << "DOKTOR KAYIT SİSTEMİNE HOŞGELDİNİZ!" << std::endl;    

        do {
            std::cout << "(Çıkış yapmak için isime 'exit' yazınız.)" << std::endl;
            
            std::cout << "Doktor İsmini Girin: "; 
            std::getline(std::cin, fullName);

            if (fullName == "exit")
            {
                clearConsole();
                return 0;
            }

            if (fullName.empty()) {
                std::cout << "Doktor ismi boş olamaz. Lütfen tekrar girin.\n";
            }

            if (isdigit(fullName[0]))
            {
                std::cout << "Doktor ismi sayı olamaz. Lütfen tekrar girin.\n";
                fullName = "";
            }

        } while (fullName.empty());

        do {
            std::cout << "Şifre Giriniz (En az 6 karakter olmalı): "; 
            std::getline(std::cin, password);
            
            if (password == "exit")
            {
                clearConsole();
                return 0;
            }

            if (password.empty()) {
                std::cout << "Şifre boş olamaz. Lütfen tekrar girin.\n";
            }

            if (password.length() < 6)
            {
                std::cout << "\nŞifre en az 6 karakter olmalıdır. Lütfen tekrar girin.\n";
                password = "";
            }
        } while (password.empty());

        std::cout << std::endl; 

        int result = createDoctor(fullName, password);

        if(result){
            std::cout << "Doktor başarıyla oluşturuldu! Kayıt sisteminden çıkmak için entera basınız..."; std::cin.get();
            break;
        }
    }

    clearConsole();
    return 1;
}