//-------------------------------------------------------------------//
//--------------------------- FORMAT TEKS ---------------------------//
int centerPrint;
void textCenter(int y,String Msg) {
  
  centerPrint = int((Disp.width()-Disp.textWidth(Msg)) / 2);
  Disp.drawText(centerPrint,y,Msg);
  
}

void textCenter1PKanan(int y,String Msg) {
  
  int center = (((Disp.width()-Disp.textWidth(Msg)) / 2) + (Disp.width() / 4)) + 1;
  Disp.drawText(center,y,Msg);
  
}

void textCenterMasjid(int y,String Msg) {
  
  int center = ((Disp.width()-Disp.textWidth(Msg)) / 2) + (Disp.width() / 8);
  Disp.drawText(center,y,Msg);
  
}


//-------------------------------------------------------------------//
//---------- ALARM SHOLAT BERJALAN SAAT MASUK WAKTU SHOLAT ----------//
void AlarmSholat() {  

  int hours, minutes, seconds;

  // Tanbih Imsak
  get_float_time_parts(times[0], hours, minutes);
  minutes = minutes + configjws.ihti;
  if (minutes < 10) {
    minutes = 60 - minutes;
    hours --;
  } else {
    minutes = minutes - 10 ;
  }
  if (rJam == hours && rMen == minutes && rDet == 0) {
    tipealarm = 0;
    detikbeep = 0;
    Disp.clear();
    tmputama = 1;
  }

  // Subuh
  get_float_time_parts(times[0], hours, minutes);
  minutes = minutes + configjws.ihti;
  if (minutes >= 60) {    
    minutes = minutes - 60;
    hours ++;    
  }
  if (rJam == hours && rMen == minutes && rDet == 0) {    
    tipealarm = 1;
    detikbeep = 0;
    Disp.clear();
    tmputama = 1;    
  }

  // Dzuhur
  get_float_time_parts(times[2], hours, minutes);
  minutes = minutes + configjws.ihti;
  if (minutes >= 60) {    
    minutes = minutes - 60;
    hours ++;    
  }
  if (rJam == hours && rMen == minutes && rDet == 0 && rHar != 4) {    
    tipealarm = 2;
    detikbeep = 0;
    Disp.clear();
    tmputama = 1;
  } else if (rJam == hours && rMen == minutes && rDet == 0 && rHar == 4) {    
    tipealarm = 3;
    detikbeep = 0;
    Disp.clear();
    tmputama = 1;
  }

  // Ashar
  get_float_time_parts(times[3], hours, minutes);
  minutes = minutes + configjws.ihti;
  if (minutes >= 60) {    
    minutes = minutes - 60;
    hours ++;    
  }
  if (rJam == hours && rMen == minutes && rDet == 0) {    
    tipealarm = 4;
    detikbeep = 0;
    Disp.clear();
    tmputama = 1;    
  }

  // Maghrib
  get_float_time_parts(times[5], hours, minutes);
  minutes = minutes + configjws.ihti;
  if (minutes >= 60) {    
    minutes = minutes - 60;
    hours ++;    
  }
  if (rJam == hours && rMen == minutes && rDet == 0) {    
    tipealarm = 5;
    detikbeep = 0;
    Disp.clear();
    tmputama = 1;    
  }
  
  // Isya'
  get_float_time_parts(times[6], hours, minutes);
  minutes = minutes + configjws.ihti;
  if (minutes >= 60) {    
    minutes = minutes - 60;
    hours ++;    
  }
  if (rJam == hours && rMen == minutes && rDet == 0) {    
    tipealarm = 6;
    detikbeep = 0;
    Disp.clear();
    tmputama = 1;    
  }    
}


//-------------------------------------------------------------------//
//------------------------- TAMPILAN ADZAN --------------------------//
void TampilAdzan() {
  BunyiBeep(2,20);
    
  static uint32_t pM;
  uint32_t cM = millis();
  static uint16_t det;

 if (beep){
  Disp.setFont(ElektronMart6x8);
  if (tipealarm == 0) {      
    textCenter(0, "TANBIH");
    textCenter(8, "IMSAK");    
  }
  
  if (tipealarm == 1) {
    textCenter(0, "ADZAN");
    textCenter(8, "SUBUH");
    menitiqmh = configjws.iqmhs;    
  }
  
  if (tipealarm == 2) {    
    textCenter(0, "ADZAN");
    textCenter(8, "DZUHUR");
    menitiqmh = configjws.iqmhd;    
  }
  
  if (tipealarm == 3) {    
    textCenter(0, "ADZAN");
    textCenter(8, "JUM'AT");    
  }
  
  if (tipealarm == 4) {
    textCenter(0, "ADZAN");
    textCenter(8, "ASHAR");    
    menitiqmh = configjws.iqmha;    
  }
  
  if (tipealarm == 5) {
    textCenter(0, "ADZAN");
    textCenter(8, "MAGHRIB");
    menitiqmh = configjws.iqmhm;    
  } 
  
  if (tipealarm == 6) {
    textCenter(0, "ADZAN");
    textCenter(8, "ISYA");
    menitiqmh = configjws.iqmhi;    
  }  
 }else { Disp.clear(); }

  if (cM-pM >= 1000) {    
   det++;   
   if (det >= 20) {
    if (tipealarm == 0 or tipealarm == 3) {
        digitalWrite(buzzer,LOW);
        Disp.clear();
        det = 0;
        detikbeep = 0;
        tmputama = 0;
    }  
    if (tipealarm == 1 or tipealarm == 2 or tipealarm == 4 or tipealarm == 5 or tipealarm == 6) { 
        digitalWrite(buzzer,LOW);
        Disp.clear();
        det = 0;
        detikbeep = 0;
        tmputama = 2;
    }
   }    
    pM = cM;    
  }    
}


//-------------------------------------------------------------------//
//----- HITUNG MUNDUR WAKTU SETELAH ADZAN SAMPAI MULAI IQOMAH -------//
void Iqomah() {

  static uint32_t pM;
  uint32_t cM = millis();
  static char hitungmundur[6];  

  Disp.setFont(ElektronMart6x8);
  textCenter(0, "- IQOMAH -");

  if (detikiqmh == 60) {
    detikiqmh = 0;
  }

  if (cM - pM >= 1000) {
    pM = cM;
    detikiqmh--;
    if (menitiqmh == 0 && detikiqmh == 0) {
      Disp.clear();
      detikbeep = 0;
      detikiqmh = 59;
      tmputama = 3;
    }
    if (detikiqmh < 0) {
      detikiqmh = 59;
      menitiqmh--;
    }    
  }

  sprintf(hitungmundur, "%02d:%02d", menitiqmh, detikiqmh);
  Disp.setFont(verdanav2);
  textCenter(8, hitungmundur);
  
}


//-------------------------------------------------------------------//
//---------------------- TAMPILAN SAAT SHOLAT -----------------------//
void TampilSaatSholat() {
  BunyiBeep(1,10);

  static uint32_t pM;
  uint32_t cM = millis();
  static uint32_t durasi = 30; // Detik
  static uint32_t det;
  
  char jamFull[5];
  sprintf(jamFull, "%02d %02d", rJam, rMen);

  if (cM - pM >= 1000) {
    pM = cM;
    det++;
    if (det < durasi) {
      Disp.drawRect(0,0,63,15,0,0);
      Disp.setFont(DejaVuSansBold9);
      textCenter(-1, ((det % 4) < 2) ? "LURUSKAN" : "RAPATKAN"); 
      textCenter(8, "SHAF");
    }else{
      digitalWrite(buzzer,LOW); 
      Disp.drawRect(0,0,63,15,0,0);
      Disp.setFont(verdanav2);     
      Disp.drawText(0,8, jamFull); 
      if (det %2 == 0) { Disp.drawRect(62,14,63,15,0,0); Disp.drawText(15,8, " "); }
      else {             Disp.drawRect(62,14,63,15,1,1); Disp.drawText(15,8, ":"); }  
    }    
    if (det >= configjws.durasiadzan * 60) {
      Disp.clear();
      detikbeep = 0;
      det = 0;
      tmputama = 0;
    }
  }    
}


//-------------------------------------------------------------------//
//--------------------- TAMPILAN RUNNING TEKS -----------------------//
void TeksBerjalanKananKiri() {
  static uint32_t pM;
  uint32_t cM = millis();
  static uint8_t Kecepatan = 50;
  static uint32_t x;
  static uint32_t pMJam;
  uint32_t cMJam = millis();
  static uint8_t cntDet;
  
  char jam[3];
  char menit[3];
  char detik[3];

  sprintf(jam, "%02d", rJam);
  sprintf(menit, "%02d", rMen);
  sprintf(detik, "%02d", rDet);

  char hari[8];
  String tgl;
  
  sprintf(hari, "%s", namaHari[rHar - 1]);
  tgl = String(rTgl) + "-" + String(rBul) + "-" + String(rTah);
  
  int width = Disp.width();
  int fullScroll = Disp.textWidth(configjws.namamasjid) + (width * 2);

  if (cMJam - pMJam >= 1000) {   
    pMJam = cMJam;   
    cntDet++;
    Disp.setFont(ElektronMart6x8);
    Disp.drawText(-2, 0,"#"); // print bitmap masjid
    Disp.setFont(verdanav2); 
    Disp.drawText(15, -1," ");
    
    Disp.drawRect(17,0,Disp.width(),7,0,0); // matikan dengan print kotak kosong
    Disp.drawText(31, -1, (cntDet % 2 == 0) ? " " :":"); // print titik dua bergantian setiap detik
    Disp.drawText(48, -1, (cntDet % 2 == 0) ? " " :":");
    
    Disp.drawText(17, -1, String(jam)); // print jam, menit, detik
    Disp.drawText(34, -1, String(menit));
    Disp.drawText(51, -1, String(detik));
    
    Disp.drawRect(0,8,Disp.width(),8,1,1);
  }
  
  if (cntDet <= 4) {  
    Disp.drawRect(17,0,Disp.width(),7,0,0); // matikan dengan print kotak kosong
    
    Disp.setFont(ElektronMart6x8);
    textCenterMasjid(0, hari); // print nama hari diatas
    
    Disp.setFont(verdanav2); 
    textCenter(8, tgl); // print tanggal selama 4 detik
    
    Disp.drawRect(0,8,Disp.width(),8,1,1);
  }
  else if (cM - pM > Kecepatan) {
    pM = cM;
    if (x < fullScroll) {
      ++x;      
    } else {
      x = 0;
      Disp.clear();
      cntDet = 0; 
      tmpjws = 0;     
    }
    
    Disp.drawRect(0,9,Disp.width(),15,0,0); // hapus dengan print kotak kosong dibawah
    
    Disp.setFont(verdanav2); 
    Disp.drawText(centerPrint - x, 8, tgl); //print tanggal bergeser dengan runningtext
    
    Disp.setFont(ElektronMart6x8);
    Disp.drawText(width - x, 9, configjws.namamasjid); // print informasi nama masjid berjalan
    Disp.drawRect(0,8,Disp.width(),8,1,1);  
  }  
}

bool jadwalOut = false;
static uint8_t ijadwal;
void JadwalAnim(String atas, String bawah){
  static uint8_t x;
  static uint8_t s; // 0=in, 1=out
  static uint32_t pM;
  uint32_t cM = millis();
  
  char jam[3];
  char menit[3];
  sprintf(jam, "%02d", rJam);
  sprintf(menit, "%02d", rMen);

  if (cM - pM > 20) {
    if (s == 0 and x < 8) {
      pM = cM;
      x++;
    }
    if (s == 1 and x > 0) {
      pM = cM;
      x--;
    }
  }
  
  if (cM - pM > 2000 and x == 8) {
    s = 1; 
  }
  
  if (x == 0 and s == 1) { 
    Disp.clear();
    ijadwal++;
    s = 0;
  }
  
  if (atas == "IMSAK" and s == 1){
//    DEBUG_PRINTLN("jadwalOut"); 
    Disp.drawRect(0,0,32,15,0,0);
    Disp.setFont(Font_6x14);
    Disp.drawText(1, (x -8)*2, jam);
    Disp.drawText(18, (x -8)*2, menit);
  }
     
  Disp.setFont(verdanav2);
  textCenter1PKanan(x -8, atas); 
  textCenter1PKanan(15 - x, bawah);
}


//-------------------------------------------------------------------//
//---------------------- TAMPILKAN TANGGAL --------------------------//

static uint8_t yJam;
void TampilTanggal() {
  char hari[8];
  char tanggal[7]; 
  static uint32_t pM = 0;
  uint32_t cM = millis();
  static uint8_t flag;
  
  sprintf(hari, "%s", namaHari[rHar - 1]);
  sprintf(tanggal, "%02d %s", rTgl, namaBulan[rBul]); 
//  JadwalAnim (hari, tanggal);

  if(cM - pM > 20) {    
    pM = cM;
    flag++;
    sprintf(hari, "%s", namaHari[rHar - 1]);
    sprintf(tanggal, "%02d %s", rTgl, namaBulan[rBul]); 
    Disp.setFont(verdanav2);
    textCenter1PKanan(yJam - 20, hari);
    Disp.setFont(ElektronMart5x6);
    textCenter1PKanan(28 - yJam, tanggal); 
    if (flag >= 145) {
      flag = 0;
      Disp.drawRect(32,0,64,15,0,0);
      tmpinfo = 1;      
    }    
  }   
}


//-------------------------------------------------------------------//
//------------------- TAMPILKAN JADWAL SHOLAT -----------------------//
void TampilJadwalSholat() {  
  static uint8_t i;
  static uint32_t pM;
  uint32_t cM = millis();
  char sholat[7];
  char jam[5];
  char TimeName[][8] = {"SUBUH","TRBIT","DZUHR","ASHAR","TRBNM","MGHRB","ISYA"};
  int hours, minutes;  

  if (cM - pM >= 0) {
    pM = cM;
    Disp.drawRect(31,0,64,15,0,0);
    
//    if (i == 1) {i = 2;} // Abaikan Terbit
    if (ijadwal == 4) {ijadwal = 5;} // Abaikan Terbenam

    get_float_time_parts(times[ijadwal], hours, minutes);
    minutes = minutes + configjws.ihti;

    if (minutes >= 60) {
      minutes = minutes - 60;
      hours ++;
    }

    char *sholat = TimeName[ijadwal];
    sprintf(jam,"%02d:%02d", hours, minutes);     
    Disp.setFont(verdanav2);
    JadwalAnim(sholat, jam);
//    textCenter(-1, sholat); 
//    textCenter(7, jam);
     
//    ijadwal++;

    if (ijadwal > 6) {
      get_float_time_parts(times[0], hours, minutes);
      minutes = minutes + configjws.ihti;
      if (minutes < 11) {
        minutes = 60 - minutes;
        hours --;
      } else {
        minutes = minutes - 10 ;
      }
      sprintf(jam,"%02d:%02d", hours, minutes);
      Disp.drawRect(31,0,64,15,0,0);
      Disp.setFont(verdanav2);
      JadwalAnim("IMSAK", jam);
//      textCenter(-1, "IMSAK"); 
//      textCenter(7, jam); 
      
      if (ijadwal > 7) { 
        jadwalOut = true; 
        ijadwal = 0;
//        Disp.drawRect(0,0,32,15,0,0);
        tmpinfo = 0;
        tmpjws = 2;
      }
    }   
  }
}


//-------------------------------------------------------------------//
//----------------------- TAMPILKAN JAM -----------------------------//
static uint8_t sAnim; // 0=in, 1=out
void JamBesar2(uint16_t xAwal) {
  char jam[3];
  char menit[3];
  char detik[3];

  sprintf(jam, "%02d", rJam);
  sprintf(menit, "%02d", rMen);
  sprintf(detik, "%02d", rDet);

//  static boolean huruf;

  static uint8_t x;
  static uint8_t d;           
  static uint32_t pM;
  uint32_t cM = millis();

  static uint32_t pMPulse;
  static uint8_t pulse;

  if (cM - pMPulse >= 100) {
    pMPulse = cM;
    pulse++;
  }
  
  if (pulse > 8) {
    pulse=0;
  }

  if(cM - pM > 33) { 
    if(d == 0 and x < 12) {
      pM=cM;
      x++;
      Disp.clear();
    }
    if(sAnim and x == 12) {
      d=1;
    }
    if(d  == 1 and x > 0) {
      pM=cM;
      x--;
      Disp.clear();
    }    
  }
  
  
  
  if(x == 12) { Disp.drawRect(xAwal+14, 3+pulse, xAwal+17, 11-pulse, 0, 1); }
  
  if(x < 12) { Disp.drawRect(xAwal+14, 3,xAwal+17, 11, 0, 0); }
   
  if(x == 0 and d == 1) {
    d=0;
//    huruf = !huruf;
  }
  
  //JAM
//  if (huruf) {
    Disp.setFont(Font_6x14);
//  } else {
//    Disp.setFont(ElektronMartArabic6x16);
//  }  
  Disp.drawText(xAwal-12 + x, 0, jam);

  //MENIT
//  if (huruf) {
    Disp.setFont(verdanav2);
//  } else {
//    Disp.setFont(ElektronMartArabic5x6);
//  }  
  Disp.drawText(xAwal+31 - x, -1, menit);

  //DETIK          
//  if (huruf) {
    Disp.setFont(verdanav2);
//  } else {
//    Disp.setFont(ElektronMartArabic5x6);
//  }  
  Disp.drawText(xAwal+31  - x, 8, detik);
}

void JamBesar(int x) {
  char jam[3];
  char menit[3];
  sprintf(jam, "%02d", rJam);
  sprintf(menit, "%02d", rMen);
  
//  char hari[8];
//  char tanggal[7]; 
//  sprintf(hari, "%s", namaHari[rHar - 1]);
//  sprintf(tanggal, "%02d %s", rTgl, namaBulan[rBul]); 


//  static boolean huruf;

  static uint8_t d = 0;           
  static uint32_t pM;
  uint32_t cM = millis();

  static uint32_t pMPulse;
  static uint8_t pulse;

  if (cM - pMPulse >= 100) {    
    pMPulse = cM;
    pulse++;    
  }
  if (pulse > 8) {
    pulse=0;
  }
  if(cM - pM > 20) { 
    if(d == 0 and yJam < 20) {
      DEBUG_PRINTLN("yJam ++ :" + String(yJam));
      pM = cM;
      yJam++;
      Disp.clear();
    }    
    if(d  == 1 and yJam > 0) {
      DEBUG_PRINTLN("yJam -- :" + String(yJam));
      pM = cM;      
      yJam--;
      Disp.clear();
    }
  }
  if(jadwalOut and yJam == 20) {
    d=1;
    jadwalOut = false;
    DEBUG_PRINTLN("jadwalOut diterima");
  }
  if(yJam == 20) {
    Disp.drawRect(x+14,3+pulse,x+17,11-pulse,0,1);       
  }
  if(yJam < 20) {
    Disp.drawRect(x+14,3,x+18,17,0,0);
  }
  if(yJam == 0 and d == 1) {
    d=0;
//    huruf = !huruf;
  }

  Disp.setFont(Font_6x14);
  Disp.drawText(x+1, yJam - 20, jam);
  Disp.drawText(x+18, yJam - 20, menit);

  
//  if(cM - pM < 3000) {
////    pM = cM;
//    Disp.setFont(verdanav2);
//    textCenter1PKanan(yJam - 20, hari);
//    Disp.setFont(ElektronMart5x6);
//    textCenter1PKanan(28 - yJam, tanggal);
//  }
//  else{
//    d=0;
//    tmpinfo = 1;
//  }
}


//-------------------------------------------------------------------//
//----------------------- TAMPILKAN LOGO ----------------------------//
void LogoA(uint32_t x) {
  static const uint8_t LogoA[] PROGMEM = {
    16, 16,
    0x00, 0x00, 0x7f, 0xfe, 0x7f, 0xfe, 0x00, 0x00, 0x7e, 0x66, 0x7e, 0x66, 0x06, 0x66, 0x06, 0x66, 0x7e, 0x66, 0x7e, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7f, 0xfe, 0x7f, 0xfe, 0x00, 0x00
  };
  Disp.drawBitmap(x, 0, LogoA);
}

void LogoM(uint32_t x) {
  static const uint8_t LogoM[] PROGMEM = {
    16, 16,
    0x00, 0x00, 0x66, 0x7e, 0x66, 0x7e, 0x66, 0x66, 0x66, 0x66, 0x7e, 0x7e, 0x7e, 0x7e, 0x60, 0x60, 0x60, 0x60, 0x7e, 0x7e, 0x7e, 0x7e, 0x66, 0x06, 0x66, 0x06, 0x7f, 0xfe, 0x7f, 0xfe, 0x00, 0x00
  };  
  Disp.drawBitmap(x, 0, LogoM);
}


//-------------------------------------------------------------------//
//----------------------- ANIMASI LOGO ------------------------------//
void animLogoX() {
  static uint8_t x;
  static uint32_t pM;
  uint32_t cM = millis();

  JamBesar2(16);

  if (cM - pM > 20) {    
    if (sAnim == 0 and x < 16) {      
      pM = cM;
      x++;      
    }    
    if (sAnim == 1 and x > 0) {      
      pM = cM;
      x--;      
    }    
  }  
  if (cM - pM > 20000 and x == 16) {    
    sAnim = 1;        
  }
  if (x == 0 and sAnim == 1) {    
    Disp.clear();
    sAnim = 0;
    tmpjws = 1;     
  }
  LogoA(Disp.width() - x);
  LogoM(x - 16);  
}

//void TeksBerjalanKiriKanan() {
//
//  static uint32_t pM;
//  uint32_t cM = millis();
//  static uint8_t Kecepatan = 50;
//  static uint32_t x;
//
//  int width = Disp.width();
//  int fullScroll = Disp.textWidth(configjws.namamasjid) + width;
//
//  Disp.setFont(ElektronMart6x8);
//
//  if(cM - pM > Kecepatan) {
//
//    pM = cM;
//
//    if (x < fullScroll) {
//
//      ++x;
//      
//    } else {
//
//      x = 0;
//      
//    }
//
//    Disp.drawText(x - fullScroll + width, 8, configjws.namamasjid);
//    
//  }
//  
//}

//----------------------------------------------------------------------
//TAMPILKAN TANGGAL HIJRIAH

//void TampilTanggalHijr() {
//
//  char hari[8];
//  char tanggal[7]; // 19 ROM
//
//  static uint32_t pM;
//  uint32_t cM = millis();
//
//  static uint8_t flag;
//
//  if(cM - pM > 3000) {
//    
//    pM = cM;
//    flag++;
//
//    sprintf(hari, "%s", hariHijriah[rHar]);
//    sprintf(tanggal, "%02d %s", Hjr_Date, bulanHijriah[Hjr_Month]);
//  
//    Disp.setFont(ElektronMart5x6);
//    textCenter1PKanan(0, hari);
//    textCenter1PKanan(8, tanggal);
//
//    if (flag >= 2) {
//      flag = 0;
//      Disp.drawRect(32,0,64,15,0,0);
//      tmpinfo = 2;
//      
//    }
//    
//  } 
//  
//}



//----------------------------------------------------------------------
// TAMPILKAN SUHU

//void TampilSuhu() {
//
//  char suhu[3];
//  int koreksisuhu = 2; // Perkiraan selisih suhu mesin dengan suhu ruangan
//
//  static uint32_t pM;
//  uint32_t cM = millis();
//
//  static uint8_t flag;
//
//  if(cM - pM > 3000) {
//    
//    pM = cM;
//    flag++;
//
//    sprintf(suhu, "%02d*C", celsius - koreksisuhu);
//    
//    Disp.setFont(ElektronMart5x6);
//    textCenter1PKanan(0, "SUHU");
//    textCenter1PKanan(8, suhu);
//
//    if (flag >= 2) {
//      flag = 0;
//      Disp.drawRect(32,0,64,15,0,0);
//      tmpinfo = 3;
//      
//    }
//  
//  }
//  
//}
//void TampilJamKecil() {
//  static uint32_t pMJam;
//  uint32_t cM = millis();
//  
//  char jam[9];
//  
//  if (cM - pMJam >= 1000) {
//   
//    pMJam = cM;
//    
//    //JAM
//    sprintf(jam,"%02d:%02d:%02d", rJam, rMen, rDet);
//    Disp.setFont(ElektronMart5x6);
//    Disp.drawText(20,7, jam);
//        
//  }
// 
//}
