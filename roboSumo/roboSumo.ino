/*---------------------------------------------------------------------
Projeto: Sumo Robot
Versao: 1.0

Lista de recursos:
- Servo motor - DONE
- Sensor Sharp Infravermelho - DONE
- Botao - DONE
- Sensor Faixa - DONE
- Buzzer - DONE
- Ponte H - DONE

Itens testados:
- Codigo Validado - DONE
- Servo motor - DONE
- Sensor Sharp Infravermelho - DONE
- Botao - DONE
- Sensor Faixa - DONE
- Buzzer - DONE
- Ponte H - DONE
---------------------------------------------------------------------*/
//Bibliotecas
//#include <Ultrasonic.h>

//Configuracao e definicao dos pinos
#define servoMotorPort 11
#define IN1 4
#define IN2 6
#define velocidadeA 5
#define IN3 7
#define IN4 8
#define velocidadeB 9
//#define sensorInfra A0
#define pino_trigger A4
#define pino_echo A3
#define sensorFaixaFrontal 2
#define sensorFaixaTraseiro 3
#define botaoStart 12
int ledArduino = 13;
int portSpeak(10);

//Definicoes de variaveis
int serialSpeed = 9600;
int delayInicial = 5000;
int pos;
int valor = 0;
int tamanhoPeriodoSegundos = 25 * 1000;
int inicioAngulo = 0.7 * 1000;
int fimAngulo = 1.1 * 1000;
int incremento = 0.04 * 1000;
int melodiaMario[] = {660,660,660,510,660,770,380,510,380,320,440,480,450,430};
int duracaodasnotas[] = {100,100,100,100,100,100,100,100,100,100,100,80,100,100,100};
int contagemBotao = 0;
int contagemFaixa = 0;
long duracao;
long HR_dist=0;
int minimumRange=5;
int maximumRange=200;
//Ultrasonic ultrasonic(pino_trigger, pino_echo);

//Metodos para serem utilizados com o interrupt, mover para frente e para tras
void moveFrenteFaixa()
{
  analogWrite(velocidadeA,100);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(velocidadeB,100);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}

void moveTrasFaixa()
{
  analogWrite(velocidadeA,100);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(velocidadeB,100);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}

//Configuracao inicial
void setup()
{
  Serial.println("Inicializando setup...");
  Serial.begin(serialSpeed);
  Serial.println("Velocidade da serial definida...");
  
  Serial.println("Configurando pinos...");
  //pinMode(sensorInfra, INPUT);
  pinMode(ledArduino, OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(velocidadeA,OUTPUT);
  pinMode(velocidadeB,OUTPUT);
  pinMode(sensorFaixaFrontal,INPUT);
  pinMode(sensorFaixaTraseiro,INPUT);
  pinMode(servoMotorPort,OUTPUT);
  pinMode(pino_trigger,OUTPUT);
  pinMode(pino_echo,INPUT);
  Serial.println("Pinos configurados...");
}

//Funcoes de movimentacao
void moveFrente()
{
  Serial.println("Direcao: Frente");
  analogWrite(velocidadeA,90);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(velocidadeB,80);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}

void moveTras()
{
  Serial.println("Direcao: Tras");
  analogWrite(velocidadeA,100);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(velocidadeB,100);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}

void moveEsquerda()
{
  Serial.println("Direcao: Esquerda");
  analogWrite(velocidadeA,100);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(velocidadeB,100);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}

void moveDireita()
{
  Serial.println("Direcao: Direita");
  analogWrite(velocidadeA,80);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(velocidadeB,80);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);

}

void moveParar()
{
  Serial.println("Direcao: Parado");
  //Corrigindo angulo quando encontra o oponente, primeira condicao da Direita para Esquerda e segunda condicao da Esquerda para Direita
  if(digitalRead(IN1) == HIGH  && digitalRead(IN2) == LOW && digitalRead(IN3) == LOW && digitalRead(IN4) == HIGH)
  {
    analogWrite(velocidadeA,100);
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    analogWrite(velocidadeB,100);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
    delay(400);
  }else if(digitalRead(IN1) == LOW  && digitalRead(IN2) == HIGH && digitalRead(IN3) == HIGH && digitalRead(IN4) == LOW)
  {
    analogWrite(velocidadeA,100);
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    analogWrite(velocidadeB,100);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    delay(400);
  }
  
  //Parando robo
  analogWrite(velocidadeA,0);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,HIGH);
  analogWrite(velocidadeB,0);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,HIGH);
}

//Funcoes de leitura de sensores Infravermelho e Sensores de Faixa
int distanciaSensorInfra()
{
  Serial.println("Iniciando leiura ultrasonico...");
  int resultado;
  /*float cmMsec;
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);*/
  digitalWrite(pino_trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(pino_trigger,HIGH);
  delayMicroseconds(10);
  digitalWrite(pino_trigger,LOW);
  duracao = pulseIn(pino_echo,HIGH);
  HR_dist = duracao/56;
  Serial.print("Leitura ultrasonico completa... Resultado: ");
  Serial.println(HR_dist);
  if(HR_dist > 250)
  {
    resultado = 1;
  }else

  {
    resultado = 0;  
  }
  return resultado;
}

int resultadoSensorFaixaFrontal()
{
  Serial.println("Checando faixa dianteiro...");
  int resultadoSensorFrontal = digitalRead(sensorFaixaFrontal);
  if(resultadoSensorFrontal>0)
  {
    Serial.println("Faixa nao encontrada...");
    return resultadoSensorFrontal;
  }else
  {
    Serial.println("Faixa encontrada...");
    return resultadoSensorFrontal;
  }
}

int resultadoSensorFaixaTraseiro()
{
  Serial.println("Checando faixa traseiro...");
  int resultadoSensorTraseiro = digitalRead(sensorFaixaTraseiro);
  if(resultadoSensorTraseiro>0)
  {
    Serial.println("Faixa nao encontrada...");
    return resultadoSensorTraseiro;
  }else
  {
    Serial.println("Faixa encontrada...");
    return resultadoSensorTraseiro;
  }
}

//Funcoes de Servo Motor
void acionaServo()
{
  int atual = 0;
  Serial.println("Levantando pa...");
    for(atual = inicioAngulo; atual < fimAngulo; atual += incremento){
         digitalWrite(servoMotorPort, HIGH);
         delayMicroseconds(atual);
         digitalWrite(servoMotorPort, LOW);
         delayMicroseconds(tamanhoPeriodoSegundos - atual); 
    }
  Serial.println("Abaixando pa...");
    for(atual = fimAngulo; atual > inicioAngulo; atual -= incremento){
         digitalWrite(servoMotorPort, HIGH);
         delayMicroseconds(atual);
         digitalWrite(servoMotorPort, LOW);
         delayMicroseconds(tamanhoPeriodoSegundos - atual);
    }
}

void levantaServo()
{
  int atual = 0;
  Serial.println("Levantando pa...");
  //Angulo da pa (inicioAngulo) manualmente configurado por nao ser utilizado em mais uma parte do codigo
    for(atual = inicioAngulo; atual < 2.2*1000; atual += incremento){
         digitalWrite(servoMotorPort, HIGH);
         delayMicroseconds(atual);
         digitalWrite(servoMotorPort, LOW);
         delayMicroseconds(tamanhoPeriodoSegundos - atual); 
    }
}

void abaixaServo()
{
  int atual = 0;
  Serial.println("Abaixando pa...");
    for(atual = fimAngulo; atual > inicioAngulo; atual -= incremento){
         digitalWrite(servoMotorPort, HIGH);
         delayMicroseconds(atual);
         digitalWrite(servoMotorPort, LOW);
         delayMicroseconds(tamanhoPeriodoSegundos - atual);
    }
}

//Funcao de ataque
void executaAtaque()
{
  if(resultadoSensorFaixaFrontal() == 1 && resultadoSensorFaixaTraseiro() == 1)
  {
    moveFrente();
    acionaServo();
  }
}

//Funcao de inicio 
void iniciaPartida()
{
  Serial.println("Iniciando partida com delay inicial...");
  levantaServo();
  for (int nota = 0; nota < 15; nota++) {
              int duracaodanota = duracaodasnotas[nota];
              tone(portSpeak, melodiaMario[nota],duracaodanota);
              int pausadepoisdasnotas[] ={150,300,300,100,300,550,575,450,400,500,300,330,150,300,200};
              delay(pausadepoisdasnotas[nota]);}
              noTone(portSpeak);
  Serial.println("COMECOU!!!");
}

//Loop principal de instrucoes
void loop()
{
  //Sensor faixa = 1 pista preta, sensor faixa = 0 pista branca
  valor = digitalRead(botaoStart);
  if(contagemBotao == 0)
  {
    if(valor == HIGH)
    {
      iniciaPartida();
      abaixaServo();
      contagemBotao = 1;  
    }
    else
    {
      contagemBotao = 0;
    }
  }
  if(contagemBotao == 1)
  {
    while(resultadoSensorFaixaFrontal() == 1 && resultadoSensorFaixaTraseiro() == 1)
    {
      if(distanciaSensorInfra() == 1 && resultadoSensorFaixaFrontal() == 1 && resultadoSensorFaixaTraseiro() == 1)
      {
        Serial.println("Oponente encontrado...");
        moveParar();
        executaAtaque();
      }
      else
      {
        Serial.println("Oponente nao encontrado, rotacionando...");
        moveDireita();
      }
    }
    if(!resultadoSensorFaixaFrontal() == 1)
    {
      moveTras();
      delay(500);
    }
  }
}