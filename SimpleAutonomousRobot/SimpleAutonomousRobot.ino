//This is the code for the robot
// make sure to  include the NewPing and Servo Libraries  before uploading else it will show error
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
#define MAX_DISTANCE 200
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

//==================================================
// [S] pino de controle do servo conectado ao pino 7
//==================================================
int servo = 7;
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
  myservo.attach(7);
  myservo.write(115);
  delay(2000);

  //===============================
  //Define a velocidade dos motores
  //===============================
  motorA.setSpeed(70);
  motorB.setSpeed(70);

  distance = readPing();
  delay(100);

  distance = readPing();
  delay(100);

  distance = readPing();
  delay(100);

  distance = readPing();
  delay(100);
}

void loop()
{
  int distanceR = 0;
  int distanceL =  0;
  delay(40);

  if (distance <= 20)
  {
    moveStop();
    delay(100);

    moveBackward();
    delay(600);

    moveStop();
    delay(200);

    distanceR = lookRight();
    delay(200);

    distanceL = lookLeft();
    delay(200);

    if (distanceR >= distanceL)
    {
      turnRight();
      moveStop();
    }
    else
    {
      turnLeft();
      moveStop();
    }
  }
  else
  {
    moveForward();
  }

  distance = readPing();
}

int lookRight()
{
  myservo.write(50);
  delay(500);

  int distance = readPing();
  delay(100);

  myservo.write(115);
  return distance;
}

int lookLeft()
{
  myservo.write(170);
  delay(500);

  int distance = readPing();
  delay(100);

  myservo.write(115);
  return distance;
  delay(100);
}

int readPing()
{
  delay(70);
  int cm = sonar.ping_cm();

  if (cm == 0)
  {
    cm = 250;
  }
  return cm;
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

  delay(350);
  moveForward();
}

void turnLeft()
{
  //Move Left
  motorA.backward();
  motorB.forward();

  delay(350);
  moveForward();
}
