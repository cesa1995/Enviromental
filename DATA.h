void SendData(PubSubClient mqtt){
  char attributes[100];
  String Mensaje="{";
  Mensaje+="\"Temperatura\":"; Mensaje+=Temperatura_copy; Mensaje+=",";
  Mensaje+="\"Humedad\":"; Mensaje+=Humedad_copy; Mensaje+=",";
  Mensaje+="\"Presion\":"; Mensaje+=Presion_copy; Mensaje+=",";
  Mensaje+="\"Altitud\":"; Mensaje+=Altitud_copy;
  Mensaje+="}";
  Mensaje.toCharArray( attributes, 100 );
  Serial.print("Sending BME data...");
  if(mqtt.publish( "v1/devices/me/telemetry", attributes )){
    Serial.println(" OK");
  }else{
    Serial.println(" Fail");
    saveLogs("No se pudo enviar los datos");
  }
  delay(1000);
  
  //Enviar datos del convertidor
  Mensaje="{";
  Mensaje+="\"CO\":"; Mensaje+=CO_copy; Mensaje+=",";
  Mensaje+="\"CH4\":"; Mensaje+=CH4_copy; Mensaje+=",";
  Mensaje+="\"CO2\":"; Mensaje+=CO2_copy;
  Mensaje+="}";
  Mensaje.toCharArray( attributes, 100 );
  Serial.print("Sending Convertidor data...");
  if(mqtt.publish( "v1/devices/me/telemetry", attributes )){
    Serial.println(" OK");
  }else{
    Serial.println(" Fail");
    saveLogs("No se pudo enviar los datos");
  }
  ready_for_send = false;
  delay(1000);
}

void SaveData(){
  //------------Save Data-----------
  String datos=getTiempo()+"  "+String(Temperatura_copy)+"  "+String(Humedad_copy)+" "+String(Presion_copy)+" "+String(Altitud_copy)+" "+String(CO_copy)+"  "+String(CH4_copy)+" "+String(CO2_copy)+"\n";
  int sizeStr=datos.length()+1;
  char toSave[sizeStr];
  datos.toCharArray(toSave, sizeStr);
  
  if(SD.exists(Log)){
    appendFile(SD, Log, toSave);
  }else{
    writeFile(SD, Log, "Fecha  Hora  Temperatura(°C)  Humedad(%) Presion(hPa) Altitud(m) CO/MQ9(V)  CH4/MQ4(V) CO2/MQ135(V)\n");
    appendFile(SD, Log, toSave);
  }
}
void MakeData(){
  //----------datos----------------
  float Temperatura, Humedad, Presion, Altitud, CO, CH4, CO2;
  int16_t mq9, mq4, mq135, none;// COB, CH4B, CO2B;
  int leer=10;
  
  //------------Leer Sensores---------- 
  Temperatura=0;
  Humedad=0;
  Presion=0;
  Altitud=0;
  CO=0;
  CH4=0;
  CO2=0;
  
  for(int i=0; i<leer; i++){
    //Lectura BME
    Temperatura+=bme.readTemperature();
    Humedad+=bme.readHumidity();
    Presion+=bme.readPressure()/100.0F;
    Altitud+=bme.readAltitude(SEALEVELPRESSURE_HPA);
    
    //Lectura Conversor
    mq9=ads.readADC_SingleEnded(0);
    mq4=ads.readADC_SingleEnded(1);
    mq135=ads.readADC_SingleEnded(2);
    none=ads.readADC_SingleEnded(3);

    //Convertir en Voltaje
    CO+=mq9*mul;
    CH4+=mq4*mul;
    CO2+=mq135*mul;
    delay(500);
  }
  
  Temperatura=Temperatura/leer;
  Humedad=Humedad/leer;
  Presion=Presion/leer;
  Altitud=Altitud/leer; 

  CO=CO/leer;
  CO2=CO2/leer;
  CH4=CH4/leer;
  
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

  Temperatura_copy=Temperatura;
  Humedad_copy=Humedad;
  Presion_copy=Presion;
  Altitud_copy=Altitud; 

  CO_copy=CO;
  CO2_copy=CO2;
  CH4_copy=CH4;
}
