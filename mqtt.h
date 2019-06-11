boolean mqttConnect(PubSubClient mqtt) {
  Serial.print("Connecting to ");
  Serial.print(broker);

  // Connect to MQTT Broker
  //boolean status = mqtt.connect("GsmClientTest");

  // Or, if you want to authenticate MQTT:
  boolean status = mqtt.connect("Nodemcu_estacion", TOKEN, NULL);

  if (status == false) {
    Serial.println(" fail");
    return false;
  }
  Serial.println(" OK");
  
  String payload = "{";
  payload += "\"firmware\":"; payload += FIRMWARE; payload += ",";
  payload += "\"Numero Serial\":"; payload += NUMERO_SERIAL;
  payload += "}";

  // Send payload
  char attributes[100];
  payload.toCharArray( attributes, 100 );
  mqtt.publish( "v1/devices/me/attributes", attributes );
  Serial.println( attributes );
  delay(300); 
  return mqtt.connected();
}

void ConectSend(PubSubClient mqtt){
  //------------Enviar Data----------------
  //Enviar datos del BME
  if(!mqtt.connected()){
    Serial.print("Broker connecting...");
    if(!mqttConnect(mqtt)){
      Serial.println("Fail");    
    }else{
      Serial.println("OK");
      delay(300);
      SendData(mqtt);
    }
  }else{
    SendData(mqtt);
  }  
}

boolean mqttChoose(PubSubClient mqtt){
  delay(200);
  boolean statusmqtt=true;
  mqtt.setServer(broker, 1883);
  if (!mqtt.connected()) {
    statusmqtt=mqttConnect(mqtt);
    if(!statusmqtt){
      Serial.println("No conectado al servidor MQTT");
      error+="No se pudo conectar al servidor MQTT-";
    }
  }
  return statusmqtt;
}
