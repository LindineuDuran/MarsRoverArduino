//Controle 2 motores DC usando Ponte H L298N

/*
                      +-----+
         +------------| USB |------------+
         |            +-----+            |
    B5   | [ ]D13/SCK        MISO/D12[ ] |   B4
         | [ ]3.3V           MOSI/D11[H]~|   B3
         | [ ]V.ref     ___    SS/D10[H]~|   B2
    C0   | [ ]A0       / N \       D9[H]~|   B1
    C1   | [ ]A1      /  A  \      D8[ ] |   B0
    C2   | [ ]A2      \  N  /      D7[ ] |   D7
    C3   | [ ]A3       \_0_/       D6[ ]~|   D6
    C4   | [ ]A4/SDA               D5[ ]~|   D5
    C5   | [ ]A5/SCL               D4[H] |   D4
         | [ ]A6              INT1/D3[H]~|   D3
         | [ ]A7              INT0/D2[H] |   D2
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

#include <Ultrasonic.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <L298N.h>

//=============
// [H] H-Bridge
//=============
//Motor A
#define ENA 11
#define IN1 9
#define IN2 10

//Motor B
#define ENB 3
#define IN3 4
#define IN4 2

//========================
//create a motor instances
//========================
L298N motorA(ENA, IN1, IN2);
L298N motorB(ENB, IN3, IN4);

void stop() //stop
{
  //stop running
  motorA.stop();
  motorB.stop();
}

void AndaFrente() //Anda Para Frente
{
  //tell the motorA to go forward
  motorA.forward();

  //tell the motorB to go forward
  motorB.forward();
}

void MarchaRe() //Move para trás
{
  //tell the motorA to go back
  motorA.backward();

  //tell the motorB to go back
  motorB.backward();
}

void Esquerda() //Vira a esquerda
{
  //tell the motorA to go back
  motorA.backward();

  //tell the motorB to go forward
  motorB.forward();
}

void Direita() //Vira a direita
{
  //tell the motorA to go forward
  motorA.forward();

  //tell the motorB to go back
  motorB.backward();
}

void setup()
{
  // an integer between 0 and 255
  motorA.setSpeed(70);
  motorB.setSpeed(70);
}

void loop()
{
  AndaFrente(); //move forward in max speed
  delay(1000);

  Esquerda();
  delay(400);

  AndaFrente(); //move forward in max speed
  delay(1000);

  Direita();
  delay(400);

  MarchaRe(); //move back in max speed
  delay(1000);

  stop();
}
