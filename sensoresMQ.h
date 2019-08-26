/*variables 
CO2_mq-135
a=116.6020682
b=-2.769034857
CH4_mq-4
a=1051.932
b=-2.773532
CO_mq-9
a=580.074
b=-2.398271
*/
// Resistencia RL
#define RLOAD 10.0
//--------------CO2-------------------------
// Parametros de la curva
#define scaleFactorCO2 116.6020682
#define exponentCO2 2.769034857
// R0 despues del calentamiento de 24 horas
float RoCO2=0.0;
// Valor atmoferico en ppm del CO2
float atmCO2=0.0;
//------------------------------------------
//------------CH4---------------------------
// Parametros de la curva
#define scaleFactorCH4 1051.932
#define exponentCH4 2.773532
// R0 despues del calentamiento de 24 horas
float RoCH4=0.0;
// Valor atmoferico en ppm del CH4
float atmCH4=0.0;
//------------------------------------------
//------------CO----------------------------
// Parametros de la curva
#define scaleFactorCO 580.074
#define exponentCO 2.398271
// R0 despues del calentamiento de 24 horas
float RoCO=0.0;  
// Valor atmoferico en ppm del CO
float atmCO=0.0;
//------------------------------------------

// Funciones para calcular y calibrar el CO2
float getVoltage(int pin)
{
  int i;
  float voltage=0;
 
  for (i=0;i<5;i++) {
    voltage += ads.readADC_SingleEnded(pin);
    delay(50);
  }
 
  voltage = voltage/5;
  voltage = voltage*mul;//voltage*5.0/32767;
 
  return voltage;
}

float getResistance(int pin) {
  int val=0;
  for(int i=0;i<10;i++){
    val=+ads.readADC_SingleEnded(pin);
    delay(200);
  }
  val=val/10;
  return ((32767./(float)val) * 5. - 1.)*RLOAD;
}

float getPPM(int pin, float Ro, float scaleFactor, float exponent) {
  // pin=convertidor A/D, Ro=encontrado al calibrar el sensor, scaleFactor exponent=curva del sensor
  return scaleFactorCO2 * pow((getResistance(pin)/Ro), -exponentCO2);
}

float getRZero(int pin, float atm, float scaleFactor, float exponent) { 
  // pin=convertidor A/D, atm=ppm del gas en ambiente, scaleFactor exponent=curva del sensor
  return getResistance(pin) * pow((atm/scaleFactor), (1./exponent));
}

void calibrar_CO2(){
  Serial.print("Calibrating...Dioxido de carbono");
  RoCO2=getRZero(2, atmCO2, scaleFactorCO2, exponentCO2);
  Serial.print("Ro=");
  Serial.print(RoCO2);
  Serial.println("ohm");
  delay(2000);
}

void calibrar_CH4(){
  Serial.print("Calibrating...Metano");
  RoCH4=getRZero(1, atmCH4, scaleFactorCH4, exponentCH4);
  Serial.print("Ro=");
  Serial.print(RoCH4);
  Serial.println("ohm");
  delay(2000);
}

void calibrar_CO(){
  Serial.print("Calibrating...Monoxido de carbono");
  RoCO=getRZero(0, atmCO, scaleFactorCO, exponentCO);
  Serial.print("Ro=");
  Serial.print(RoCO);
  Serial.println("ohm");
  delay(2000);
}

void readSensor_CH4(){
  iPPM_CH4 = getPPM(1, RoCH4, scaleFactorCH4, exponentCH4);
  delay(500);
  voltage_CH4 = getVoltage(1);
  delay(500);
  resistence_CH4 = getResistance(1);
}

void readSensor_CO(){
  iPPM_CO = getPPM(0, RoCO, scaleFactorCO, exponentCO);
  delay(500);
  voltage_CO = getVoltage(0);
  delay(500);
  resistence_CO = getResistance(0);
}


void readSensor_CO2(){  
  iPPM_CO2 = getPPM(2, RoCO2, scaleFactorCO2, exponentCO2);
  delay(500);
  voltage_CO2 = getVoltage(2);
  delay(500);
  resistence_CO2 = getResistance(2);
}
