#include <FastLED.h>

#define LED_PIN  3
#define COLOR_ORDER GRB
#define CHIPSET     NEOPIXEL

#define BRIGHTNESS 64

const uint8_t kMatrixWidth  = 20;
const uint8_t kMatrixHeight = 20;
const bool    kMatrixSerpentineLayout = true;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[NUM_LEDS];

//int a[400] = {0, 6, 13, 20, 26, 33, 40, 46, 53, 60, 67, 73, 80, 87, 93, 100, 107, 114, 120, 127, 6, 13, 20, 26, 33, 40, 46, 53, 60, 67, 73, 80, 87, 93, 100, 107, 114, 120, 127, 134, 13, 20, 26, 33, 40, 46, 53, 60, 67, 73, 80, 87, 93, 100, 107, 114, 120, 127, 134, 140, 20, 26, 33, 40, 46, 53, 60, 67, 73, 80, 87, 93, 100, 107, 114, 120, 127, 134, 140, 147, 26, 33, 40, 46, 53, 60, 67, 73, 80, 87, 93, 100, 107, 114, 120, 127, 134, 140, 147, 154, 33, 40, 46, 53, 60, 67, 73, 80, 87, 93, 100, 107, 114, 120, 127, 134, 140, 147, 154, 161, 40, 46, 53, 60, 67, 73, 80, 87, 93, 100, 107, 114, 120, 127, 134, 140, 147, 154, 161, 167, 46, 53, 60, 67, 73, 80, 87, 93, 100, 107, 114, 120, 127, 134, 140, 147, 154, 161, 167, 174, 53, 60, 67, 73, 80, 87, 93, 100, 107, 114, 120, 127, 134, 140, 147, 154, 161, 167, 174, 181, 60, 67, 73, 80, 87, 93, 100, 107, 114, 120, 127, 134, 140, 147, 154, 161, 167, 174, 181, 187, 67, 73, 80, 87, 93, 100, 107, 114, 120, 127, 134, 140, 147, 154, 161, 167, 174, 181, 187, 194, 73, 80, 87, 93, 100, 107, 114, 120, 127, 134, 140, 147, 154, 161, 167, 174, 181, 187, 194, 201, 80, 87, 93, 100, 107, 114, 120, 127, 134, 140, 147, 154, 161, 167, 174, 181, 187, 194, 201, 208, 87, 93, 100, 107, 114, 120, 127, 134, 140, 147, 154, 161, 167, 174, 181, 187, 194, 201, 208, 214, 93, 100, 107, 114, 120, 127, 134, 140, 147, 154, 161, 167, 174, 181, 187, 194, 201, 208, 214, 221, 100, 107, 114, 120, 127, 134, 140, 147, 154, 161, 167, 174, 181, 187, 194, 201, 208, 214, 221, 228, 107, 114, 120, 127, 134, 140, 147, 154, 161, 167, 174, 181, 187, 194, 201, 208, 214, 221, 228, 234, 114, 120, 127, 134, 140, 147, 154, 161, 167, 174, 181, 187, 194, 201, 208, 214, 221, 228, 234, 241, 120, 127, 134, 140, 147, 154, 161, 167, 174, 181, 187, 194, 201, 208, 214, 221, 228, 234, 241, 248, 127, 134, 140, 147, 154, 161, 167, 174, 181, 187, 194, 201, 208, 214, 221, 228, 234, 241, 248, 255};

void setup() {
//  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.addLeds<CHIPSET, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
}

void loop()
{
    uint32_t ms = millis();
    int32_t yHueDelta32 = ((int32_t)cos16( ms * 27 ) * (350 / kMatrixWidth));
    int32_t xHueDelta32 = ((int32_t)cos16( ms * 39 ) * (310 / kMatrixHeight));
    DrawOneFrame( ms / 65536, yHueDelta32 / 32768, xHueDelta32 / 32768);
    FastLED.show();

}

void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
  byte lineStartHue = startHue8;
  for( byte y = 0; y < kMatrixHeight; y++) {
    lineStartHue += yHueDelta8;
    byte pixelHue = lineStartHue;      
    for( byte x = 0; x < kMatrixWidth; x++) {
      pixelHue += xHueDelta8;
//      leds[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
      leds[ XY(x, y)]  = CHSV( 255, 255, 0 + (y%2) * 100 + (x%2) * 100);
    }
  }
}


// Helper function that translates from x, y into an index into the LED array
// Handles both 'row order' and 'serpentine' pixel layouts.
uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;

  // to incorporate the mess we made. First LED at bottom left (facepalm)
  y = kMatrixHeight - 1 - y;
  
  if( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  } else {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }
  
  return i;
}
