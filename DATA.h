void SendData(PubSubClient mqtt){
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
    saveLogs("No se pudo enviar los datos");
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
    saveLogs("No se pudo enviar los datos");
  }
  delay(1000);
}

void SaveData(){
  //------------Save Data-----------
  String datos=getTiempo()+"  "+String(Temperatura)+"  "+String(Humedad)+" "+String(Presion)+" "+String(Altitud)+" "+String(CO)+"  "+String(CH4)+" "+String(CO2)+"\n";
  int sizeStr=datos.length()+1;
  char toSave[sizeStr];
  datos.toCharArray(toSave, sizeStr);
  
  if(SD.exists(Log)){
    appendFile(SD, Log, toSave);
  }else{
    writeFile(SD, Log, "Fecha  Hora  Temperatura(°C)  Humedad(%) Presion(hPa) Altitud(m) CO/MQ9(V)  NH4/MQ4(V) CO2/MQ135(V)\n");
    appendFile(SD, Log, toSave);
  }
}

void MakeData(){
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
    delay(250);
    //Lectura Conversor
    mq9=ads.readADC_SingleEnded(0);
    mq9=ads.readADC_SingleEnded(0);
    mq4=ads.readADC_SingleEnded(1);
    mq135=ads.readADC_SingleEnded(2);
    none=ads.readADC_SingleEnded(3);

    //Convertir en Voltaje
    CO+=mq9*mul;
    CH4+=mq4*mul;
    CO2+=mq135*mul;
    delay(250);
  }
  Serial.println(mq135);
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
}
