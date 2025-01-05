#include <iostream>
#include <string>

#include "../headers/IClearConsole.h"
#include "../headers/IOfficeChanger.h"
#include "../headers/IBusyTimeAsigner.h"

int doctorSystemHandler(std::string doctorName)
{

    while (1)
    {
        std::cout << "Doktor Sistemine Hoşgeldiniz!\n" << std::endl;
        std::cout << "Merhaba Dr." << doctorName << "!" << std::endl;
        std::cout << "1-) Meşgul Saatleri Belirle\n2-) Ofis Değiştir\n3-) Randevulu Hastaları Görüntüle\n[0]-ÇIKIŞ YAP" << std::endl;

        int choice = 0;
        std::cout << "Sistem seçimini yapınız: ";
        std::cin >> choice; std::cout << std::endl;

        if (std::cin.fail()) {
            std::cin.clear(); // clear the error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore the invalid input
            clearConsole();
            std::cerr << "Geçersiz giriş! Lütfen bir sayı giriniz." << std::endl;
            continue;
        }

        if (choice == 1)
        {
            clearConsole();
            busyTimeAsigner(doctorName);
        }
        else if (choice == 2)
        {
            clearConsole();
            changeOffice(doctorName);
        }
        else if (choice == 0)
        {
            clearConsole();
            return 2;
        }
        else
        {
            clearConsole();
            std::cout << "Geçerli bir seçenek girmediniz! Tekrar deneyin!" << std::endl;
        }
    }


    return 1;
}