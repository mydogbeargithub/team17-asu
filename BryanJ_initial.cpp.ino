
////////////////////////////////////////////////////////////////////////
////////////////////Author: Bryan Jiron/////////////////////////////////
////////////////////////////////////////////////////////////////////////



// UPDATES:
// 1.Create Loop to run from 0-14 m/ft to show the 
// serial monitor as a video for the presentation

#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
 
#define LED_PIN 8
#define TRIGGER_PIN 13
#define SONAR_ECHO_PIN 7

#define CAUTION_LED_COUNT     4  
#define DANGER_LED_COUNT      8  
#define SPEED_OF_SOUND_IPUS    0.0135039
#define DELAY_MS           10
#define RED            255

LiquidCrystal Display(12, 11, 5, 4, 3, 2); // LCD interface pins

Adafruit_NeoPixel Caution_Pixels = Adafruit_NeoPixel(
  CAUTION_LED_COUNT,
  LED_PIN,
  NEO_GRB + NEO_KHZ800
  );
Adafruit_NeoPixel Danger_Pixels = Adafruit_NeoPixel(
  DANGER_LED_COUNT, 
  LED_PIN, 
  NEO_GRB + NEO_KHZ800
  );
long  now,
      last_now,
      time_traveled_in_ms,
      speed_of_trailing_car,
      last_distance_ft,
      distance_traveled_ft,
      sonar_echo_duration;

float distance_inches,
      speed_fps,    
      distance_ft;


int testbenchVAR=1; // 3 states 0=off, 1=Caution, 2=STOP

void setup() {
  Caution_Pixels.begin();
  Danger_Pixels.begin();  
  Display.begin(16, 1); //(col,rows)
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(SONAR_ECHO_PIN, INPUT);
  Serial.begin(9600);
  }

void loop() {
  
//Reset Trigger
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(DELAY_MS);  //10us is required trigger input

////////////////////////////////////////////////////////////////////////
////////////////////First compute the distance//////////////////////////
////////////////////////////////////////////////////////////////////////


//Compute Distance
  sonar_echo_duration= pulseIn(SONAR_ECHO_PIN, HIGH); //in ms, 2way
  distance_inches=sonar_echo_duration*SPEED_OF_SOUND_IPUS/2;
  distance_ft = distance_inches/12;
  if (distance_ft > 14) return;


//Compute Speed
  now = millis();
  last_now = now;
  time_traveled_in_ms = now - last_now;
  
  last_distance_ft = distance_ft;
  distance_traveled_ft = last_distance_ft - distance_ft;
  if (distance_traveled_ft < 0) return;


  speed_fps = distance_traveled_ft / (time_traveled_in_ms / 1000);

//Display in serial monitor what the distance and speed
  Serial.print("Distance: ");
  Serial.println(distance_ft);
  Serial.print("Speed: ");
  Serial.println(speed_of_trailing_car);
  
////////////////////////////////////////////////////////////////////////
///////////////////////////Decision Diamonds////////////////////////////
////////////////////////////////////////////////////////////////////////

//IF statements
  if (distance_ft <= 6 && speed_fps>0){
  for (int i=0; i < CAUTION_LED_COUNT; i++) {
    // Caution_pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    Caution_Pixels.setPixelColor(i, Caution_Pixels.Color(RED, 0, 0));
    // This sends the updated pixel color to the hardware.
    Caution_Pixels.show();
    // Delay for a period of time (in milliseconds).
    delay(DELAY_MS);
    Display.print("    !CAUTION!     ");
    }
  }

  if (distance_ft >= 14 && distance_ft<=14 && speed_fps>5){
    for (int i=0; i < DANGER_LED_COUNT; i++) {
    // Danger_pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    Danger_Pixels.setPixelColor(i, Danger_Pixels.Color(RED, 0, 0));

    // This sends the updated pixel color to the hardware.
    Danger_Pixels.show();

    // Delay for a period of time (in milliseconds).
    delay(DELAY_MS);
    Display.print("    !!STOP!!     ");
    }
  }
  
}
