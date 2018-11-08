//Programa : Robô Controlado por Bluetooth que Toca Buzina e Música.
//Controle 2 motores DC usando Ponte H L298N

/*
                      +-----+
         +------------| USB |------------+
         |            +-----+            |
    B5   | [ ]D13/SCK        MISO/D12[Z] |   B4
         | [ ]3.3V           MOSI/D11[H]~|   B3
         | [ ]V.ref     ___    SS/D10[H]~|   B2
    C0   | [ ]A0       / N \       D9[H]~|   B1
    C1   | [ ]A1      /  A  \      D8[L] |   B0
    C2   | [ ]A2      \  N  /      D7[S] |   D7
    C3   | [ ]A3       \_0_/       D6[B]~|   D6
    C4   | [D]A4/SDA               D5[B]~|   D5
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

#include <NewPing.h> //#define TIMER_ENABLED false // Set to "false" to disable the timer ISR (if getting "__vector_7" compile errors set this to false). Default=true
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

//========================================================
// [B] Bluetooth
// Definição de um objeto SoftwareSerial.
// Usaremos os pinos 5 e 6, como RX e TX, respectivamente.
// Isto evita o erro do Avrdude
//========================================================
#define RX 5
#define TX 6
SoftwareSerial serial(RX, TX);

//==================================================
// [S] pino de controle do servo conectado ao pino 7
//==================================================
#define servo 7
Servo myservo;

//=========================================
// [L] Criando variável para o led vermelho
//=========================================
#define LedEstado 8

//======================================================
// [H] H-Bridge
// Definicoes pinos Arduino ligados a entrada da Ponte H
//======================================================
//Motor A
#define ENA 11
#define IN1 9
#define IN2 10

//Motor B
#define ENB 3
#define IN3 4
#define IN4 2

//===========================
// create the motor instances
//===========================
L298N motorA(ENA, IN1, IN2);
L298N motorB(ENB, IN3, IN4);

//================================
// [Z] Buzzer conectado ao pino 12
//================================
#define buzzer 12

//=======================
// melodia do MARIO THEME
//=======================
int melodia[] = {660, 660, 660, 510, 660, 770, 380};

//=====================
// duração de cada nota
//=====================
int duracaodasnotas[] = {100, 100, 100, 100, 100, 100, 100};

//=================
// Varáveis Globais
//=================
int distance = 100;
int autoMode = 0; //Indica quando o modo autônomo está ativado.
int velocidade = 70; //Velocidade dos Motores

void setup()
{
  //===========================================
  // Sets the baud for serial data transmission
  //===========================================
  serial.begin(9600);

  //================
  // [S] Servo Motor
  //================
  myservo.attach(servo);
  myservo.write(90);

  //=============================
  // Pino 8 do arduino como saída
  //=============================
  pinMode(LedEstado, OUTPUT);

  //===============================
  //Define a velocidade dos motores
  //===============================
  motorA.setSpeed(velocidade); //Para bateria carregada, usar valor = 70
  motorB.setSpeed(velocidade); //Para bateria carregada, usar valor = 70

  //============================================
  // Pino 12 do arduino como saída para o Buzzer
  //============================================
  pinMode(buzzer, OUTPUT);

  //==========================================
  // Realiza uma primeira leitura de distância
  //==========================================
  distance = readPing();
}

void loop()
{
  //Criando uma variável do tipo caracter para receber o comando
  static char cmd;

  if (autoMode == 0) //MODO BLUETOOTH
  {
    //Variável 'cmd' recebe o valor da porta Serial
    cmd = serial.read();

    switch (cmd)
    {
      case 'A' : //Se 'A' for recebido, define o modo autônomo
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Autonomous Mode");

        // Define Modo Autônomo
        autoMode = 1;
        break;

      case 'a' : //Se 'a' for recebido, define o modo manual
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Manual Mode");

        // Define Modo Manual
        autoMode = 0;

        //Move Stop
        moveStop();

        //Play the Buzzer
        playBuzzer();
        break;

      case 'F' : //Se 'F' for recebido, vai para Frente
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Move Forward");

        // Move Forward
        moveForward();

        //Acende o LED.
        digitalWrite(LedEstado, HIGH);
        break;

      case 'T' : //Se 'T' for recebido, vai para Trás
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Move Backward");

        //Move Backward
        moveBackward();

        //Acende o LED.
        digitalWrite(LedEstado, HIGH);
        break;

      case 'E' : //Se 'E' for recebido, vira para Esquerda
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Move Left");

        //Move Left
        turnLeft();

        //Acende o LED.
        digitalWrite(LedEstado, HIGH);
        break;

      case 'D' : //Se 'D' for recebido, vira para Direita
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Move Right");

        //Move Right
        turnRight();

        //Acende o LED.
        digitalWrite(LedEstado, HIGH);
        break;

      case 'P' : //Se 'P' for recebido, Pára o Movimento
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Stop the Move");

        //Move Stop
        moveStop();

        //Apaga o LED.
        digitalWrite(LedEstado, LOW);
        break;

      case 'B' : //Se 'B' for recebido, Toca a Buzina
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Play the Buzzer");

        //Play the Buzzer
        playBuzzer();
        break;

      case 'M' : //Se 'M' for recebido, Toca a Música
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Play the Music");

        //Play Super Mario Theme
        playSuperMarioTheme();
        break;

      case 'V' : //Se 'V' for recebido, Aumenta a Velocidade
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.print("Velocidade: ");
        serial.println(velocidade);

        //Aumenta a Velocidade
        velocidade += 10;
        if (velocidade > 250)
        {
          velocidade = 250;
        }

        //=================================
        //Redefine a velocidade dos motores
        //=================================
        motorA.setSpeed(velocidade); //Para bateria carregada, usar valor = 70
        motorB.setSpeed(velocidade); //Para bateria carregada, usar valor = 70
        break;

      case 'v' : //Se 'v' for recebido, Diminui a Velocidade
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.print("Velocidade: ");
        serial.println(velocidade);

        //Aumenta a Velocidade
        velocidade -= 10;
        if (velocidade < 0)
        {
          velocidade = 0;
        }

        //=================================
        //Redefine a velocidade dos motores
        //=================================
        motorA.setSpeed(velocidade); //Para bateria carregada, usar valor = 70
        motorB.setSpeed(velocidade); //Para bateria carregada, usar valor = 70
        break;
    }
  }
  else if (autoMode == 1) //MODO AUTÔNOMO
  {
    //Variável 'cmd' recebe o valor da porta Serial
    cmd = serial.read();

    switch (cmd)
    {
      case 'A' : //Se 'A' for recebido, define o modo Autônomo
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Autonomous Mode");

        // Define Modo Autônomo
        autoMode = 1;
        break;

      case 'a' : //Se 'a' for recebido, define o modo manual
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.println("Manual Mode");

        // Define Modo Manual
        autoMode = 0;

        //Move Stop
        moveStop();

        //Play the Buzzer
        playBuzzer();

        //        //Retorna Servo para posição inicial
        //        myservo.write(90);
        break;

      case 'V' : //Se 'V' for recebido, Aumenta a Velocidade
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.print("Velocidade: ");
        serial.println(velocidade);

        //Aumenta a Velocidade
        velocidade += 10;
        if (velocidade > 250)
        {
          velocidade = 250;
        }

        //=================================
        //Redefine a velocidade dos motores
        //=================================
        motorA.setSpeed(velocidade); //Para bateria carregada, usar valor = 70
        motorB.setSpeed(velocidade); //Para bateria carregada, usar valor = 70
        break;

      case 'v' : //Se 'v' for recebido, Diminui a Velocidade
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.print("Velocidade: ");
        serial.println(velocidade);

        //Aumenta a Velocidade
        velocidade -= 10;
        if (velocidade < 0)
        {
          velocidade = 0;
        }

        //=================================
        //Redefine a velocidade dos motores
        //=================================
        motorA.setSpeed(velocidade); //Para bateria carregada, usar valor = 70
        motorB.setSpeed(velocidade); //Para bateria carregada, usar valor = 70
        break;
    }
  }

  if (autoMode == 1)
  {
    digitalWrite(LedEstado, HIGH); //Acende o LED quando o robô entrar  no modo autônomo.
    obstacleavoidance();
  }

  //Delay(200)
  pausa(200);
}

void obstacleavoidance()
{
  int distanceR = 0;
  int distanceL =  0;

  if (distance <= MIN_DISTANCE)
  {
    moveStop();
    moveBackward();

    //delay(300) - Recua por 300 milisegundos e pára
    pausa(300);

    moveStop();

    //Verifica distância à direita
    distanceR = lookSide(10);

    //delay(200) - Espera 200 milisegundos
    pausa(200);

    //Verifica distância à esquerda
    distanceL = lookSide(170);

    if (distanceR >= distanceL && distanceR > MIN_DISTANCE)
    {
      serial.print("Distancia do obstaculo a DIREITA: ");
      serial.println(distanceR);

      turnRight();
      moveStop();
    }
    else if (distanceL > MIN_DISTANCE)
    {
      serial.print("Distancia do obstaculo a ESQUERDA: ");
      serial.println(distanceL);

      turnLeft();
      moveStop();
    }
    else
    {
      serial.print("Distancia do obstaculo a FRENTE: ");
      serial.println(distance);

      moveBackward();

      //delay(300) - Recua por 500 milisegundos e pára
      pausa(300);

      moveStop();
    }
  }
  else
  {
    serial.print("Distancia do obstaculo a FRENTE: ");
    serial.println(distance);

    moveForward();
  }

  distance = readPing();
}

// Olha para o Lado
int lookSide(int angulo)
{
  myservo.write(angulo);

  int distance = readPing();

  //delay(300) - tempAux
  pausa(300);

  //Retorna Servo para posição inicial
  myservo.write(90);

  return distance;
}

// Lê Objetos Próximos
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

void playBuzzer()
{
  /*
    o número 12 indica que o pino positivo do buzzer está na porta 12
    o número 300 é a frequência que será tocado
    o número 300 é a duração do som
  */
  //aqui sai o som
  tone(buzzer, 300, 300);

  //delay(500)
  pausa(500);

  //aqui sai o som
  tone(buzzer, 100, 300);

  //delay(500);
  pausa(500);

  //aqui sai o som
  tone(buzzer, 900, 300);

  //delay(500);
  pausa(500);
}

void playSuperMarioTheme()
{
  //for para tocar as 6 primeiras notas começando no 0 até 6 ++ incrementado
  for (int nota = 0; nota < 6; nota++)
  {
    int duracaodanota = duracaodasnotas[nota];
    tone(buzzer, melodia[nota], duracaodanota);

    //pausa depois das notas
    int pausadepoisdasnotas[] = {150, 300, 300, 100, 300, 550, 575};
    delay(pausadepoisdasnotas[nota]);
  }

  noTone(buzzer);
}
