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

// ACS712 variables
int sensorValueAux = 0;
float valorSensor = 0;
float valorCorrente = 0;
float voltsporUnidade = 0.005;
float sensibilidade = 0.185;

String buf;


void setup(){
  Serial.begin(115200);
   }

void loop() {

  for(int i=0; i<1000; i++){
    
    valorSensor = (analogRead(A0));
    sensorValueAux += valorSensor;
  }
  Serial.println(sensorValueAux/1000);
  sensorValueAux = 0;
}

