/*
  FireplaceBacklight.cpp - Library for powering neopixel fireplace backlight
  Created by Tony Gaitatzis, February 8, 2015.
  Released into the public domain.
*/
#include "FireplaceBacklight.h"

FireplaceBacklight::FireplaceBacklight(unsigned char neopixelPin, unsigned int numNeopixels) {
  this->_neopixelPin = neopixelPin;
  
  // initialize _pixels and _oldPixels  
  this->_pixels = (Pixel*) malloc (numNeopixels*sizeof(Pixel));
  this->_oldPixels = (Pixel*) malloc (numNeopixels*sizeof(Pixel));
  
  // make room for the neopixels
  this->_pixelStrip = (Adafruit_NeoPixel*)malloc(sizeof(Adafruit_NeoPixel*));
        
  this->_pixelStrip = new Adafruit_NeoPixel((uint16_t) numNeopixels, (uint8_t) this->_neopixelPin, NEO_GRB + NEO_KHZ800);
  this->_pixelStrip->begin();
  
  this->_initialize();
}

FireplaceBacklight::~FireplaceBacklight() {
  free(this->_pixelStrip);
  this->_pixelStrip = NULL;
  free(this->_pixels);
  this->_pixels = NULL;
  free(this->_oldPixels);
  this->_oldPixels = NULL;
}

void FireplaceBacklight::_initialize() {
  this->_COLOR_RANGE.min.h = this->__min_hsv_color_h * 360;
  this->_COLOR_RANGE.min.s = this->__min_hsv_color_s;
  this->_COLOR_RANGE.min.v = this->__min_hsv_color_h;
  
  this->_COLOR_RANGE.max.h = this->__max_hsv_color_h * 360;
  this->_COLOR_RANGE.max.s = this->__max_hsv_color_s;
  this->_COLOR_RANGE.max.v = this->__max_hsv_color_v;
  
  for (unsigned int index = 0; index < this->_pixelStrip->numPixels(); index++) {
    this->_pixels[index].timeout = 0;
    this->_oldPixels[index].timeout = 0;
  }
  
}




hsv FireplaceBacklight::_makeRandomHSVColor() {
  hsv hsvColor;
  hsvColor.h = this->_makeRandomValue(this->_COLOR_RANGE.min.h, this->_COLOR_RANGE.max.h);
  hsvColor.s = this->_makeRandomValue(this->_COLOR_RANGE.min.s, this->_COLOR_RANGE.max.s);
  hsvColor.v = this->_makeRandomValue(this->_COLOR_RANGE.min.v, this->_COLOR_RANGE.max.v);
 
  return hsvColor;
}

float FireplaceBacklight::_makeRandomValue(float min, float max) {
  float multiplier = 1000;
  float retval = 0;
  retval = max-min;
  retval = floor(retval * multiplier);
  retval = float(rand() % (int)retval);
  retval = retval + (multiplier*min);
  retval = retval/multiplier;
  
  return retval;
}

hsv FireplaceBacklight::_tweenHsvColor(hsv &startColor, hsv &endColor, float percent) {
  hsv hsvColor;
  hsvColor.h = this->_tweenValue(startColor.h, endColor.h, percent);
  hsvColor.s = this->_tweenValue(startColor.s, endColor.s, percent);
  hsvColor.v = this->_tweenValue(startColor.v, endColor.v, percent);
        
  return hsvColor;
}

float FireplaceBacklight::_tweenValue(float previousValue, float nextValue, float percent) {
  float tween = (nextValue-previousValue)*percent + previousValue; 
  return tween;
}



int FireplaceBacklight::_makeRandomTimeout(int min, int max) {
  return (rand() % (max-min)) + min;
}

int FireplaceBacklight::_randomizePixel(Pixel &pixel) {
  long currentTime = millis();
  hsv hsvColor = this->_makeRandomHSVColor();
    
  pixel.hsvColor.h = hsvColor.h;
  pixel.hsvColor.s = hsvColor.s;
  pixel.hsvColor.v = hsvColor.v;
  pixel.timeout = currentTime + this->_makeRandomTimeout(this->_TIME_INTERVAL_MIN_MS, this->_TIME_INTERVAL_MAX_MS);
	
}

void FireplaceBacklight::_copyPixel(Pixel &fromPixel, Pixel &toPixel) {
  toPixel.hsvColor.h = fromPixel.hsvColor.h;
  toPixel.hsvColor.s = fromPixel.hsvColor.s;
  toPixel.hsvColor.v = fromPixel.hsvColor.v;
  toPixel.timeout = fromPixel.timeout;
}


void FireplaceBacklight::updateFrame() {
  unsigned long currentTime = millis();
  
  unsigned long timeOut;
  unsigned long timeScale;
  float percentTween;
  for (unsigned int index = 0; index < this->_pixelStrip->numPixels(); index++) {
    timeOut = this->_pixels[index].timeout - currentTime;
    
    if (timeOut > 0) { // tween this pixel if the timeout has not expired
      timeScale = this->_pixels[index].timeout - this->_oldPixels[index].timeout;
      
      if (timeScale <= 0) {
        percentTween = 1.0;
      } else {
        percentTween =  ((float)currentTime - (float)this->_oldPixels[index].timeout) / timeScale;
      }
      
      hsv hsvColor = this->_tweenHsvColor(this->_oldPixels[index].hsvColor, this->_pixels[index].hsvColor, percentTween);
      
      
      rgb rgbColor = ColorConverter::hsv2rgb(hsvColor);
      
      this->_pixelStrip->setPixelColor(index, this->_pixelStrip->Color(rgbColor.r*255, rgbColor.g*255, rgbColor.b*255));
      
    } else { // time to make a new pattern for this pixel
      //Serial.println('timeout expired');
      // copy current pixel over to old pixel
      this->_copyPixel(this->_pixels[index], this->_oldPixels[index]);
      
      this->_randomizePixel(this->_pixels[index]);
    }
  }
  this->_pixelStrip->show();
}
