#include "Timer.h" // biblioteca de timer

Timer t; //Objeto Timer

// o q foi alterado para funcionar no uno
// - matriz numbers


//---------------- PINOS ----------------------------------------

#define CLK       8       // pino do clock (pino 11 do ci) 
#define DATA      9        // pino do data (pino 14 do ci)
#define LATCH     7       // pino do latch das saídas(leds)
#define LATCH_C   11        // pino do latch do recorde (pino 12 do ci)
#define LATCH_D   10       // pino do latch do tempo (pino 12 do ci)
#define VITORIA   A0       // pino que irá acionar o alerta vitória
#define ENTRADA   A4       // pino que lê a entrada dos sensores
#define SELA      A5       // pino do seletor A das entradas
#define SELB      4       // pino do seletor B das entradas
#define SELC      5       // pino do seletor C das entradas
#define M1        3        // pino que faz motor girar em uma direção
#define M2        2        // pino que faz motor girar na outra direção
#define FC1       A2        // pino do fim de curso 1
#define FC2       A3       // pino do fim de curso 2
#define EMISSOR   A1       // pino que ativa o emissor


//---------------- OUTRAS DEFINIÇÕES --------------------------------

// alterar para funcionar de acordo com o tipo de display
#define DIG_ON      LOW
#define DIG_OFF     HIGH
#define SEG_ON      LOW
#define SEG_OFF     HIGH
#define ENTRADA_ON  LOW


#define TEMPO_VOLTA_INICIO            7000 //tempo para o jogo voltar ao modo de espera depois de finalizado
#define INTERVALO_DISPLAY_TEMPO       600
#define INTERVALO_DISPLAY_RECORDE     600
#define INTERVALO_VITORIA             1000
#define INTERVALO_BUZZER              200

#define ENTRADA_START                 7
#define SAIDA_BUZZER                  7

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
int digitos_tempo[2];
int digitos_recorde[2];
int dig_cont = 0;

//armazena o byte que representa a saída
byte saida;


//// combinações dos bytes para os números de 0 a 9
//byte numbers[11] {B01111110, //0
//                  B00011000, //1
//                  B01101101, //2
//                  B00111101, //3
//                  B00011011, //4
//                  B00110111, //5
//                  B01110111, //6
//                  B00011100, //7
//                  B01111111, //8
//                  B00011111, //9
//                  B00000000};//display apagado

                  
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
                  B00000000};//display 

//---------------- EVENTOS TEMPORIZADORES ----------------
int evento_timer_atualiza_display = 0;
int evento_timer_atualizaContador = 0;
int evento_timer_atualizaSaida = 0;
int evento_timer_atualizaEntrada = 0;
int evento_pisca_tempo = 0;
int evento_pisca_recorde = 0;
int evento_timer_espera = 0;
int evento_timer_buzzer = 0;
int evento_volta_inicio = 0;
int evento_beep = 0;
int evento_vitoria = 0;


int cont_beep = 0; // contador do número de beeps
int tempo_beep = 0; // intervalo do beep
bool estado_buzzer = false; // armazena o estado do buzzer 

// vetores com as combinações da chave seletora das entradas
int seletor_A[] = {LOW, LOW, HIGH, LOW, HIGH, HIGH, LOW, HIGH};
int seletor_B[] = {LOW, HIGH, LOW, LOW, HIGH, LOW, HIGH, HIGH};
int seletor_C[] = {HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, HIGH};

// variáveis de controle
bool timer_setado = false;

int saida_aux = 1;
int aux_buzzer = 0;
int aux_entrada = 0;
byte pinos = 127;
byte pinos_anterior = 127;

int cont_pisca_tempo = 0;
int cont_pisca_recorde = 0;
bool estado_pisca_tempo = 0;
bool estado_pisca_recorde = 0;

enum estados {
  recolhe_pinos,
  espera,
  posiciona_pinos,
  jogo,
  compara_placar,
  fim_tempo
  
};

estados estado = espera;


void setup() {

  //inicialização das entradas e saídas
  pinMode(LATCH_C, OUTPUT);
  pinMode(LATCH_D, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(SELA, OUTPUT);
  pinMode(SELB, OUTPUT);
  pinMode(SELC, OUTPUT);
  pinMode(ENTRADA, INPUT);
  pinMode(VITORIA, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(FC1, INPUT);
  pinMode(FC2, INPUT);
  pinMode(EMISSOR, OUTPUT); 

//  tempo = 0;
//  recorde = 99;
  tempo = 99;
  recorde = 0;

  digitalWrite(VITORIA, LOW);
  digitalWrite(EMISSOR, LOW);
  
  //escreve zero nos displays
  digitalWrite(LATCH_D, LOW);
  shiftOut(DATA, CLK, LSBFIRST, numbers[0]);
  shiftOut(DATA, CLK, LSBFIRST, numbers[0]);
  digitalWrite(LATCH_D, HIGH);

  digitalWrite(LATCH_C, LOW);
  shiftOut(DATA, CLK, LSBFIRST, numbers[0]);
  shiftOut(DATA, CLK, LSBFIRST, numbers[0]);
  digitalWrite(LATCH_C, HIGH);

  Serial.begin(9600);
  timer_setado = false;
  bitWrite(saida, SAIDA_BUZZER, false);
  atualizaSaida();
}

void loop()
{
  t.update(); //sempre atualiza o timer

  switch (estado)
  {
    case(recolhe_pinos):
      //atualizaSaida();
      digitalWrite(VITORIA, LOW);
      digitalWrite(M1, HIGH);
      digitalWrite(M2, LOW);
      if(digitalRead(FC2)==0){
        digitalWrite(M1, LOW);
        digitalWrite(M2, LOW);
        estado = posiciona_pinos;
        timer_setado = false;
      }
      break;
      
    case(espera):
      if(timer_setado == false){        
        evento_timer_atualizaSaida = t.every(20, atualizaSaida);
        evento_timer_espera = t.every(200, funcaoEspera);
        digitalWrite(EMISSOR, LOW);
        timer_setado = true;
        digitalWrite(SELA, seletor_A[ENTRADA_START]);
        digitalWrite(SELB, seletor_B[ENTRADA_START]);
        digitalWrite(SELC, seletor_C[ENTRADA_START]);
      }      
      if(digitalRead(ENTRADA) == ENTRADA_ON){
        estado = recolhe_pinos;
        //piscaRecorde(8);
        //piscaTempo(8);
        timer_setado = false;
  }
      break;

    case(posiciona_pinos):
      digitalWrite(M1, LOW);
      digitalWrite(M2, HIGH);
      if(digitalRead(FC1)==0){
        digitalWrite(M1, LOW);
        digitalWrite(M2, LOW);
        estado = jogo;
        timer_setado = false;
        tempo = 99;
        digitalWrite(EMISSOR, HIGH);
      }
      break;

    case(jogo):
      if(timer_setado == false){
        timer_setado = true;
        saida = 127;
        t.stop(evento_timer_espera);
        evento_timer_atualizaEntrada = t.every(100, atualizaEntrada);   
        evento_timer_atualizaContador = t.every(1000, atualizaContador);     
        bitWrite(saida, 7, false);        
      }
     

      if(pinos == 0){//128
        //estado = compara_placar;
        t.stop(evento_timer_atualizaContador);
        t.stop(evento_timer_atualizaEntrada);
        timer_setado = false;
        pinos = 127;
        if(tempo > recorde){
          atualizaRecorde();           
          if(!timer_setado){            
            evento_volta_inicio = t.after(TEMPO_VOLTA_INICIO, finalizaJogo);
            evento_vitoria = t.oscillate(VITORIA, INTERVALO_VITORIA, 0, 4);
            timer_setado = true;
            piscaRecorde(8);
            piscaTempo(8);
            beep(8, INTERVALO_VITORIA);
          }
        }
        else{
          if(!timer_setado){
            evento_volta_inicio = t.after(TEMPO_VOLTA_INICIO, finalizaJogo);
            timer_setado = true;
            piscaTempo(8);
            beep(4, 800);
          }
        }        
      }
      if(tempo < 1){
        estado = fim_tempo;
        t.stop(evento_timer_atualizaContador);
        t.stop(evento_timer_atualizaEntrada);
        piscaTempo(6);
        beep(2, 1600);
        timer_setado = false;
      }
      break;

     

      case(fim_tempo):
        if(!timer_setado){
          evento_volta_inicio = t.after(TEMPO_VOLTA_INICIO, finalizaJogo);
          timer_setado = true;
        }
        break;
  }


}


void atualizaContador(){

  quebraNumeroTempo(tempo);

  digitalWrite(LATCH_D, LOW);  
  shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_tempo[1]]);
  shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_tempo[0]]);
  digitalWrite(LATCH_D, HIGH);

  tempo--;

  Serial.println(tempo);
 
}

void finalizaJogo(){
  estado = espera;
  timer_setado = false;
  pinos = 127;
  
}

void beep(int num_vezes, int intervalo_beep){
  tempo_beep = intervalo_beep;
  evento_beep = t.after(tempo_beep, ativaBuzzer);
  cont_beep = num_vezes;
   Serial.print("Beep: ");
   Serial.print(cont_beep);
   Serial.print(" ");
   Serial.println(estado_buzzer);
  
}

void ativaBuzzer(){

   if(cont_beep > 0){
     estado_buzzer = !estado_buzzer;
     bitWrite(saida, SAIDA_BUZZER, estado_buzzer);
     cont_beep--;
     evento_beep = t.after(tempo_beep, ativaBuzzer);
   }
   else{
    //evento_timer_atualizaEntrada = t.every(100, evento_timer_atualizaEntrada);
   }
   Serial.print("Buzzer: ");
   Serial.print(cont_beep);
   Serial.print(" ");
   Serial.println(estado_buzzer);
}



void piscaTempo(int num_vezes){
  
  evento_pisca_tempo = t.after(INTERVALO_DISPLAY_TEMPO, piscaT);
  cont_pisca_tempo = num_vezes;
  estado_pisca_tempo = false;
  
   
}

void piscaT(){

  if(cont_pisca_tempo > 0){
    if(!estado_pisca_tempo){
      digitalWrite(LATCH_D, LOW);
      shiftOut(DATA, CLK, LSBFIRST, numbers[10]);
      shiftOut(DATA, CLK, LSBFIRST, numbers[10]);
      digitalWrite(LATCH_D, HIGH);
      estado_pisca_tempo = true;
      Serial.print("Pisca Tempo: ");
   Serial.println(estado_pisca_tempo);
      
    }
    else{
      //quebraNumero(tempo);
      digitalWrite(LATCH_D, LOW);
      shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_tempo[1]]);
      shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_tempo[0]]);
      digitalWrite(LATCH_D, HIGH);
      estado_pisca_tempo = false;
    }
    cont_pisca_tempo--;
    evento_pisca_tempo = t.after(INTERVALO_DISPLAY_TEMPO, piscaT);
    
  }
  else{
    t.stop(evento_pisca_tempo);
  }
  //Serial.println(cont_pisca);
  
}

void piscaRecorde(int num_vezes){
  
  evento_pisca_recorde = t.after(INTERVALO_DISPLAY_RECORDE, piscaR);
  cont_pisca_recorde = num_vezes;
  estado_pisca_recorde = false;
}

void piscaR(){

  if(cont_pisca_recorde > 0){
    if(!estado_pisca_recorde){
      digitalWrite(LATCH_C, LOW);
      shiftOut(DATA, CLK, LSBFIRST, numbers[10]);
      shiftOut(DATA, CLK, LSBFIRST, numbers[10]);
      digitalWrite(LATCH_C, HIGH);
      estado_pisca_recorde = true;
      
    }
    else{
      //quebraNumero(recorde);
      digitalWrite(LATCH_C, LOW);
      shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_recorde[1]]);
      shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_recorde[0]]);
      digitalWrite(LATCH_C, HIGH);
      estado_pisca_recorde = false;
    }
    cont_pisca_recorde--;
    evento_pisca_recorde = t.after(INTERVALO_DISPLAY_RECORDE, piscaR);
  }
  else{
    t.stop(evento_pisca_recorde);
  }
  //Serial.println(cont_pisca_recorde);
  
}




void atualizaRecorde(){
   recorde = tempo;
   quebraNumeroRecorde(recorde);
   digitalWrite(LATCH_C, LOW);
   shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_recorde[1]]);
   shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_recorde[0]]);
   digitalWrite(LATCH_C, HIGH);

   Serial.println(recorde);
}


void ligaDisplays(){

  //quebraNumero(recorde);
   digitalWrite(LATCH_C, LOW);
   shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_recorde[1]]);
   shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_recorde[0]]);
   digitalWrite(LATCH_C, HIGH);
   //quebraNumero(tempo);
   digitalWrite(LATCH_D, LOW); 
   shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_tempo[1]]);
   shiftOut(DATA, CLK, LSBFIRST, numbers[digitos_tempo[0]]);
   digitalWrite(LATCH_D, HIGH);
  
  
}


//separa o número em 4 digitos individuais
void quebraNumeroTempo(int num){
  
 
  dig1 = num / 10;
  digitos_tempo[0] = dig1;

  int dig2 = num - (dig1 * 10);
  digitos_tempo[1] = dig2;
}

//separa o número em 4 digitos individuais
void quebraNumeroRecorde(int num){
  
 
  dig1 = num / 10;
  digitos_recorde[0] = dig1;

  int dig2 = num - (dig1 * 10);
  digitos_recorde[1] = dig2;
}

void funcaoEspera(){

  if(saida_aux == 64){
    saida_aux = 1;
  }
  else{
    saida_aux *= 2; 
  }
  saida = saida_aux;

//    Serial.print(saida_aux);
//    Serial.print("  ");
//    Serial.println(saida, BIN);
  
}

void atualizaSaida(){
  digitalWrite(LATCH, LOW); //coloca o registrador em modo leitura
  shiftOut(DATA, CLK, LSBFIRST, saida); //envia os dados
  digitalWrite(LATCH, HIGH); //coloca o registrador em modo escrita  
}

void atualizaEntrada(){
  //escolhe a entrada que será lida
  digitalWrite(SELA, seletor_A[entrada_cont]);
  digitalWrite(SELB, seletor_B[entrada_cont]);
  digitalWrite(SELC, seletor_C[entrada_cont]);

  //Lê a entrada e armazena o valor no bit correspondente da variável saída.
  bool valor_entrada = digitalRead(ENTRADA);
  bitWrite(pinos, entrada_cont, valor_entrada); 

//  Serial.print(pinos, BIN);
//  Serial.print("  ");

  saida &= B01111111 & pinos;
  
  if(aux_entrada == 4){
    saida ^= pinos;//inverte os pinos que ainda estão em 1
    aux_entrada = 0;
  }
  else {
    aux_entrada++;
  }  
  
  if(bitRead(pinos, entrada_cont)!= bitRead(pinos_anterior, entrada_cont)){
      if(!bitRead(pinos, entrada_cont)){
        aux_buzzer = 0;
        saida ^= B10000000;
      }
      pinos_anterior = pinos;
      bitWrite(saida, entrada_cont, LOW);
  }

  
  if(aux_buzzer <= 3){
    saida ^= B10000000;
    aux_buzzer++;
  }
  
  
  Serial.print(pinos);
  Serial.print("  ");
  Serial.println(saida, BIN);    
  digitalWrite(SELA, seletor_A[ENTRADA_START]);
  digitalWrite(SELB, seletor_B[ENTRADA_START]);
  digitalWrite(SELC, seletor_C[ENTRADA_START]);
  if(digitalRead(ENTRADA) == ENTRADA_ON){
    estado = recolhe_pinos;
    timer_setado = false;
    t.stop(evento_timer_atualizaContador);
    t.stop(evento_timer_atualizaEntrada);
  }

  
  if (entrada_cont < 6) entrada_cont++;
  else entrada_cont = 0;

}

