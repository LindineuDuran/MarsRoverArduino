/* Criado por Ronilson Santos em 17/07/2014
* Testes realizados com sucesso. 
*/


/*
                      +-----+
         +------------| USB |------------+
         |            +-----+            |
    B5   | [ ]D13/SCK        MISO/D12[ ] |   B4
         | [ ]3.3V           MOSI/D11[ ]~|   B3
         | [ ]V.ref     ___    SS/D10[ ]~|   B2
    C0   | [ ]A0       / N \       D9[ ]~|   B1
    C1   | [ ]A1      /  A  \      D8[ ] |   B0
    C2   | [ ]A2      \  N  /      D7[ ] |   D7
    C3   | [ ]A3       \_0_/       D6[ ]~|   D6
    C4   | [D]A4/SDA               D5[ ]~|   D5
    C5   | [D]A5/SCL               D4[ ] |   D4
         | [ ]A6              INT1/D3[ ]~|   D3
         | [ ]A7              INT0/D2[ ] |   D2
         | [ ]5V                  GND[ ] |
    C6   | [ ]RST                 RST[ ] |   C6
         | [ ]GND   5V MOSI GND   TX1[ ] |   D0
         | [ ]Vin   [ ] [ ] [ ]   RX1[ ] |   D1
         |          [ ] [ ] [ ]          |
         |          MISO SCK RST         |
         | NANO-V3                       |
         +-------------------------------+

         http://busyducks.com/ascii-art-arduinos
*/

//====================
// [D] Distance Sensor
//====================
#define TrigPin A4 //Envia o pulso - Sensor Ultrassônico
#define EchoPin A5 // Lê o pulso - Sensor Ultrassônico

int _DistanciaObstaculo = 0;

void setup(){
  Serial.begin(9600); //Inicia a Portal Serial
  pinMode (TrigPin, OUTPUT); //Seta pino como entrada do Sensor Ultrassonico
  pinMode (EchoPin, INPUT); //Seta pino como saída  do Sensor Ultrassonico 
}

void loop(){
  SensorUltrassonico();
  Serial.print("Distancia do obstaculo: ");
  Serial.println(_DistanciaObstaculo);
  delay(500);
}

//SENSOR ULTRASSONICO
int SensorUltrassonico() //Faz o disparo de pulsos e retorna o tempo em microsegundos
{
  digitalWrite(TrigPin, LOW);
  delay(2);
  digitalWrite(TrigPin, HIGH);
  delay(10);
  digitalWrite(TrigPin, LOW);
  unsigned long Duracao = pulseIn (EchoPin, HIGH);
  delay(4);
  _DistanciaObstaculo = Duracao/58; //Acha a distancia em centímetros.
  return _DistanciaObstaculo;
  delay(10);
}
