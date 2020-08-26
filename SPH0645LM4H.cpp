#include "Arduino.h"
#include "Morse.h"
#include <I2S.h>



SPH0645LM4H::SPH0645LM4H(int SAMP)
{
	_sample = SAMP;
}



void SPH0645LM4H::init()
{
  // start I2S at 16 kHz with 32-bits per sample
  if (!I2S.begin(I2S_PHILIPS_MODE, 16000, 32)) {
    while (1); // do nothing
  }
}



double SPH0645LM4H::sample()
{
	int samples[_sample];
 
  for (int i=0; i<_sample; i++) {
    int sample = 0; 
    while ((sample == 0) || (sample == -1) ) {
      sample = I2S.read();
    }
    // convert to 18 bit signed;

    sample >>= 14; 
    samples[i] = sample;
  }
 
  // ok we have the samples, get the mean (avg)
  float meanval = 0;
  for (int i=0; i<_sample; i++) {
    meanval += samples[i];
  }
  meanval /= _sample;
  //Serial.print("# average: " ); Serial.println(meanval);
 
  // subtract it from all sapmles to get a 'normalized' output
  for (int i=0; i<_sample; i++) {
    samples[i] -= meanval;
    //Serial.println(samples[i]);
  }
 
  // find the 'peak to peak' max
  float maxsample, minsample;
  minsample = 100000;
  maxsample = -100000;
  for (int i=0; i<_sample; i++) {
    minsample = min(minsample, samples[i]);
    maxsample = max(maxsample, samples[i]);
  }
  //analogIn is the 18-bit signed digital output from the microphone
  float analogIn = maxsample - minsample;
  //convert the digital input to its corresponding dB level, the formula is 
  //calculated by calibrating using another dB-measuring device 
  double dB = 11.126*log(analogIn) - 14.435;

  return db;
}

