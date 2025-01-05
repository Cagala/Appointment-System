#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../headers/IClearConsole.h"
#include "../headers/IJsonHandler.h"

using json = nlohmann::json;

int convertTime(const std::string time) {
    std::stringstream ss(time);
    std::string part;

    // Saat kısmını al
    std::getline(ss, part, ':');
    int hour = std::stoi(part);

    // Dakika kısmını al
    std::getline(ss, part, ':');
    int minutes = std::stoi(part);

    return hour * 60 + minutes;
}

std::string convertTime(const int time) {
    int hour = time / 60;
    int minutes = time % 60;

    std::string formattedTime = std::to_string(hour) + ":" + std::to_string(minutes);

    return formattedTime;
}

std::vector<std::pair<int, int>> updateAvailableTimes( const std::vector<std::pair<int, int>>& currentAvailableTimes,
                                                       const std::vector<std::pair<int, int>>& busyTimes) 
{
    std::vector<std::pair<int, int>> updatedAvailableTimes;

    for (const auto& available : currentAvailableTimes) {
        int start = available.first;
        int end = available.second;

        for (const auto& busy : busyTimes) {
            if (busy.first >= end || busy.second <= start) {
                continue; // Çakışma yoksa atla
            }

            if (busy.first > start) {
                updatedAvailableTimes.emplace_back(start, busy.first);
            }

            start = std::max(start, busy.second);
        }

        if (start < end) {
            updatedAvailableTimes.emplace_back(start, end);
        }
    }

    return updatedAvailableTimes;
}

void updateDoctorAvailableTimes(json& doctor, const std::vector<std::pair<int, int>>& availableTimes) {
    doctor["availableTimes"] = json::array(); // "avaibleTimes" değil, doğru anahtar olduğuna emin olun.
    for (const auto& time : availableTimes) {
        doctor["availableTimes"].push_back({
            {"start", time.first},
            {"end", time.second}
        });
    }
}

int busyTimeAsigner(std::string doctorName)
{
    const int workStart = 510; // 8:30
    const int workEnd = 1050; // 17:30

    std::vector<std::pair<int, int>> busyTimes;
    std::vector<std::pair<int, int>> currentAvailableTimes;

    json doctor_json = readJsonFile("doctors");
    json doctor = doctor_json["doctors"][doctorName];

    while(1)
    {

        std::cout << "Hoş geldiniz Dr. " << doctorName << "!" << std::endl;
        std::cout << "İşlem Öncesi mevcut müsait saatleriniz: ";

        currentAvailableTimes.clear();
        for (const auto& time : doctor["availableTimes"]) {
            int start = time["start"];
            int end = time["end"];
            
            std::cout << "(" << convertTime(start) << " - " << convertTime(end) << ") ";
            currentAvailableTimes.emplace_back(start, end);
        }
        std::cout << std::endl << "NOT: Sistemden çıkana kadar müsait saatleriniz güncellenmeyecek." << std::endl;

        std::string start, end;
        int formattedStart, formattedEnd;

        std::cout << "Başlangıç saat ve dakika gir (HH:MM formatında): ";
        std::cin >> start;

        std::cout << "Bitiş saat ve dakika gir (HH:MM formatında): ";
        std::cin >> end;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearConsole();
            std::cerr << "Hata: Geçersiz saat formatı!" << std::endl;
            continue;
        }    

        try {
            formattedStart = convertTime(start);
            formattedEnd = convertTime(end);
        } catch (const std::exception& e) {
            clearConsole();
            std::cerr << "Hata: Geçersiz saat formatı!" << std::endl;
            continue;
        }

        if(formattedStart <= workStart || formattedStart >= workEnd)
        {
            clearConsole();
            std::cerr << "Hata: Başlangıç saatini mesai saatleri arasında yapınız!" << std::endl;
            continue;
        }

        if(formattedEnd >= workEnd)
        {
            formattedEnd = 1050;
        }
        else if(formattedStart <= workStart)
        {
            clearConsole();
            std::cerr << "Hata: Bitiş saatini mesai saatleri arasında yapınız!" << std::endl;
            continue;
        }

        if (formattedStart >= formattedEnd) {
            clearConsole();
            std::cerr << "Hata: Başlangıç saati bitiş saatinden büyük olamaz!" << std::endl;
            continue;
        }

        busyTimes.emplace_back(formattedStart, formattedEnd);

        char cont;
        std::cout << "Başka zaman aralığı girmek istiyor musunuz? (E/e): ";
        std::cin >> cont;

        if (cont != 'E' && cont != 'e') {
            break;
        }

    }

        // Müsait saatleri hesapla
    auto updatedAvailableTimes = updateAvailableTimes(currentAvailableTimes, busyTimes);

    std::cout << "Güncellenmiş AvailableTimes: ";
for (const auto& time : updatedAvailableTimes) {
    std::cout << "(" << time.first << ", " << time.second << ") ";
}
std::cout << std::endl;

    updateDoctorAvailableTimes(doctor, updatedAvailableTimes);

    doctor_json["doctors"][doctorName] = doctor;
    
    writeJson("doctors", doctor_json);

    return 1;

}

