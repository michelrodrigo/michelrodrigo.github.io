#include "Timer.h" // biblioteca de timer

Timer t; //Objeto Timer

//---------------- PINOS ----------------------------------------

#define DIG1      12       // pino do comum do dígito 1 (dezena do record)
#define DIG2      11       // pino do comum do dígito 2 (unidade do record)
#define DIG3      10       // pino do comum do dígito 3 (dezena do tempo)
#define DIG4      9        // pino do comum do dígito 4 (unidade do tempo)
#define CLK1      6        // pino do clock do shift register dos displays
#define LATCH1    5        // pino do latch do shift register dos displays
#define DATA1     4        // pino do data do shift register dos displays
#define CLK2      A2       // pino do clock do shift register das saídas
#define LATCH2    A1       // pino do latch do shift register das saídas
#define DATA2     A0       // pino do data do shift register das saídas
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

long numero = 0;// número que será exibido no display
int dig1 = 0;
int dig2 = 0;
int dig3 = 0;
int dig4 = 0;
int digitos_pino[4] = {DIG1, DIG2, DIG3, DIG4};

int seletor[] = {SELA, SELB, SELC};
int entrada_cont = 0;

//armazena os quatro digitos que serão exibidos no display
int digitos[4];
int dig_cont = 0;

//armazena o byte que representa a saída
byte saida;


// combinações dos bytes para os números de 0 a 9
byte numbers[10] {B11111100, //0
                  B01100000, //1
                  B11011010, //2
                  B11110010, //3
                  B01100110, //4
                  B10110110, //5
                  B10111110, //6
                  B11100000, //7
                  B11111110, //8
                  B11110110};//9


int evento_timer_atualiza_display = 0;
int evento_timer_atualiza_contador = 0;
int evento_timer_atualiza_saida = 0;
int evento_timer_atualiza_entrada = 0;
int evento_timer_espera = 0;
int evento_timer_buzzer = 0;
int evento_volta_inicio = 0;
int beep = 0;
int cont_beep = 0;
int tempo_beep = 0;
unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long previousMillis2 = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 200;  

bool estado_buzzer = false;

// vetores com as combinações da chave seletora das entradas
int seletor_A[] = {LOW, HIGH, LOW, HIGH, LOW, HIGH, LOW, HIGH};
int seletor_B[] = {LOW, LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH};
int seletor_C[] = {LOW, LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH};

int estado = 0;
int record = 12;
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
int intervalo = 500;
int cont_pisca = 0;
int piscas = 4;


void setup() {

  //inicialização das entradas e saídas
  pinMode(DIG1, OUTPUT);
  pinMode(DIG2, OUTPUT);
  pinMode(DIG3, OUTPUT);
  pinMode(DIG4, OUTPUT);
  pinMode(CLK1, OUTPUT);
  pinMode(LATCH1, OUTPUT);
  pinMode(DATA1, OUTPUT);
  pinMode(CLK2, OUTPUT);
  pinMode(LATCH2, OUTPUT);
  pinMode(DATA2, OUTPUT);
  pinMode(SELA, OUTPUT);
  pinMode(SELB, OUTPUT);
  pinMode(SELC, OUTPUT);
  pinMode(ENTRADA, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(FC1, INPUT);
  pinMode(FC2, INPUT);

  digitalWrite(DIG1, DIG_OFF);
  digitalWrite(DIG2, DIG_OFF);
  digitalWrite(DIG3, DIG_OFF);
  digitalWrite(DIG4, DIG_OFF);

  // configura os timers
  evento_timer_atualiza_display = t.every(1, atualiza_display);
  //evento_timer_atualiza_contador = t.every(1000, atualiza_contador);
  evento_timer_atualiza_saida = t.every(1, atualiza_saida);
  //evento_timer_atualiza_entrada = t.every(100, atualiza_entrada);
  evento_timer_espera = t.every(100, espera);
  //evento_timer_buzzer = t.every(100, buzzer);
  

  record = 9900;

  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  while(digitalRead(FC2)){}
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);

  Serial.begin(9600);
  
}


void loop() {
  
  
 // atualiza o temporizador
 t.update();

 if(estado == 3 && !digitalRead(FC1)){
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  estado = 1;
 }

 if(estado == 4 && !digitalRead(FC2)){
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
  evento_timer_espera = t.every(100, espera);
  numero = record;
  quebra_numero(numero);
  //t.stop(evento_volta_inicio);
  timer_setado = false;
  contador_parado = false;
  estado = 0;
 }

 if(estado == 1 && !timer_setado){
  t.stop(evento_timer_espera);
  evento_timer_atualiza_contador = t.every(1000, atualiza_contador);
  evento_timer_atualiza_entrada = t.every(100, atualiza_entrada);
  //numero = record * 100;
  timer_setado = true;
  //saida = 0;
  beep = 1;
  cont_beep = 0;
  tempo_beep = 1000;
  //evento_timer_buzzer = t.every(tempo_beep, buzzer);
 //saida_anterior = saida;
 }

   if(estado == 1 && bitRead(pinos, entrada_cont)){
        beep = 1;
        tempo_beep = 300;
        cont_beep = 0;
        bitWrite(pinos, entrada_cont, false);
   }
//  Serial.print(record);
//  Serial.print(" ");
// Serial.println(numero);
   if(estado == 1 && (saida== 0) ){
        estado = 2;
        saida = 0;
   }

   if(estado == 2 && !contador_parado){
        beep = 3;
        tempo_beep = 400;
        cont_beep = 0;
        bitWrite(pinos, entrada_cont, false);
        t.stop(evento_timer_atualiza_contador);
        t.stop(evento_timer_atualiza_entrada);
        contador_parado = true;
        if(((((numero / 10U) % 10) * 10) + (numero / 1U) % 10) < ((((record / 1000U) % 10) * 10) + (record / 100U) % 10)){
          record = ((((((numero / 10U) % 10) * 10) + (numero / 1U) % 10) - 1) * 100);
        }
        evento_volta_inicio = t.after(3000, reseta);
   }

 unsigned long currentMillis = millis();

 if ((currentMillis - previousMillis >= tempo_beep) && (cont_beep < (2*beep))) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    estado_buzzer = !estado_buzzer;

    if(estado == 2){
      saida ^= 0xff;
    }
    else{
    // if the LED is off turn it on and vice-versa:
      bitWrite(saida, 7, estado_buzzer);
    }

    cont_beep++;
    //Serial.println(cont_beep);
  }

//Serial.println(saida, BIN);
  
//  if ((currentMillis - previousMillis2 >= intervalo) && (cont_pisca < piscas)) {
//    // save the last time you blinked the LED
//    previousMillis2 = currentMillis;
//
//    pisca_digito[2] = !pisca_digito[2];
//    pisca_digito[3] = !pisca_digito[3];
//    // if the LED is off turn it on and vice-versa:
//    // bitWrite(saida, 7, estado_buzzer);
//
//    cont_pisca++;
//    //Serial.println(cont_beep);
//  }
   
 

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

void buzzer(){
  
  if(cont_beep < beep){
    saida ^= 1UL << 7;
    //Serial.println(saida, BIN);
    cont_beep++;
  }
  else{
    //beep = 0;
    t.stop(evento_timer_buzzer);
  }
}

void atualiza_saida(){
  
  digitalWrite(LATCH2, LOW); //coloca o registrador em modo leitura
  shiftOut(DATA2, CLK2, LSBFIRST, saida); //envia os dados
  digitalWrite(LATCH2, HIGH); //coloca o registrador em modo escrita  
}

void atualiza_display() { 

  limpa_displays(); //limpa os displays
  digitalWrite(LATCH1, LOW); //put the shift register to read
  shiftOut(DATA1, CLK1, LSBFIRST, numbers[digitos[dig_cont]]); //send the data
  if(!pisca_digito[dig_cont])digitalWrite(digitos_pino[dig_cont], DIG_ON); //turn on the relevent digit
  digitalWrite(LATCH1, HIGH); //put the shift register to write mode


  dig_cont++; //count up the digit
  if (dig_cont >= 4) { // keep the count between 0-3
    dig_cont = 0;
  }

}

void limpa_displays() { //turn off all 4 digits

  digitalWrite(DIG1, DIG_OFF);
  digitalWrite(DIG2, DIG_OFF);
  digitalWrite(DIG3, DIG_OFF);
  digitalWrite(DIG4, DIG_OFF);
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

void atualiza_contador(){

  quebra_numero(numero++);
}



void espera(){


  if(i == 64){
    dir = 0;
  }
  if(i == 0){
    dir = 1;
    i = 1;
  }

    if(dir)
        i = i << 1;
    else
        i =  i >> 1;
          
   saida ^= i; 

  digitalWrite(SELA, seletor_A[entrada_start]);
  digitalWrite(SELB, seletor_B[entrada_start]);
  digitalWrite(SELC, seletor_C[entrada_start]);
  if(digitalRead(ENTRADA)==1){
    estado = 3;
    digitalWrite(M1, LOW);
    digitalWrite(M2, HIGH);
  }
 
}

void reseta(){
  estado = 4;
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
}

