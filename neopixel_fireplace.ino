#include "FireplaceBacklight.h"
#include <Adafruit_NeoPixel.h>
#include <MemoryFree.h>

//#define __BOARD__ ATTINY
//#define __BOARD__ ARDUINO

//#if (__BOARD__ == ATTINY)
#define PWM_PIN 0
//#else
//#define PWM_PIN 3
//#endif

/**
 * System configuration
 */

const byte POWER_LED_PIN = 13;

/**
 *  Neopixel Configuration
 */
// each pixel consumes 2*HSVt+3 = 2*(4+4+4)+3 = 27 bytes of data.
// if we reformat, we can consume 2*(1+1+2)+3 = 9 bytes of data
// the ATTiny has 512 bytes of RAM.  MAX: ~ 18@27b - 56@9b pixels
// the Arduino has 2048 bytes of ram.  MAX: ~ 7527b - 227@9b pixels
const unsigned int LED_COUNT = 12; // number of LED pins, 144
const byte NEOPIXEL_PIN = PWM_PIN; // PWM-compatible output pin

/**
 * Animation configuration
 */
const unsigned int FRAMERATE_MS = 16; // 16 ms = 60 fps


/**
 * Internal state
 */
unsigned long frameStartTime_ms;
FireplaceBacklight fireplaceBacklight = FireplaceBacklight(NEOPIXEL_PIN, LED_COUNT);


void setup() {
  //Serial.begin(9600);
  pinMode(POWER_LED_PIN, OUTPUT);
  digitalWrite(POWER_LED_PIN, HIGH);
	
  frameStartTime_ms = millis();
}

void loop() {
  // update the fireplace when the frame expires
  
  long timeDelay = millis() - frameStartTime_ms;
  if (timeDelay > FRAMERATE_MS) {
  	fireplaceBacklight.updateFrame();
	frameStartTime_ms = millis();
  }  
  
  
  //Serial.print("freeMemory()=");
  //Serial.println(freeMemory());
  
}



