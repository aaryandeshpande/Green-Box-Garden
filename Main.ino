#include "Adafruit_seesaw.h"

Adafruit_seesaw ss;

const int moisture_threshold = 550;
int resistance;
int capacitive_reading;
int brightness;
unsigned long zeroMillis = 0;
unsigned long on_off = millis();
unsigned long time_on = 64800000;
unsigned long time_off = 21600000; 
unsigned long time_between = 900000; 

void setup() {
  Serial.begin(115200);

  pinMode(5,OUTPUT);
  pinMode(13,OUTPUT);

  Serial.println("Welcome to your smart garden!");
  
  if (!ss.begin(0x36)) {
    Serial.println("ERROR!");
    while(1) delay(1);
  }
}

// We control our pump using lines 31-37. The pump is turned on for 3 seconds when the criteria is met and turned off after 3 seconds to prevent flooding.
void pump_loop() {
  if(capacitive_reading < moisture_threshold){
    digitalWrite(13,HIGH);
    delay(3000); //6000
    digitalWrite(13,LOW);
  }  
}
  
// Lines 40-44 control the intensity of the light during the 18 hour lighting period based on the photoresistor's readings.
void light_loop() {
  brightness = map(resistance,50,1100,255,0);
  analogWrite(5,brightness);
  Serial.print("brightness: "); Serial.println(brightness);
}

// Lines 47-50 call upon the LEDs to be turned off during the 6 hour darkness period, which as a result, delays the entire program by 6 hours to conserve energy.
void light_off() {
  analogWrite(5,0);
  delay(5000); //time_off
}

// This is our main loop that will run every 15 minutes to check for water and lighting levels
void loop() {
  capacitive_reading = ss.touchRead(0);
  Serial.print("moisture: "); Serial.println(capacitive_reading);
  pump_loop();

  resistance = analogRead(A0);
  Serial.print("resistance: "); Serial.println(resistance);
  light_loop();
  
  unsigned long on_off = millis();
  if ((unsigned long)(on_off - zeroMillis) >= 30000){ //time_off
    light_off();
    zeroMillis = on_off;
  }

  Serial.print("millis: "); Serial.println(on_off); 

  delay(1000); //time_between
}