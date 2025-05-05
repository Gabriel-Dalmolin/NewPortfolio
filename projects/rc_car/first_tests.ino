#include <Ps3Controller.h> // importando as bibliotecas necessárias
#include <RoboCore_Vespa.h>

VespaMotors motores;              // inicializando os motores
int leftX, leftY, rightX, rightY; // declarando variáveis para os eixos do joystick
int square = 0;                   // declarando variável para o estado do quadrado (Vou explicar melhor depois)

// ------------------------------------- Declarando os pinos digitais dos sensores
int const O5 = 26;
int const O4 = 25;
int const O3 = 33;
int const O2 = 32;
int const O1 = 17;
// -------------------------------------

// ------------------------------------- Declarando variáveis de input dos sensores
int S1 = 0;
int S2 = 0;
int S3 = 0;
int S4 = 0;
int S5 = 0;

void setup()
{

  // ------------------------------------- inicializando a conexão bluetooth com o controle
  Serial.begin(115200);
  Ps3.begin("02:01:01:01:01:01");
  if (Ps3.begin())
  {
    Serial.println("Tentando conectar ao controle PS3...");
  }
  else
  {
    Serial.println("Falha ao iniciar o controle PS3");
    return;
  }
  // -------------------------------------

  // ------------------------------------- Definindo os pinos digitais como input
  pinMode(O5, INPUT);
  pinMode(O4, INPUT);
  pinMode(O3, INPUT);
  pinMode(O2, INPUT);
  pinMode(O1, INPUT);
  // -------------------------------------
}

void loop()
{
  // ------------------------------------- Atualizando os valores das variáveis para serem o detectado pelo TCRT5000
  S1 = digitalRead(O1);
  S2 = digitalRead(O2);
  S3 = digitalRead(O3);
  S4 = digitalRead(O4);
  S5 = digitalRead(O5);
  // -------------------------------------

  // ------------------------------------- Código para alteração entre modo seguidor de linha e modo controle remoto
  Serial.println(digitalRead(O1));
  Serial.println("Square: ");
  Serial.println(square);
  if (Ps3.data.button.square)
  {
    if (square == 0)
    {
      square = 1;
      delay(500);
    }
    else if (square == 1)
    {
      square = 0;
      delay(500);
    }
  }
  // -------------------------------------

  // ------------------------------------- Alterando os valores das variáveis para serem o equivalente ao valor do joystick do controle
  leftX = Ps3.data.analog.stick.lx;
  leftY = Ps3.data.analog.stick.ly;
  rightX = Ps3.data.analog.stick.rx;
  rightY = Ps3.data.analog.stick.ry;
  // -------------------------------------

  // ------------------------------------- Se a váriavel quadrado for igual a zero, então está no modo de controle remoto
  if (square == 0)
  {
    if (leftY > 0)
    {
      leftY = leftY - 10;
    }
    else if (leftY < 0)
    {
      leftY = leftY + 10;
    }
    Serial.println(-leftY);

    // Verifica se o joystick está na posição para frente
    if (abs(leftY) > 20)
    {
      motores.setSpeedLeft(-leftY);
      if (-leftY > 20)
      {
        Serial.println("Indo pra trás");
      }
      else if (-leftY < -20)
      {
        Serial.println("Indo pra frente");
      }
    }
    else
    {
      motores.setSpeedLeft(0);
      Serial.println("Parado");
    }

    // Verifica se o joystick está na posição para frente
    if (rightY < -20)
    {
      Serial.println("Indo pra frente");
      motores.setSpeedRight(rightY);
    }
    // Verifica se o joystick está na posição para trás
    else if (rightY > 20)
    {
      Serial.println("Indo pra trás");
      motores.setSpeedRight(rightY);
    }
    // Se o joystick estiver na zona neutra, para os motores
    else
    {
      motores.setSpeedRight(0);
      Serial.println("Parado");
    }

    // Pequeno delay para suavizar o loop
    delay(50);
  }

  // ------------------------------------- Se a variável quadrado for igual a 1, o carrinho está no modo seguir linha, então, fará os testes para ver para onde deve virar
  else if (square == 1)
  {
    S1 = !S1;
    S2 = !S2;
    S3 = !S3;
    S4 = !S4;
    S5 = !S5;

    int erro = (S5 * 2) + (S4 * 1) + (S2 * -1) + (S1 * -2);
    int baseSpeed = 80;
    int ajuste = erro * 30;

    int sLeft = -baseSpeed + ajuste;
    int sRight = baseSpeed + ajuste;

    sLeft = constrain(sLeft, -100, 100);
    sRight = constrain(sRight, -100, 100);

    motores.setSpeedLeft(sLeft);
    motores.setSpeedRight(sRight);
  }
}
