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
Adafruit_ADS1115 ads; //(0x48); // Direccion del convertidor 
const float mul= 0.0625/1000; //0.1875/1000;// Multiplicador para convertir a Voltios

//----------BME280----------------
#define SEALEVELPRESSURE_HPA (1013.25) // Definir Una constante de presion
Adafruit_BME280 bme; // Crear Objeto BME

//----------datos----------------
float Temperatura_copy, Humedad_copy, Presion_copy, Altitud_copy;// CO_copy, CH4_copy;

//--------variables finales CO2
long iPPM_CO2 = 0;
float voltage_CO2 = 0;
float resistence_CO2 = 0;
boolean toCalibrate_CO2=false;

//--------variables finales NH4
long iPPM_CH4 = 0;
float voltage_CH4 = 0;
float resistence_CH4 = 0;
boolean toCalibrate_CH4=false;

//--------variables finales CO
long iPPM_CO = 0;
float voltage_CO = 0;
float resistence_CO = 0;
boolean toCalibrate_CO=false;

//-----------SIM800L--------------
#include <HardwareSerial.h>
HardwareSerial SerialGsm(2);
gpio_num_t RST = GPIO_NUM_32;

TinyGsm modem(SerialGsm);
TinyGsmClient GsmClient(modem);

//----------MicroSD--------------
SPIClass sdCard(HSPI); // Crear Objeto SPI para la SD
#define SD_CS 19 // Definir el Pin del micro que va a CS
#define SDSPEED 27000000 // Velocidad del modulo SD
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
static RTC_NOINIT_ATTR int Mode; // 1-STA, 2-GSM, 3-noConected
int ModeTemp=2;
esp_reset_reason_t reason;

//------------Error del equipo-----
String error;

//-----------variables para enviar--
boolean ready_for_send = false;

//------------archivos creados-----
#include "tiempo.h"
#include "sensoresMQ.h"
#include "html.h"
#include "SDfile.h"
#include "links.h"
#include "DATA.h"
#include "mqtt.h"
#include "Connections.h"

//------------Task----------------
TaskHandle_t Task1;
TaskHandle_t Task2;


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
  ads.setGain(GAIN_TWO);
  ads.begin();

  //------------BME280-------------
  bool status;
  status = bme.begin(0x76);  
  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
  }

  //----------Encendido del equipo--
  reason=esp_reset_reason();
  delay(100);
  if((reason != ESP_RST_DEEPSLEEP) && (reason != ESP_RST_SW)){
      //Mode=2;//modo GSM
      //tiempo1=millis();
      //TODO: descomentar el minuto de calentar sensores en la encendida
      //delay(60000);
  }
  //------------configurar STA_AP-------
  wifiSettings();
  //-----------------task1-----------
  xTaskCreatePinnedToCore(
    Task1code,   // Task function. 
    "Task1",     // name of task. 
    10000,       // Stack size of task 
    NULL,        // parameter of the task 
    1,           // priority of the task 
    &Task1,      // Task handle to keep track of created task 
    1);          // pin task to core 0 
  delay(500);
  
  xTaskCreatePinnedToCore(
    Task2code,   // Task function. 
    "Task2",     // name of task. 
    10000,       // Stack size of task 
    NULL,        // parameter of the task 
    1,           // priority of the task 
    &Task2,      // Task handle to keep track of created task 
    0);          // pin task to core 0 
  delay(500);
}

//-----------Task1code-----------------
void Task1code( void * pvParameters ){
  delay(1000);
  btStop();
  for(;;){
    server.handleClient();
    delay(5);
  }
  vTaskDelay(10);
}

//-----------Task2code-----------------
void Task2code( void * pvParameters ){
  //------------configurar STA_AP-------
  for(;;){
    if(toCalibrate_CO2){
      calibrar_CO2();
      saveConfiguration(configuracion);
      toCalibrate_CO2=false; 
    } 
    if(toCalibrate_CH4){
      calibrar_CH4();
      saveConfiguration(configuracion);
      toCalibrate_CH4=false; 
    } 
    if(toCalibrate_CO){
      calibrar_CO();
      saveConfiguration(configuracion);
      toCalibrate_CO=false; 
    }
    MakeData();
    ready_for_send = true;
    delay(10000);
  }
  vTaskDelay(10);
}

void loop() {
  if(ready_for_send && !toCalibrate_CO2){
    switch(Mode){
      //modo Wifi
      case 1:{
        WiFi.enableSTA(true);
        staFunctions();
        WiFi.enableSTA(false);
      }break;
      //modo GSM
      case 2:{
        WiFi.enableSTA(false);
        gsmFunctions();
      }break;
      case 3:{ 
        noConnected();
      }break;
    }
    delay(TIME_TO_SLEEP*60*1000);
  }
  /*if (Mode!=0){
    rtc_gpio_set_level(RST,0);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON); 
    esp_sleep_enable_timer_wakeup((TIME_TO_SLEEP * segundos) * uS_TO_S_FACTOR);
    esp_deep_sleep_start();
  }*/
}
