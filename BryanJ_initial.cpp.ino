
////////////////////////////////////////////////////////////////////////
////////////////////Author: Bryan Jiron/////////////////////////////////
////////////////////////////////////////////////////////////////////////



// UPDATES:
// 1.Create Loop to run from 0-14 m/ft to show the 
// serial monitor as a video for the presentation

#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
 
#define LED_PIN 8
#define TRIGGER_PIN 13  //Ultrasound Sensor
#define SONAR_ECHO_PIN 7

#define CAUTION_LED_COUNT     4  
#define DANGER_LED_COUNT      8  
#define SPEED_OF_SOUND_IPUS    0.0135039
#define DELAY_MS           0.010
#define RED            255
#define MAX_DIST       5
#define MED_DIST       3
#define STOP_SPEED      4
#define CAUTION_SPEED      1

LiquidCrystal Display(12, 11, 5, 4, 3, 2); // LCD interface pins

Adafruit_NeoPixel Caution_Pixels(CAUTION_LED_COUNT,LED_PIN,NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Danger_Pixels(DANGER_LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

float now,
      last_now=0,
      distance_inches,    
      speed_of_trailing_car,
      distance_ft_now,
      distance_traveled_ft,
      sonar_echo_duration,
      distance_ft_last=0,
      time_delta;

enum state {
   zero_state,
   caution_state,
   stop_state
};

state determined_state(distance_ft_now,speed_of_trailing_car)
 {

  if (distance_ft_now <= MAX_DIST)
  {  //Step 1 yes
    if(distance_ft_now <= MED_DIST)
    { //Step 2 yes
      if(speed_of_trailing_car >= STOP_SPEED)
      {
        return stop_state;//Step 3 yes
      }
      return zero_state;//Step 3 no
    }
    else //Step 2 no
    {
      if(speed_of_trailing_car > MAX_SPEED)
      {
        return caution_state;//Step 5 yes
      }
      return zero_state;//Step 5 no
    }
  }
  return zero_state; //Step 1 no
 }

void check_determined_state(distance,speed,expected_state){
  state got_state=determined_state(distance,speed);
  if (got_state==expected_state){
    Serial.println("OK");
  }
   else {
    Serial.println("BAD!");
  }
  
}

void power_on_self_test(){ //runs legitimate and illegitimate inputs
  check_determined_state(100,200,zero_state);
}

void setup() {
  Caution_Pixels.begin();
  Danger_Pixels.begin();  
  Display.begin(16, 1); //(col,rows)
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(SONAR_ECHO_PIN, INPUT);
  Serial.begin(9600);
  power_on_self_test();
  }

void loop() {
  now=millis(); 
  time_delta=now-last_now;  
  last_now=now;                                                         
//Reset Trigger
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(DELAY_MS);  //10us is required trigger input

//Compute Distance  WORKS
  sonar_echo_duration= pulseIn(SONAR_ECHO_PIN, HIGH); //in ms, 2way
  distance_ft_now=(sonar_echo_duration*SPEED_OF_SOUND_IPUS/2)/12;
  if (distance_ft_now> 14) return;

//Display in serial monitor what the distance and speed
  distance_traveled_ft= distance_ft_last-distance_ft_now;t
  speed_of_trailing_car = distance_traveled_ft/(time_delta/1000);
  state current_state=determined_state(distance_traveled_ft,speed_of_trailing_car);
  distance_ft_last=distance_ft_now;
  Serial.print("distance_traveled_ft: ");
  Serial.print(distance_traveled_ft);
  Serial.print("     speed_of_trailing_car: ");
  Serial.print(speed_of_trailing_car);
  Serial.println();
  Serial.print("Distance: ");
  Serial.println(distance_ft_now);

//IF statements

}