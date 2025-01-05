#include <iostream>
#include <fstream>
#include <string>
#include <codecvt>
#include <locale>
#include <filesystem>

#include "../headers/IPasswordService.h"

#include "../include/json.hpp"


#define JSON_FILE_PATH(fileName) (std::string("D:\\CCCCC\\resources\\") + fileName + ".json")

using json = nlohmann::json;

// İstenilen json dosyasını okur ve json objesi döndürür
json readJsonFile(std::string jsonFileName)
{

    std::ifstream json_file(JSON_FILE_PATH(jsonFileName));
    json json_data;

    if(json_file.is_open()){
        json_file >> json_data;
        json_file.close();
    } else {
        std::cerr << "Dosya Açılamadı!" << std::endl;
        return {};
    }

    return json_data;
}

int createJsonFile(std::string jsonFileName)
{
    // Dosya yoksa, ::jsonfile:: isminde bir json oluştur ve içine ::jsonfile:: :{} kısmını ekle
    json initial_data;
    initial_data[jsonFileName] = json::object();

    std::filesystem::remove(JSON_FILE_PATH(jsonFileName));
    
    try
    {
        std::ofstream output_file(JSON_FILE_PATH(jsonFileName), std::ios::binary);

        if (output_file.is_open()) {
            output_file << std::setw(3) << initial_data;
            output_file.close();
        } else {
            std::cerr << "Dosya oluşturulamadı!" << std::endl;
            return 0;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        std::filesystem::remove(JSON_FILE_PATH(jsonFileName));
        return 0;
    }

    return 1;
}

// :params:
// return 0 = Dosya oluşturma adımlarında iş mantığı oluşturulamadı.
// return 1 == Dosya zaten var.
// return 2 == Dosya yoktu ve oluşturuldu, önceki dosya içeriği varsa da silindi anlamına geliyor.
// 
int checkJsonAndCreate(std::string jsonFileName)
{
    if (!std::filesystem::exists(JSON_FILE_PATH(jsonFileName))) {
        int result = createJsonFile(jsonFileName);
        
        if (result == 1) { return 2; }
        else { return 0; }
    }

    return 1;
}

int writeJson(std::string jsonFileName, json json_data)
{

    // Dosyaya yaz
    std::ofstream output_file(JSON_FILE_PATH(jsonFileName), std::ios::binary);

    if(output_file.is_open()){
        output_file << std::setw(3) << json_data;
        output_file.close();
    } else {
        std::cerr << "Dosyaya yazılamadı!" << std::endl;
        return 0;
    }

    return 1;
}

int createDoctor(std::string doctorName, std::string doctorPassword)
{
    // Hatalı input kontrolü
    if(doctorName == "" || doctorPassword == "")
    {
        std::cerr << "İsim veya Şifre hatalı!" << std::endl;
        return 0;
    } 

    if (!checkJsonAndCreate("doctors")) { return 0; };

    json json_data;
    json DocData;

    json_data = readJsonFile("doctors");

    for(auto& [key, val] : json_data["doctors"].items())
    {
        if(key != doctorName)
        {
            continue;
        } else{
            std::cerr << "Eklemek istediğiniz kullanıcı zaten var!" << std::endl;
            return 0;
        }
    }

    std::string password = getMD5Password(doctorPassword);

    DocData["password"] = password;
    DocData["office"] = "Genel Poliklinik";
    DocData["availableTimes"] = {{{"start", 510}, {"end", 1050}}}; // 8:30-17:30 mesai saatleri
    DocData["patients"] = {};

    json_data["doctors"][doctorName] = DocData;

    if (!writeJson("doctors", json_data)) { return 0; }

    return 1;
}

int checkAndCreateOffices()
{
    int jsonResult = checkJsonAndCreate("offices");

    if (jsonResult == 1) { return 1; }
    if (!jsonResult) { return 0; }

    json json_data;
    json office_data;

    json_data = readJsonFile("offices");
    
    json_data["offices"]["KBB Poliklinik I"] = {{"doctor", ""}};
    json_data["offices"]["KBB Poliklinik II"] = {{"doctor", ""}};
    json_data["offices"]["KBB Poliklinik III"] = {{"doctor", ""}};

    json_data["offices"]["Göz Poliklinik I"] = {{"doctor", ""}};
    json_data["offices"]["Göz Poliklinik II"] = {{"doctor", ""}};
    json_data["offices"]["Göz Poliklinik III"] = {{"doctor", ""}};

    json_data["offices"]["Dahiliye Poliklinik I"] = {{"doctor", ""}};
    json_data["offices"]["Dahiliye Poliklinik II"] = {{"doctor", ""}};
    json_data["offices"]["Dahiliye Poliklinik III"] = {{"doctor", ""}};

    json_data["offices"]["Cildiye Poliklinik I"] = {{"doctor", ""}};
    json_data["offices"]["Cildiye Poliklinik II"] = {{"doctor", ""}};
    json_data["offices"]["Cildiye Poliklinik III"] = {{"doctor", ""}};

    json_data["offices"]["Kardiyoloji Poliklinik I"] = {{"doctor", ""}};
    json_data["offices"]["Kardiyoloji Poliklinik II"] = {{"doctor", ""}};
    json_data["offices"]["Kardiyoloji Poliklinik III"] = {{"doctor", ""}};

    if (!writeJson("offices", json_data)) { return 0; }

    return 1;

}

/*void test(string doctorName)
{
    std::string Data{R"(
            {
            "doctors":{
                "John":{"name":"aa", "surname":"bb"},
                "Walker":{"name":"ww", "surname":"cc"}
            }
        }
    )"};

    json DocData;
    DocData["avaibleTimes"] = {{{"start", 630},{"end",700}},{{"start",710},{"end", 740}}};
    DocData["Password"] = "asc2rfs5hd75s";

    json Doc = {{doctorName, DocData}};

    std::cout << Doc.dump() << std::endl;

    json jsonData = json::parse(Data);
    std::cout << jsonData.dump(2) << std::endl;

    cout << "items:" << Doc["John Walker"]["avaibleTimes"][0]["start"];

    jsonData["doctors"][doctorName] = DocData;
    cout << jsonData.dump();

}*/