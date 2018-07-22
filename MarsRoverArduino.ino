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
    C1   | [ ]A1      /  A  \      D8[H] |   B0
    C2   | [ ]A2      \  N  /      D7[S] |   D7
    C3   | [ ]A3       \_0_/       D6[B]~|   D6
    C4   | [D]A4/SDA               D5[B]~|   D5
    C5   | [D]A5/SCL               D4[L] |   D4
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

#include <Ultrasonic.h>
#include <Servo.h>
#include <SoftwareSerial.h>

//=========================================
// [L] Criando variável para o led vermelho
//=========================================
int LedEstado = 4;

//========================================================
// [B] Bluetooth
// Definição de um objeto SoftwareSerial.
// Usaremos os pinos 5 e 6, como RX e TX, respectivamente.
// Isto evita o erro do Avrdude
//========================================================
SoftwareSerial serial(5, 6);

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
int IN1 = 11;
int IN2 = 10;

//Motor B
int IN3 = 9;
int IN4 = 8;

//================================
// [Z] Buzzer conectado ao pino 12
//================================
int buzzer = 12;

//melodia do MARIO THEME
int melodia[] = {660, 660, 660, 510, 660, 770, 380};

//duração de cada nota
int duracaodasnotas[] = {100, 100, 100, 100, 100, 100, 100};

//====================
// [D] Distance Sensor
//====================
#define pino_trigger A4 // Pino trigger conectado ao pino A4 do Arduino
#define pino_echo A5 // Pino echo conectado ao pino A5 do Arduino
Ultrasonic ultrasonic(pino_trigger, pino_echo); // Inicializa o sensor nos pinos definidos

float leftLook;
float rightLook;
float distance;
int autoMode = 0; //Indica quando o modo autônomo está ativado.

void setup()
{
  //=============================
  // Pino 4 do arduino como saída
  //=============================
  pinMode(LedEstado, OUTPUT);

  //===========================================
  // Sets the baud for serial data transmission
  //===========================================
  serial.begin(9600);

  //================
  // [S] Servo Motor
  //================
  myservo.attach(servo);
  myservo.write(90);

  //=======================================================
  //Define os pinos 8,9, 10 e 11 como saída para os motores
  //=======================================================
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  //============================================
  // Pino 12 do arduino como saída para o Buzzer
  //============================================
  pinMode(buzzer, OUTPUT);

  long microsec = ultrasonic.timing(); // Inicia medição
  distance = ultrasonic.convert(microsec, Ultrasonic::CM);

  delay(100);
}

void loop()
{
  //Criando uma variável do tipo caracter
  char z;

  if (autoMode == 0) //MODO BLUETOOTH
  {
    //Variável 'z' recebe o valor da porta Serial
    z = serial.read();

    switch (z)
    {
      case 'A' : //Se 'A' for recebido, define o modo autônomo
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.print("Autonomous Mode");

        // Define Modo Autônomo
        autoMode = 1;
        break;

      case 'a' : //Se 'a' for recebido, define o modo manual
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.print("Manual Mode");

        // Define Modo Manual
        autoMode = 0;

        //Move Stop
        moveStop();

        //Play the Buzzer
        playBuzzer();
        break;

      case 'F' : //Se 'F' for recebido, vai para Frente
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.print("Move Forward");

        // Move Forward
        moveForward();

        //Acende o LED.
        digitalWrite(LedEstado, HIGH);

        break;

      case 'T' : //Se 'T' for recebido, vai para Trás
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.print("Move Backward");

        //Move Backward
        moveBackward();

        //Acende o LED.
        digitalWrite(LedEstado, HIGH);

        break;

      case 'E' : //Se 'E' for recebido, vira para Esquerda
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.print("Move Left");

        //Move Left
        turnLeft();

        //Acende o LED.
        digitalWrite(LedEstado, HIGH);

        break;

      case 'D' : //Se 'D' for recebido, vira para Direita
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.print("Move Right");

        //Move Right
        turnRight();

        //Acende o LED.
        digitalWrite(LedEstado, HIGH);

        break;

      case 'P' : //Se 'P' for recebido, Pára o Movimento
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.print("Stop the Move");

        //Move Stop
        moveStop();

        //Apaga o LED.
        digitalWrite(LedEstado, LOW);

        break;

      case 'B' : //Se 'B' for recebido, Toca a Buzina
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.print("Play the Buzzer");

        //Play the Buzzer
        playBuzzer();

        break;

      case 'M' : //Se 'M' for recebido, Toca a Música
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.print("Play the Music");

        //Play Super Mario Theme
        playSuperMarioTheme();

        break;
    }
  }
  else if (autoMode == 1) //MODO AUTÔNOMO
  {
    //Variável 'z' recebe o valor da porta Serial
    z = serial.read();

    switch (z)
    {
      case 'A' : //Se 'A' for recebido, define o modo autônomo
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.print("Autonomous Mode");

        // Define Modo Autônomo
        autoMode = 1;
        break;

      case 'a' : //Se 'a' for recebido, define o modo manual
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.print("Manual Mode");

        // Define Modo Manual
        autoMode = 0;

        //Move Stop
        moveStop();

        //Play the Buzzer
        playBuzzer();
        break;
    }
  }

  if (autoMode == 1)
  {
    digitalWrite(LedEstado, HIGH); //Acende o LED quando o robô entrar  no modo autônomo.
    obstacleavoidance();
  }

  delay(300);
}

void obstacleavoidance()
{
  distance = readPing();

  serial.print("Há objeto a uma distancia de ");
  serial.print(distance); // Informa a distância medida no
  serial.println(" cm a frente"); // serial monitor

  delay (50);// check for objects

  if (distance > 30)
  {
    moveForward();
  }
  else if (distance <= 30)
  {
    moveBackward();// Act as a brake
    moveStop(); //Object detected
    delay(500);
    distances(); // Proceed to the next function
  }
}

void distances()
{
  myservo.write(170);//take distances from the left side
  delay(350);
  leftLook = readPing();

  serial.print("Há objeto a uma distancia de ");
  serial.print(leftLook); // Informa a distância medida no
  serial.println(" cm a esquerda"); // serial monitor

  myservo.write(90);
  delay(500);
  
  myservo.write(10);//take distances from the right side
  delay(350);
  rightLook = readPing();

  serial.print("Há objeto a uma distancia de ");
  serial.print(rightLook); // Informa a distância medida no
  serial.println(" cm a direita"); // serial monitor

  myservo.write(90);
  delay(40);

  comparison();
}

float readPing()
{
  float cm; // Cria variável para armazenar medição
  long microsec = ultrasonic.timing(); // Inicia medição
  cm = ultrasonic.convert(microsec, Ultrasonic::CM);

  if (cm == 0)
  {
    cm = 250;
  }
  return cm;
}

void comparison()
{
  if (leftLook > rightLook)
  {
    turnLeft();
    delay(400);
    
    obstacleavoidance();
  }
  else if (leftLook < rightLook)
  {
    turnRight();
    delay(400);
    
    obstacleavoidance();
  }
  else if (leftLook < 15 && rightLook < 15)
  {
    moveBackward();
    delay(350);
    moveStop();
    distances();
  }
}

void moveStop()
{
  //Move Stop
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void moveForward()
{
  // Move Forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward()
{
  //Move Backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight()
{
  //Move Right
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft()
{
  //Move Left
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
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
  delay(500);

  //aqui sai o som
  tone(buzzer, 100, 300);
  delay(500);

  //aqui sai o som
  tone(buzzer, 900, 300);
  delay(500);
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
