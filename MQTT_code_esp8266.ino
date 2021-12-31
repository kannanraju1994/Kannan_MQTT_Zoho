/* 
MQTT Server Details:
Link : https://io.adafruit.com/
create new project, and copy the username, password, key, topic.
*/

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "MQTT_Server_config.h"
#define Mode_switch 14 
WiFiClient client;


void Server_values_init();
int wifi_connection();
void MQTT_communication();

void setup() {
  Serial.begin(115200);
  delay(10);
  SPIFFS.begin();  
  Serial.println(F("Kannan - Zoho MQTT demo"));  
  Server_values_init(); 
  pinMode(Mode_switch, INPUT);
}
void loop() 
{
  int i;
  //  Initially mode switch is press continuously 100 ms its change configuration mode else MQTT Random number is update every 10s
 for (i=0; i<100 && digitalRead(Mode_switch)==0; i++)             
        delay(1);
 if(i == 100)      
     server_api_confic_function();    //  server page handle      
  else
      wifi_connection();              //  connect wifi network
  MQTT_communication();               // publisher start communication 
}

void MQTT_communication()
{  
uint32_t potAdcValue = 0;
Adafruit_MQTT_Client mqtt(&client, server_name.c_str(), atoi(server_port.c_str()), server_username.c_str(), server_key.c_str());
Adafruit_MQTT_Publish potValue = Adafruit_MQTT_Publish(&mqtt,user_name.c_str() );

  while(1)
  {
  if (mqtt.connected())                         //  check MQTT communication
  {
    if (! potValue.publish(potAdcValue++)) 
      Serial.println(F("Failed"));
    else 
      Serial.println(F("OK!"));    
         delay(10000); 
  }
else                                            
{
  int8_t ret;
  Serial.print("Connecting to MQTT... ");
  while ((ret = mqtt.connect()) != 0) 
  { 
       mqtt.disconnect();
       delay(5000);  
  }
}  
}
}

void Server_values_init()                                               // appling all server input datas
{     
  server_name     = GetPage("/server_name.txt");                        
  server_port     = GetPage("/server_port.txt");      
  server_username = GetPage("/server_username.txt");
  server_key      = GetPage("/server_key.txt");
  server_feedname = GetPage("/server_feedname.txt");
  wifi_name       = GetPage("/wifi_name.txt");
  wifi_pass       = GetPage("/wifi_pass.txt");
  user_name = server_username+server_feedname;
}

int wifi_connection()
{ 
  WiFi.begin(wifi_name.c_str(), wifi_pass.c_str());
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
