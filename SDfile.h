void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        error += "No se pudo abrir el archivo, para ser escrito- ";
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
        error += "No se pudo escribir en el archivo- ";
    }
    file.close();
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
        error += "No se pudo borrar el archivo- ";
    }
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        error += "No se pudo abrir el archivo, para ser agregado- ";
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
        error += "No se pudo agregar en el archivo- ";
    }
    file.close();
}

void loadConfiguration(const char *filename) {
  File file = SD.open(filename);

  if(!file){
        Serial.println("Failed to open file for reading");
        error += "No se pudo leer el archivo de configuracion- ";
  }
  DeserializationError errorD = deserializeJson(doc, file);
  if (errorD){
    Serial.println(F("Failed to read file, using default configuration"));
    error += "No se pudo deserializar el Json del archivo- ";
  }

  strlcpy(sim.apn, doc["sim"][0] | "", sizeof(sim.apn)); 
  strlcpy(sim.user, doc["sim"][1] | "", sizeof(sim.user)); 
  strlcpy(sim.pass, doc["sim"][2] | "", sizeof(sim.pass)); 
  strlcpy(STA.ssid, doc["sta"][0] | "", sizeof(STA.ssid)); 
  strlcpy(STA.pass, doc["sta"][1] | "", sizeof(STA.pass));
  strlcpy(AP.ssid, doc["ap"][0] | "9-COCO2NH4", sizeof(AP.ssid)); 
  strlcpy(AP.pass, doc["ap"][1] | "", sizeof(AP.pass)); 

  file.close();
}

void saveConfiguration(const char *filename) {

  SD.remove(filename);

  File file = SD.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    error += "No se pudo crear el archivo de configuracion- ";
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

  if (serializeJson(doc, file) == 0) {
    Serial.println(F("Failed to write to file"));
    error +="No se pudo serializar el archivo de configuracion- ";
  }

  file.close();
}

void printFile(const char *filename) {
  File file = SD.open(filename);
  if (!file) {
    Serial.println(F("Failed to read file"));
    error += "No se pudo leer leer el archivo- ";
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
    } else {error +="No existe el Archivo-"; retorno=false; }
  } else {error +="SD no insertada-"; retorno=false;}
  return retorno;
}
