// Esse é o código do robô
// certifique-se de incluir as Bibliotecas do NewPing e do Servo antes de fazer o upload. Caso contrário, ele mostrará erro
// Controle 2 motores DC usando Ponte H L298N

/*
                      +-----+
         +------------| USB |------------+
         |            +-----+            |
    B5   | [ ]D13/SCK        MISO/D12[ ] |   B4
         | [ ]3.3V           MOSI/D11[H]~|   B3
         | [ ]V.ref     ___    SS/D10[H]~|   B2
    C0   | [ ]A0       / N \       D9[H]~|   B1
    C1   | [ ]A1      /  A  \      D8[ ] |   B0
    C2   | [ ]A2      \  N  /      D7[S] |   D7
    C3   | [ ]A3       \_0_/       D6[ ]~|   D6
    C4   | [D]A4/SDA               D5[ ]~|   D5
    C5   | [D]A5/SCL               D4[H] |   D4
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

#include <NewPing.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <L298N.h>

//====================
// [D] Distance Sensor
//====================
#define TRIG_PIN A4
#define ECHO_PIN A5
#define MIN_DISTANCE 30
#define MAX_DISTANCE 200
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

//==================================================
// [S] pino de controle do servo conectado ao pino 7
//==================================================
#define servo 7
Servo myservo;

//=============
// [H] H-Bridge
//=============
//Definicoes pinos Arduino ligados a entrada da Ponte H
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

boolean goesForward = false;
int distance = 100;
int speedSet = 0;

void setup()
{
  //================
  // [S] Servo Motor
  //================
  myservo.attach(servo);
  myservo.write(90);

  //===============================
  //Define a velocidade dos motores
  //===============================
  motorA.setSpeed(70);
  motorB.setSpeed(70);

  distance = readPing();
}

void loop()
{
  int distanceR = 0;
  int distanceL =  0;

  if (distance <= MIN_DISTANCE)
  {
    moveStop();
    pausa(100);

    moveBackward();
    pausa(600);

    moveStop();
    pausa(200);

    distanceR = lookSide(10);
    pausa(200);

    distanceL = lookSide(170);
    pausa(200);

    if (distanceR >= distanceL && distanceR > MIN_DISTANCE)
    {
      turnRight();
      moveStop();
    }
    else if (distanceL > MIN_DISTANCE)
    {
      turnLeft();
      moveStop();
    }
    else
    {
      moveBackward();
      pausa(600);
      moveStop();
    }
  }
  else
  {
    moveForward();
  }

  distance = readPing();
}

// Olha para o Lado
int lookSide(int angulo)
{
  myservo.write(angulo);

  int distance = readPing();
  pausa(300);

  //Retorna Servo para posição inicial
  myservo.write(90);

  return distance;
}

int readPing()
{
  pausa(70);
  int cm = sonar.ping_cm();
  return cm;
}

void pausa(unsigned int milisegundos)
{
  volatile unsigned long compara = 0;
  volatile int contador = 0;
  do
  {
    if (compara != millis())
    {
      contador++;
      compara = millis();
    }
  } while (contador <= milisegundos);
  return;
}

void moveStop()
{
  //Move Stop
  motorA.stop();
  motorB.stop();
}

void moveForward()
{
  // Move Forward
  motorA.forward();
  motorB.forward();
}

void moveBackward()
{
  //Move Backward
  motorA.backward();
  motorB.backward();
}

void turnRight()
{
  //Move Right
  motorA.forward();
  motorB.backward();

  pausa(350);
  moveForward();
}

void turnLeft()
{
  //Move Left
  motorA.backward();
  motorB.forward();

  pausa(350);
  moveForward();
}
