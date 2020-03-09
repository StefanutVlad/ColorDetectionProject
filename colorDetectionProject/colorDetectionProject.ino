

//Pini Arduino
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define sensorOut 6
#define redLED 7
#define greenLED 8
#define blueLED 9


//Variabile frecv iesire de la senzor
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

//Variabile culori
int redColor = 0;
int greenColor = 0;
int blueColor = 0;

//Variabile de calibrare
int redMin;
int redMax;
int greenMin;
int greenMax;
int blueMin;
int blueMax;

//Variabila switch culori
int color = 0;

//Variabile sincronizare
long loopTime = 5000;   // 5secunde
unsigned long timer = 0;

void setup() {
 //Declarari
 pinMode(S0, OUTPUT);
 pinMode(S1, OUTPUT);
 pinMode(S2, OUTPUT);
 pinMode(S3, OUTPUT);
 pinMode(redLED, OUTPUT);
 pinMode(greenLED, OUTPUT);
 pinMode(blueLED, OUTPUT);
 pinMode(13, OUTPUT);
 pinMode(sensorOut, INPUT);
 
 //Setare scala frecventa 20%
 digitalWrite(S0, HIGH);
 digitalWrite(S1, LOW);

 //Start comunicare seriala 9600 baud rate
 Serial.begin(9600);

 timer = micros();

 //apelare functie calibrare senzor
 calibrate();
}

void loop() {
 readColor(); //citirea datelor de la senzor
 decideColor();//formatarea culorilor
 printColor();//afisare valori

 timeSync(loopTime); //sincronizare loop
 double val = (analogRead(0)-512)/512.0;
 printColor(); //afisare culoare
}



void timeSync(unsigned long deltaT){ //Functie sincronizare
  unsigned long currTime = micros();
  long timeToDelay = deltaT - (currTime - timer);
  if (timeToDelay > 5000) //daca delay prea mare => restrangere
  {
    delay(timeToDelay / 1000);
    delayMicroseconds(timeToDelay % 1000);
  }
  else if (timeToDelay > 0) //daca nu marire
  {
    delayMicroseconds(timeToDelay);
  }
  else
  {
      // timeToDelay negativ
  }
  timer = currTime + timeToDelay;
}


int value = 234;
int* intPointer = &value;
byte* bytePointer = (byte*)(intPointer);


void sendToPC()
{
  //byte* byteData = (byte*)(data);
 // Serial.write(printColor(),16);
}

void sendToPC(double* data)
{
  byte* byteData = (byte*)(data);
  Serial.write(byteData, 4);
}




void decideColor() {//Decidere valori formatare
 //Limita posibila a valorilor:
 redColor = constrain(redColor, 0, 255);
 greenColor = constrain(greenColor, 0, 255);
 blueColor = constrain(blueColor, 0, 255);

 //Gasire cea mai deschisa culoare:
 int maxVal = max(redColor, blueColor);
 maxVal = max(maxVal, greenColor);

 //Maparea valorilor
 redColor = map(redColor, 0, maxVal, 0, 255);
 greenColor = map(greenColor, 0, maxVal, 0, 255);
 blueColor = map(blueColor, 0, maxVal, 0, 255);
 redColor = constrain(redColor, 0, 255);
 greenColor = constrain(greenColor, 0, 255);
 blueColor = constrain(blueColor, 0, 255);

 //Culoarea led-ului
 analogWrite(redLED, redColor);
 analogWrite(greenLED, greenColor);
 analogWrite(blueLED, blueColor);
 
 //Switch culori
 if (redColor > 250 && greenColor > 250 && blueColor > 250) {
   color = 1;//alb
 }
 else if (redColor < 25 && greenColor < 25 && blueColor < 25) {
   color = 2;//negru
 }
 else if (redColor > 240 &&  greenColor > 245 && blueColor < 210) {
   color = 4;//galben
 }
 else if (redColor > 220 &&  greenColor > 25 /*&& blueColor < 100*/) {
   color = 3;//portocaliu
 }
 else if (redColor > 200 &&  greenColor < 100 && blueColor > 200) {
   color = 5;//mov
 }
 else if (redColor > 250 && greenColor < 200 && blueColor < 200) {
   color = 6;//rosu
 }
 else if (redColor < 215 && greenColor > 250 && blueColor < 210) {
   color = 7;//verde
 }
 else if (redColor < 200 /*&& greenColor < 200*/ && blueColor > 250) {
   color = 8;//albastru
 }
 else if(redColor<150 && greenColor>250 && blueColor>240){
  color = 9;//cyan
 }
 else {
   color = 0;//neidentificat
 }
  
  delay(300);  //Delay pentru rata de refresh monitor
}


void calibrate() { //Functie calibrare senzor
 Serial.println("Calibrare...");
 Serial.println("Alb:");//Initializare val alb
 
 //Setare valori calibrare:
 digitalWrite(13, HIGH);
 delay(2000);
 digitalWrite(S2, LOW);
 digitalWrite(S3, LOW);
 redMin = pulseIn(sensorOut, LOW);
 delay(100);
 digitalWrite(S2, HIGH);
 digitalWrite(S3, HIGH);
 greenMin = pulseIn(sensorOut, LOW);
 delay(100);
 digitalWrite(S2, LOW);
 digitalWrite(S3, HIGH);
 blueMin = pulseIn(sensorOut, LOW);
 delay(100);
 
 Serial.println("Urmatoarea...");//Initializare val negru
 digitalWrite(13, LOW);
 delay(2000);
 Serial.println("Negru:");

 //Setare valori calibrare:
 digitalWrite(13, HIGH);
 delay(2000);
 digitalWrite(S2, LOW);
 digitalWrite(S3, LOW);
 redMax = pulseIn(sensorOut, LOW);
 delay(100);
 digitalWrite(S2, HIGH);
 digitalWrite(S3, HIGH);
 greenMax = pulseIn(sensorOut, LOW);
 delay(100);
 digitalWrite(S2, LOW);
 digitalWrite(S3, HIGH);
 blueMax = pulseIn(sensorOut, LOW);
 delay(100);
 Serial.println("Senzor calibrat!");
 digitalWrite(13, LOW);
}

void printColor() { //Afisare culoare + lungime de unda
 Serial.print("R = ");
 Serial.print(redColor);
 Serial.print(" G = ");
 Serial.print(greenColor);
 Serial.print(" B = ");
 Serial.print(blueColor);
 Serial.print(" Color: ");
 
 switch (color) {
   case 1:Serial.println("ALB");break;
   case 2: Serial.println("NEGRU"); break;
   //case 3:{
  //  Serial.println("PORTOCALIU");
   // Serial.println("Lungimea de unda: 590-650nm");
   // break;
  // }
   case 4:{
    Serial.println("GALBEN"); 
    Serial.println("Lungimea de unda: 575-590nm");
    break;
   }
   case 5:{
    Serial.println("PURPLE"); 
    Serial.println("Lungimea de unda: 380-420nm");
    break;
   }
   case 6:{
    Serial.println("ROSU");
    Serial.println("Lungimea de unda: 590-650nm");
    break;
   }
   case 7:{
    Serial.println("VERDE"); 
    Serial.println("Lungimea de unda: 510-560nm");
    break;
   }
   case 8:{
    Serial.println("Albastru");
    Serial.println("Lungimea de unda: 452-470nm");
    break;
   }
   case 9:{
  Serial.println("Cyan");
  Serial.println("Lungimea de unda: 485-500nm");
  break;
 }
   default: Serial.println("Neidentificat"); break;
 }
 
}

void readColor(){ //Citire date senzor
 //Rosu
 digitalWrite(S2, LOW);
 digitalWrite(S3, LOW);
 redFrequency = pulseIn(sensorOut, LOW);
 redColor = map(redFrequency, redMin, redMax, 255, 0);
 delay(100);

 //Verde
 digitalWrite(S2, HIGH);
 digitalWrite(S3, HIGH);
 greenFrequency = pulseIn(sensorOut, LOW);
 greenColor = map(greenFrequency, greenMin, greenMax, 255, 0);
 delay(100);

 //Albastru
 digitalWrite(S2, LOW);
 digitalWrite(S3, HIGH);
 blueFrequency = pulseIn(sensorOut, LOW);
 blueColor = map(blueFrequency, blueMin, blueMax, 255, 0);
 delay(100);
}



void printData(void) //Afisare seriala
{
  Serial.print("Rosu= ");
  Serial.print(redColor);
  Serial.print("Verde= ");
  Serial.print(greenColor);
  Serial.print("Albastru= ");
  Serial.print(blueColor);
  Serial.print (" - ");
  Serial.print (color);
  Serial.println ("Detectat!");
}
