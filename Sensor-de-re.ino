/*
 * Trabalho de Introdução aos Algoritmos
 * Sensor de ré
 * Copyright 2019 by Gustavo Ribeiro da Fonseca Santos, Caio Rocha, João Paulo Paiva Lima, Pedro Paulo Araujo Carvalho
*/

#include <NewPing.h>                 //Inclui a biblioteca que facilita o uso do sensor ultrassônico

/* Foram feitas algumas mudanças na biblioteca NewPing.
 * Ela estava apresentando um conflito com a função tone() para o uso do buzzer e  as vezes o sensor falhava e retornava o valor zero, que é um valor indesejado
 * Para resolver o conflito com a função tone, foi feita a mudança na linha 170 do arquivo NewPing.h, trocando a variável TIMER_ENABLED de true para false
 * O problema do retorno zero é que zero é um número possível de ser obtido, então foi editada a linha 173 do arquivo NewPing.h.
 * O valor que por padrão é zero foi editado para 100000000000, que é um valor absurdo.
 * Quando esse valor absurdo é retornado, o código pega o último valor normal retornado como distância real para que essa seja mostrada no display LCD e usada na estrutura condicional 
 * Dessa forma, esse valor absurdo é ignorado 
*/

#include <LiquidCrystal.h>           //Inclui a biblioteca para o funcionamento do display

NewPing sensor(A1,A2);               //Configura o sensor ultrassonico, em que A1 e A2 sao os pinos de entrada no arduino
LiquidCrystal Display(2,3,4,5,6,7);  //Configura o Display lcd, em que os pinos 2,3,4,5,6,7 são os pinos de dados necessários para o uso do display. 

bool bateu = false;                  //Booleano para que quando o "carro" bater, reproduza a música apenas uma vez
int distancia_anterior;              //Variavel que guarda o último valor válido, para que essa seja atribuída à variável "distancia_real" quando o sensor falhar e retornar um valor absurdo

void setup() {
  Serial.begin(9600);                //Inicia a comunicação com o monitor serial
  Display.begin(16,2);               //Inicia a comunicação com o display de dimensões 16x2
  pinMode(A0,OUTPUT);                //Configura o pino A0 do arduino, onde o buzzer está inserido, como dispositivo de saída
}

void musica(){
  enum frequencia_das_notas {G4=396,A4=444,B4=498,C5=528,D5=592,E5=665,As5=941,B5=997,C6=1056,D6=1185,E6=1331,F6=1410};  //Enumera as notas com suas respectivas frequências em hertz
  
  int notas[]={0,B5,As5,B5,0,C6,F6,0,F6,F6,0,E6,0,D6,0,C6,E5,0,D5,C5};                                //Vetor das notas que serão tocadas ordenadamente
  int tempo_das_notas[]={50,100,100,200,225,130,170,94,130,150,14,160,10,160,5,130,153,142,165,178};  //Vetor que armazena o tempo de duração de cada nota do vetor anterior 
  
  for (int i=0;i<20;i++){           //Estrutura de repetição para percorrer os vetores de notas e de seus respectivos tempos de duração
    if (notas[i]==0){               
      delay(tempo_das_notas[i]);    //Se a nota no vetor "notas" for 0, nada será tocado, será apenas um tempo sem nota
    }
    else{                               
      tone(A0,notas[i]);            //Toca a nota da lista
      delay(tempo_das_notas[i]);    //Deixa o tempo da nota passar
      noTone(A0);                   //Para a nota
    }
  }
  delay(100);
}


void loop() {
  int distancia;            //Distância do sensor para o objeto
  int distancia_real;       //Distância do carro, supondo que temos 8 centimetros de estrutura após o sensor
  
  distancia=sensor.ping_cm(); //Distância do sensor para algum objeto à sua frente
  delay(200);

  
  Serial.print("Distancia atual do sensor: ");
  Serial.print(distancia);
  Serial.println(" centimetros");   //Imprime no monitor serial a mensagem e a distância obtida pelo sensor


  if (distancia==1041){             //1041 é o resultado do valor absurdo convertido em centímetros configurado no arquivo NewPing.h
    distancia=distancia_anterior;   //A distância deixa de ser o valor absurdo e passa a ser o último valor válido obtido
  }
  else{
    distancia_anterior=distancia;   //Se o valor for válido, esse valor também é armazenado na variável "distancia_anterior" para que possa substituir a distância em uma possível falha do sensor
  }
  
  distancia_real=distancia-8;       //distancia - 8, pois como dito anteriormente, estamos considerando que o carro tem 8 centimetros de estrutura após o sensor 

  if (distancia_real < 0){
    distancia_real = 0;             //A distância real não pode ser negativa     
  }
  
  //Comandos para mostrar a distância no Display LCD
  Display.clear();
  Display.setCursor(2,0);
  Display.print("Distancia: ");     
  Display.print(distancia_real);
  Display.setCursor(2,1);
  Display.print("Centimetros");

  //Estrutura condicional para que cada intervalo de distância tenha seu próprio tempo de repetição da nota no buzzer

  int nota=1056;           //Variável que armazena a frequência da nota que o buzzer vai tocar 
  if (distancia_real > 70 and distancia_real!=1033){
    tone(A0,nota,100);    //tone(porta do buzzer,frequencia da nota, tempo de duração da nota)
    delay(800);          //tempo de intervalo entre os beeps do buzzer
    bateu=false;          //"bateu" recebe false sempre que a distância_real é maior que 0, para que quando tenha a batida, a música toque apenas uma vez
  }
  else if (distancia_real <= 70 and distancia_real>30){
    tone(A0,nota,100);
    delay(600);
    bateu=false;
  }
  else if (distancia_real <=30 and distancia_real>15){
    tone(A0,nota,100);
    delay(400);
    bateu=false;
  }
  else if (distancia_real <=15 and distancia_real>10){
    tone(A0,nota,80);
    delay(200);
    bateu=false;
  }
  else if (distancia_real <= 10 and distancia_real>5){
    tone(A0,nota,60);
    delay(70);
    bateu=false;
  }
  else if (distancia_real==5){
    tone(A0,nota,50);
    delay(50);
    bateu=false;
  }
  else if (distancia_real ==4){
    tone(A0,nota,50);
    delay(40);
    bateu=false;
  }
  else if (distancia_real == 3){
    tone(A0,nota,50);
    delay(30);
    bateu=false;
  }
  else if (distancia_real == 2){
    tone(A0,nota,50);
    delay(20);
    bateu=false;
  }
  else if (distancia_real == 1){
    tone(A0,nota);
    bateu=false;
  }
  else if(distancia_real<1){
    Serial.println("Bateu");
    if (not bateu){  //A música só será reproduzida se "bateu for false
      musica();
      bateu=true;    //"bateu" recebe true para que não toque a música novamente enquanto a distancia_real não se torna maior que 0
    }
  }
}
