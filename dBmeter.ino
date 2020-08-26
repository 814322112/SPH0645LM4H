#include <I2S.h>
 
void setup() {
  // Open serial communications and wait for port to open:
  // A baud rate of 115200 is used instead of 9600 for a faster data rate
  // on non-native USB ports
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
 
  // start I2S at 16 kHz with 32-bits per sample
  if (!I2S.begin(I2S_PHILIPS_MODE, 16000, 32)) {
    Serial.println("Failed to initialize I2S!");
    while (1); // do nothing
  }
}
 
#define SAMPLES 128 // make it a power of two for best DMA performance
 
void loop() {
  // read a bunch of samples:
  int samples[SAMPLES];
 
  for (int i=0; i<SAMPLES; i++) {
    int sample = 0; 
    while ((sample == 0) || (sample == -1) ) {
      sample = I2S.read();
    }
    // convert to 18 bit signed;

    sample >>= 14; 
    samples[i] = sample;
  }
 
  // ok we hvae the samples, get the mean (avg)
  float meanval = 0;
  for (int i=0; i<SAMPLES; i++) {
    meanval += samples[i];
  }
  meanval /= SAMPLES;
  //Serial.print("# average: " ); Serial.println(meanval);
 
  // subtract it from all sapmles to get a 'normalized' output
  for (int i=0; i<SAMPLES; i++) {
    samples[i] -= meanval;
    //Serial.println(samples[i]);
  }
 
  // find the 'peak to peak' max
  float maxsample, minsample;
  minsample = 100000;
  maxsample = -100000;
  for (int i=0; i<SAMPLES; i++) {
    minsample = min(minsample, samples[i]);
    maxsample = max(maxsample, samples[i]);
  }
  //analogIn is the 18-bit signed digital output from the microphone
  float analogIn = maxsample - minsample;
  //convert the digital input to its corresponding dB level, the formula is 
  //calculated by calibrating using another dB-measuring device 
  double dB = 11.126*log(analogIn) - 14.435;
  Serial.println(dB);
  //Serial.println(analogIn);
}
