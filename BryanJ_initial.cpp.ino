
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
 
#define LED_PIN 8
#define TRIGGER_PIN 13  //Ultrasound Sensor
#define SONAR_ECHO_PIN 7

#define ZERO_LED_COUNT     8
#define CAUTION_LED_COUNT     4  
#define DANGER_LED_COUNT      8  
#define SPEED_OF_SOUND_IPUS    0.0135039
#define DELAY_MS           0.010
#define LED_OFF 0
#define RED            255
#define MAX_DIST       5
#define MED_DIST       3
#define STOP_SPEED      4
#define CAUTION_SPEED      1

LiquidCrystal Display(12, 11, 5, 4, 3, 2); // LCD interface pins

Adafruit_NeoPixel Zero_Pixels(ZERO_LED_COUNT,LED_PIN,NEO_GRB + NEO_KHZ800);
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

state determine_state(distance_ft_now,speed_of_trailing_car)
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
      if(speed_of_trailing_car > CAUTION_SPEED)
      {
        return caution_state;//Step 5 yes
      }
      return zero_state;//Step 5 no
    }
  }
  return zero_state; //Step 1 no
 }

void check_determine_state(distance,speed,expected_state){
  state got_state=determine_state(distance,speed);
  Serial.print(distance,speed,expected_state,":");
  if (got_state==expected_state){
    Serial.println("OK outcome");
  }
   else {
    Serial.println("BAD!");
  }
  
}

void power_on_self_test(){ 
//Legitimate inputs
  check_determine_state(100,200,zero_state);
  check_determine_state(MAX_DIST+100,-1,zero_state);
  check_determine_state(MAX_DIST+100,200,zero_state);
  check_determine_state(MAX_DIST+100,CAUTION_SPEED,zero_state);
  check_determine_state(MAX_DIST+100,STOP_SPEED,zero_state);
  check_determine_state(0,0,stop_state); 
  check_determine_state(0,STOP_SPEED,zero_state);
  check_determine_state(0,STOP_SPEED+100,zero_state);
//Illegitimate inputs
  check_determine_state(MAX_DIST,CAUTION_SPEED,caution_state);
  check_determine_state(-1,CAUTION_SPEED*200,zero_state);
  check_determine_state(-1,CAUTION_SPEED+100,zero_state);
  check_determine_state(MAX_DIST,CAUTION_SPEED*-1000,zero_state);
  check_determine_state(MAX_DIST+100,-1,zero_state);
  check_determine_state(-1,CAUTION_SPEED*-1000,caution_state);
  check_determine_state(-1,CAUTION_SPEED*200,caution_state);
  check_determine_state(MAX_DIST,CAUTION_SPEED*200,caution_state);
  check_determine_state(-1,CAUTION_SPEED*-1000,stop_state);
  check_determine_state(-1,CAUTION_SPEED*200,stop_state);
  check_determine_state(MAX_DIST,CAUTION_SPEED*200,stop_state);
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

//Compute Distance
  sonar_echo_duration= pulseIn(SONAR_ECHO_PIN, HIGH); //in ms, 2way
  distance_ft_now=(sonar_echo_duration*SPEED_OF_SOUND_IPUS/2)/12;
  if (distance_ft_now> 14) return;

//Display in serial monitor what the distance and speed
  distance_traveled_ft= distance_ft_last-distance_ft_now;
  distance_ft_last=distance_ft_now;
  speed_of_trailing_car = distance_traveled_ft/(time_delta/1000);
  state current_state=determine_state(distance_traveled_ft,speed_of_trailing_car);
  act_on_state(current_state); 
}
  void act_on_state (state current_state) 
{ 
  
//IF statements

 if (current_state == caution_state){
 for (int i=0; i < CAUTION_LED_COUNT; i++) {
   // Caution_pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
   Caution_Pixels.setPixelColor(i, Caution_Pixels.Color(RED, 0, 0));
   // This sends the updated pixel color to the hardware.
   Caution_Pixels.show();
   // Delay for a period of time (in milliseconds).
   delay(DELAY_MS);
 }
Display.print("    !CAUTION!     ");
return;
 }
 if (current_state == stop_state){
 for (int i=0; i < DANGER_LED_COUNT; i++) {
   // Danger_pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
   Danger_Pixels.setPixelColor(i, Danger_Pixels.Color(RED, 0, 0));

   // This sends the updated pixel color to the hardware.
   Danger_Pixels.show();

   // Delay for a period of time (in milliseconds).
   delay(DELAY_MS);
 }
Display.print("    !!STOP!!     ");
return;
 }
if (current_state == zero_state)
{
for (int i=0; i < ZERO_LED_COUNT; i++) {
   Zero_Pixels.setPixelColor(i, Zero_Pixels.Color(LED_OFF, 0, 0));

   // This sends the updated pixel color to the hardware.
   Zero_Pixels.show();

   // Delay for a period of time (in milliseconds).
   delay(DELAY_MS);
 }
 Display.print("                ");
return;

}
}
