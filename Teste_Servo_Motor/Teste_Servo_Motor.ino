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
    C2   | [ ]A2      \  N  /      D7[S] |   D7
    C3   | [ ]A3       \_0_/       D6[ ]~|   D6
    C4   | [ ]A4/SDA               D5[ ]~|   D5
    C5   | [ ]A5/SCL               D4[ ] |   D4
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

#include <Servo.h>  //Inclui a bibioteca utilizada pelo servo motor. 

const int ServoMotorPin  = 7; //[S] Setando o servo motor da porta 7

Servo MeuServoMotor; //Criando um objeto da classe Servo 

void setup() 
{ 
  Serial.begin(9600); //Inicia a comunicação com a Porta Serial
  MeuServoMotor.attach(ServoMotorPin); //Associando o pino ao objeto da classe Servo
}

void loop() 
{ 
  ServoMotorFrente();
  delay(5000);
  ServoMotorDireita();
  delay(5000);
  ServoMotorFrente();
  delay(5000);
  ServoMotorEsquerda();
  delay(5000);
} 

void ServoMotorFrente() //Volta o Servo Motor para frente
{
  MeuServoMotor.write(90); //Servo voltado para a frente
  Serial.println("Servo Motor apontando para a Frente");
}

void ServoMotorDireita() //Volta o Servo Motor para Direita
{
  MeuServoMotor.write(10); //Servo voltado para a Direita
  Serial.println("Servo Motor apontando para a Direita");
}

void ServoMotorEsquerda() //Volta o Servo Motor para Esquerda
{
  MeuServoMotor.write(170); //Servo voltado para a Esquerda
  Serial.println("Servo Motor apontando para a Esquerda");
}
