#define TINY_GSM_MODEM_SIM800
//----------Librerias-------------
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>
#include <TinyGsmClient.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <driver/rtc_io.h>
#include <time.h>
#include <sys/time.h>

//------------Dormir---------------
#define uS_TO_S_FACTOR 1000000  //Conversion factor for micro seconds to seconds
#define segundos 60 //conversion factor for minutes to seconds
int TIME_TO_SLEEP=1;     //Time ESP32 will go to sleep (in minute) default 1 min

//----------NTP Time------------
const char* ntpServer = "0.south-america.pool.ntp.org";
const long  gmtOffset_sec = -14400;
const int   daylightOffset_sec = 0;

//----------Convertidor I2C-------
Adafruit_ADS1115 ads(0x48); // Direccion del convertidor 
const float mul=0.1875/1000; // Multiplicador para convertir a Voltios

//----------BME280----------------
#define SEALEVELPRESSURE_HPA (1013.25) // Definir Una constante de presion
Adafruit_BME280 bme; // Crear Objeto BME

//----------datos----------------
float Temperatura, Humedad, Presion, Altitud, CO, CH4, CO2;
int16_t mq9, mq4, mq135, none;// COB, CH4B, CO2B;
int leer=10;

//-----------SIM800L--------------
#include <HardwareSerial.h>
HardwareSerial SerialGsm(2);
gpio_num_t RST = GPIO_NUM_32;

TinyGsm modem(SerialGsm);
TinyGsmClient GsmClient(modem);

//----------MicroSD--------------
SPIClass sdCard(HSPI); // Crear Objeto SPI para la SD
#define SD_CS 19 // Definir el Pin del micro que va a CS
#define SDSPEED 20000000 // Velocidad del modulo SD
boolean SD_present = true;

//----------WIFI-----------------
WiFiClient WifiClient;
WebServer server(80);

//----------JSON---------------
DynamicJsonDocument doc(700);

//configuraciones de conexion
//Wifi AP configuracion
struct wifiAP{
  char ssid[50];
  char pass[50];
};

//Wifi Sta configuracion
struct wifiSTA{
  char ssid[50];
  char pass[50];  
};

//Apn Configuracion
struct SIM{
  char apn[50];
  char user[50];
  char pass[50];
};

wifiSTA STA;
SIM sim;
wifiAP AP;

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
  
const char *configuracion="/config.txt";
const char *Log="/Data.txt";
const char *errorFile="/Log.txt";

//------------MQTTSettings(ThingBoard)--------
PubSubClient mqttGSM(GsmClient);
PubSubClient mqttWIFI(WifiClient);
char broker[] = "demo.thingsboard.io";
#define TOKEN "chW5YCF2eXJjqeTSquvC"
#define FIRMWARE "0.1"
#define NUMERO_SERIAL  "SN-009"

//-----------Reset Variables------
static RTC_NOINIT_ATTR int Mode; //0-AP, 1-STA, 2-GSM, 3-noConected
int ModeTemp=2;
esp_reset_reason_t reason;
unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;

//------------Error del equipo-----
String error;
int APB=0;
int APBT=0;

//------------archivos creados-----
#include "tiempo.h"
#include "html.h"
#include "SDfile.h"
#include "links.h"
#include "DATA.h"
#include "mqtt.h"
#include "Connections.h"

//------------Task----------------
TaskHandle_t Task1;

void setup() {
  //Iniciar Puerto Serial
  Serial.begin(115200);
  //------------------SIM800L-----------
  delay(10);
  rtc_gpio_init(RST);
  rtc_gpio_set_direction(RST,RTC_GPIO_MODE_OUTPUT_ONLY);
  rtc_gpio_set_level(RST,0);
  delay(2000);
  //------------MicroSD-------------
  sdCard.begin(5,17,18,19);
  int trysd=0;
  while(!SD.begin(SD_CS,sdCard,SDSPEED) && trysd<50){
    Serial.print(".");
    trysd++;
    delay(200);
  }
  if(trysd>49){
      Serial.println(" ");
      Serial.println("Card Mount Failed");
      Serial.println(" ");
      SD_present=false;
      error+="Card Mount Failed-";
  }
  if(trysd<50){
    Serial.println(" ");
    Serial.println("Card Mount Success");
    Serial.println(" ");
    delay(1000);
    loadLogs();
  }
  //------------Cargar configuracion---
  loadConfiguration(configuracion);
  ModeTemp=Mode;
  printFile(configuracion);
  
  //------------Convertidor I2C----
  ads.begin();
  //------------BME280-------------
  bool status;
  status = bme.begin(0x76);  
  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
  }
  reason=esp_reset_reason();
  delay(100);
  if((reason != ESP_RST_DEEPSLEEP) && (reason != ESP_RST_SW)){
      //Mode=2;//modo GSM
      tiempo1=millis();
      //TODO: descomentar el minuto de calentar sensores en la encendida
      //delay(60000);
  }  
  //------------Convertidor I2C----
  ads.begin();
  //------------BME280-------------
  bool statusBME;
  statusBME = bme.begin(0x76);  
  if (!statusBME) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
  }
  //-----------------task1--------------
  xTaskCreatePinnedToCore(
    Task1code,   /* Task function. */
    "Task1",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */                  
  delay(500); 
}

//-----------Task1code-----------------
void Task1code( void * pvParameters ){
  for(;;){
    //Serial.print("Task1 running on core ");
    //Serial.println(xPortGetCoreID());
    MakeData();
    delay(10000);
  }
  vTaskDelay(10);
}

void loop() {
  //MakeData();
  delay(10000);
  /*Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());
  if(tiempo1!=0){
    tiempo2=millis();
    if(tiempo2 < (tiempo1+(3*60*1000))){
      Mode=0;
    }else if(APBT==0){
      APBT++;
      Mode=ModeTemp;
    }
  }
  switch(Mode){
    //modo configuracion del wifi
    case 0:{
      if (APB==0){
        apSettings(); 
        APB++;
      }
      apFunctions();
    }break;
    //modo Wifi
    case 1:{
      MakeData();
      staFunctions();
    }break;
    //modo GSM
    case 2:{
      MakeData();  
      gsmFunctions();
    }break;
    case 3:{
      MakeData();  
      noConnected();
    }break;
  }
  
  if (Mode!=0){
    rtc_gpio_set_level(RST,0);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON); 
    esp_sleep_enable_timer_wakeup((TIME_TO_SLEEP * segundos) * uS_TO_S_FACTOR);
    esp_deep_sleep_start();
  }*/
}
