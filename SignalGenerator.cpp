#include "SignalGenerator.h"

SineSignalGenerator::SineSignalGenerator(
    uint8_t framerate,
    float sineFrequency,
    uint8_t minBrightness,
    uint8_t maxBrightness,
    uint32_t color,
    RadioshackStrip& leds)
  : enabled(false),
    color(color),
    leds(leds),
    microsecondsPerFrame(
      (long) (
        ((double) MICROSECONDS_PER_SECOND / (double) framerate) + .5
      )
    ),
    signalPosition(-PI/2),
    stepSize(sineFrequency*2*PI/framerate),
    multiplier((maxBrightness - minBrightness +.5)/2),
    base((maxBrightness - minBrightness +.5)/2 + minBrightness)
{
  if(
    framerate == 0
    || sineFrequency == 0
    || minBrightness >= maxBrightness
    || color == 0
  ) { return; }
  SineSignalGenerator::latestGenerator = this;
}

SineSignalGenerator::~SineSignalGenerator() {
  if(this == SineSignalGenerator::latestGenerator) {
    Timer1.detachInterrupt();
    Timer1.stop();
  }
}

SineSignalGenerator* SineSignalGenerator::latestGenerator = NULL;

void SineSignalGenerator::begin() {
  Timer1.initialize((long) (microsecondsPerFrame + .5));
  Timer1.attachInterrupt(SineSignalGenerator::outputFrameFromLatestGenerator);
  Timer1.start();
}

void SineSignalGenerator::enable() {
  enabled = true;
}

void SineSignalGenerator::disable() {
  enabled = false;
}

void SineSignalGenerator::outputFrameFromLatestGenerator() {
  if(SineSignalGenerator::latestGenerator == NULL) { return; }
  SineSignalGenerator::latestGenerator->outputFrame();
}

void SineSignalGenerator::outputFrame() {
  signalPosition = fmod(signalPosition + stepSize, 2*PI);
  if(!enabled) { return; }
  
  uint8_t brightness = base + (sin(signalPosition) * multiplier);
  
  leds.setBrightness(brightness);
  for (int i = 0; i<leds.numPixels(); ++i) {
    leds.setPixelColor(i, color);
  }
  leds.show();
}
