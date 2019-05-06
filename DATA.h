void makeData(float Temperatura,float Humedad,float Presion,float Altitud,float CO,float CH4,float CO2, PubSubClient mqtt){
  char attributes[100];
  String Mensaje="{";
  Mensaje+="\"Temperatura\":"; Mensaje+=Temperatura; Mensaje+=",";
  Mensaje+="\"Humedad\":"; Mensaje+=Humedad; Mensaje+=",";
  Mensaje+="\"Presion\":"; Mensaje+=Presion; Mensaje+=",";
  Mensaje+="\"Altitud\":"; Mensaje+=Altitud;
  Mensaje+="}";
  Mensaje.toCharArray( attributes, 100 );
  Serial.print("Sending BME data...");
  if(mqtt.publish( "v1/devices/me/telemetry", attributes )){
    Serial.println(" OK");
  }else{
    Serial.println(" Fail");
  }
  delay(1000);
  
  //Enviar datos del convertidor
  Mensaje="{";
  Mensaje+="\"CO\":"; Mensaje+=CO; Mensaje+=",";
  Mensaje+="\"CH4\":"; Mensaje+=CH4; Mensaje+=",";
  Mensaje+="\"CO2\":"; Mensaje+=CO2;
  Mensaje+="}";
  Mensaje.toCharArray( attributes, 100 );
  Serial.print("Sending Convertidor data...");
  if(mqtt.publish( "v1/devices/me/telemetry", attributes )){
    Serial.println(" OK");
  }else{
    Serial.println(" Fail");
  }
  delay(1000);
}

void SendData(PubSubClient mqtt){
  //------------Leer Sensores----------
  float Temperatura, Humedad, Presion, Altitud, CO, CH4, CO2;
  int16_t mq9, mq4, mq135, none;// COB, CH4B, CO2B;
  int leer=10;

  //Lectura BME
  Temperatura+=bme.readTemperature();
  Humedad+=bme.readHumidity();
  Presion+=bme.readPressure()/100.0F;
  Altitud+=bme.readAltitude(SEALEVELPRESSURE_HPA);

  CO=0;
  CH4=0;
  CO2=0;
  
  //Lectura Conversor
  for(int i=0; i<leer; i++){
    mq9=ads.readADC_SingleEnded(0);
    mq9=ads.readADC_SingleEnded(0);
    mq4=ads.readADC_SingleEnded(1);
    mq135=ads.readADC_SingleEnded(2);
    none=ads.readADC_SingleEnded(3);

    //Convertir en Voltaje
    CO+=mq9*mul;
    CH4+=mq4*mul;
    CO2+=mq135*mul;
    delay(300);
  }

  CO=CO/leer;
  CO2=CO2/leer;
  CH4=CH4/leer;

  //------------Save Data-----------
  String dateTime;
  switch(Mode){
    case 1:{
      dateTime=printLocalTime();
      Serial.println(dateTime);
    }break;
    case 2:{
      dateTime= modem.getGSMDateTime(DATE_FULL);   
    }break;
  }
  String Date=dateTime.substring(0,8);
  String Time=dateTime.substring(9,17);
  String datos=Date+" "+Time+"  "+String(Temperatura)+"  "+String(Humedad)+" "+String(Presion)+" "+String(Altitud)+" "+String(CO)+"  "+String(CH4)+" "+String(CO2)+"\n";
  int sizeStr=datos.length()+1;
  char toSave[sizeStr];
  datos.toCharArray(toSave, sizeStr);
  
  if(SD.exists("/Log.txt")){
    appendFile(SD, "/Log.txt", toSave);
  }else{
    writeFile(SD, "/Log.txt", "Fecha  Hora  Temperatura(°C)  Humedad(%) Presion(hPa) Altitud(m) CO/MQ9(V)  NH4/MQ4(V) CO2/MQ135(V)\n");
    appendFile(SD, "/Log.txt", toSave);
  }
  
  //------------Mostrar Datos en el Serial-----
  Serial.println("Temperatura I Humedad I Presion I Altitud I MQ9    I MQ4     I MQ135  ");
  Serial.println("(°C)        I (%)     I (hPa)   I (m)     I (V) CO I (V) CH4 I (V) CO2");
  Serial.print(Temperatura);
  Serial.print("        ");
  Serial.print(Humedad);
  Serial.print("     ");
  Serial.print(Presion);
  Serial.print("    ");
  Serial.print(Altitud);
  Serial.print("    ");
  Serial.print(CO);
  Serial.print("      ");
  Serial.print(CH4);
  Serial.print("       ");
  Serial.print(CO2);
  Serial.println("");

  //------------Enviar Data----------------
  //Enviar datos del BME
  if(!mqtt.connected()){
    Serial.print("Broker connecting...");
    if(!mqttConnect(mqtt)){
      Serial.println("Fail");    
    }else{
      Serial.println("OK");
      delay(300);
      makeData(Temperatura, Humedad, Presion, Altitud, CO, CH4, CO2, mqtt);
    }
  }else{
    makeData(Temperatura, Humedad, Presion, Altitud, CO, CH4, CO2, mqtt);
  }
}

