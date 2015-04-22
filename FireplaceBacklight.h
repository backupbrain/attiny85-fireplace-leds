/*
  FireplaceBacklight.h - Library for powering neopixel fireplace backlight
  Created by Tony Gaitatzis, February 8, 2015.
  Released into the public domain.  
*/

#ifndef FireplaceBacklight_h
#define FireplaceBacklight_h

#include "Arduino.h"
#include "ColorConverter.h"
#include <Adafruit_NeoPixel.h>

struct Pixel {
  hsv hsvColor;
  long timeout;
};

struct HsvColorRange {
  hsv min;
  hsv max;
};

class FireplaceBacklight
{
  public:
    FireplaceBacklight(unsigned char neopixelPin, unsigned int numNeopixels);
    ~FireplaceBacklight();
    void updateFrame();
  private:
    static const float __min_hsv_color_h = 0.0;
    static const float __min_hsv_color_s = 0.75;
    static const float __min_hsv_color_v = 0.0;
    static const float __max_hsv_color_h = 0.15;
    static const float __max_hsv_color_s = 1.0;
    static const float __max_hsv_color_v = 0.3;
  
    static const unsigned int _TIME_INTERVAL_MIN_MS = 500;  // minimum time between new pixels
    static const unsigned int _TIME_INTERVAL_MAX_MS = 1750; // maximum time between new pixels
    
    HsvColorRange _COLOR_RANGE;
    unsigned char _neopixelPin; 
    Pixel* _pixels;     
    Pixel* _oldPixels;
	
    void _initialize();
    hsv _makeRandomHSVColor();
    float _makeRandomValue(float min, float max);
    hsv _tweenHsvColor(hsv &previousPixel, hsv &nextPixel, float percent);
    float _tweenValue(float previousValue, float nextValue, float percent);
    int _makeRandomTimeout(int min, int max);
    int _randomizePixel(Pixel &pixel);
    void _copyPixel(Pixel &fromPixel, Pixel &toPixel);
    Adafruit_NeoPixel *_pixelStrip;
	
};

#endif

