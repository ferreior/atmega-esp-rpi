#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include "PubSubClient.h" //Conecta-se ao broker

// Network Parameters
const char* ssid          = "ssid";
const char* wifi_password = "pass";

// MQTT
const char* mqtt_server = "192.168.1.8"; // IP of the MQTT broker
const char* temperature_topic = "casa1/temperatura";
const char* humidity_topic = "casa1/humidade";
const char* clientID = "Estacao_temperatura"; // MQTT client ID

// ThingSpeak information
char* server = "api.thingspeak.com";
unsigned long channelID = 1284079;
char* readAPIKey = "L4DIJFYN4JZKOPKR";
unsigned int dataField1 = 1; // Field to write temperature data
unsigned int dataField2 = 2; 

// Initialise the WiFi and MQTT Client objects
WiFiClient wificlient;
// 1883 is the listener port for the Broker
PubSubClient client(mqtt_server, 1883, wificlient); 

// connect broker to wifi
void connect_MQTT(){
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Connect to the WiFi
  WiFi.begin(ssid, wifi_password);

  // Wait until the connection has been confirmed before continuing
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //check if esp is connected
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //connect to thingspeak client
  ThingSpeak.begin( wificlient );
  
  // Connect to MQTT Broker
  if (client.connect(clientID, NULL, NULL)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }
}

float readTSData( long TSChannel, unsigned int TSField ) {

  float data =  ThingSpeak.readFloatField( TSChannel, TSField, readAPIKey );
  //Serial.println( " Temperature: " + String( data, 9 ) );
  return data;
}

float readTSData2( long TSChannel2, unsigned int TSField2 ) {

  float data2 =  ThingSpeak.readFloatField( TSChannel2, TSField2, readAPIKey );
  //Serial.println( " Humidity: " + String( data2, 9 ) );
  return data2;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {

  connect_MQTT();
  Serial.setTimeout(2000);
  float t = readTSData( channelID, dataField1 );
  float h = readTSData2( channelID, dataField2 );

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");

  // PUBLISH to the MQTT Broker (topic = Temperature, defined at the beginning)

  client.connect(clientID, NULL, NULL);
  // PUBLISH to the MQTT Broker (topic = Humidity, defined at the beginning)
  if (client.publish(humidity_topic, String(h).c_str())) {
    Serial.println("Humidity sent!");
  }
  else {
    Serial.println("Humidity failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID, NULL, NULL);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    
    if(client.publish(humidity_topic, String(h).c_str()))
    Serial.println("Humidity sent!");
    else
    Serial.println("Failed");
  }

    if (client.publish(temperature_topic, String(t).c_str())) {
    Serial.println("Temperature sent!");
  }
  else {
    Serial.println("Temperature failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID, NULL, NULL);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    if(client.publish(temperature_topic, String(t).c_str()))
    Serial.println("Temperature sent!");
    else
    Serial.println("Failed");
  }
  client.disconnect();  // disconnect from the MQTT broker
  delay(1000*30);
}
