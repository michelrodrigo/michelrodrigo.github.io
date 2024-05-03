

int timer_event = 0;
int pinos[] = {7, 6, 5};
int pinosIn[] = {A3, A4, A5};
int pinEntrada = 3;
int entrada[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int num_pinos = 7;
int timer = 100;
int controle =2;
int mapa_led[][3] = 
{
  {0, 0, 0}, 
  {0, 0, 1},
  {0, 1, 0},
  {0, 1, 1},
  {1, 0, 0},
  {1, 0, 1},
  {1, 1, 0}
};
//byte numbers[8] {B00000000, B01100000, B11011010, B11110010, B01100110, B10110110, B10111110, B11100000, B11111110, B11110110};
int mapa_entrada[][3] = 
{
  {0, 0, 0}, 
  {0, 0, 1},
  {0, 1, 0},
  {0, 1, 1},
  {1, 0, 0},
  {1, 0, 1},
  {1, 1, 0},
  {1, 1, 1}
};


void setup() {

  int i;
  for(i = 0; i <= 2; i++)
    pinMode(pinos[i], OUTPUT);

  for(i = 0; i <= 2; i++)
    pinMode(pinosIn[i], OUTPUT);

  pinMode(pinEntrada, INPUT);

  pinMode(controle, OUTPUT);

  Serial.begin(9600);

  //timer_event = t.every(100, le_entrada);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  le_entrada();
  imprime_entrada();

//  for(int i = 0; i <= 7; i++){
//    if(entrada[i] == 1){
//      set_led(i);
//      delayMicroseconds(10);
//    }
//    else{
//      apaga_led();
//    }
//  }
  //acende_led();
}

void acende_led(){
  int i, j;

//  for(j = 0; j <= 2; j++){
//    for (i = 0; i < num_pinos; i++) { // varrer cada pino...
//      set_led(i);
//      delay(timer);
//    }
//    for (i = num_pinos - 1; i >= 0; i--) { 
//     set_led(i);
//     delay(timer);
//    }
//  }
//  
//  for(i = 0; i <= 10; i++){
//    int pino_aleatorio = random(num_pinos);
//    set_led(pino_aleatorio);
//    delay(timer);
//  }

  for(j = 0; j <= 5; j++){
    for(i = 0; i <= 6*1000; i++){
      set_led(i%7);
      delayMicroseconds(50);
    }
    apaga_led();
    delay(timer+500);
  }
}

void apaga_led(){
  for(int i = 0; i <= 2; i++){
    digitalWrite(pinos[i], 1);
  }
}

void set_led(int led){
  //Serial.print(led);
  for(int i = 0; i <= 2; i++){
    digitalWrite(pinos[i], mapa_led[led][i]);
  }
}

void le_entrada(){

 for(int i = 0; i <= 7; i++){
  for(int j = 0; j <= 2; j++){
    digitalWrite(pinosIn[j], mapa_entrada[i][j]);
    //Serial.println(j);
  }
      digitalWrite(controle, LOW);
      entrada[i] = digitalRead(pinEntrada);
      digitalWrite(controle, HIGH);
      //delayMicroseconds(2);
 }
 
}

void set_entrada(int key_entrada){

  for(int i = 0; i <= 2; i++){
    digitalWrite(pinosIn[i], mapa_entrada[key_entrada][i]);
  }
}

void imprime_entrada(){
  for(int i = 0; i <= 7; i++){
    Serial.print(entrada[i]);
    Serial.print(" ");
  }
  Serial.println(" ");
}

