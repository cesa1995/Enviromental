#define TINY_GSM_MODEM_SIM800
//----------Librerias-------------
#include <ArduinoJson.h>
#include "WiFi.h"
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
#include "index.h"
#include "SDfile.h"
#include "time.h"

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

//----------MicroSD--------------
SPIClass sdCard(HSPI); // Crear Objeto SPI para la SD
#define SD_CS 19 // Definir el Pin del micro que va a CS
#define SDSPEED 20000000 // Velocidad del modulo SD

//-----------SIM800L--------------
#include <HardwareSerial.h>
HardwareSerial SerialGsm(2);
gpio_num_t RST = GPIO_NUM_32;

//Apn Configuracion
const char apn[]  = "internet.movistar.ve";
const char user[] = "";
const char pass[] = "";

TinyGsm modem(SerialGsm);
TinyGsmClient GsmClient(modem);

//----------WIFI-----------------
const char* APssid = "7-COCO2NH4";
const char* APpassword =  "12345678";

char STAssid[50];
char STApassword[50];

WiFiServer serverHtml(80); 
WiFiClient WifiClient;

//----------JSON---------------
DynamicJsonDocument areglo(1000);
const size_t CAPACITYobj = JSON_OBJECT_SIZE(10);
StaticJsonDocument<CAPACITYobj> objeto;

//------------MQTTSettings(ThingBoard)--------
PubSubClient mqttGSM(GsmClient);
PubSubClient mqttWIFI(WifiClient);
char broker[] = "demo.thingsboard.io";
#define TOKEN "fVo547kfUrRsMA5KQ31w"
#define FIRMWARE "0.1"
#define NUMERO_SERIAL  "SN-007"

//-----------Reset Variables------
static RTC_NOINIT_ATTR int Mode; //0-AP, 1-STA, 2-GSM
esp_reset_reason_t reason;

#include "Connections.h"
#include "DATA.h"
void setup() {
  //Iniciar Puerto Serial
  Serial.begin(115200);
  //------------MicroSD-------------
  sdCard.begin(5,17,18,19);
  if(!SD.begin(SD_CS,sdCard,SDSPEED)){
      Serial.println(" ");
      Serial.println("Card Mount Failed");
      Serial.println(" ");
      while(true);
  }else{
    Serial.println(" ");
    Serial.println("Card Mount Success");
    Serial.println(" ");
  }
  
  //------------------SIM800L-----------
  delay(10);
  rtc_gpio_init(RST);
  rtc_gpio_set_direction(RST,RTC_GPIO_MODE_OUTPUT_ONLY);

  //----------Chose WIFI AND MQTT Connection (GSM-WIFI)-----------------
  reason=esp_reset_reason();
  delay(100);
  if((reason != ESP_RST_DEEPSLEEP) && (reason != ESP_RST_SW)){
    Mode=2;
  }
  if(Mode==1){
    staSettings();
  }else if(Mode==2) {
    gsmSettings();
  }
  //------------Convertidor I2C----
  ads.begin();
  //------------BME280-------------
  bool status;
  status = bme.begin(0x76);  
  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
  }
}

void loop() {
  switch(Mode){
    case 0:{
      apFunctions();
    }break;
    case 1:{
      staFunctions();
      Serial.println("Modo: STATION");
      SendData(mqttWIFI);
      mqttWIFI.loop();
      delay(2000);
      Serial.println("Sleeping...");
      mqttWIFI.disconnect();
      rtc_gpio_set_level(RST,0);
      esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON); 
      esp_sleep_enable_timer_wakeup(120*1000*1000);
      esp_deep_sleep_start();
    }break;
    case 2:{
      Serial.println("Modo: GSM");
      SendData(mqttGSM);
      mqttGSM.loop();
      delay(2000);
      Serial.println("Sleeping...");
      mqttGSM.disconnect(); 
      rtc_gpio_set_level(RST,0);
      esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON); 
      esp_sleep_enable_timer_wakeup(120*1000*1000);
      esp_deep_sleep_start();
    }break;
  }
}
