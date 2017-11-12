//*
*******HackatonUSP - SmartCities********
**********SancaLigths Team**************

***           Integrantes:           ***
***           André Perez            ***
***         Daniel Fernandes         ***
***         Guilherme Rocha          ***
***         Leonardo Parente         ***

***  Programa referente ao rotiador  ***
***  conversa via serial com o nó    ***
***  principal da rede mesh          ***
****************************************
 */

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>

//const char* ssid     = "Leonardo's iPhone";
//const char* password = "lnparente";
const char* ssid     = "Lufa Lufa - 2.4GHz";
const char* password = "HelgaHufflepuff";
const char* uuidl01 = "ee5480f1-fe48-47ea-af13-b0ee2c5c9ac3";
const char* uuidl02 = "ec3967d5-8483-4ce2-a880-3bec87835378";
const char* hostuuid = "143.107.45.126";
const char* hostuuid00 = "/adaptor/resources/ee5480f1-fe48-47ea-af13-b0ee2c5c9ac3/data/lightpole_monitoring";
const char* hostuuid01 = "/adaptor/resources/ec3967d5-8483-4ce2-a880-3bec87835378/data/lightpole_monitoring";
const char* initString = "{\"data\":[{\"current\":";
const char* endString = ",\"timestamp\":\"2017-11-12T10:00:00.000Z\"}]}";
char buffer[256];
String sendxString;
int cont = 0;  


void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  digitalWrite(LED_BUILTIN, LOW); 
}


void loop() {
   
    while (Serial.available() > 0) {
    // read the incoming byte:
      buffer[cont] = (char)Serial.read();
       Serial.print(buffer[cont]);
      cont++;
    }
    if(buffer[cont-1] == 'x'){
    char  buffer_copy[256];
    char * split_pointer;
    //char* pPosition = strchr(buffer, ',');
   //if(pPosition){
    memcpy(&buffer_copy,&buffer,sizeof(buffer_copy));
    split_pointer = strtok (buffer_copy,",");
    if (strlen(split_pointer) == 7){
      HTTPClient http;
      if(split_pointer[4] == '0'){
      sendxString = initString;
      sendxString.concat("0");
      sendxString.concat(endString);
      Serial.println(sendxString);  
      http.begin(hostuuid, 30134, hostuuid01);
      http.addHeader("Content-Type", "application/json");
      http.POST(sendxString);
      http.getString();
      http.end();
      }
      else{
          char chArray[4];
          memcpy(chArray, &split_pointer[4], 3);
          chArray[3] = '\0';
          String str(chArray);
          sendxString = initString;
          sendxString.concat(str);
          sendxString.concat(endString);
          Serial.println(sendxString);  
          http.begin(hostuuid, 30134, hostuuid01);
          http.addHeader("Content-Type", "application/json");
          http.POST(sendxString);
          http.getString();
          http.end();
          }
      }
      split_pointer = strtok (NULL,",");
      if (strlen(split_pointer) == 7){
      HTTPClient http;
      if(split_pointer[4] == '0'){
      sendxString = initString;
      sendxString.concat("0");
      sendxString.concat(endString);
      Serial.println(sendxString);  
      http.begin(hostuuid, 30134, hostuuid00);
      http.addHeader("Content-Type", "application/json");
      http.POST(sendxString);
      http.getString();
      http.end();
      }
      else{
          char chArray[4];
          memcpy(chArray, &split_pointer[4], 3);
          chArray[3] = '\0';
          String str(chArray);
          sendxString = initString;
          sendxString.concat(str);
          sendxString.concat(endString);
          Serial.println(sendxString);  
          http.begin(hostuuid, 30134, hostuuid00);
          http.addHeader("Content-Type", "application/json");
          http.POST(sendxString);
          http.getString();
          http.end();
          }
       }
     // }
      memset(buffer, 0, 255);
      cont = 0;
    }    
    
}

