///////////////////////////////////////////////////////
/////////////////////This is a unit test///////////////
///////////////////////////////////////////////////////

#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
#include <BryanJ_initial.cpp.ino> 

#define LED_PIN 8
#define TRIGGER_PIN 13
#define SONAR_ECHO_PIN 7

#define CAUTION_LED_COUNT     4  
#define DANGER_LED_COUNT      8  
#define SPEED_OF_SOUND_IPUS    0.0135039
#define DELAY_MS           10
#define RED            255


struct {
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

} Car;

car.distance_ft = 0; 

Car car
int main(int arg, char* argv[]) {
test_case_1(); 
test_case_2(); 
test_case_3(); 
}



void test_case_1() {
car.distance_ft = 10; 
car.speed = 2; 
}

void test_case_2() {
car.distance_ft = 7; 
car.speed = 7; 
}

void test_case_3() {
car.distance_ft = 20
car.speed = 6; 


}


car.