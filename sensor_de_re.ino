/***********************************************************************
  Programa criado por Walisson da Silva Soares e Laryssa Maria Duarte.
  Criado em 15 de janeiro de 2016.
  Ultima revisao feita em 22 de janeiro de 2016.
*/

#include <Ultrasonic.h>

int seg[] = {0, 1, 2, 3, 4, 5, 6, 7};
const byte sem_ponto[] = {B00111111, B00000110, B01011011, B01001111, B01100110, B01101101, B01111101, B00000111,
                    B01111111, B01101111}; // EM BINÁRIO
const byte com_ponto[] = {B10111111, B10000110, B11011011, B11001111, B11100110, B11101101, B11111101, B10000111,
                    B01111111, B01101111}; // EM BINÁRIO
const byte NULO = B00000000;
bool alto = true;

unsigned long atual, anterior = 0, tempo, b_anterior = 0;
int X1 = A0, X2 = A1, AUX = A4;
const int gatilho = 10; // pino TRIG do sensor ultrassônico
const int echo = 9; // pino ECHO do sensor ultrassônico
int LED_R = 12, LED_G = 11, LED_Y = 13, BUZZER = 8; // pinos dos LEDs e do BUZZER
float distancia_cm, aux; // para armazenar a distância em centímetros
int centena = 0, dezena = 0, unidade = 0;

Ultrasonic ultrasonic(gatilho, echo);

void setup(){  
  for (int i = 0; i < 8; i++){
    pinMode(seg[i], OUTPUT);
  }
  
  pinMode(X1, OUTPUT);
  pinMode(X2, OUTPUT);
  pinMode(AUX, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  digitalWrite(AUX, HIGH);
}

void loop(){
  atual = millis();
  
  if (atual - anterior >= 700){
    anterior = millis();
    tempo = ultrasonic.timing();
    distancia_cm = ultrasonic.convert(tempo, Ultrasonic::CM);  //centímetros
    
    if (distancia_cm > 4000) distancia_cm = aux;
    
    centena = distancia_cm / 100;
    dezena = ((int) distancia_cm % 100) / 10;
    unidade = (int) distancia_cm % 10;
    
    aux = distancia_cm;                                                                     
  }
  
  // a distancia e inferior a 1 metro
  if (distancia_cm < 100){
    // colocar nivel baixo em todos os segmentos, evitando que se escreva o numero de um display em outro em um curto intervalo de tempo
    escreve_numero(NULO);
  
    // escrevendo a dezena
    digitalWrite(X1, HIGH);
    digitalWrite(X2, LOW);
    escreve_numero(sem_ponto[dezena]);
    
    // colocar nivel baixo em todos os segmentos, evitando que se escreva o numero de um display em outro em um curto intervalo de tempo  
    escreve_numero(NULO);
    
    // escrevendo a unidade
    digitalWrite(X1, LOW);
    digitalWrite(X2, HIGH);
    escreve_numero(sem_ponto[unidade]);
  } else {
    // colocar nivel baixo em todos os segmentos, evitando que se escreva o numero de um display em outro em um curto intervalo de tempo
    escreve_numero(NULO);
  
    // escrevendo a centena seguida de um ponto
    digitalWrite(X1, HIGH);
    digitalWrite(X2, LOW);
    escreve_numero(com_ponto[centena]);
    
    // colocar nivel baixo em todos os segmentos, evitando que se escreva o numero de um display em outro em um curto intervalo de tempo  
    escreve_numero(NULO);
    
    // escrevendo a dezena
    digitalWrite(X1, LOW);
    digitalWrite(X2, HIGH);
    escreve_numero(sem_ponto[dezena]);
  }
  
  // Oscila o LED vermelho e o Buzzer entre nivel alto e nivel baixo
  // (em um intervalo de tempo igual 10 vezes a distancia em cm), caso
  // a distancia seja inferior a 31 cm (ate 30.999... cm)
  if (distancia_cm < 31){
    if (atual - b_anterior >= distancia_cm * 7){
      b_anterior = millis();
      alto = !alto;
      digitalWrite(LED_R, alto);
      digitalWrite(BUZZER, alto);
    }
    
    digitalWrite(LED_Y, LOW);
    digitalWrite(LED_G, LOW);
  } else if (distancia_cm < 51){
    // Acende o LED amarelo, caso a distancia seja inferior a 51 cm (ate 50.999... cm)
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_Y, HIGH);
    digitalWrite(LED_G, LOW);
    digitalWrite(BUZZER, LOW);
  } else {
    // Acende o LED verde, caso a distancia seja superior a 51 cm
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_Y, LOW);
    digitalWrite(LED_G, HIGH);
    digitalWrite(BUZZER, LOW);
  }
}

// Funcao que escreve um numero no display
void escreve_numero(char m){
   for (int j = 0; j < 8; j++){
    digitalWrite(seg[j], bitRead(m, j));
  }
  
  if (m != NULO) delayMicroseconds(500);
}
