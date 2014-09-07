#ifndef SIGNAL_GENERATOR_H
#define SIGNAL_GENERATOR_H

#define MICROSECONDS_PER_SECOND 1000000
#define PI 3.14159

#include <Arduino.h>
#include "../RadioshackStrip/Radioshack_Strip.h"
#include "../TimerOne/TimerOne.h"
#include <math.h>

class SignalGenerator {
public:
  virtual ~SignalGenerator() {}
  virtual void begin() = 0;
  virtual void enable() = 0;
  virtual void disable() = 0;
};

class SineSignalGenerator : public SignalGenerator {
public:
  SineSignalGenerator(
    uint8_t framerate,
    float sineFrequency,
    uint8_t minBrightness,
    uint8_t maxBrightness,
    uint32_t color,
    RadioshackStrip& leds);
  virtual ~SineSignalGenerator();
  
  virtual void begin();
  virtual void enable();
  virtual void disable();

private:
  static SineSignalGenerator* latestGenerator;
  static void outputFrameFromLatestGenerator();
  void outputFrame();

  boolean enabled;
  uint32_t color;
  RadioshackStrip& leds;
  long microsecondsPerFrame;
  double signalPosition;
  double stepSize;
  const uint8_t base;
  uint8_t multiplier;
};

#endif // SIGNAL_GENERATOR_H
