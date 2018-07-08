//=============
// [H] H-Bridge
//=============
//Motor A
int IN1 = 11;
int IN2 = 10;

//Motor B
int IN3 = 9;
int IN4 = 8;

void stop() //stop
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void AndaFrente() //Anda Para Frente
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void MarchaRe() //Move para trás
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void Esquerda() //Vira a esquerda
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void Direita() //Vira a direita
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void setup(void)
{
  int i; //contador utilizado pelo For a seguir

  //Setando pinos de 8 a 11, como saída (OUTPUT)
  for (i = 8; i <= 11; i++)
    pinMode(i, OUTPUT);
  Serial.begin(9600); //Set Baud Rate
  Serial.println("Controle via teclado");
}

void loop(void)
{
  //  AndaFrente(); //move forward in max speed
  //  delay(4000);
  //  MarchaRe(); //move back in max speed
  //  delay(4000);
  //  Esquerda();
  //  delay(750);
  //  Direita();
  //  delay(750);
  //  stop();

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
