#include "Timer.h" // biblioteca de timer

Timer t; //Objeto Timer

//---------------- PINOS ----------------------------------------

#define CLK       6       // pino do clock (pino 11 do ci) 
#define DATA      5       // pino do data (pino 14 do ci)
#define LATCH     A1       // pino do latch das saídas(leds)
#define LATCH_A   1       // pino do latch do dígito 1 (leds) (pino 12 do ci)
#define LATCH_B   11        // pino do latch do dígito 2 (leds)
#define LATCH_C   9        // pino do latch do dígito 3 (leds)(pino 12 do ci)
#define LATCH_D   4        // pino do latch do dígito 4 (leds)(pino 12 do ci)
#define ENTRADA   3        // pino que lê a entrada dos sensores
#define SELA      A3       // pino do seletor A das entradas
#define SELB      A4       // pino do seletor B das entradas
#define SELC      A5       // pino do seletor C das entradas
#define M1        7        // pino que faz motor girar em uma direção
#define M2        8        // pino que faz motor girar na outra direção
#define FC1       2        // pino do fim de curso 1
#define FC2       13       // pino do fim de curso 2

//---------------- OUTRAS DEFINIÇÕES --------------------------------

// alterar para funcionar de acordo com o tipo de display
#define DIG_ON    LOW
#define DIG_OFF   HIGH
#define SEG_ON    LOW
#define SEG_OFF   HIGH

//---------------- DECLARAÇÃO DE VARIÁVEIS -----------------------

long tempo = 0;// número que será exibido no display
int recorde = 99;
int dig1 = 0;
int dig2 = 0;
int dig3 = 0;
int dig4 = 0;
//int digitos_pino[4] = {DIG1, DIG2, DIG3, DIG4};

int seletor[] = {SELA, SELB, SELC};
int entrada_cont = 0;

//armazena os quatro digitos que serão exibidos no display
int digitos[4];
int dig_cont = 0;

//armazena o byte que representa a saída
byte saida;


//// combinações dos bytes para os números de 0 a 9
//byte numbers[10] {B11111100, //0
//                  B01100000, //1
//                  B11011010, //2
//                  B11110010, //3
//                  B01100110, //4
//                  B10110110, //5
//                  B10111110, //6
//                  B11100000, //7
//                  B11111110, //8
//                  B11110110};//9

                  // combinações dos bytes para os números de 0 a 9
byte numbers[11] {B01111110, //0
                  B00110000, //1
                  B01101101, //2
                  B01111001, //3
                  B00110011, //4
                  B01011011, //5
                  B01011111, //6
                  B01110000, //7
                  B01111111, //8
                  B01111011, //9
                  B00000000};//display apagado


int evento_timer_atualiza_display = 0;
int evento_timer_atualiza_contador = 0;
int evento_timer_atualiza_saida = 0;
int evento_timer_atualiza_entrada = 0;
int evento_timer_espera = 0;
int evento_beep = 0;
int evento_volta_inicio = 0;
int evento_pisca_display = 0;
int evento_vitoria = 0;
int aux_buzzer = 0;
int aux_display = 0;
int cont_beep = 0;
int tempo_beep = 0;
unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long previousMillis2 = 0;        // will store last time LED was updated

// constants won't change:
const long interval =  1000;  

bool estado_buzzer = false;

// vetores com as combinações da chave seletora das entradas
int seletor_A[] = {LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH};
int seletor_B[] = {LOW, LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH};
int seletor_C[] = {LOW, LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH};



int entrada_start = 0;
bool timer_setado = false;
bool contador_parado = false;

int saida_anterior = 0;
byte pinos = 0;
byte pinos_anterior = 0;
int i = 1;
bool dir = 1;

bool pisca_digito[] = {false, false, false, false};
bool pisca_record = false;
int intervalo_display = 500;
int intervalo_buzzer = 300;
int cont_pisca = 0;
bool estado_pisca_dig = 0;
bool aux_beep = 0;
int piscas = 4;
bool aux_stop = false;

enum estados {
  recolhe_pinos,
  espera,
  posiciona_pinos,
  jogo,
  compara_placar,
  fim_tempo
  
};

estados estado = recolhe_pinos;


void setup() {

  //inicialização das entradas e saídas
  pinMode(LATCH_A, OUTPUT);
  pinMode(LATCH_B, OUTPUT);
  pinMode(LATCH_C, OUTPUT);
  pinMode(LATCH_D, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(SELA, OUTPUT);
  pinMode(SELB, OUTPUT);
  pinMode(SELC, OUTPUT);
  pinMode(ENTRADA, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(FC1, INPUT);
  pinMode(FC2, INPUT);
  pinMode(10, OUTPUT);

//  digitalWrite(DIG1, DIG_OFF);
//  digitalWrite(DIG2, DIG_OFF);
//  digitalWrite(DIG3, DIG_OFF);
//  digitalWrite(DIG4, DIG_OFF);

//  // configura os timers
//  evento_timer_atualiza_display = t.every(1, atualiza_display);
// evento_timer_atualiza_contador = t.every(1000, atualiza_contador);
evento_timer_atualiza_saida = t.every(10, atualiza_saida);
//  //evento_timer_atualiza_entrada = t.every(100, atualiza_entrada);
//  evento_timer_espera = t.every(100, espera);
//  //evento_timer_buzzer = t.every(100, buzzer);
//  
  saida = 0;
  tempo = 0;
  evento_timer_atualiza_contador = t.every(1000, atualiza_contador);
  Serial.begin(9600);
  digitalWrite(10, HIGH);

  
}

void loop(){
  t.update();

  
  if(tempo > 5){   
    if(!aux_stop) {
      t.stop(evento_timer_atualiza_contador);
      pisca_display(6);
      beep(6);
      aux_stop = true;
      evento_vitoria = t.pulse(10, 1000, 1);
    }
  }
}


void beep(int num_vezes){
  evento_beep = t.after(intervalo_buzzer, ativa_buzzer);
  cont_beep = num_vezes;
}

void ativa_buzzer(){

   if(cont_beep > 0){
     estado_buzzer = !estado_buzzer;
     bitWrite(saida, 7, estado_buzzer);
     cont_beep--;
     evento_beep = t.after(intervalo_buzzer, ativa_buzzer);
   }
}



void pisca_display(int num_vezes){
  
  evento_pisca_display = t.after(intervalo_display, pisca);
  cont_pisca = num_vezes;
}

void pisca(){

  if(cont_pisca > 0){
    if(!estado_pisca_dig){
      digitalWrite(LATCH_D, LOW);
      shiftOut(DATA, CLK, LSBFIRST, numbers[10]);
      shiftOut(DATA, CLK, LSBFIRST, numbers[10]);
      digitalWrite(LATCH_D, HIGH);
      estado_pisca_dig = true;
      Serial.println("teste");
    }
    else{
      digitalWrite(LATCH_D, LOW);
      shiftOut(DATA, CLK, LSBFIRST, numbers[digitos[2]]);
      shiftOut(DATA, CLK, LSBFIRST, numbers[digitos[3]]);
      digitalWrite(LATCH_D, HIGH);
      estado_pisca_dig = false;
    }
  }
  cont_pisca--;
  int evento_pisca_display = t.after(intervalo_display, pisca);
}

void atualiza_contador(){

  quebra_numero(tempo++);

  digitalWrite(LATCH_D, LOW);
  shiftOut(DATA, CLK, LSBFIRST, numbers[digitos[2]]);
  //digitalWrite(LATCH_D, HIGH);
  //digitalWrite(LATCH_C, LOW);
  shiftOut(DATA, CLK, LSBFIRST, numbers[digitos[3]]);
  digitalWrite(LATCH_D, HIGH);


  //Serial.println(tempo);
 
}

void atualiza_recorde(){
   recorde = tempo;
   quebra_numero(recorde);
   digitalWrite(LATCH_A, LOW);
   shiftOut(DATA, CLK, LSBFIRST, numbers[digitos[0]]);
   digitalWrite(LATCH_A, HIGH);
   digitalWrite(LATCH_B, LOW);
   shiftOut(DATA, CLK, LSBFIRST, numbers[digitos[1]]);
   digitalWrite(LATCH_B, HIGH);

   Serial.println(recorde);
}

//separa o número em 4 digitos individuais
void quebra_numero(long num){
  
  dig1 = num / 1000;
  digitos[0] = dig1;

  int primeiro_esquerda = num - (dig1 * 1000);
  dig2 =  primeiro_esquerda / 100;
  digitos[1] = dig2;
  
  int segundo_esquerda = primeiro_esquerda - (dig2 * 100);
  dig3 = segundo_esquerda / 10;
  digitos[2] = dig3;
  
  dig4 = segundo_esquerda - (dig3 * 10);
  digitos[3] = dig4;
}

void funcao_espera(){

  if(saida == 0){
    saida = 127;
  }
  else
    saida = 0;
  
}

void atualiza_saida(){
  
  digitalWrite(LATCH, LOW); //coloca o registrador em modo leitura
  shiftOut(DATA, CLK, LSBFIRST, saida); //envia os dados
  digitalWrite(LATCH, HIGH); //coloca o registrador em modo escrita  
}

void atualiza_entrada(){
  //escolhe a entrada que será lida
  digitalWrite(SELA, seletor_A[entrada_cont]);
  digitalWrite(SELB, seletor_B[entrada_cont]);
  digitalWrite(SELC, seletor_C[entrada_cont]);

  //Lê a entrada e armazena o valor no bit correspondente da variável saída.
  bool valor_entrada = digitalRead(ENTRADA);
  bitWrite(saida, entrada_cont, valor_entrada);

  
  
  if(valor_entrada != bitRead(pinos_anterior, entrada_cont)){
      bitWrite(pinos_anterior, entrada_cont, valor_entrada);
      if(!valor_entrada){
        bitWrite(pinos, entrada_cont, true);
    }
    
  }

  
 
  if (entrada_cont < 7) entrada_cont++;
  else entrada_cont = 0;

}

