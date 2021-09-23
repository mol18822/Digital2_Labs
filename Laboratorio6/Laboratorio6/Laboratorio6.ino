// Universidad del Valle de Guatemala
// Electrónica Digital II
// Laboratorio No. 6 - Juego de carrera - Tiva C + Energia
// Luis Pedro Molina - 18822

// Declaración de leds de semáforo y push bottons

#define RED PF_1
#define BLUE PF_2
#define GREEN PF_3
#define PB1 PF_4
#define PB2 PF_0

// Declaración de leds de jugador No. 1

#define LED1 PD_0
#define LED2 PD_1
#define LED3 PD_2
#define LED4 PD_3
#define LED5 PE_1
#define LED6 PE_2
#define LED7 PE_3
#define LED8 PA_7

// Declaración de leds de jugador No. 2

#define LED9  PB_3
#define LED10 PC_4
#define LED11 PC_5
#define LED12 PC_6
#define LED13 PC_7
#define LED14 PD_6
#define LED15 PD_7
#define LED16 PA_2

// Declaración de variables

int statusP1   = 0;
int statusP2   = 0;
int counterP1  = 0;
int counterP2  = 0;
int PB_P1;
int PB_P2;

void setup() {

// Declaración de pines como outputs

  pinMode(BLUE,  OUTPUT);
  pinMode(RED,   OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(LED1,  OUTPUT);
  pinMode(LED2,  OUTPUT);
  pinMode(LED3,  OUTPUT);
  pinMode(LED4,  OUTPUT);
  pinMode(LED5,  OUTPUT);
  pinMode(LED6,  OUTPUT);
  pinMode(LED7,  OUTPUT);
  pinMode(LED8,  OUTPUT);
  pinMode(LED9,  OUTPUT);
  pinMode(LED10, OUTPUT);
  pinMode(LED11, OUTPUT);
  pinMode(LED12, OUTPUT);
  pinMode(LED13, OUTPUT);
  pinMode(LED14, OUTPUT);
  pinMode(LED15, OUTPUT);
  pinMode(LED16, OUTPUT);
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(PUSH2, INPUT_PULLUP);
}

void loop() {

// Lectura de Push bottons
  
  statusP1 = digitalRead(PB1);
  statusP2 = digitalRead(PB2);

// Antirrebote para Push bottons
  
  if (statusP1 == HIGH){
    PB_P1 = 1;
  }
  if (statusP2 == HIGH){
    PB_P2 = 1;
  }

// Rutina de semáforo para comenzar juego

  if (counterP1 == 1 || counterP2 == 1){
    digitalWrite(RED, HIGH);
    delay(1500);
    digitalWrite(GREEN, HIGH);
    delay(1500);
    digitalWrite(RED, LOW);
    delay(1500);
    digitalWrite(GREEN, LOW);
    counterP1 = 2;
    counterP2 = 2;
  }
    
  if (statusP1 == LOW && PB_P1 == 1){
    counterP1 = counterP1 + 1;
    PB_P1 = 0;
  }

  if (statusP2 == LOW && PB_P2 == 1){
    counterP2 = counterP2 + 1;
    PB_P2 = 0;
  }

// Rutina de Jugador No. 1

  if (counterP1 == 3){
    digitalWrite(LED1, HIGH);
    //delay(250);
  }
  if(counterP1 == 4){
    digitalWrite(LED2, HIGH);
    digitalWrite(LED1, LOW);
    delay(100);
  }
  if(counterP1 == 5){
    digitalWrite(LED3, HIGH);
    digitalWrite(LED2, LOW);
    delay(100);
  }
  if(counterP1 == 6){
    digitalWrite(LED4, HIGH);
    digitalWrite(LED3, LOW);
    delay(100);
  }
  if(counterP1 == 7){
    digitalWrite(LED5, HIGH);
    digitalWrite(LED4, LOW);
    delay(100);
  }
  if(counterP1 == 8){
    digitalWrite(LED6, HIGH);
    digitalWrite(LED5, LOW);
    delay(100);
  }
  if(counterP1 == 9){
    digitalWrite(LED7, HIGH);
    digitalWrite(LED6, LOW);
    delay(100);
  }
  if(counterP1 == 10){
    digitalWrite(LED8, HIGH);
    digitalWrite(LED7, LOW);
    delay(100);
  }
  if(counterP1 == 11){
    counterP1 = 0;
    digitalWrite(LED8, LOW);
    delay(100);
  }

// Rutina de Jugador No. 2

  if (counterP2 == 3){
    digitalWrite(LED9, HIGH);
    //delay(250);
  }
  if(counterP2 == 4){
    digitalWrite(LED10, HIGH);
    digitalWrite(LED9, LOW);
    delay(100);
  }
  if(counterP2 == 5){
    digitalWrite(LED11, HIGH);
    digitalWrite(LED10, LOW);
    delay(100);
  }
  if(counterP2 == 6){
    digitalWrite(LED12, HIGH);
    digitalWrite(LED11, LOW);
    delay(100);
  }
  if(counterP2 == 7){
    digitalWrite(LED13, HIGH);
    digitalWrite(LED12, LOW);
    delay(100);
  }
  if(counterP2 == 8){
    digitalWrite(LED14, HIGH);
    digitalWrite(LED13, LOW);
    delay(100);
  }
  if(counterP2 == 9){
    digitalWrite(LED15, HIGH);
    digitalWrite(LED14, LOW);
    delay(100);
  }
  if(counterP2 == 10){
    digitalWrite(LED16, HIGH);
    digitalWrite(LED15, LOW);
    delay(100);
  }
  if(counterP2 == 11){
    counterP1 = 0;
    digitalWrite(LED16, LOW);
    delay(100);
  }

// Rutinas para determinar al jugador ganador

// Rutina de jugador No. 1 como ganador 

  if (counterP1 == 10){ 
    digitalWrite(BLUE, HIGH);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);
    digitalWrite(LED6, LOW);
    digitalWrite(LED7, LOW);
    digitalWrite(LED8, LOW);
    digitalWrite(LED9, LOW);
    digitalWrite(LED10, LOW);
    digitalWrite(LED11, LOW);
    digitalWrite(LED12, LOW);
    digitalWrite(LED13, LOW);
    digitalWrite(LED14, LOW);
    digitalWrite(LED15, LOW);
    digitalWrite(LED16, LOW);
    counterP1 = 0;
    counterP2 = 0;
    delay(1000);
    digitalWrite(BLUE, LOW);
  }

// Rutina de jugador No. 2 como ganador  
 
  if (counterP2 == 10){  
    digitalWrite(GREEN, HIGH);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);
    digitalWrite(LED6, LOW);
    digitalWrite(LED7, LOW);
    digitalWrite(LED8, LOW);
    digitalWrite(LED9, LOW);
    digitalWrite(LED10, LOW);
    digitalWrite(LED11, LOW);
    digitalWrite(LED12, LOW);
    digitalWrite(LED13, LOW);
    digitalWrite(LED14, LOW);
    digitalWrite(LED15, LOW);
    digitalWrite(LED16, LOW);
    counterP1 = 0;
    counterP2 = 0;
    delay(1000);
    digitalWrite(GREEN, LOW);
  }
  
}
