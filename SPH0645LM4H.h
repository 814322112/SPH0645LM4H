/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef SPH0645LM4H_h
#define SPH0645LM4H_h

#include "Arduino.h"

class SPH0645LM4H
{
  public:
    SPH0645LM4H(int SAMP);
    void init();
    double sample();
  private:
  	int _sample
};

#endif