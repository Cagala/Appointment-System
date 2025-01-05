#include <iostream>
#include <locale.h>
#include <string>

#include "./headers/IAppointmentSystem.h"
#include "./headers/IAccountSystem.h"
#include "./headers/IJsonHandler.h"
#include "./headers/IClearConsole.h"

#include "./headers/IOfficeChanger.h"
#include "./headers/IBusyTimeAsigner.h"


using namespace std;
int main() {
    setlocale(LC_ALL, "TURKISH");

    int choice;

    while (true)
    {

        cout << "##############\n";
        cout << "1-) Randevu Al\n2-) Doktor Sistemine Giris Yap\n";

        cout << "Seceneginizi Giriniz:";
        cin >> choice;


        if (choice == 1)
        {
            clearConsole();
            appointmentSystemHandler();
        }
        else if (choice == 2)
        {
            clearConsole();
            accountHandler();
        }
        else if (choice == 3)
        {
            clearConsole();
            busyTimeAsigner("John Walker");
        }
        else
        {
            clearConsole();

            cout << "Dogru Aralikta Secim Yapiniz!\n";
            cout << "##############\n\n";

        }

    }

    return 0;
}


/* A-)Muayne Randevu Alma:

1-) Kullanıcı TC'sini girer. (int)
2-) Sistem TC kimlik algoritmasına göre girilen numarayı kontrol eder ve geçersiz bir TC numarası girildiyse
kullanıcıyı uyarır ve geçerli bir TC girilene kadar işlem devam eder.
3-) Girilen TC numarasını bir JSON dosyasından kontrol eder ve daha öncesinde bu TC ile işlem yapıldıysa
kişinin ismi buradan belirlenir.
3-) Kullanıcı daha öncesinde girilen TC numarası ile bir randevu almadıysa isim-soyisim girer.
4-) doctors.json'dan çekilen doktorlardan birini seçer ve karşısına bu doktorun müsait olduğu saat aralığı
yazdırılır.
5-) Kullanıcı randevu almak istediği saati 24lük saat sistemiyle ve saat:dakika formatıyla giriş yapar.
6-) Eğer girdiği saat doluysa kullanıcıya tekrardan uygun saatleri yazar ve geçerli giriş yapana kadar tekrar
edilir.
7-) Eğer girdiği saat dolu değilse doktorun müsait olduğu saat aralığına kullanıcının girdiği saat + 30 dakika
ilerisi dolu olarak gösterilecek şekilde düzenlenir.
8-) Seçilen doktorun "patients" listesine şu kayıt girilir: {"Name:X, "Time":Y}. Burada X hastanın isim soyismi
Y ise girilen saatin 60 ile çarpılıp dakikasıyla toplandığı numerik sayıdır.
8-) Kullanıcının bilgileri ekrana yazdırılır ve altına doktorun bilgileriyle beraber muayne yeri ve saati de
yazdırılır.

B -)Doktor Sistemi:
1-) Kullanıcı tekrardan 2 seçenek ile karşı karşıya kalır:
1.a.1-) Sisteme Doktor Kayıtla: Kişi öncelikle doktorun ismini ve soyismini girer. Ardından bir şifre belirler.
1.a.2-) doctors.json'a İsimSoyisim şeklinde doktorun ismi kaydedilir ve bu kaydın içine "password" key'i ile 
beraber doktorun şifresi SHA-256 hash ile şifrelenerek yerleştirilir.
1.a.3-) Doktorun json kaydında "avaibleTimes" kısmında bir boş liste oluşturulur, "office" şeklinde bir key
oluşturulur. Ve en son "patients" olarak boş bir liste daha oluşturulur.
2.b-) Giriş Yap: Kullanıcı isim soyisim ve şifreyi sırasıyla girer.
3-) Eğer kombinasyon doğru değilse kullanıcıya hata mesajı döndürür ve doğru giriş yapılana kadar tekrar eder.
4-) Doktorun tekrardan karşısına 3 seçenek çıkar:
a-) Meşgul saatleri belirle: Doktor'a önce başlangıç ardından bitiş saatinin girilmesi istenir. Doktor
saat:dakika formatıyla bu bilgileri doldurur.
a.2-) Eğer girilen saatler halihazırda doluysa(Önceden dolu olarak belirtilmiş veya hasta tarafından alınmış)
doktora tekrar giriş yapması istenir.
a.4-) Girilen saatlerin saat kısmı 60 ile çarpılır ve dakika kısmı bu sayıya eklenir. Başlangıç ve bitiş için
girilmiş saatlerin bu formülleri sırasıyla X ve Y'de tutulur.
a.3-) Eğer girilen saat müsaitse doktorun json kaydındaki "avaibleTimes" listesinde {"start":X,"end":Y} şeklinde
bir kayıt oluşturulur.
b-) Ofis değiştir: Doktor bu seçeneği seçtiğinde program offices.json'da value'su true olan tüm ofisleri listeler.
b.2-) Doktor buradan 1-2-3 gibi tuşlarla bir seçim yapar. doktorun json kaydındaki "Office" key'i bu ofisin adıyla
değiştirilir ve seçilen ofisin valuesu false olarak değiştirilir.
b.3-) Eğer varsa doktorun eski ofisinin değeri tekrardan true olarak değiştirilir.
c-) Hastaları Görüntüle: Doktor bu seçeneği seçtiğinde program doktorun "patients" listesindeki kayıtları tek tek 
dolaşır.
c.2-) Dolaştığı kayıtlardaki "Time" kısmındaki numerik sayının 60'a göre modu alınır ve M değişkenine atanır.
c.3-) Ardından "Time" kısmındaki numerik sayı 60'a bölünür ve tam sayısı alınır. Bu değer H değişkenine atanır.
c.4-) Ekrana şu formatta alınan bilgiler yazdırılır: "Name - H:M"
En son seçenek ise programı ana menüye döndürür.*/