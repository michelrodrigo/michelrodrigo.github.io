int pinos[] = {7, 6, 5};
int num_pinos = 7;
int timer = 100;
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

void setup() {

  int i;
  for(i = 0; i <= 2; i++)
    pinMode(pinos[i], OUTPUT);


  
}

void loop() {
  // put your main code here, to run repeatedly:
  acende_led();
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


