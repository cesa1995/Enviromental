void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

String readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);
    String datos;
    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
    }

    Serial.print("Read from file: ");
    while(file.available()){
        datos=file.readString();
        //Serial.write(datos);
    }
    file.close();
    return datos;
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

void loadLogs(){
  if(SD.exists(errorFile)){
    error=readFile(SD, errorFile);
  }else{
    error+="No hay errores Guardados -";  
  }
}

void saveLogs(String errorString){
  if (SD_present) { 
    String errortime="["+getTiempo()+"] -"+errorString;
    int sizeStr=errortime.length()+1;
    char toSave[sizeStr];
    errortime.toCharArray(toSave, sizeStr);
    
    if(SD.exists(errorFile)){
      appendFile(SD, errorFile, toSave);
    }else{
      writeFile(SD, errorFile, toSave);
    }
  }else{
    error="SD no insertada";
  }
}

void loadConfiguration(const char *filename) {
  File file = SD.open(filename);

  if(!file){
    Serial.println("Failed to open file for reading");
    saveLogs("No se pudo leer el archivo de configuracion");
  }
  DeserializationError errorD = deserializeJson(doc, file);
  if (errorD){
    Serial.println(F("Failed to read file, using default configuration"));
    saveLogs("No se pudo deserializar el Json del archivo");
  }

  strlcpy(sim.apn, doc["sim"][0] | "", sizeof(sim.apn)); 
  strlcpy(sim.user, doc["sim"][1] | "", sizeof(sim.user)); 
  strlcpy(sim.pass, doc["sim"][2] | "", sizeof(sim.pass)); 
  strlcpy(STA.ssid, doc["sta"][0] | "", sizeof(STA.ssid)); 
  strlcpy(STA.pass, doc["sta"][1] | "", sizeof(STA.pass));
  strlcpy(AP.ssid, doc["ap"][0] | "9-COCO2NH4", sizeof(AP.ssid)); 
  strlcpy(AP.pass, doc["ap"][1] | "", sizeof(AP.pass)); 
  TIME_TO_SLEEP=doc["sleep"] | 1;
  Mode=doc["mode"] | 0;

  file.close();
  delay(1000);
}

void saveConfiguration(const char *filename) {

  SD.remove(filename);

  File file = SD.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    saveLogs("No se pudo crear el archivo de configuracion");
    return;
  }
  
  JsonArray SIM=doc.createNestedArray("sim");
  SIM.add(sim.apn);
  SIM.add(sim.user);
  SIM.add(sim.pass);

  JsonArray sta=doc.createNestedArray("sta");
  sta.add(STA.ssid);
  sta.add(STA.pass);

  JsonArray ap=doc.createNestedArray("ap");
  ap.add(AP.ssid);
  ap.add(AP.pass);

  doc["sleep"]=TIME_TO_SLEEP;

  doc["mode"]=ModeTemp;

  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
    saveLogs("No se pudo serializar el archivo de configuracion");
  }

  file.close();
  delay(1000);
}

void printFile(const char *filename) {
  File file = SD.open(filename);
  if (!file) {
    Serial.println(F("Failed to read file"));
    saveLogs("No se pudo leer leer el archivo");
    return;
  }

  while (file.available()) {
    Serial.print((char)file.read());
  }
  Serial.println();

  file.close();
}

boolean SD_file_download(String filename){
  boolean retorno=true;
  if (SD_present) { 
    File download = SD.open("/"+filename);
    if (download) {
      server.sendHeader("Content-Type", "text/text");
      server.sendHeader("Content-Disposition", "attachment; filename="+filename);
      server.sendHeader("Connection", "close");
      server.streamFile(download, "application/octet-stream");
      download.close();
    } else {saveLogs("No existe el Archivo"); retorno=false; }
  } else {error ="SD no insertada-"; retorno=false;}
  return retorno;
}
