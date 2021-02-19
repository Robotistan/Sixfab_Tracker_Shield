/*
  tracker_test.ino - This is example of testing tracker shield roughly.
  Created by Saeed Johar (saeedjohar), October 2, 2019.
  Updated by Mehmet Suat Morkan (msmorkan), February 17, 2021.
*/

#include "Sixfab_Tracker.h"

SixfabTracker node;

// Indexes
int ind1 = 0;
int ind2 = 0;
int ind3 = 0;
int ind4 = 0;
int ind5 = 0;
int ind6 = 0;
int ind7 = 0;
int ind8 = 0;
int ind9 = 0;
int ind10 = 0;

// String variables
String gpsSentence;
String latitude;
String NE;
String longtitude;
String EW;
String data_type;
String gps_time;
String fix_quality;
String no_of_sat;
String hdo_precision;
String altitude;

String SMS_text = "Henuz kilitlenmedim!";
String received_SMS = "";

String gsmBuffer = "";

String gps_char;

unsigned long last_read_time = millis();
unsigned long current_time = millis();


void deleteAllSMS () {    // Bu fonksiyon okunmuş tüm SMS'leri silmektedir
  node.sendATComm("at+cmgd=1,2","OK");
}

String readSMS(uint8_t index){
  node.sendATComm("AT+CMGF=1","OK");    // Quectel M95 AT komutları ile yeni SMS sorgusu
  M95_AT.print("AT+CMGR=");
  M95_AT.print(index);
  M95_AT.print("\r");
  gsmBuffer="";
  delay(20);
  if (M95_AT.available() > 5) {
    gsmBuffer = M95_AT.readString();
  }
  DEBUG.println(gsmBuffer);
  if (gsmBuffer.indexOf("CMGR:")!=-1)   //  Yeni SMS varsa, SMS metnini döndürüyoruz
  {
    return gsmBuffer;
  }
  else return "";
}

// setup
void setup() {
  delay(2000);
  node.init();
  delay(100);
  node.turnOnUserLED();
  delay(TIMEOUT);
  node.turnOffUserLED();

  node.getModemStatus();
  node.getIMEI();
  node.getSignalQuality();
  
  L96.flush();    // GPS seri kanal buffer'ını temizle
}

// loop
void loop() {

  current_time = millis();
    if (current_time - last_read_time > 10000) // 10 saniyede bir yeni SMS olup olmadığını kontrol ediyoruz
    {
      last_read_time = current_time;
      received_SMS = readSMS(1);    // SMS kontrolü yapılıyor
      deleteAllSMS();               // Okunmuş tüm SMS'ler siliniyor
      delay(10);
      if (received_SMS.indexOf("NEREDESIN") != -1)   
      // Eğer SMS içeriğinde "NEREDESIN" metni bulunuyorsa, 
      // uydudan aldığımız en son konumu Google Maps linki olarak, girilen telefon numarasına SMS gönderiyoruz
      {
        node.sendSMS("05335962144", &SMS_text[0]);
        received_SMS = "";
      }
    }
    gpsSentence = "";   // GPS buffer stringi temizle
    gps_char = "";
    while (1)
    {
      if (L96.available() > 0)
      {
        gps_char = char(L96.read());            // L96'dan gelen veriyi byte-byte okuyoruz ve veriye String olarak ihtiyacımız olduğu için her byte'ı "char" tipine çeviriyoruz
        if (gps_char.equals("$")) {             // Eğer okuduğumuz veri "$" sembolüne eşitse bir sonraki satıra gelmişiz demektir ve buraya kadar okuduğumuzu işlememiz gerekiyor
          if (gpsSentence.startsWith("GPGGA"))  // GPGGA tipi data, yani "Global Positioning System Fix Data" bizim için yeterli veriyi sağlıyor
          {
            DEBUG.print("Ham Veri: ");        
            DEBUG.println(gpsSentence);         // Verinin ham halinden anlaşılacağı üzere virgüllere göre ayırmamız gerekiyor
            ind1 = gpsSentence.indexOf(',');
            data_type = gpsSentence.substring(0, ind1);
            ind2 = gpsSentence.indexOf(',', ind1+1);
            gps_time = gpsSentence.substring(ind1+1, ind2);
            ind3 = gpsSentence.indexOf(',', ind2+1);
            latitude = gpsSentence.substring(ind2+1, ind3);
            ind4 = gpsSentence.indexOf(',', ind3+1);
            NE = gpsSentence.substring(ind3+1, ind4);
            ind5 = gpsSentence.indexOf(',', ind4+1);
            longtitude = gpsSentence.substring(ind4+1, ind5);
            ind6 = gpsSentence.indexOf(',', ind5+1);
            EW = gpsSentence.substring(ind5+1, ind6);
            ind7 = gpsSentence.indexOf(',', ind6+1);
            fix_quality = gpsSentence.substring(ind6+1, ind7);
            ind8 = gpsSentence.indexOf(',', ind7+1);
            no_of_sat = gpsSentence.substring(ind7+1, ind8);
            ind9 = gpsSentence.indexOf(',', ind8+1);
            hdo_precision = gpsSentence.substring(ind8+1, ind9);
            ind10 = gpsSentence.indexOf(',', ind9+1);
            altitude = gpsSentence.substring(ind9+1, ind10);
            
            DEBUG.print("Latitude: ");
            DEBUG.print(latitude);
            DEBUG.print(" NE: ");
            DEBUG.print(NE);
            DEBUG.print(" Longtitude: ");
            DEBUG.print(longtitude);
            DEBUG.print(" EW: ");
            DEBUG.print(EW);
            DEBUG.print(" Fix Quality: ");
            DEBUG.print(fix_quality);
            DEBUG.print(" No of Sat: ");
            DEBUG.print(no_of_sat);
            DEBUG.print(" Altitude: ");
            DEBUG.print(altitude);
            DEBUG.print(" Time: ");
            DEBUG.println(gps_time);
            // Enlem ve boylam değerleri 1'den büyükse, uydudan veri geliyor.
            // Eğer bu if'e giremiyorsak, henüz uydudan yorumlanabilir bir konum verisi gelmemiştir.
            if ((latitude.length() > 1) && (longtitude.length() > 1)) 
            { 
                DEBUG.println("GPS_DATA_OK");
                // Enlem ve boylam değerlerini, Google Maps linkine uygun formatta, linkin devamına ekliyoruz
                SMS_text =  "www.google.com.tr/maps/place/" + latitude.substring(0, 2) + "+" + latitude.substring(2, 9) + "," + longtitude.substring(0, 3) + "+" + longtitude.substring(3, 10);
                DEBUG.println(SMS_text);
            }
          }
          break;
        }
        gpsSentence += gps_char;
      }
    }
}
