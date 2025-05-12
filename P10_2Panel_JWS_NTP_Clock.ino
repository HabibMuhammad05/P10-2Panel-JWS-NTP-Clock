/*
 * JADWAL WAKTU SHOLAT MENGGUNAKAN NODEMCU ESP8266, LED P10, RTC DS3231, BUZZER
 * FITUR :  JADWAL SHOLAT 5 WAKTU DAN TANBIH IMSAK, JAM BESAR, TANGGAL, SUHU, ALARAM ADZAN DAN TANBIH IMSAK,
 *          DAN HITUNG MUNDUR IQOMAH DAN UBAH WAKTU LEWAT WIFI DENGAN BROWSER.
 * 

Pin on  DMD P10     GPIO      NODEMCU               Pin on  DS3231      NODEMCU                   Pin on  Buzzer       NODEMCU
        2  A        GPIO16    D0                            SCL         D1 (GPIO 5)                       +            RX (GPIO 3)
        4  B        GPIO12    D6                            SDA         D2 (GPIO 4)                       -            GND
        8  CLK      GPIO14    D5                            VCC         3V
        10 SCK      GPIO0     D3                            GND         GND
        12 R        GPIO13    D7
        1  NOE      GPIO15    D8
        3  GND      GND       GND

Catatan : 
o Perlu Power Eksternal 5V ke LED P10.
o Saat Flashing (upload program) cabut sementara pin untuk buzzer.

Project Git
- https://github.com/busel7/Arduino
- https://github.com/busel7/Arduino/tree/master/MembuatJWS

Youtube Channel : 
- https://www.youtube.com/channel/UCEDJLuOw8cld2rvy8Sic2xA  <<-- Mohon bantu Subscribe untuk mendukung saya membuat konten Video bermanfaat lainnya :)


Eksternal Library
- DMDESP : https://github.com/busel7/DMDESP
- PrayerTime : https://github.com/asmaklad/Arduino-Prayer-Times
- RTC DS3231 : https://github.com/Makuna/Rtc
- ArduinoJson V6 : https://github.com/bblanchon/ArduinoJson
- F1kM_Hisab : https://github.com/wardi1971/F1kM_Hisab

Diskusi Grup Untuk1000Masjid : https://www.facebook.com/groups/761058907424496/

Tools : 
- Desain bitmap dan font : http://dotmatrixtool.com
- LittleFS Uploader : https://github.com/earlephilhower/arduino-esp8266littlefs-plugin/releases


www.grobak.net - www.elektronmart.com

Updated : 30 Januari 2025 By Habib M.
- NTP Clock data Instead of RTC, device must connected to WiFi to get the NTP data.

*/

//UPLOAD SKETCH ON LOLIN (WEMOS) D1 MINI(CLONE)

// Define DEBUG to enable debugging; comment it out to disable
//#define DEBUG

#ifdef DEBUG
  #define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
  #define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
  #define DEBUG_BEGIN(baud) Serial.begin(baud)
#else
  #define DEBUG_PRINT(...)    // Do nothing
  #define DEBUG_PRINTLN(...)  // Do nothing
  #define DEBUG_BEGIN(baud)   // Do nothing
#endif

#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

#include <TimeLib.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <cstring>  // Required for strcat()

#include <DMDESP.h>

#include <fonts/ElektronMart5x6.h>
#include <fonts/ElektronMart6x8.h>
//#include <fonts/ElektronMart6x12.h>
//#include <fonts/ElektronMart6x16.h>
//#include <fonts/ElektronMartArabic5x6.h>
//#include <fonts/ElektronMartArabic6x16.h>
#include <fonts/DejaVuSansBold9.h>
#include <verdanav2.h>
#include <Font_6x14.h>
#include <SystemFont5x7.h>


#include <PrayerTimes.h>
#include "Display.h"
#include "Buzzer.h"
#include "RTC.h"
#include "FSConfig.h"
#include "WebServer.h"
#include "WebPage.h"
#include "JWS.h"



//----------------------------------------------------------------------
// SETUP

void setup() {

  DEBUG_BEGIN(115200);

  //Buzzer
  pinMode(buzzer, OUTPUT);
//  digitalWrite(buzzer,LOW);
//  delay(50);

  //RTC
//  mulaiRTC();

  //FILE SYSTEM
  LittleFS.begin();
  loadJwsConfig(fileconfigjws, configjws);

  //WIFI
  wifiConnect();

  timeClient.begin();
  timeClient.setTimeOffset(3600 * configjws.zonawaktu); //3600 x gmt+7
  setSyncProvider(requestSync);
  
  DEBUG_PRINTLN("Getting data ...");
  getTime();
  DEBUG_PRINTLN("Data loaded");

  server.on("/", []() {
    server.send_P(200, "text/html", setwaktu);

    // Kalau ada perubahan tanggal
//    if (server.hasArg("date")) {
//    
//      uint16_t jam;
//      uint8_t menit;
//      uint8_t detik;      
//      String sd=server.arg("date");
//      String lastSd;
//      
//      jam = ((sd[0]-'0')*1000)+((sd[1]-'0')*100)+((sd[2]-'0')*10)+(sd[3]-'0');
//      menit = ((sd[5]-'0')*10)+(sd[6]-'0');
//      detik = ((sd[8]-'0')*10)+(sd[9]-'0');
//      
//      if (sd != lastSd){
//        RtcDateTime now = Rtc.GetDateTime();
//        uint8_t hour = now.Hour();
//        uint8_t minute = now.Minute();
//        Rtc.SetDateTime(RtcDateTime(jam, menit, detik, hour, minute, 0));
//        lastSd=sd;
//      }
//
//      BacaDataJam();
////  
//      server.send ( 404 ,"text", message );
//    
//    }
//
//     //Kalau ada perubahaan jam
//    if (server.hasArg("time")) {
//      
//      String st=server.arg("time");
//      String lastSt;
//      uint8_t jam = ((st[0]-'0')*10)+(st[1]-'0');
//      uint8_t menit = ((st[3]-'0')*10)+(st[4]-'0');
//      
//      if (st != lastSt){
//         RtcDateTime now = Rtc.GetDateTime();
//         uint16_t year = now.Year();
//         uint8_t month = now.Month();
//         uint8_t day = now.Day();
//         Rtc.SetDateTime(RtcDateTime(year, month, day, jam, menit, 0));
//         lastSt=st;
//       }
//       
//       BacaDataJam();
      
       server.send ( 404 ,"text", message );
  
//    }
  });

  server.on("/simpandatajws", HTTP_POST, handleSettingJwsUpdate);

  server.on("/xmlwaktu", handleXMLWaktu); // http://192.168.4.1/xmlwaktu
  server.on("/xmldatajws", handleXMLDataJWS); // http://192.168.4.1/xmldatajws


  httpUpdater.setup(&server);
  server.begin();
  DEBUG_PRINTLN("HTTP server started"); 

  // Tampilkan Parameter yang tersimpan
  bacaParameter();

  //DMD
  Disp.start();  
  Disp.setBrightness(5);

  UpdateWaktu();
  digitalWrite(buzzer,LOW);  
}




//----------------------------------------------------------------------
// LOOP

void loop() {

  server.handleClient();

  switch(tmputama) {

    case 0 :      
      tampiljws();
      break;

    case 1 :
      TampilAdzan();
      break;

    case 2 :
      Iqomah();
      break;

    case 3 :
      TampilSaatSholat();
      break;
    
  }

  Disp.loop();
  UpdateWaktu();
  AlarmSholat();
  WifiShutdown();
  
}



void tampiljws() {

  switch(tmpjws) {

    case 0 :
      animLogoX();
      break;

    case 1 :
      JamBesar(-1);
      tampilinfo();
      break;

    case 2 :
      TeksBerjalanKananKiri();
      break;
    
  }
  
}



void tampilinfo() {

  switch(tmpinfo) {

    case 0 :
      TampilTanggal();
      break;

    case 1 :
    TampilJadwalSholat();
//      TampilTanggalHijr();
//      tmpinfo = 2;
      break;

//    case 2 :
////      TampilSuhu();
//      tmpinfo = 3;
//      break;
//
//    case 3 :
//      TampilJadwalSholat();
//      break;
    
  }
  
}
