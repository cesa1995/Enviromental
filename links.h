void principal(){
  boolean pagina=true;
    if (server.args() > 0 ) { // Arguments were received
      if (server.hasArg("download")) {pagina=SD_file_download(server.arg(0));}
    }
    if(pagina){
      server.send(200, "text/html", LoadPage(0));
    }else{
      server.send(200, "text/html", LoadPage(9));
    }
  }

void apn(){
  server.send(200, "text/html", LoadPage(1));
  }

void sta(){
  server.send(200, "text/html", LoadPage(2));
  }

void ap(){
  server.send(200, "text/html", LoadPage(3));
  }

void reloj(){
  server.send(200, "text/html", LoadPage(4));
  }

void borrar(){
  if (server.args() > 0 ) { // Arguments were received
    if (server.hasArg("type")) {
      if(server.arg("type").toInt()==0){
        SD.remove(Log);
      }
      if(server.arg("type").toInt()==1){
        SD.remove(configuracion);
      }
    }
  }
  
  server.send(200, "text/html", LoadPage(8));
}

void save(){
  int type=server.arg("type").toInt();
  char datos[50];
  switch(type){
    //apn
    case 1:{
      server.arg("APN").toCharArray(datos, 50);
      strlcpy(sim.apn, datos, sizeof(sim.apn));
      server.arg("USUARIO").toCharArray(datos, 50);
      strlcpy(sim.user, datos, sizeof(sim.user));
      server.arg("PASSWORD").toCharArray(datos, 50);
      strlcpy(sim.pass, datos, sizeof(sim.pass));
      }break;
    //sta
    case 2:{
      server.arg("SSID").toCharArray(datos, 50);
      strlcpy(STA.ssid, datos, sizeof(STA.ssid));
      server.arg("PASSWORD").toCharArray(datos, 50);
      strlcpy(STA.pass, datos, sizeof(STA.pass));
      }break;
    //ap
    case 3:{
      server.arg("SSID").toCharArray(datos, 50);
      strlcpy(AP.ssid, datos, sizeof(AP.ssid));
      server.arg("PASSWORD").toCharArray(datos, 50);
      strlcpy(AP.pass, datos, sizeof(AP.pass));
      }break;
    case 4:{
      int yeard=server.arg("date").substring(0,4).toInt();
      int mes=server.arg("date").substring(5,7).toInt()-1;
      int dia=server.arg("date").substring(8,10).toInt();
      int hora=server.arg("time").substring(0,2).toInt();
      int minu=server.arg("time").substring(3,6).toInt();

      String prueba= String(yeard)+";"+String(mes)+";"+String(dia)+"/"+String(hora)+"*"+String(minu);
      Serial.println(prueba);
      setTiempo(yeard, mes, dia, hora, minu);
      }break;
      
    }
  if(type==0 || type==1 || type==2 || type==3){  
    saveConfiguration(configuracion);
  }else{
     
  }
  server.send(200, "text/html", LoadPage(5));
  }

void errores(){
  server.send(200, "text/html", LoadPage(6));
  }

void reboot(){
  if (server.args() > 0 ) { // Arguments were received
    if (server.hasArg("Mode")) {
      if(server.arg("Mode").toInt()==1){
        Mode=1;
      }
      if(server.arg("Mode").toInt()==3){
        Mode=3;
      }
    }
  }else{
    Mode=2;
  }
  server.send(200, "text/html", LoadPage(7));
  delay(500);
  ESP.restart();
}

void handle_NotFound(){
    server.send(404, "text/plain", "Not found");
  }
