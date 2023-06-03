/*===============================================================
  
  Controlador de seta eletrônico para motocicletas
  
  Autor: Gabriel Leonardo e Euler
  Data: 24/04/2023

  Este projeto visa substituir acionadores mecânicos para seta de motos. Preferencialmente deve ser utilizado um arduino nano e relés.
  Os tempos são configuráveis de acordo com a preferência de cada motociclista.
  É aplicácel tanto com botões mecânicos quanto capacitivos. Botão é acionado em LOW
  
==============================================================*/

#define BUTTON1 PD2 // Botão usado para piscar a lâmpada esquerda
#define BUTTON2 PD3 // Botão usado para parar de piscar todas as lâmpadas ou acender o modo pisca-alerta
#define BUTTON3 PD4 // Botão usado para piscar a lâmpada direita
#define LIGHT_L PD6
#define LIGHT_R PD7

int i = 0;              // contador 1
int i1 = 0;             // contador 2
int limit = 0;
unsigned long t0 = 0.0; // armazena o tempo inicial
unsigned long t1 = 0.0; // armazena o tempo final

void setup() {

  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(LIGHT_L, OUTPUT);
  pinMode(LIGHT_R, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON2), stop_light, FALLING); // Ativa a interrupção no pino do BUTTON2. A interrupção será usada para parar de piscar qualquer lâmpada imediatamente (exceto pisca-alerta)

}

void loop() {
  if(digitalRead(BUTTON1)==LOW){                   // Caso pressionado BUTTON 1, chama direction_left()
    direction_left();
  }
  if(digitalRead(BUTTON3)==LOW){                  // Caso pressionado BUTTON 3, chama direction_right()
    direction_right();
  }
  if(digitalRead(BUTTON2)==LOW){                  
    t0 = millis();
    while(1){
      t1 = millis();
      if ((unsigned long)(t1 - t0) >= 3000) {     // Caso o BUTTON 2 seja segurado por mais de 3 segundos, entra no modo pisca-alerta
        digitalWrite(LIGHT_R, HIGH);
        digitalWrite(LIGHT_L, HIGH);
        t0 = millis();
        i1 = 0;

        while (1){
          t1 = millis();
          if ((unsigned long)(t1 - t0) >= 500) {    // Muda o estado das lâmpadas a cada 0,5 segundos
            t0 = millis();
          if (i1 % 2 == 0){
            digitalWrite(LIGHT_R, LOW);
            digitalWrite(LIGHT_L, LOW);
          }
          else {
            digitalWrite(LIGHT_R, HIGH);
            digitalWrite(LIGHT_L, HIGH);
          }
          i1++;
          }
    
          if (i1 > 5) {
            if (digitalRead(BUTTON2)==LOW){        // Caso o BUTTON 2 seja pressionado novamente depois ocorrerem ao menos 5 mudanças de estado, sai do modo pisca-alerta
              break;
            }
          }
        }
        break;
      }
      if(digitalRead(BUTTON2)==HIGH){              // Caso o BUTTON 2 não tenha sido segurado por pelo menos 3 segundos, voltará ao estado HIGH e retornará para o loop
        break;
      }
    }
  }
}

void direction_left(){                        // Função responsável por piscar a lâmpada da esquerda

  digitalWrite(LIGHT_L, HIGH);
  t0 = millis();
  limit = 30;    // Define o limite para 30 mudanças de estado
  i = 0;
  i1 = 0;

  while (1){
    t1 = millis();
    if ((unsigned long)(t1 - t0) >= 500) {   // Troca o estado das lâmpadas a cada 0,5 segundos
      t0 = millis();
      if (i % 2 == 0){
        digitalWrite(LIGHT_L, LOW);
      }
      else {
        digitalWrite(LIGHT_L, HIGH);
      }
      i++;

      if ((i == 4) && (i1 == 0) && (digitalRead(BUTTON1) == LOW)){    // Se o BUTTON1 for segurado por ao menos 4 mudanças de estado, aumenta o limite para 60 mudanças de estado
        limit = 60;
        i1 = 1;   // registra que esta função já foi realizada
      }
      
      if(i > limit){    // Condicional que encerra a direction_left() de acordo com o limite definido
      digitalWrite(LIGHT_L, LOW);
      i = 0;
      break;
      }    
    }
  }
  
  loop();
}

void direction_right(){    // Funciona da mesma forma que a direction_left(), mas para a lâmpada direita

  digitalWrite(LIGHT_R, HIGH);
  limit = 30;
  i = 0;
  i1 = 0;

  while (1){
    t1 = millis();
    if ((unsigned long)(t1 - t0) >= 500) {
      t0 = millis();
      if (i % 2 == 0){
        digitalWrite(LIGHT_R, LOW);
      }
      else {
        digitalWrite(LIGHT_R, HIGH);
      }
      i++;
    
      if ((i == 4) && (i1 == 0) && (digitalRead(BUTTON3) == LOW)){
        limit = 60;
        i1 = 1;
      }

      if(i > limit){
        digitalWrite(LIGHT_R, LOW);
        i = 0;
        break;
      }
    }
  }

  loop();
}

void stop_light(){  // Função usada para encerrar a direction_left() e direction_right() em qualquer instante
  digitalWrite(LIGHT_L, LOW);
  digitalWrite(LIGHT_R, LOW);
  i = 61;
}