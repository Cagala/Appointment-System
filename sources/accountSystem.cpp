#include <iostream>

#include "../headers/IAccountRegistrationSystem.h"
#include "../headers/IAccountLoginSystem.h"

#include "../headers/IClearConsole.h"

int accountHandler() 
{
    while(1)
    {
        int choice;

        std::cout << "DOKTOR VERİTABANI SİSTEMNİNE HOŞGELDİNİZ!" << std::endl;

        std::cout << "0-) Çıkış Yap\n1-) Giriş Yap\n2-) Doktor Kayıt" << std::endl;

        std::cout << "Seçenek:";
        std::cin >> choice; std::cout << std::endl;

        if (choice == 0)
        {
            clearConsole();
            break;
        }
        else if(choice == 1)
        {
            clearConsole();
            doctorLoginHandler();
        }
        else if(choice == 2)
        {
            clearConsole();
            registrationHandler();
        } 
        else {
            
            std::cout << "Geçerli bir seçenek girmediniz! Tekrar deneyin!" << std::endl;
        }
    }

    return 1;
}