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
  server.on("/time", Ptime);
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
      saveLogs("Error conectar al punto de acceso");
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
        saveLogs("Error obtener hora Wifi");
        readyS=false;
      }else{ 
        readyS=mqttChoose(mqttWIFI);
      }
     }
  }else{
    saveLogs("No existe el archivo de Configuracion");
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
    saveLogs("Modulo Gsm no encendido");
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
      cargar=0;
      while(!modem.syncTime(-16) && cargar<10){
        Serial.print(".");
        cargar++;
      }
      delay(500);
      Serial.println("Getting date...");
      if(cargar<10){
        String fecha=modem.getGSMDateTime(DATE_FULL);
        int yy=fecha.substring(0, fecha.indexOf("/")).toInt()+2000;
        fecha=fecha.substring(fecha.indexOf("/")+1, fecha.length());
        int mm=fecha.substring(0, fecha.indexOf("/")).toInt()-1;
        int dd=fecha.substring(fecha.indexOf("/")+1, fecha.indexOf(",")).toInt();
        int hh=fecha.substring(fecha.indexOf(",")+1, fecha.indexOf(":")).toInt();
        fecha=fecha.substring(fecha.indexOf(":")+1, fecha.length());
        int mi=fecha.substring(0, fecha.indexOf(":")).toInt();
  
        String date=String(yy)+"-"+String(mm)+"-"+String(dd)+"/"+String(hh)+";"+String(mi);
        Serial.println(date);
        setTiempo(yy,mm,dd,hh,mi);
        Serial.println(getTiempo());
        readyC=mqttChoose(mqttGSM);
      }else{
        rtc_gpio_set_level(RST,0);
        saveLogs("Problemas obtener hora sim");
        readyC=false; 
      }
    }else{
      Serial.println(" fail");
      rtc_gpio_set_level(RST,0);
      saveLogs("Problemas con el apn, asegure tener datos mobiles en la SIM");
      readyC=false; 
    }
  }else{
    Serial.println(" fail");
    rtc_gpio_set_level(RST,0);
    saveLogs("No hay SIM en la ranura GSM");
    readyC=false; 
  }
  return readyC;
}

void staFunctions(){
  if(!WiFi.isConnected()){
    if(staSettings()){
      ConectSend(mqttWIFI);
    }else{
      saveLogs("No se ha podido conectar al punto de acceso");
    }
  }else{
    ConectSend(mqttWIFI);
  }
  SaveData();
}

void apFunctions(){
  server.handleClient();
}

void gsmFunctions(){
  if(!modem.isGprsConnected()){
    if(gsmSettings()){
      ConectSend(mqttGSM);
    }else{
      saveLogs("No se ha podido conectar a la red GSM");
    }
  }else{
    ConectSend(mqttGSM);
  }
  SaveData();
}

void noConnected(){
  SaveData();
}
