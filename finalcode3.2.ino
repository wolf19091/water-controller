#include <Time.h>
#include <Console.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);


volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 3; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;



float watertotal ;
int watercale ;
int autorest ;
int Time ;
int totalTime ;

int led = 6;
int led2 = 5;
int led1 = 4;
int value;
int conVal;


volatile int restButton = 1;
volatile int autoset = 2;
volatile int manuset = 3;

void flow () // Interrupt function
{
  flow_frequency++;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Bridge.begin();
  Console.begin();


  pinMode(flowsensor, INPUT);
  pinMode(led, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode (led2, OUTPUT);

  digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
  Serial.begin(9600);

  attachInterrupt(0, flow, RISING); // Setup Interrupt for sensor
  sei(); // Enable interrupts
  currentTime = millis();
  cloopTime = currentTime;

  attachInterrupt(1, interruptFunction, HIGH); //setup interrupt for rest push button
  attachInterrupt(2, interruptFunction1, HIGH );//setup interrupt for automatic push button
  attachInterrupt(3, interruptFunction2, HIGH  );//setup interrupt for manual push button

  //setTime(hr,min,sec,day,month,yr); 13:15:10 22/6/16
  setTime(00, 00, 00, 00, 00, 00);

  Serial.begin(1200);
  Serial.println("Serial Connected");
  
  lcd.begin(16, 2);
  lcd.setCursor (0, 0);
  lcd.print("Water controller");
  lcd.setCursor (0, 1);
  lcd.print("with self power");
  delay(5000);
  lcd.clear();
}

void loop() {

  conVal = Console.read();
  value = Serial.read();
  currentTime = millis();
  // Every second, calculate and print litres/hour
  if (currentTime >= (cloopTime + 1000))
  {
    cloopTime = currentTime; // Updates cloopTime
    // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
    l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
    flow_frequency = 0; // Reset Counter

    watercale += l_hour ;
    watertotal += (l_hour / 60) ;
    autorest += l_hour ;

    // show data in Serial monitor
    Serial.print(l_hour, DEC); // Print litres/hour
    Serial.print(" L/hour\t");
    Serial.print(hour());
    Serial.print(":");
    Serial.print(minute());
    Serial.print(":");
    Serial.print(second());
    Serial.print(" \t");
    Serial.print (watertotal, DEC);
    Serial.print(" Total Water \t");
    Serial.print(hour());
    Serial.print(":");
    Serial.print(minute());
    Serial.print(":");
    Serial.print(second());
    Serial.print("");
    Serial.println(" Total Time");


    //show  data in console monitor
    /*  Console.print(l_hour, DEC);
      Console.print(" L/hour\t");
      Console.print(Time++, DEC);
      Console.print(" Sec.\t");
      Console.print (watertotal, DEC);
      Console.print(" Total Water \t");
      Console.print(totalTime++, DEC);
      Console.println(" Total Time");
    */
    Console.print(l_hour, DEC); // Print litres/hour
    Console.print(" L/hour\t");
    Console.print(hour());
    Console.print(":");
    Console.print(minute());
    Console.print(":");
    Console.print(second());
    Console.print(" \t");
    Console.print (watertotal, DEC);
    Console.print(" Total Water \t");
    Console.print(hour());
    Console.print(":");
    Console.print(minute());
    Console.print(":");
    Console.print(second());
    Console.print("");
    Console.println(" Total Time");

    //output title for lcd
    lcd.setCursor(0, 0);
    lcd.print("TIME : ");
    lcd.print(hour());
    lcd.print(":");
    lcd.print(minute());
    lcd.print(":");
    lcd.print(second());
    lcd.print("");

    lcd.setCursor(0, 1);
    lcd.print("VOLUM : ");
    lcd.print(watertotal);
    lcd.print(" L");
  }

  if (manuset)//Manual program
  {
    digitalWrite(led, HIGH);
    //digitalRead(autoset,HIGH)
    digitalWrite(led2, LOW);
    delay(1000);
    conVal = Console.read();
    value = Serial.read();

    if (value == '3' || conVal == '3')
    {
      digitalWrite(led1, HIGH);
      Serial.println("led is on");
      Console.println("led is on");
    }
    else if (value == '4' || conVal == '4') {
      digitalWrite(led1, LOW);
      Serial.println("led is off");
      Console.println("led is off");
    }
    Serial.print("Manual \t");
    Console.print("Manual \t");

  }
  else if (autoset)//Automatic
  {
    digitalWrite(led2, HIGH);
    //digitalRead(autoset,HIGH)
    digitalWrite(led, LOW);
    delay(1000);

    if (autorest >= 2000)//after 2000 litrs auto close
    {
      autorest = 0;
      digitalWrite(led1, HIGH);
      delay(2000);

      digitalWrite(led1, LOW);

    }

    Serial.print("Auto\t");
    Console.print("Auto\t");
  }


}
void interruptFunction()//function for rest push button
{
  autorest = 0;
  l_hour = 0;
  Time = 0;
  watertotal = 0;
  watercale = 0;
  setTime(00, 00, 00, 00, 00, 00);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Resetting...");
  delay(3000);

  lcd.clear();
}

void interruptFunction1()//function for active auto mode
{
  autoset;

}
void interruptFunction2()//function for active manual mode
{
  manuset;

}

