//************************************************************
E//ste código foi desenvolvido para o uso durante o Hackathon
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
#include "SoftwareSerial.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

painlessMesh  mesh;

// ACS712 variables
int sensorValueAux = 0;
float valorSensor = 0;
float valorCorrente = 0;
int valor = 0;
float voltsporUnidade = 0.005;
float sensibilidade = 0.185;
String buf;


// Send my ID every 10 seconds to inform others
Task logServerTask(10000, TASK_FOREVER, []() {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& msg = jsonBuffer.createObject();
    msg["topic"] = "logServer";
    msg["nodeId"] = mesh.getNodeId();

    String str;
    msg.printTo(str);
    mesh.sendBroadcast(str);

    // log to serial
 //   msg.printTo(Serial);
//    Serial.printf("\n");
});

void setup() {
  Serial.begin(9600);
  delay(10);
  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE | DEBUG ); // all types on
  //mesh.setDebugMsgTypes( ERROR | CONNECTION | SYNC | S_TIME );  // set before init() so that you can see startup messages
 // mesh.setDebugMsgTypes( ERROR | CONNECTION | S_TIME );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, MESH_PORT, STA_AP, AUTH_WPA2_PSK, 6 );
  mesh.onReceive(&receivedCallback);

  mesh.onNewConnection([](size_t nodeId) {
 //   Serial.printf("New Connection %u\n", nodeId);
  });

  mesh.onDroppedConnection([](size_t nodeId) {
 //   Serial.printf("Dropped Connection %u\n", nodeId);
  });

  // Add the task to the mesh scheduler
  mesh.scheduler.addTask(logServerTask);
  logServerTask.enable();
}

void loop() {
  mesh.update();
}

void receivedCallback( uint32_t from, String &msg ) {
//  Serial.printf("logServer: Received from %u msg=%s\n", from, msg.c_str());
  Serial.print(msg.substring(10,13));
  Serial.print(msg.substring(26,30));

     for(int i = 1000; i>0 ; i--){
        sensorValueAux = (analogRead(A0) - 764.0);
        valorSensor += pow(sensorValueAux,2);
      }

      valorSensor = (sqrt(valorSensor/1000)) * voltsporUnidade;
      valorCorrente = (valorSensor/sensibilidade) - 0.1;

      valorCorrente = valorCorrente * 1000;
      valor = (int) valorCorrente;

      if (valor <= 100){
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
  //  delay(100);
    valorSensor = 0;
 //   Serial.print("{\"topic\":\"L00\",\"current\":\"");
    Serial.print(",");
    Serial.print("L00");
    Serial.print(buf);
    Serial.print(",x");
//    Serial.print("\"}");    
}
