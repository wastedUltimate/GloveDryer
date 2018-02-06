#include <SimpleDHT.h>

const int gloveDhtPin = 2;
const int controlDhtPin = 4;
SimpleDHT11 dht11;

const int buttonPin = 7;
int buttonState = 0;
const int ledPin = 13;
int runBlower = LOW;

const long sampleRate = 1000 * 2;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  //checkButton
  int previousState = buttonState;
  buttonState = digitalRead(buttonPin);
  boolean buttonPushed = buttonState == LOW && previousState == HIGH;
  if(buttonPushed){
    toggleLed();
  }

  unsigned long currentMillis = millis();
  boolean wait = (currentMillis - previousMillis < sampleRate);
  if(runBlower && !wait){
    previousMillis = currentMillis;
    checkAll();
   }
  
}

void toggleLed(){
  previousMillis = millis();
  runBlower = !runBlower;
  Serial.println(runBlower? "Begin Blowing": "Blow Job Complete");
  if(runBlower) {
    checkAll();
  }
  digitalWrite(ledPin, runBlower);
}

void checkAll(){
  checkDht(gloveDhtPin);
  checkDht(controlDhtPin);
  Serial.println(previousMillis);
}

void checkDht(int dhtPin){
  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  
  dht11.read(dhtPin, &temperature, &humidity, data);

  printOutput(dhtPin, temperature, humidity);
}

void printOutput(int dhtPin, byte temp, byte humidity){
  Serial.print(dhtPin == gloveDhtPin ? "glove" : "control");
  Serial.print(", ");
  Serial.print((int)temp);
  Serial.print(", ");
  Serial.print((int)humidity);
  Serial.print(", ");
}

