#include <ESP8266WiFi.h> 
#include "Adafruit_MQTT.h" 
#include "Adafruit_MQTT_Client.h" 

/************************* WiFi Access Point *********************************/ 
#define WLAN_SSID       "MobinNet5977" 
#define WLAN_PASS       "M77887788" 
#define MQTT_SERVER      "192.168.1.105"  // give static address
#define MQTT_PORT         1883                    
#define MQTT_USERNAME    "" 
#define MQTT_PASSWORD         "" 

// Create an ESP8266 WiFiClient class to connect to the MQTT server. 
WiFiClient client; 

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details. 
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD); 

/****************************** Feeds ***************************************/ 
// Setup a feed called 'hipi' for publishing. 
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname> 
Adafruit_MQTT_Publish hipi = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/pi"); 
// Setup a feed called 'hiesp' for subscribing to changes. 
Adafruit_MQTT_Subscribe hiesp = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "/espn1"); 

/*************************** Sketch Code ************************************/ 
void MQTT_connect(); 

void setup() { 
 Serial.begin(115200); 
 delay(10); 
 Serial.println(F("RPi-ESP-MQTT")); 
 // Connect to WiFi access point. 
 Serial.println(); Serial.println(); 
 Serial.print("Connecting to "); 
 Serial.println(WLAN_SSID); 
 WiFi.begin(WLAN_SSID, WLAN_PASS); 
 while (WiFi.status() != WL_CONNECTED) { 
   delay(500); 
   Serial.print("."); 
 } 
 Serial.println(); 
 Serial.println("WiFi connected"); 
 Serial.println("IP address: "); Serial.println(WiFi.localIP()); 
 // Setup MQTT subscription for hiesp feed. 
 mqtt.subscribe(&hiesp); 
} 



uint32_t x=0; 



void loop() { 
 // Ensure the connection to the MQTT server is alive (this will make the first 
 // connection and automatically reconnect when disconnected).  See the MQTT_connect 
 MQTT_connect(); 
 // this is our 'wait for incoming subscription packets' busy subloop 
 // try to spend your time here 
 // Here its read the subscription 
 Adafruit_MQTT_Subscribe *subscription; 
 while ((subscription = mqtt.readSubscription())) { 
   if (subscription == &hiesp) { 
     Serial.print(F("Recieved: ")); 
     Serial.println((char *)hiesp.lastread); 
   } 
 } 
 
  Mqtt_Publish();
} 



void Mqtt_Publish(){
  // Now we can publish stuff!
  delay(8000);
  Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");
  if (! hipi.publish(x++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
}
// Function to connect and reconnect as necessary to the MQTT server. 
void MQTT_connect() { 
 int8_t ret; 
 // Stop if already connected. 
 if (mqtt.connected()) { 
   return; 
 } 
 Serial.print("Connecting to MQTT... "); 
 uint8_t retries = 3; 
 while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected 
      Serial.println(mqtt.connectErrorString(ret)); 
      Serial.println("Retrying MQTT connection in 5 seconds..."); 
      mqtt.disconnect(); 
      delay(5000);  // wait 5 seconds 
      retries--; 
      if (retries == 0) { 
        // basically die and wait for WDT to reset me 
        while (1); 
      } 
 } 
 Serial.println("MQTT Connected!"); 
}
