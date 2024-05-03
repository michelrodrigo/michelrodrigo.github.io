#include "Timer.h"

Timer t;

//todos os tempos são em milisegundos

#define TEMPO_ESPERA            200 //intervalo de tempo em que os leds vão ficar piscando no modo espera
#define TEMPO_CONTADOR          500 //intervalo de tempo de atualização do contador 
#define TEMPO_ALVO_LIGADO       1500 //tempo em que o alvo fica ligado
#define TEMPO_ENTRE_RODADAS     2500 //tempo entre duas rodadas. Esse valor deve ser maior que o TEMPO_ALVO_LIGADO
#define INTERVALO_DISPLAY_TEMPO 300 // intervalo de tempo em que o display pisca
#define INTERVALO_DISPLAY_J1    300 // intervalo de tempo em que o display pisca
#define INTERVALO_DISPLAY_J2    300 // intervalo de tempo em que o display pisca
#define INTERVALO_PISCA_LED     300 // intervalo de tempo em que o led do alvo pisca quando foi atingido
#define TEMPO_VOLTA_INICIO      4000 // tempo depois do qual o jogo volta para o modo espera depois de finalizado

#define PONTUACAO_MAXIMA        10 // pontuação máxima que um jogador atinge para ganhar o jogo
#define PONTUACAO_ESPECIAL      2 // pontuação das naves maiores
#define NAVE_ESPECIAL1          1 // número da nave que corresponderá à pontuação especial para o jogador 1
#define NAVE_ESPECIAL2          5 // número da nave que corresponderá à pontuação especial para o jogador 2


//não alterar 
#define TEMPO_ATUALIZA_ENTRADA        20 // tempo para atualização da entrada
#define TEMPO_ATUALIZA_SAIDA          50 // tempo entre atualizações da saída
#define VALOR_REFERENCIA 200

//pinos do arduino
#define CLK         4
#define LATCH       6
#define DATA        5
#define SELA        A0
#define SELB        A1
#define SELC        A2
#define ENTRADA1    A3
#define ENTRADA2    A4
#define ENTRADA3    A5
#define LASER       12
#define SAIDA_LED1  11
#define SAIDA_LED2  13
#define LATCH_J1    9
#define LATCH_J2    8
#define LATCH_T     7
#define START       2
#define MOTOR       10





int evento_jogo = 0;
int evento_espera = 0;
int evento_le_entrada = 0;
int evento_desliga_entrada = 0;
int evento_pisca_led1 = 0;
int evento_pisca_led2 = 0;
int evento_saida_led1 = 0;
int evento_saida_led2 = 0;
int evento_contador = 0;
int evento_saida = 0;
int evento_pisca_tempo = 0;
int evento_pisca_placarJ1 = 0;
int evento_pisca_placarJ2 = 0;
int evento_volta_inicio = 0;


int cont_pisca_led1 = 0;
bool estado_pisca_led1 = false;
bool timer_led1_setado = false;

int cont_pisca_led2 = 0;
bool estado_pisca_led2 = false;
bool timer_led2_setado = false;

int led1 = 0;
int led2 = 0;

int i = 0;
int rand_num1 = 0;
int rand_num2 = 0;

int aux_espera_j1 = 5;
int aux_espera_j2 = 0;


int tempo = 99;
int digitos_tempo[2];
int digitos_J1[2];
int digitos_J2[2];
int dig1 = 0;
int dig2 = 0;

int cont_pisca_tempo = 0;
int cont_pisca_placarJ1 = 0;
int cont_pisca_placarJ2 = 0;
bool estado_pisca_tempo = 0;
bool estado_pisca_placarJ1 = 0;
bool estado_pisca_placarJ2 = 0;
int contador_aux = 0;

bool j1_acertou = false;
bool j2_acertou = false;
int pontoJ1 = 0;
int pontoJ2 = 0;

byte saida[3] = {0, 0, 0};

// vetores com as combinações da chave seletora das entradas
int seletor_A[] = {LOW, LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH};
int seletor_B[] = {LOW, HIGH, HIGH, LOW, HIGH, LOW, LOW, HIGH};
int seletor_C[] = {HIGH, HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW};

//// combinações dos bytes para os números de 0 a 9
byte numbers[11] {B01111110, //0
                  B00011000, //1
                  B01101101, //2
                  B00111101, //3
                  B00011011, //4
                  B00110111, //5
                  B01110111, //6
                  B00011100, //7
                  B01111111, //8
                  B00011111, //9
                  B00000000};//display apagado

enum estados {
  espera,
  inicializa,
  jogo,
  aguarda_fim,
  fim_tempo
  
  
};

estados estado = espera;

bool inicializa_estado = false;

void setup(){
  
  pinMode(CLK, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(SELA, OUTPUT);
  pinMode(SELB, OUTPUT);
  pinMode(SELC, OUTPUT);
  pinMode(LASER, OUTPUT);
  pinMode(SAIDA_LED1, OUTPUT);
  pinMode(SAIDA_LED2, OUTPUT);
  pinMode(LATCH_J1, OUTPUT);
  pinMode(LATCH_J2, OUTPUT);
  pinMode(LATCH_T, OUTPUT);
  pinMode(MOTOR, OUTPUT);
  
  evento_saida = t.every(TEMPO_ATUALIZA_SAIDA, atualizaSaida);
 
  
  
  led1 = 0;
  led2 = 0;

  randomSeed(analogRead(7));

  Serial.begin(9600);

  led1 &= 0x0000FFFF;
  led2 |= 0x0000FFFF;

  zeraDisplays();
}

void loop() {
  t.update();

  switch(estado){
    case(espera):
      if(!inicializa_estado){
        inicializa_estado = true;
        evento_espera = t.every(TEMPO_ESPERA, funcaoEspera);
      }
      if(!digitalRead(START)){
        estado = inicializa;
        inicializa_estado = false;   
        t.stop(evento_espera);
      }      
      break;
    case(inicializa):
      zeraDisplays();
      estado = jogo;
      inicializa_estado = false;
      tempo = 100;
      pontoJ1 = 0;
      pontoJ2 = 0;
      quebraNumeroJ1(pontoJ1);
      quebraNumeroJ2(pontoJ2);
      desligaLeds();   
      break;
    case(jogo):
      if(!inicializa_estado){
        inicializa_estado = true;
        evento_contador = t.every(TEMPO_CONTADOR, atualizaContador);
        evento_jogo = t.every(TEMPO_ENTRE_RODADAS, funcaoJogo);
        digitalWrite(MOTOR, HIGH);
             
      }
      if(!digitalRead(START)){
        estado = inicializa;
        t.stop(evento_contador);
        t.stop(evento_jogo);
      }

      if(tempo < 1){
        estado = fim_tempo;
        inicializa_estado = false;
      }

      if(pontoJ1 == PONTUACAO_MAXIMA){
        t.stop(evento_contador);
        t.stop(evento_jogo);
        piscaPlacarJ1(8);
        evento_saida_led1 = t.oscillate(SAIDA_LED1, INTERVALO_PISCA_LED, LOW, 4); 
       evento_volta_inicio = t.after(TEMPO_VOLTA_INICIO, voltaInicio); 
        estado = aguarda_fim; 
        tempo = 1;
        atualizaContador();
        digitalWrite(MOTOR, LOW);      
      }
      else if (pontoJ2 == PONTUACAO_MAXIMA){
        t.stop(evento_contador);
        t.stop(evento_jogo);
        piscaPlacarJ2(8);
        evento_saida_led2 = t.oscillate(SAIDA_LED2, INTERVALO_PISCA_LED, LOW, 4);
        evento_volta_inicio = t.after(TEMPO_VOLTA_INICIO, voltaInicio);
        estado = aguarda_fim; 
        tempo = 1;
        atualizaContador();
        digitalWrite(MOTOR, LOW);    
      }
      break;

    case(aguarda_fim):

      break;

    case(fim_tempo):
      if(!inicializa_estado){
        inicializa_estado = true;
        Serial.println("fim tempo");
        t.stop(evento_contador);
        t.stop(evento_jogo);
        digitalWrite(MOTOR, LOW);
        if(pontoJ1 > pontoJ2){
          piscaPlacarJ1(8);
          piscaTempo(8);
          evento_saida_led1 = t.oscillate(SAIDA_LED1, INTERVALO_PISCA_LED, LOW, 4);
                    
        }
        else if(pontoJ2 >  pontoJ1){
          piscaPlacarJ2(8);
          piscaTempo(8);
          evento_saida_led2 = t.oscillate(SAIDA_LED2, INTERVALO_PISCA_LED, LOW, 4);
        }
        else{
          piscaPlacarJ1(8);
          piscaPlacarJ2(8);
          piscaTempo(8);
          evento_saida_led1 = t.oscillate(SAIDA_LED1, INTERVALO_PISCA_LED, LOW, 4);
          evento_saida_led2 = t.oscillate(SAIDA_LED2, INTERVALO_PISCA_LED, LOW, 4);
        }

        evento_volta_inicio = t.after(TEMPO_VOLTA_INICIO, voltaInicio);
        
      }
      break;
    
  }

}


void voltaInicio(){
  estado = espera;
  inicializa_estado = false;
}

void piscaTempo(int num_vezes){
  
  evento_pisca_tempo = t.after(INTERVALO_DISPLAY_TEMPO, piscaT);
  cont_pisca_tempo = num_vezes;
  estado_pisca_tempo = false;
  
   
}

void piscaT(){

  if(cont_pisca_tempo > 0){
    if(!estado_pisca_tempo){
      digitalWrite(LATCH_T, LOW);
      shiftOut(DATA, CLK, LSBFIRST, numbers[10]);
      shiftOut(DATA, CLK, LSBFIRST, numbers[10]);
      digitalWrite(LATCH_T, HIGH);
      estado_pisca_tempo = true;     
    }
    else{
      digitalWrite(LATCH_T, LOW);
      shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_tempo[0]]);
      shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_tempo[1]]);
      digitalWrite(LATCH_T, HIGH);
      estado_pisca_tempo = false;
    }
    cont_pisca_tempo--;
    evento_pisca_tempo = t.after(INTERVALO_DISPLAY_TEMPO, piscaT);
    
  }
  else{
    t.stop(evento_pisca_tempo);
  } 
}

void piscaPlacarJ1(int num_vezes){
  
  evento_pisca_placarJ1 = t.after(INTERVALO_DISPLAY_J1, piscaJ1);
  cont_pisca_placarJ1 = num_vezes;
  estado_pisca_placarJ1 = false;
  
   
}

void piscaJ1(){

  if(cont_pisca_placarJ1 > 0){
    if(!estado_pisca_placarJ1){
      digitalWrite(LATCH_J1, LOW);
      shiftOut(DATA, CLK, LSBFIRST, numbers[10]);
      shiftOut(DATA, CLK, LSBFIRST, numbers[10]);
      digitalWrite(LATCH_J1, HIGH);
      estado_pisca_placarJ1 = true;     
    }
    else{
      digitalWrite(LATCH_J1, LOW);
      shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_J1[0]]);
      shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_J1[1]]);
      digitalWrite(LATCH_J1, HIGH);
      estado_pisca_placarJ1 = false;
    }
    cont_pisca_placarJ1--;
    evento_pisca_placarJ1 = t.after(INTERVALO_DISPLAY_J1, piscaJ1);
    
  }
  else{
    t.stop(evento_pisca_placarJ1);
  } 
}

void piscaPlacarJ2(int num_vezes){
  
  evento_pisca_placarJ2 = t.after(INTERVALO_DISPLAY_J2, piscaJ2);
  cont_pisca_placarJ2 = num_vezes;
  estado_pisca_placarJ2 = false;
  
   
}

void piscaJ2(){

  if(cont_pisca_placarJ2 > 0){
    if(!estado_pisca_placarJ2){
      digitalWrite(LATCH_J2, LOW);
      shiftOut(DATA, CLK, LSBFIRST, numbers[10]);
      shiftOut(DATA, CLK, LSBFIRST, numbers[10]);
      digitalWrite(LATCH_J2, HIGH);
      estado_pisca_placarJ2 = true;     
    }
    else{
      digitalWrite(LATCH_J2, LOW);
      shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_J2[0]]);
      shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_J2[1]]);
      digitalWrite(LATCH_J2, HIGH);
      estado_pisca_placarJ2 = false;
    }
    cont_pisca_placarJ2--;
    evento_pisca_placarJ2 = t.after(INTERVALO_DISPLAY_J2, piscaJ2);
    
  }
  else{
    t.stop(evento_pisca_placarJ2);
  } 
}


void funcaoEspera(){
//  Serial.print(led1, BIN);
//  Serial.print("  ");
//  Serial.println((led1 | 0x0001) << 1, BIN);

  ligaLed1(aux_espera_j1+1);
  ligaLed2(aux_espera_j2+1);
    
  if(aux_espera_j1 <= 8){    
    aux_espera_j1++;
  }
  else {
    aux_espera_j1 = 0;
  }

  if(aux_espera_j2 <= 8){    
    aux_espera_j2++;
  }
  else {
    aux_espera_j2 = 0;
  }

//  if(aux_espera_j1 == 0){
//    desligaLeds();
//  }
//  if(aux_espera_j1 <= 9){    
//    aux_espera_j1++;
//    led1 = (led1 << 1) | 0x0001;
//  }
//  else {
//    if(aux_espera_j2 <= 9){    
//      aux_espera_j2++;
//      led2 = (led2 << 1) | 0x0001;
//      }
//    else {
//      if(aux_espera_j1 <= 19){    
//        aux_espera_j1++;
//        led1 = (led1 << 1);
//      }
//      else{
//        if(aux_espera_j2 <= 19){    
//          aux_espera_j2++;
//          led2 = (led2 << 1);
//        }
//        else{
//           led1 = 0;
//           led2 = 0;
//           aux_espera_j1 = 0;
//           aux_espera_j2 = 0;
//        }   
//      }
//    }
//  } 
}

void zeraDisplays(){
  digitalWrite(LATCH_T, LOW);  
  shiftOut(DATA, CLK, LSBFIRST, numbers[0]);
  shiftOut(DATA, CLK, LSBFIRST, numbers[0]);
  digitalWrite(LATCH_T, HIGH);
  digitalWrite(LATCH_J1, LOW);  
  shiftOut(DATA, CLK, LSBFIRST, numbers[0]);
  shiftOut(DATA, CLK, LSBFIRST, numbers[0]);
  digitalWrite(LATCH_J1, HIGH);
  digitalWrite(LATCH_J2, LOW);  
  shiftOut(DATA, CLK, LSBFIRST, numbers[0]);
  shiftOut(DATA, CLK, LSBFIRST, numbers[0]);
  digitalWrite(LATCH_J2, HIGH);
}

// função que atualiza o contador de tempo
void atualizaContador(){
  tempo--;
  quebraNumeroTempo(tempo);

  digitalWrite(LATCH_T, LOW);  
  shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_tempo[0]]);
  shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_tempo[1]]);
  digitalWrite(LATCH_T, HIGH);
}

//separa o número em 4 digitos individuais
void quebraNumeroTempo(int num){
   
  dig1 = num / 10;
  digitos_tempo[0] = dig1;

  int dig2 = num - (dig1 * 10);
  digitos_tempo[1] = dig2;
}

// função que atualiza o contador de tempo
void atualizaPlacarJ1(){
  if(rand_num1 != NAVE_ESPECIAL1){
    pontoJ1++;    
  }
  else{
    pontoJ1 = pontoJ1 + PONTUACAO_ESPECIAL;
  }
  quebraNumeroJ1(pontoJ1);

  digitalWrite(LATCH_J1, LOW);  
  shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_J1[0]]);
  shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_J1[1]]);
  digitalWrite(LATCH_J1, HIGH);
}

//separa o número em 4 digitos individuais
void quebraNumeroJ1(int num){
   
  dig1 = num / 10;
  digitos_J1[0] = dig1;

  int dig2 = num - (dig1 * 10);
  digitos_J1[1] = dig2;
}

// função que atualiza o contador de tempo
void atualizaPlacarJ2(){
  if(rand_num2 != NAVE_ESPECIAL2){
    pontoJ2++;    
  }
  else{
    pontoJ2 = pontoJ2 + PONTUACAO_ESPECIAL;
  }
  quebraNumeroJ2(pontoJ2);

  digitalWrite(LATCH_J2, LOW);  
  shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_J2[0]]);
  shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_J2[1]]);
  digitalWrite(LATCH_J2, HIGH);
}

//separa o número em 4 digitos individuais
void quebraNumeroJ2(int num){
   
  dig1 = num / 10;
  digitos_J2[0] = dig1;

  int dig2 = num - (dig1 * 10);
  digitos_J2[1] = dig2;
}

void funcaoJogo(){
  digitalWrite(LASER, HIGH);

  //teste de ldr
  //rand_num1 = 0;
  //rand_num2 = aux_teste;
  
  rand_num1 = random(1, 11);
  rand_num2 = random(1, 11);

  while(rand_num1 == rand_num2){
    rand_num2 = random(1, 11);
  }

  if(rand_num1 == NAVE_ESPECIAL1){
    rand_num2 = NAVE_ESPECIAL2;
  }  
  if(rand_num2 == NAVE_ESPECIAL2){
    rand_num1 = NAVE_ESPECIAL1;
  }
  
   ligaLed1(rand_num1);
   ligaLed2(rand_num2);
   

   evento_le_entrada = t.every(TEMPO_ATUALIZA_ENTRADA, leEntrada);
   evento_desliga_entrada = t.after(TEMPO_ALVO_LIGADO, desligaEntrada);  
   //aux_teste++;
  
}

void desligaEntrada(){
  digitalWrite(LASER, LOW);
  desligaLeds();
  t.stop(evento_le_entrada);
  j1_acertou = false;
  j2_acertou = false;
  timer_led1_setado = false;
  timer_led2_setado = false;
}

void piscaLed1(int num_vezes){
  
  evento_pisca_led1 = t.after(INTERVALO_PISCA_LED, piscaL1);
  Serial.print("Pisca led 1: ");
  Serial.println(evento_pisca_led1);
  cont_pisca_led1 = num_vezes;
  estado_pisca_led1 = false;   
}

void piscaL1(){
//  Serial.print(cont_pisca_led1);
//  Serial.print("  ");
//  Serial.print(estado_pisca_led1);
  if(cont_pisca_led1 > 0){
    if(!estado_pisca_led1){      
      led1 = 0;     
      estado_pisca_led1 = true;
    }
    else{
      ligaLed1(rand_num1);
      estado_pisca_led1 = false;
    }
    cont_pisca_led1--;
    evento_pisca_led1 = t.after(INTERVALO_PISCA_LED, piscaL1);
    
  }
  else{
    t.stop(evento_pisca_led1);
  } 
}

void piscaLed2(int num_vezes){
  
  evento_pisca_led2 = t.after(INTERVALO_PISCA_LED, piscaL2);
  Serial.print("Pisca led 2: ");
  Serial.println(evento_pisca_led2);
  cont_pisca_led2 = num_vezes;
  estado_pisca_led2 = false;   
}

void piscaL2(){
//  Serial.print(cont_pisca_led2);
//  Serial.print("  ");
//  Serial.print(estado_pisca_led2);
  if(cont_pisca_led2 > 0){
    if(!estado_pisca_led2){      
      led2 = 0;     
      estado_pisca_led2 = true;
    }
    else{
      ligaLed2(rand_num2);
      estado_pisca_led2 = false;
    }
    cont_pisca_led2--;
    evento_pisca_led2 = t.after(INTERVALO_PISCA_LED, piscaL2);
    
  }
  else{
    t.stop(evento_pisca_led2);
  } 
}



void leEntrada(){
//  Serial.print(rand_num1);
//  Serial.print("  ");
//  Serial.print(rand_num2);
//  Serial.print("  ");
//  Serial.println(j1_acertou);
//  Serial.print(led1, HEX);
//  Serial.print("  ");
//  Serial.print(led2, HEX);
//  Serial.print("  ");
//  Serial.print(saida[0], BIN);
//  Serial.print("  ");
//  Serial.print(saida[1], BIN);
//  Serial.print("  ");
//  Serial.println(saida[2], BIN);
  switch(rand_num1)
  {
    case(10):
      if(analogRead(ENTRADA2) < VALOR_REFERENCIA){
        j1_acertou = true;
      }
      break;
    case(9):
      if(analogRead(ENTRADA3) < VALOR_REFERENCIA){
        j1_acertou = true;
      }
      break;
    default:
      digitalWrite(SELA, seletor_A[rand_num1-1]);
      digitalWrite(SELB, seletor_B[rand_num1-1]);
      digitalWrite(SELC, seletor_C[rand_num1-1]);
      if(analogRead(ENTRADA1) < VALOR_REFERENCIA){
        j1_acertou = true;
      }
      
      break;
  }

  switch(rand_num2)
  {
    case(10):
      if(analogRead(ENTRADA2) < VALOR_REFERENCIA){
        j2_acertou = true;
      }
      break;
    case(9):
      if(analogRead(ENTRADA3) < VALOR_REFERENCIA){
        j2_acertou = true;
      }
      break;
    default:
      digitalWrite(SELA, seletor_A[rand_num2-1]);
      digitalWrite(SELB, seletor_B[rand_num2-1]);
      digitalWrite(SELC, seletor_C[rand_num2-1]);
      if(analogRead(ENTRADA1) < VALOR_REFERENCIA){
        j2_acertou = true;
      }
      
      break;
  }

  if(j1_acertou && !timer_led1_setado){
    piscaLed1(3);
    timer_led1_setado = true;
    evento_saida_led1 = t.oscillate(SAIDA_LED1, INTERVALO_PISCA_LED, LOW, 1);
    atualizaPlacarJ1();
  }
  if(j2_acertou && !timer_led2_setado){
    piscaLed2(3);
    timer_led2_setado = true;
    evento_saida_led2 = t.oscillate(SAIDA_LED2, INTERVALO_PISCA_LED, LOW, 1);
    atualizaPlacarJ2();
  }
}

void teste2(){
//  if(i < 10){
//    ligaLed1(i);
//    ligaLed2(9-i);
//    i++;
//  }
//  else{
//    i = 0;
//  }

  if(i < 10){
    ligaLed1(i);
    ligaLed2(i);
    i++;
  }
  else{
    i = 0;
  }
}


void ligaLed1(int led){
  led1 = 0;
  bitWrite(led1, led-1, 1);
}

void ligaLed2(int led){
  led2 = 0;
  bitWrite(led2, led-1, 1);
}

void desligaLeds(){
  led1 = 0;
  led2 = 0;
}


void teste1(){
  led1 = (led1 ^ 0xFFFF);
  led2 = led2 ^ 0xFFFF;

  Serial.print(led1, HEX);
  Serial.print("  ");
  Serial.print(led2, HEX);
  Serial.print("  ");
  Serial.print(saida[2], BIN);
  Serial.print("  ");
  Serial.print(saida[1], BIN);
  Serial.print("  ");
  Serial.println(saida[0], BIN);
}


void atualizaSaida(){ 
//  saida[0] = (led1 & 0x000003F8) >> 2;
//  saida[1] = ((led1 & 0x00000007) << 5) | (led2 & 0x00000380) >> 6;
//  saida[2] = (led2 & 0x0000007F) << 1;  


  saida[0] = (led1&0x0001)<<1 | (led2&0x0001)<<2 |    //1
             (led1&0x0002)<<4 | (led2&0x0002)<<2 |    //2
             (led1&0x0004)<<2 | (led2&0x0004)<<5 |    //3
             (led2&0x0008)<<3;                        //4
  saida[1] = (led1&0x0008)>>1 |                       //4
             (led1&0x0010)>>1 | (led2&0x0010)>>3 |    //5
             (led1&0x0020)<<2 | (led2&0x0020) |       //6
             (led1&0x0040)    | (led2&0x0040)>>2;     //7  
  saida[2] = (led1&0x0080)>>6 | (led2&0x0080)>>5 |    //8
             (led1&0x0100)>>3 | (led2&0x0100)>>5 |    //9
             (led1&0x0200)>>5 | (led2&0x0200)>>2;     //10
                      


  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLK, MSBFIRST, saida[2]);
  shiftOut(DATA, CLK, MSBFIRST, saida[1]);
  shiftOut(DATA, CLK, MSBFIRST, saida[0]);
  digitalWrite(LATCH, HIGH);
}
