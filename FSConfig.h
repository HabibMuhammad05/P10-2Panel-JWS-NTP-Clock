// ----------------------
// FS CONFIG

int detikiqmh;
int menitiqmh;
uint8_t tipealarm;
uint8_t tmputama;
uint8_t tmpjws;
uint8_t tmpinfo;

struct ConfigJws {
  String ssid;     
  String password; 
  
  uint8_t iqmhs; // menit
  uint8_t iqmhd; // menit
  uint8_t iqmha; // menit
  uint8_t iqmhm; // menit
  uint8_t iqmhi; // menit
  uint8_t durasiadzan; // Menit 
  uint8_t ihti; // Koreksi Waktu Menit Jadwal Sholat
  float latitude;
  float longitude;
  int8_t zonawaktu;
  char namamasjid[512];
};
ConfigJws configjws;

const char *fileconfigjws = "/configjws.json";

String message, XML;

// -------------------------------------------
// Membuat file config JWS JSON di File Sistem

void membuatDataAwal() {

  String dataawal = "{\"iqmhs\":\"15\",\"iqmhd\":\"5\",\"iqmha\":\"5\",\"iqmhm\":\"13\",\"iqmhi\":\"12\",\"durasiadzan\":\"1\",\"ihti\":\"2\",\"latitude\":\"-6.16\",\"longitude\":\"106.61\",\"zonawaktu\":\"7\",\"namamasjid\":\"UNTUK 1000 MASJID PROJEK - NTP P10 JWS - 2025\",\"ssidData\":\"JWS\",\"passData\":\"12345678\"}";

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, dataawal);

  File configFileJws = LittleFS.open(fileconfigjws, "w");
  
  if (!configFileJws) {
    DEBUG_PRINTLN("Gagal membuat file configjws.json untuk ditulis mungkin partisi belum dibuat");
    return;
  }  
  serializeJson(doc, configFileJws);
  if (error) {    
    DEBUG_PRINT(F("deserializeJson() gagal kode sebagai berikut: "));
    DEBUG_PRINTLN(error.c_str());
    return;    
  } else {    
    configFileJws.close();
    DEBUG_PRINTLN("Berhasil membuat file configjws.json");  
  }  
}


// -------------------------------------------
// Membaca file config JWS JSON di File Sistem

void loadJwsConfig(const char *fileconfigjws, ConfigJws &configjws) {  
  File configFileJws = LittleFS.open(fileconfigjws, "r");
  if (!configFileJws) {    
    DEBUG_PRINTLN("Gagal membuka file configjws.json untuk dibaca");
    membuatDataAwal();
    DEBUG_PRINTLN("Sistem restart...");
    digitalWrite(buzzer,HIGH); delay(700); digitalWrite(buzzer,LOW);
    ESP.restart();    
  }

  size_t size = configFileJws.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFileJws.readBytes(buf.get(), size);

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, buf.get());  

  if (error) {
    DEBUG_PRINTLN("Gagal parse fileconfigjws");
    return;
  }
  
  configjws.iqmhs = doc["iqmhs"];
  configjws.iqmhd = doc["iqmhd"];
  configjws.iqmha = doc["iqmha"];
  configjws.iqmhm = doc["iqmhm"];
  configjws.iqmhi = doc["iqmhi"];
  configjws.durasiadzan = doc["durasiadzan"];
  configjws.ihti = doc["ihti"];
  configjws.latitude = doc["latitude"];
  configjws.longitude = doc["longitude"];
  configjws.zonawaktu = doc["zonawaktu"];
  strlcpy(configjws.namamasjid, doc["namamasjid"] | "", sizeof(configjws.namamasjid));
  
  configjws.ssid = doc["ssidData"].as<String>();
  configjws.password = doc["passData"].as<String>();

  configFileJws.close(); 

}


// -------------------------------------------
// MEMBACA PARAMETER YANG TERSIMPAN

void bacaParameter() {

  DEBUG_PRINTLN(" ");
  DEBUG_PRINTLN("PARAMETER TERSIMPAN");
  DEBUG_PRINT("Iqomah Subuh   : "); DEBUG_PRINTLN(configjws.iqmhs);
  DEBUG_PRINT("Iqomah Dzuhur  : "); DEBUG_PRINTLN(configjws.iqmhd);
  DEBUG_PRINT("Iqomah Ashar   : "); DEBUG_PRINTLN(configjws.iqmha);
  DEBUG_PRINT("Iqomah Maghrib : "); DEBUG_PRINTLN(configjws.iqmhm);
  DEBUG_PRINT("Iqomah Isya    : "); DEBUG_PRINTLN(configjws.iqmhi);
  DEBUG_PRINT("Durasi Sholat  : "); DEBUG_PRINTLN(configjws.durasiadzan);
  DEBUG_PRINT("Ihtiyati       : "); DEBUG_PRINTLN(configjws.ihti);
  DEBUG_PRINT("Latitude       : "); DEBUG_PRINTLN(configjws.latitude);
  DEBUG_PRINT("Longitude      : "); DEBUG_PRINTLN(configjws.longitude);
  DEBUG_PRINT("Zona Waktu     : "); DEBUG_PRINTLN(configjws.zonawaktu);
  DEBUG_PRINT("Nama Masjid    : "); DEBUG_PRINTLN(configjws.namamasjid);
  DEBUG_PRINT("SSID tersimpan : "); DEBUG_PRINTLN(configjws.ssid);
  DEBUG_PRINT("PASS tersimpan : "); DEBUG_PRINTLN(configjws.password);
  DEBUG_PRINTLN(" ");
  
}


// PENGHITUNG JADWAL SHOLAT

void JadwalSholat() {

  /*
    CALCULATION METHOD
    ------------------
    Jafari,   // Ithna Ashari
    Karachi,  // University of Islamic Sciences, Karachi
    ISNA,     // Islamic Society of North America (ISNA)
    MWL,      // Muslim World League (MWL)
    Makkah,   // Umm al-Qura, Makkah
    Egypt,    // Egyptian General Authority of Survey
    Custom,   // Custom Setting

    JURISTIC
    --------
    Shafii,    // Shafii (standard)
    Hanafi,    // Hanafi

    ADJUSTING METHOD
    ----------------
    None,        // No adjustment
    MidNight,   // middle of night
    OneSeventh, // 1/7th of night
    AngleBased, // angle/60th of night

    TIME IDS
    --------
    Fajr,
    Sunrise,
    Dhuhr,
    Asr,
    Sunset,
    Maghrib,
    Isha
  
  */

  set_calc_method(Karachi);
  set_asr_method(Shafii);
  set_high_lats_adjust_method(AngleBased);
  set_fajr_angle(20);
  set_isha_angle(18);

  get_prayer_times(rTah, rBul, rTgl, configjws.latitude, configjws.longitude, configjws.zonawaktu, times);

}



// ------------------------------
// UPDATE HISAB TANGGAL

//F1kM_Hisab HisabTanggal;
//
//int Hjr_Date, Hjr_Month, Hjr_Year;
//
//void TanggalHijriah() {
//
//  HisabTanggal.setLocationOnEarth(configjws.latitude, configjws.longitude); //Latitude Longitude TANGERANG
//  HisabTanggal.setHilalVisibilityFactor(configjws.hilal);
//  HisabTanggal.Greg2HijrDate(rTgl,rBul,rTah,Hjr_Date,Hjr_Month,Hjr_Year);
//
//}



//-----------------------------------
// UPDATE WAKTU

void UpdateWaktu() {

  static long pM;
  static uint16_t d;
  

  if (millis() - pM > 1000) {
    d++;
    if (d < 2) {
      BacaDataJam();
      JadwalSholat();
//      TanggalHijriah();
    } else if (d < 60) { // Update setiap 60 detik
      rDet++;
      if (rDet > 59) {
        rMen++;
        rDet=0;
        if (rMen > 59) {
          rJam++;
          rMen = 0;
        }
      }
    } else {
      d = 0; 
    }
    pM = millis();    
  }
  
}



//----------------------------------------------------------------------
// XML UNTUK JEMBATAN DATA MESIN DENGAN WEB

void XMLWaktu(){

  XML="<?xml version='1.0'?>";
  XML+="<t>";
  
    XML+="<Tahun>";
    XML+=rTah;
    XML+="</Tahun>";
    XML+="<Bulan>";
    XML+=rBul;
    XML+="</Bulan>";
    XML+="<Tanggal>";
    XML+=rTgl;
    XML+="</Tanggal>";
    XML+="<Jam>";
      if(rJam<10){
        XML+="0";
        XML+=rJam;
      }else{XML+=rJam;}
    XML+="</Jam>";
    XML+="<Menit>";
      if(rMen<10){
        XML+="0";
        XML+=rMen;
      }else{XML+=rMen;}
    XML+="</Menit>";
    XML+="<Detik>";
      if(rDet<10){
        XML+="0";
        XML+=rDet;
      }else{XML+=rDet;}
    XML+="</Detik>";
    
  XML+="</t>"; 
}


void XMLDataJWS(){

  XML="<?xml version='1.0'?>";
  XML+="<t>";

    XML+="<IqomahSubuh>";
    XML+= configjws.iqmhs;
    XML+="</IqomahSubuh>";
    XML+="<IqomahDzuhur>";
    XML+= configjws.iqmhd;
    XML+="</IqomahDzuhur>";
    XML+="<IqomahAshar>";
    XML+= configjws.iqmha;
    XML+="</IqomahAshar>";
    XML+="<IqomahMaghrib>";
    XML+= configjws.iqmhm;
    XML+="</IqomahMaghrib>";
    XML+="<IqomahIsya>";
    XML+= configjws.iqmhi;
    XML+="</IqomahIsya>";
    XML+="<DurasiAdzan>";
    XML+= configjws.durasiadzan;
    XML+="</DurasiAdzan>";
    XML+="<Ihtiyati>";
    XML+= configjws.ihti;
    XML+="</Ihtiyati>";
    XML+="<Latitude>";
    XML+= configjws.latitude;
    XML+="</Latitude>";
    XML+="<Longitude>";
    XML+= configjws.longitude;
    XML+="</Longitude>";
    XML+="<ZonaWaktu>";
    XML+= configjws.zonawaktu;
    XML+="</ZonaWaktu>";
    XML+="<NamaMasjid>";
    XML+= configjws.namamasjid;
    XML+="</NamaMasjid>";
    
    XML+="<ssidData>";
    XML+= configjws.ssid;
    XML+="</ssidData>";
    
    XML+="<passData>";
    XML+= configjws.password;
    XML+="</passData>";
    
  XML+="</t>"; 
}
