//************************************************************
//Este código foi desenvolvido para o uso durante o Hackathon
//oferecido pela Universidade de São Paulo sobre o tema Cidades
//Inteligentes.
//Os realizadores deste projeto: 
//- André Perez
//- Daniel Fernandes da Nóbrega
//- Guilherme Rocha Gonçalves
//- Leonardo Parente
//Este código contém bibliotecas que não são de nossa autoria.
//************************************************************
#include "painlessMesh.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

painlessMesh  mesh;

size_t logServerId = 0;

// ACS712 variables
int sensorValueAux = 0;
float valorSensor = 0;
float valorCorrente = 0;
int   valor = 0;
float voltsporUnidade = 0.005;
float sensibilidade = 0.185;
String buf;
unsigned long time;

// Send message to the logServer every 10 seconds 
Task myLoggingTask(10000, TASK_FOREVER, []() {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& msg = jsonBuffer.createObject();
    msg["topic"] = "L01";
    
   //if (time > 12000){
      for(int i = 1000; i>0 ; i--){
        sensorValueAux = (analogRead(A0) - 750.0);
        valorSensor += pow(sensorValueAux,2);
      }

      valorSensor = (sqrt(valorSensor/1000)) * voltsporUnidade;
      valorCorrente = (valorSensor/sensibilidade) - 0.13;

      valorCorrente = valorCorrente * 1000;
      valor = (int) valorCorrente;

      if (valor <= 115){
        valor = 0;
        buf = "0000";
      } else {
        if (valor < 1000){
          buf = "0";
          buf.concat(String(valor));
          buf = buf.substring(0,4);
        } else {
           buf = String(valor); 
           buf = buf.substring(0,4);
        }
      }
    //}
    valorSensor = 0;
    msg["current"] = buf;
    String str;
    msg.printTo(str);
    if (logServerId == 0) // If we don't know the logServer yet
        mesh.sendBroadcast(str);
    else
        mesh.sendSingle(logServerId, str);

    // log to serial
    msg.printTo(Serial);
    Serial.printf("\n");
});

void setup() {
  Serial.begin(115200);
  delay(10);
  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, STA_AP, AUTH_WPA2_PSK, 6 );
  mesh.onReceive(&receivedCallback);

  // Add the task to the mesh scheduler
  mesh.scheduler.addTask(myLoggingTask);
  myLoggingTask.enable();
}

void loop() {
  mesh.update();
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("logClient: Received from %u msg=%s\n", from, msg.c_str());

  // Saving logServer
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(msg);
  if (root.containsKey("topic")) {
      if (String("logServer").equals(root["topic"].as<String>())) {
          // check for on: true or false
          logServerId = root["nodeId"];
          Serial.printf("logServer detected!!!\n");
      }
      Serial.printf("Handled from %u msg=%s\n", from, msg.c_str());
  }
}
