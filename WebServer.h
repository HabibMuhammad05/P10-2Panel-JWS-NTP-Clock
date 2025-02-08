
// WIFI

//WEB Server
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

// Sebagai Station
//const char* wifissid = "ZAINI MIFTAH"; //kalau gagal konek
//const char* wifipassword = "120812rj";

char ipAddress[16];
char ipBuffer[30];

// Sebagai AccessPoint
//const char* ssid = "JWSP10"; //kalau gagal konek
//const char* password = "elektronmart";

IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress netmask(255, 255, 255, 0);

void removeSubstring(char *source, const char *toRemove) {
    char *pos = strstr(source, toRemove);
    if (pos) {
        memmove(pos, pos + strlen(toRemove), strlen(pos + strlen(toRemove)) + 1);
    }
}

void wifiConnect() {
  bool setCnt;

  DEBUG_PRINTLN("");
  DEBUG_PRINTLN("Menghubungkan ke WiFi...");
  DEBUG_PRINT("SSID: "); DEBUG_PRINT(configjws.ssid);
  DEBUG_PRINT(" - PASS: "); DEBUG_PRINTLN(configjws.password);
//  WiFi.mode(WIFI_STA);
  WiFi.begin(configjws.ssid, configjws.password);
  
  while (WiFi.status() != WL_CONNECTED) {
    setCnt = !setCnt;
    if(setCnt){ digitalWrite(buzzer,HIGH); delay(150);  digitalWrite(buzzer,LOW);}
    else{ digitalWrite(buzzer,LOW); }
    DEBUG_PRINT(".");
    delay(400);    
  }
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(buzzer,LOW);
    DEBUG_PRINT("MAC: "); DEBUG_PRINTLN(WiFi.macAddress());
    DEBUG_PRINT("IP: ");  DEBUG_PRINTLN(WiFi.localIP());
    
//    IPAddress ip = WiFi.localIP();
//    sprintf(ipAddress, " - IP: %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    strcpy(ipBuffer, "  -  IP: ");
    WiFi.localIP().toString().toCharArray(ipAddress, sizeof(ipAddress));
    
    strcat(ipBuffer, ipAddress);
    strcat(configjws.namamasjid, ipBuffer);
  } 
}

void WifiShutdown() {
  static bool cekWifi = false;

  if (!cekWifi && millis() >= 3600000) {
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);   
    cekWifi = true;  
    removeSubstring(configjws.namamasjid, ipBuffer);
    DEBUG_PRINTLN("WiFi off after 1 hour.");
    digitalWrite(buzzer,HIGH); delay(80);  digitalWrite(buzzer,LOW);
  }
}
void handleXMLWaktu(){
  XMLWaktu();
  server.send(200,"text/xml",XML);
}

void handleXMLDataJWS(){
  XMLDataJWS();
  server.send(200,"text/xml",XML);
}

void handleSettingJwsUpdate() {
  String datajws = server.arg("plain");
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, datajws);
  File configFileJws = LittleFS.open(fileconfigjws, "w");
  
  if (!configFileJws) {
    DEBUG_PRINTLN("Gagal membuka Info configFileJws untuk ditulis");
    return;
  }  
  serializeJson(doc, configFileJws);
  if (error) {    
    DEBUG_PRINT(F("deserializeJson() gagal kode sebagai berikut: "));
    DEBUG_PRINTLN(error.c_str());
    return;    
  } else {    
    configFileJws.close();
    DEBUG_PRINTLN("Berhasil mengubah configFileJws");
    loadJwsConfig(fileconfigjws, configjws); 
    bacaParameter();
    digitalWrite(buzzer,HIGH); delay(80);  digitalWrite(buzzer,LOW); 
  }  
}
  
//  else {
//    
//    DEBUG_PRINTLN("Gagal tersambung ke Hotspot, mode Hotspot aktif.");
//    
//    WiFi.mode(WIFI_AP);
//    WiFi.softAPConfig(local_ip, gateway, netmask);
//    WiFi.softAP(ssid, password);
//    
//    digitalWrite(buzzer,LOW);
//
//    DEBUG_PRINT("MAC: ");
//    DEBUG_PRINTLN(WiFi.macAddress());
//    DEBUG_PRINT("IP: ");
//    DEBUG_PRINTLN(local_ip);
//    
//  }
