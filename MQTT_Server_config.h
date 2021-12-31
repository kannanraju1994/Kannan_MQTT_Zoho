
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <SPI.h>
#include <FS.h>
#include <ArduinoJson.h>
const char* temp_json;
FS* filesystem = &SPIFFS;
ESP8266HTTPUpdateServer httpUpdater;
ESP8266WebServer server(80);

const char * headerkeys[] = {"User-Agent", "Cookie"} ;
size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);

String wifi_name          =  "";
String wifi_pass          =  "";
String server_name        =  "";
String server_port        =  "";                   
String server_username    =  "";
String server_key         =  "";
String server_feedname    =  "";
String user_name          =  "";
StaticJsonDocument<300> json_string;
String GetPage(String FileName)             // get file and read the data
{
  File page = SPIFFS.open(FileName, "r");
  if (!page)                                        //  check file pressent or not
  {
  Serial.println("Failed to open"+FileName);
  return "";                                      // return NULL
  }
  String content = page.readString();// read file content
  page.close();
  return content;             //  return content
}

void save_file(String path, String data)            /// give path amd payload data is stored
{
  File f = SPIFFS.open(path, "w");
  if (!f)
      Serial.println(path+" FILE NOT OPEN");
  else
  {
    Serial.println("Writing Data to "+path+" File\n"+data);
    f.print(data);
    f.close();
  }
}

bool save_server_values(String data)
{

  if(data == "")     
      return 0;
  else  
  {                        
    DeserializationError error = deserializeJson(json_string, data);
   if (error)
      Serial.println(F("deserializeJson() failed: "));
    else
    {
      String file_tittle = "";
      String file_value = "";
     if(json_string.containsKey("name"))
      {
        temp_json = json_string["name"];             
        file_tittle = String(temp_json);      
        if(json_string.containsKey("value"))
        {
          temp_json = json_string["value"];             
          file_value = String(temp_json);      
          save_file("/"+file_tittle+".txt",file_value);          
        }
      }
    }
  }
  return 1;
}
void save()
{  
  String data_in = server.arg(0);
  if(save_server_values(data_in))
    server.send(200, "text/plain", "Save Done!");
  else
    server.send(200, "text/plain", "Save Failed! 1");
}

void restartesp()
{
  server.send(200, "text/plain", "device is restart 10s wait");
  delay(2000);
  ESP.restart();
}

void server_api_confic_function(void)
{
       WiFi.disconnect(); 
      delay(500); 
      WiFi.mode(WIFI_AP);
      delay(500);
      WiFi.softAP("KANNAN R MQTT", "kannan@zoho");
    server.on("/save", save);
    server.on("/restart", restartesp);
     server.onNotFound([](){            
            server.send(200, "text/plain", "FileNotFound");
      });
      httpUpdater.setup(&server);
      server.collectHeaders(headerkeys, headerkeyssize );      
      server.begin();
  while (1)
  {
    server.handleClient();
  }
 }
