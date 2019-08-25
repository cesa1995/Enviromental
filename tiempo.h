boolean saveWifiTime(){
  boolean statust;
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    statust=false;
  }else{
    statust=true;
  }
  return statust;
}

void setTiempo(int set_year, int set_mon, int set_mday, int set_hour, int set_min){
  struct tm tm;
  tm.tm_year = set_year - 1900;//2018 - 1900;
  tm.tm_mon = set_mon;//10;
  tm.tm_mday = set_mday;//15;
  tm.tm_hour = set_hour;//14;
  tm.tm_min = set_min;//10;
  tm.tm_sec = 00;//10;
  time_t t = mktime(&tm);
  printf("Setting time: %s", asctime(&tm));
  struct timeval now = { .tv_sec = t };
  settimeofday(&now, NULL);
  }

String getTiempo(){
  String tiempo;
  time_t now = time(0);
  // Convert now to tm struct for local timezone
  tm* localtm = localtime(&now);
  tiempo=String(localtm->tm_year+1900)+"/"+String(localtm->tm_mday)+"/"+String(localtm->tm_mon+1)+" "+String(localtm->tm_hour)+":"+String(localtm->tm_min);
  //Serial.println(tiempo);
  return tiempo;
}
