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

//================================
// [Z] Buzzer conectado ao pino 12
//================================
int buzzer = 12;

//=======================
// melodia do MARIO THEME
//=======================
int melodia[] = {660, 660, 660, 510, 660, 770, 380};

//=====================
// duração de cada nota
//=====================
int duracaodasnotas[] = {100, 100, 100, 100, 100, 100, 100};

//====================
// [D] Distance Sensor
//====================
#define pino_trigger A4 // Pino trigger conectado ao pino A4 do Arduino
#define pino_echo A5 // Pino echo conectado ao pino A5 do Arduino
Ultrasonic ultrasonic(pino_trigger, pino_echo); // Inicializa o sensor nos pinos definidos

#define LedEstadoCycle 100U
unsigned long LedEstadoLastMillis = 0;
boolean LedEstadoState = false;
char cmd; //Armazena os caracteres enviados pela portal Serial

void setup()
{
  // put your setup code here, to run once:

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

  //==============================
  // Pino 12 do arduino como saída
  //==============================
  pinMode(buzzer, OUTPUT);

}

void loop()
{
  // put your main code here, to run repeatedly:

  // Verifica se chegou algum caractere pela porta serial.
  if (serial.available())
  {
    //==============================================
    // Variável 'cmd' recebe o valor da porta Serial
    //==============================================
    cmd = serial.read();

    switch (cmd)
    {
      case 'P' : //Se 'P' for recebido, Pára o Movimento
        //Mensagem será enviada para o módulo HC-06 e daí para o Android.
        serial.print("Pisca LED");

        //==============================================
        // Pisca o Led de Estado, informando modo Manual
        //==============================================
        piscaLed();
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

        //      default : //Se 'P' for recebido, Pára o Movimento
        //        //Mensagem será enviada para o m?dulo HC-06 e daí para o Android.
        //        serial.print("Stop the Move");
        //
        //        //Move Stop
        //        moveStop();
    }
  }
}

void playBuzzer()
{
  /*
    o número 12 indica que o pino positivo do buzzer está na porta 12
    o número 300 é a frequência que será tocada
    o número 300 é a duração do som
  */

  //==============
  //aqui sai o som
  //==============
  tone(buzzer, 300, 300);
  delay(500);

  //==============
  //aqui sai o som
  //==============
  tone(buzzer, 100, 300);
  delay(500);

  //==============
  //aqui sai o som
  //==============
  tone(buzzer, 900, 300);
  delay(500);
}

void playSuperMarioTheme()
{
  //for para tocar as 6 primeiras notas, começando no 0 ate 6 ++ incrementado
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

void piscaLed()
{
  if (cycleCheck(&LedEstadoLastMillis, LedEstadoCycle))
  {
    digitalWrite(LedEstado, LedEstadoState);
    LedEstadoState = !LedEstadoState;
  }
}

boolean cycleCheck(unsigned long *lastMillis, unsigned int cycle)
{
  unsigned long currentMillis = millis();
  if (currentMillis - *lastMillis >= cycle)
  {
    *lastMillis = currentMillis;
    return true;
  }
  else
    return false;
}
