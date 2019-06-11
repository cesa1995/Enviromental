void apSettings(){
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  Serial.println("Configuring access point...");
  WiFi.softAP(AP.ssid, AP.pass);
  delay(500);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  server.on("/", principal);
  server.on("/apn", apn);
  server.on("/sta", sta);
  server.on("/ap", ap);
  server.on("/log", errores);
  server.on("/save", save);
  server.on("/reloj", reloj);
  server.on("/borrar", borrar);
  server.on("/reboot", reboot);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}

boolean staSettings(){
  boolean readyS=true;
  if(SD.exists(configuracion)){
    int trys=0;
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    Serial.print("Connecting to ");
    Serial.println(STA.ssid);
    WiFi.begin(STA.ssid, STA.pass);
    do{
      delay(500);
      Serial.print(".");
      trys++;
    }while(WiFi.status() != WL_CONNECTED && trys<=10);
        
    if(WiFi.status() != WL_CONNECTED){
      error+="Error conectar al punto de acceso-";
      Mode=0;
      readyS=false;
    }else{
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
      delay(100);
      int cargar=0;
      while(!saveWifiTime() && cargar<5){
        cargar++;
        Serial.print(".");
        delay(200);
      }
      if(cargar>4){
        Serial.println("Failed to obtain time");
        error+="Error obtener hora Wifi-";
        Mode=0;
        readyS=false;
      }else if(!mqttChoose(mqttWIFI)){
        Mode=0;  
        APB=0;
      }
     }
  }else{
    error+="No existe el archivo de Configuracion-";
    Mode=0;
    readyS=false;
  }
  return readyS;
}

boolean gsmSettings(){
  boolean readyC=true;
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  rtc_gpio_set_level(RST,1);
  delay(100);
  if(!TinyGsmAutoBaud(SerialGsm)){
    error+="Modulo Gsm no encendido-";
    readyC=false;
  }
  delay(100);
  if(modem.getSimStatus()==1){
    WiFi.mode( WIFI_MODE_NULL );
    Serial.println("Initializing modem...");
    modem.restart();
    String modemInfo = modem.getModemInfo();
    Serial.print("Modem: ");
    Serial.println(modemInfo);
    Serial.print("Waiting for network...");
    modem.waitForNetwork();
    Serial.println(" OK");
    Serial.print("Connecting to ");
    Serial.print(sim.apn);
    int cargar=0;
    while(!modem.isGprsConnected() && cargar<10){
      modem.gprsConnect(sim.apn,sim.user,sim.pass);
      cargar++;
      Serial.print(".");
      delay(2000);
    }
    if (modem.isGprsConnected()) {
      Serial.println(" OK");
      delay(500);
      Serial.println("Getting date...");
      String fecha=modem.getGSMDateTime(DATE_FULL);
      int yy=fecha.substring(0, fecha.indexOf("/")).toInt()+2000;
      fecha=fecha.substring(fecha.indexOf("/")+1, fecha.length());
      int mm=fecha.substring(0, fecha.indexOf("/")).toInt()-1;
      int dd=fecha.substring(fecha.indexOf("/")+1, fecha.indexOf(",")).toInt();
      int hh=fecha.substring(fecha.indexOf(",")+1, fecha.indexOf(":")).toInt();
      fecha=fecha.substring(fecha.indexOf(":")+1, fecha.length());
      int mi=fecha.substring(0, fecha.indexOf(":")).toInt();

      String date=String(yy)+"-"+String(mm)+"-"+String(dd)+"/"+String(hh)+";"+String(mi);
      if(yy>2000){
        rtc_gpio_set_level(RST,0);
        error+="Problemas obtener hora sim-";
        Mode=1;
        readyC=false; 
      }
      Serial.println(date);
      setTiempo(yy,mm,dd,hh,mi);
      Serial.println(getTiempo());
      if(!mqttChoose(mqttGSM)){
          Mode=1;
      }
    }else{
      Serial.println(" fail");
      rtc_gpio_set_level(RST,0);
      error+="Problemas con el apn, asegure tener datos mobiles en la SIM-";
      Mode=1;
      readyC=false; 
    }
  }else{
    Serial.println(" fail");
    rtc_gpio_set_level(RST,0);
    error+="No hay SIM en la ranura GSM-";
    Mode=1;
    readyC=false; 
  }
  return readyC;
}

void staFunctions(){
  if(!WiFi.isConnected()){
    error+="Se ha desconectado del punto de acceso-";
    apSettings();
    return;
  }
  SaveData();
  ConectSend(mqttWIFI);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON); 
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void apFunctions(){
  server.handleClient();
}

void gsmFunctions(){
  SaveData();
  ConectSend(mqttGSM);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON); 
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
  }

void noConnected(){
  SaveData();
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON); 
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
  }
