/*
  ColorConverter.cpp - Library for converting between color space
  Created by Tony Gaitatzis, 2015
  Released into the public domain.
*/

#ifndef ColorConverter_h
#define ColorConverter_h

#include "Arduino.h"

typedef struct rgb {
    double r;       // percent
    double g;       // percent
    double b;       // percent
} rgb;

typedef struct hsv {
    double h;       // angle in degrees
    double s;       // percent
    double v;       // percent
} hsv;

class ColorConverter
{
  public:
    
    static hsv rgb2hsv(rgb in);
    static rgb hsv2rgb(hsv in);
};


#endif


