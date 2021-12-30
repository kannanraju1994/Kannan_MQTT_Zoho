/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

//String WLAN_SSID  =     "Kannan_Viji";
String WLAN_SSID  =     "Kannan_Viji";
String WLAN_PASS   =    "viji@123";

/************************* Adafruit.io Setup *********************************/

String AIO_SERVER     =  "io.adafruit.com";
String AIO_SERVERPORT =  "1883";                   // use 8883 for SSL
String AIO_USERNAME   =  "kannanraju1994";
String AIO_KEY        =  "aio_Ibyx545KjLJlusudbRkCz8g0DT8y";
String FEED_NAME      =  "/feeds/potValue";
String user_name = AIO_USERNAME+FEED_NAME;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER.c_str(), atoi(AIO_SERVERPORT.c_str()), AIO_USERNAME.c_str(), AIO_KEY.c_str());
Adafruit_MQTT_Publish potValue = Adafruit_MQTT_Publish(&mqtt,user_name.c_str() );
void MQTT_connect();


uint32_t potAdcValue = 0;
uint16_t ledBrightValue = 0;

int wifi_connection()
{
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID.c_str(), WLAN_PASS.c_str());
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  return true;
}


void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));
  Serial.println("user_name : " +user_name);
  wifi_connection();


}

void loop() 
{
  MQTT_connect();
    Serial.print(F("Sending pot val "));
    Serial.print(potAdcValue++);
    Serial.print("...");
    if (! potValue.publish(potAdcValue)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
    }
    delay(10000);

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
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
