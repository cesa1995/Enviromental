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
  Mensaje+="\"CO\":"; Mensaje+=iPPM_CO; Mensaje+=",";
  Mensaje+="\"CH4\":"; Mensaje+=iPPM_CH4; Mensaje+=",";
  Mensaje+="\"CO2\":"; Mensaje+=iPPM_CO2;
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
  String datos=getTiempo()+"  "+String(Temperatura_copy)+"  "+String(Humedad_copy)+" "+String(Presion_copy)+" "+String(Altitud_copy)+" ";
  datos += String(voltage_CO,8)+" "+String(resistence_CO)+" "+String(iPPM_CO)+" ";
  datos += String(voltage_CH4,8)+" "+String(resistence_CH4)+" "+String(iPPM_CH4)+" ";
  datos += String(voltage_CO2,8)+" "+String(resistence_CO2)+" "+String(iPPM_CO2)+"\n";
  int sizeStr=datos.length()+1;
  char toSave[sizeStr];
  datos.toCharArray(toSave, sizeStr);
  
  if(SD.exists(Log)){
    appendFile(SD, Log, toSave);
  }else{
    writeFile(SD, Log, "Fecha  Hora  Temperatura(°C)  Humedad(%) Presion(hPa) Altitud(m) CO/MQ9(V) CO/MQ9(ohm) CO/MQ9(PPM) CH4/MQ4(V) CH4/MQ4(ohm) COH4/MQ4(PPM) CO2/MQ135(V) CO2/MQ135(ohm) CO2/MQ135(PPM)\n");
    appendFile(SD, Log, toSave);
  }
}
void MakeData(){
  //----------datos----------------
  float Temperatura, Humedad, Presion, Altitud, CO, CH4;
  int16_t mq9, mq4;// COB, CH4B, CO2B;
  int leer=10;
  
  //------------Leer Sensores---------- 
  Temperatura=0;
  Humedad=0;
  Presion=0;
  Altitud=0;
  //CO=0;
  //CH4=0;
  
  for(int i=0; i<leer; i++){
    //Lectura BME
    Temperatura+=bme.readTemperature();
    Humedad+=bme.readHumidity();
    Presion+=bme.readPressure()/100.0F;
    Altitud+=bme.readAltitude(SEALEVELPRESSURE_HPA);
    
    //Lectura Conversor
    //mq9=ads.readADC_SingleEnded(0);
    //mq4=ads.readADC_SingleEnded(1);

    //Convertir en Voltaje
    //CO+=mq9*mul;
    //CH4+=mq4*mul;
    delay(500);
  }
  
  Temperatura=Temperatura/leer;
  Humedad=Humedad/leer;
  Presion=Presion/leer;
  Altitud=Altitud/leer; 

  //CO=CO/leer;
  //CH4=CH4/leer;

  readSensor_CO();
  readSensor_CH4();
  readSensor_CO2();
  
  //------------Mostrar Datos en el Serial-----
  Serial.println("Temperatura I Humedad I Presion I Altitud I MQ9        I MQ9        I MQ9        I MQ4        I MQ4          I MQ4          I MQ135      I MQ135         I MQ135");
  Serial.println("(°C)        I (%)     I (hPa)   I (m)     I (V) CO     I (ohm) CO   I (PPM) CO   I (V) CH4    I (ohm) CH4    I (PPM) CH4    I (V) CO2    I (Kohm) CO2    I (PPM) CO2");
  Serial.print(Temperatura);
  Serial.print("        ");
  Serial.print(Humedad);
  Serial.print("     ");
  Serial.print(Presion);
  Serial.print("    ");
  Serial.print(Altitud);
  Serial.print("    ");
  //CO
  Serial.print(voltage_CO,5);
  Serial.print("       ");
  Serial.print(resistence_CO,0);
  Serial.print("      ");
  Serial.print(iPPM_CO);
  Serial.print("            ");
  //CH4
  Serial.print(voltage_CH4,5);
  Serial.print("      ");
  Serial.print(resistence_CH4,0);
  Serial.print("            ");
  Serial.print(iPPM_CH4);
  Serial.print("             ");
  //CO2
  Serial.print(voltage_CO2,5);
  Serial.print("        ");
  Serial.print(resistence_CO2,0);
  Serial.print("        ");
  Serial.print(iPPM_CO2);
  Serial.println();

  Temperatura_copy=Temperatura;
  Humedad_copy=Humedad;
  Presion_copy=Presion;
  Altitud_copy=Altitud; 

  //CO_copy=CO;
  //CH4_copy=CH4;
}
