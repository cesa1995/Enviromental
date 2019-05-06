boolean mqttConnect(PubSubClient mqtt) {
  Serial.print("Connecting to ");
  Serial.print(broker);

  // Connect to MQTT Broker
  //boolean status = mqtt.connect("GsmClientTest");

  // Or, if you want to authenticate MQTT:
  boolean status = mqtt.connect("Nodemcu_estacion", TOKEN, NULL);

  if (status == false) {
    Serial.println(" fail");
    return false;
  }
  Serial.println(" OK");
  
  String payload = "{";
  payload += "\"firmware\":"; payload += FIRMWARE; payload += ",";
  payload += "\"Numero Serial\":"; payload += NUMERO_SERIAL;
  payload += "}";

  // Send payload
  char attributes[100];
  payload.toCharArray( attributes, 100 );
  mqtt.publish( "v1/devices/me/attributes", attributes );
  Serial.println( attributes );
  delay(300); 
  return mqtt.connected();
}

void mqttChoose(PubSubClient mqtt){
  delay(200);
  mqtt.setServer(broker, 1883);
  if (!mqtt.connected()) {
    boolean status=mqttConnect(mqtt);
    if(!status){
      Serial.println("No conectado al servidor MQTT");
    }
  }
}

String printLocalTime(){
  time_t rawtime;
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
  }
  char timeStringBuff[50]; //50 chars should be enough
  strftime(timeStringBuff, sizeof(timeStringBuff), "%m/%d/%y %H:%M:%S", &timeinfo);
  //print like "const char*"
  //Optional: Construct String object 
  String asString(timeStringBuff);
  return asString;
}

void apSettings(){
    Mode=0;
    WiFi.disconnect();
    Serial.println("Configuring access point...");
    // You can remove the password parameter if you want the AP to be open.
    WiFi.softAP(APssid, APpassword);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    serverHtml.begin();
    Serial.println("HTTP server started");
    Serial.println("Server started");
}

void staSettings(){
    Mode=1;
    if(SD.exists("/config.txt")){
      int trys=0;
      String datos=readFile(SD, "/config.txt");
      char json[500];
      datos.toCharArray(json, 500);
      DeserializationError error = deserializeJson(areglo, json);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        apSettings();
      }
      int WifiSaved=areglo.size();
      
      WiFi.softAPdisconnect(true);
      WiFi.disconnect();
      int i=0;
      while (i<WifiSaved && WiFi.status() != WL_CONNECTED){
        String ssid=areglo[i]["ssid"];
        String password=areglo[i]["pass"];
        ssid.toCharArray(STAssid, 50);
        password.toCharArray(STApassword, 50);
        
        Serial.print("Connecting to ");
        Serial.println(STAssid);
        WiFi.begin(STAssid, STApassword);
        do{
          delay(500);
          Serial.print(".");
          trys++;
        }while(WiFi.status() != WL_CONNECTED && trys<=25);
        trys=0;
        i++;
      }
      if(WiFi.status() != WL_CONNECTED){
        apSettings();
      }else{
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        mqttChoose(mqttWIFI);
      }
    }else{
      apSettings();
    }
}

void gsmSettings(){
  rtc_gpio_set_level(RST,1);
  delay(100);
  Mode=2;
  TinyGsmAutoBaud(SerialGsm);
  delay(100);
  Serial.println("Initializing modem...");
  modem.restart();
  String modemInfo = modem.getModemInfo();
  Serial.print("Modem: ");
  Serial.println(modemInfo);
  Serial.print("Waiting for network...");
  if (modem.waitForNetwork()) {
    Serial.println(" OK");
    do{
      Serial.print("Connecting to ");
      Serial.print(apn);
      modem.gprsConnect(apn,user,pass);
    }while(!modem.isGprsConnected());
    Serial.println(" OK");
    WiFi.mode( WIFI_MODE_NULL );
    delay(500);
    Serial.println("Getting date...");
    Serial.println(modem.getGSMDateTime(DATE_FULL));
    mqttChoose(mqttGSM);
  }else{
    Serial.println(" fail");
    rtc_gpio_set_level(RST,0);
    staSettings();
  }
}

void staFunctions(){
  if(!WiFi.isConnected()){
    apSettings();
  }
  delay(10000);
}

void saveFirts(String ssid, String pass){
      writeFile(SD, "/config.txt", " ");
      JsonArray wifi = areglo.to<JsonArray>();
      JsonObject datos = objeto.to<JsonObject>();
      datos["ssid"]=ssid;
      datos["pass"]=pass;
      wifi.add(datos);
      String Json;
      serializeJson(areglo, Json);
      int sizeJson=Json.length()+1;
      char toSave[sizeJson];
      Json.toCharArray(toSave, sizeJson);
      appendFile(SD, "/config.txt", toSave);
}

void saveConfigSD(String ssid, String pass){
    if(SD.exists("/config.txt")){
      String datos=readFile(SD, "/config.txt");
      char json[500];
      datos.toCharArray(json, 500);
      DeserializationError error = deserializeJson(areglo, json);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
      int WifiSaved=areglo.size();
      if (WifiSaved>8){
        deleteFile(SD, "/config.txt");
        saveFirts(ssid, pass);
      }else{
        writeFile(SD, "/config.txt", " ");
        JsonObject datos = objeto.to<JsonObject>();
        datos["ssid"]=ssid;
        datos["pass"]=pass;
        areglo.add(datos);
        String Json;
        serializeJson(areglo, Json);
        int sizeJson=Json.length()+1;
        char toSave[sizeJson];
        Json.toCharArray(toSave, sizeJson);
        appendFile(SD, "/config.txt", toSave);
      }     
    }else{
      saveFirts(ssid, pass);
    }
}

void apFunctions(){
  WiFiClient client = serverHtml.available();   // Listen for incoming clients
 if (client) {
   // an http request ends with a blank line
   boolean currentLineIsBlank = true;
   while (client.connected()) {
     while(client.available()) {
       char c = client.read();       
       if (c == '\n' && currentLineIsBlank) {
          // Here is where the POST data is.
          String userpass;  
          if(client.available()){
            //Serial.write(client.read());
            const char separador='&';
            const char igual='=';
            //leer documento y separar valores
            userpass=client.readStringUntil('\n');
            String ssid=userpass.substring(0, userpass.indexOf(separador));
            String pass=userpass.substring(userpass.indexOf(separador)+1);
            ssid=ssid.substring(ssid.indexOf(igual)+1);
            pass=pass.substring(pass.indexOf(igual)+1);
            //guardar variables SSID
            ssid.toCharArray(STAssid, 50);
            //guardar variables PASS
            pass.toCharArray(STApassword, 50);
            saveConfigSD(ssid, pass);
            Serial.println("Sending response");
            // send a standard http response header
            client.println("HTTP/1.0 200 OK");
            client.println("Content-Type: text/html");
            client.println();
            String s = DONE;
            client.print(s);
            client.stop();
            delay(100);
            staSettings();
            delay(100);
         }else{
          Serial.println("Sending response");
          // send a standard http response header
          client.println("HTTP/1.0 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          String s = MAIN_page;
          client.print(s);
          client.stop();
         }
       }
       else if (c == '\n') {
         // you're starting a new line
         currentLineIsBlank = true;
       }
       else if (c != '\r') {
         // you've gotten a character on the current line
         currentLineIsBlank = false;
       }
 
     }
   }
 }
 delay(1);
}

