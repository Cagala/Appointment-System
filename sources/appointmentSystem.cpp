#include <iostream>
#include <string>
#include <set>
#include <map>
#include <regex>

#include "../headers/IClearConsole.h"
#include "../headers/IJsonHandler.h"
#include "../headers/IBusyTimeAsigner.h"

using json = nlohmann::json;

int TcNoValidator(std::string tc_no)
{
    if (tc_no.length() != 11)
    {
        std::cerr << "Hata: TC Kimlik numarası 11 haneli olmalıdır!" << std::endl;
        return 0;
    }

    for (int i = 0; i < 11; i++)
    {
        if (!isdigit(tc_no[i]))
        {
            std::cerr << "Hata: TC Kimlik numarası sadece rakamlardan oluşmalıdır!" << std::endl;
            return 0;
        }
    }

    if (tc_no[0] == '0')
    {
        std::cerr << "Hata: TC Kimlik numarasının ilk hanesi 0 olamaz!" << std::endl;
        return 0;
    }

    int sum1 = 0;
    int sum2 = 0;

    for (int i = 0; i < 9; i++)
    {
        if (i % 2 == 0)
        {
            sum1 += tc_no[i] - '0';
        }
        else
        {
            sum2 += tc_no[i] - '0';
        }
    }

    if ((sum1 * 7 - sum2) % 10 != tc_no[9] - '0')
    {
        return 0;
    }

    return 1;
}

std::set<std::string> extractUniqueOffices(const json& officeJson) {
    std::set<std::string> uniqueOffices;
    std::regex suffixRegex(R"( Poliklinik\s+\w+$)"); // Poliklinik ve sonrasındaki rakam/sayıyı eşler

    for (auto& [key, value] : officeJson["offices"].items()) {
        std::string baseName = std::regex_replace(key, suffixRegex, " Poliklinik");
        uniqueOffices.insert(baseName);
    }

    return uniqueOffices;
}

std::vector<std::string> getDoctorsFromOffice(const json& officeJson, const std::string& selectedOffice) 
{
    std::vector<std::string> doctors;

    for (const auto& [key, value] : officeJson["offices"].items()) {
        if (key.find(selectedOffice) != std::string::npos) { // Seçilen polikliniği kontrol et
            std::string doctorName = value["doctor"];
            if (!doctorName.empty()) {
                doctors.push_back(doctorName);
            }
        }
    }

    return doctors;
}

std::vector<std::string> getDoctorsFromGeneral(const json& doctorJson) 
{
    std::vector<std::string> doctors;

    for (const auto& [key, value] : doctorJson["doctors"].items()) {
        if (doctorJson["doctors"][key]["office"] == "Genel Poliklinik") {
            std::string doctorName = key;
            if (!doctorName.empty()) {
                doctors.push_back(doctorName);
                if (doctors.size() == 3) {
                    break; // İlk 3 doktoru aldıktan sonra dur
                }
            }
        }
    }

    return doctors;
}

int handleDoctorSelection(const std::string& selectedOffice, std::string& selectedDoctor) 
{
    std::vector<std::string> doctors;
    json doctor_json = readJsonFile("doctors");
    json office_json = readJsonFile("offices");

    if (selectedOffice == "Genel Poliklinik") {
        doctors = getDoctorsFromGeneral(doctor_json);
    } else {
        doctors = getDoctorsFromOffice(office_json, selectedOffice);
    }

    if (doctors.empty()) {
        std::cout << "Maalesef, seçtiğiniz poliklinikte mevcut doktor bulunmamaktadır!\nLütfen başka bir poliklinik veya Genel Poliklinik seçiniz." << std::endl;
        return 2;
    }

    std::cout << "Mevcut doktorlar: " << std::endl;
    for (size_t i = 0; i < doctors.size(); ++i) {
        std::cout << (i + 1) << "-) " << doctors[i] << std::endl;
    }

    int doctorChoice;
    do {
        std::cout << "Randevu almak istediğiniz doktoru seçiniz: ";
        std::cin >> doctorChoice;

        if (doctorChoice < 1 || doctorChoice > doctors.size() || std::cin.fail()) {
            std::cout << "Geçersiz bir seçim yaptınız, lütfen tekrar deneyin." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            break;
        }
    } while (true);

    selectedDoctor = doctors[doctorChoice - 1];
    return 1;
}

void updateDoctorPatients(json& doctorJson, const std::string& doctorName, const std::string& patientName, int appointmentTime) {
    json newAppointment = { {"patientName", patientName}, {"date", appointmentTime} };
    doctorJson["doctors"][doctorName]["patients"].push_back(newAppointment);
}

int appointmentSystemHandler() {
    json office_json = readJsonFile("offices");
    json doctor_json = readJsonFile("doctors");
    std::set<std::string> uniqueOffices = extractUniqueOffices(office_json);

    while (1) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string tc_no;

        std::cout << "Randevu Sistemine Hoşgeldiniz!" << std::endl;
        std::cout << "TC'nizi giriniz: ";
        std::cin >> tc_no;
        std::cout << std::endl;

        if (!TcNoValidator(tc_no)) {
            clearConsole();
            std::cerr << "Hata: TC Kimlik numarası geçersiz!" << std::endl;
            continue;
        }

        std::string patientName;
        do {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "(Çıkış yapmak için 'exit' yazınız.)" << std::endl;
            std::cout << "İsminizi Girin: ";
            std::getline(std::cin, patientName);

            if (patientName == "exit") {
                clearConsole();
                return 0;
            }

            if (patientName.empty() || isdigit(patientName[0])) {
                std::cout << "Geçersiz isim girdiniz. Lütfen tekrar deneyin.\n";
                patientName = "";
            }
        } while (patientName.empty());

        int errorCode = 0;
        do {
            std::map<int, std::string> dict;
            std::cout << "1-) Genel Poliklinik" << std::endl;
            dict[0] = "Genel Poliklinik";

            int counter = 1;
            for (const auto& office : uniqueOffices) {
                counter++;
                std::cout << counter << "-) " << office << std::endl;
                dict[counter - 1] = office;
            }

            if (errorCode == 1) {
                std::cout << "Geçersiz poliklinik seçimi yaptınız. Lütfen tekrar deneyin." << std::endl;
            }

            int choice;
            std::cout << "Randevu almak istediğiniz polikliniği seçiniz: ";
            std::cin >> choice;
            std::cout << std::endl;

            if (choice < 1 || choice > counter || std::cin.fail()) {
                clearConsole();
                errorCode = 1;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            std::string selectedOffice = dict[choice - 1];
            std::string selectedDoctor;
            int result = handleDoctorSelection(selectedOffice, selectedDoctor);

            if (result == 2) {
                std::cout << "Seçtiğiniz poliklinikte doktor bulunamadı. Lütfen başka bir poliklinik seçiniz.\n";
                continue;
            }

            std::cout << "Randevu başlama saatini giriniz (SS:MM): ";
            std::string appointmentTime;
            std::cin >> appointmentTime;
            int startTime = convertTime(appointmentTime);

            int endTime = startTime + 20;
            std::vector<std::pair<int, int>> busyTimes = { {startTime, endTime} };

            std::vector<std::pair<int, int>> currentAvailableTimes;
            currentAvailableTimes.clear();
            for (const auto& time : doctor_json["doctors"][selectedDoctor]["availableTimes"]) 
            {
            int start = time["start"];
            int end = time["end"];
            
            currentAvailableTimes.emplace_back(start, end);
            }
            auto updatedTimes = updateAvailableTimes(currentAvailableTimes, busyTimes);

            updateDoctorAvailableTimes(doctor_json["doctors"][selectedDoctor], updatedTimes);
            updateDoctorPatients(doctor_json, selectedDoctor, patientName, startTime);

            writeJson("doctors", doctor_json);

            std::cout << "Randevunuz başarıyla oluşturuldu.\n";
            return 1;

        } while (errorCode);
    }
}