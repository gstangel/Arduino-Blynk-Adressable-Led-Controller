#include "FastLED.h"
#define LED_DT 7                                            // Data pin to connect to the strip.
#define LED_CK 11                                             // Clock pin for WS2801 or APA102.
#define COLOR_ORDER GRB // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE WS2812B // Using APA102, WS2812, WS2812B, WS2801. Don't forget to change LEDS.addLeds.
#define NUM_LEDS 150 // Total Number of Leds in the Array

int millisTimer1 = millis();
int millisTimer2 = millis(); // timers used for some of the patterns in place of delays

uint8_t gHue = 0;       // Starting hue for patterns using sin functions
uint8_t brightness = 255; // Initial brightness (0-255)
struct CRGB leds[NUM_LEDS]; // Initalize LED struct
int mode = 0;   // Variable for the mode

void setup() {
  Serial.begin(2400); // Start serial port, faster baud rates create problems when reading input through serial
  delay(1000); // Soft startup
  //LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2801 or APA102
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // Use this for WS2812
  FastLED.setBrightness(brightness);// start off at max brightness
}

int currentRed = 255;  //start off white
int currentGreen = 255; // start off white
int currentBlue = 255; // start off white

//set a solid color
void setColors(int r, int g, int b) {
  currentRed = r;
  currentGreen = g;
  currentBlue = b;
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].r = currentRed;
    leds[i].g = currentGreen;
    leds[i].b = currentBlue;
  }
  FastLED.show();
}

int pin, data1, data2, data3; // Incoming data bytes from serial stored as int here
void loop() {
  
  //change the rate at which colors change
  //used for mutiple patterns
  EVERY_N_MILLISECONDS(25) {
    gHue++;
  }
  //wait for all 4 data bytes to fill buffer
  if (Serial.available() > 3) {
    
    pin = Serial.read(); // pin byte
    data1 = Serial.read(); // data byte 1
    data2 = Serial.read(); // data byte 2
    data3 = Serial.read(); // data byte 3

    if (pin == 2) {// change mode
      mode = data1;
    }

    if (pin == 0) {// change brightness
      FastLED.setBrightness(data1);
      FastLED.show();
    }
    else if (pin == 1) {// change solid rgb color
      mode = 0;
      currentRed = data1;//
      currentGreen = data2;//used to keep track of previous color after changing mode
      currentBlue = data3; //
      setColors(currentRed, currentGreen, currentBlue);// set the current color
    }
  }

  switch (mode) {
    case 0:                               
      setColors(currentRed, currentGreen, currentBlue);// solid color
      break;
    case 1:
      beatwave(5); // beatwave slow
      break;
    case 2:
      beatwave(10); // beatwave medium
      break;
    case 3:
      beatwave(20); // beatwave fast
      break;
    case 4:
      rainbow_march(200, 10); // rainbow 1
      break;
    case 5:
      rainbow_march(220, 5); // rainbow 2
      break;
    case 6:
      rainbow(); // rainbow 3
      break;
    case 7:
      rainbowWithGlitter(); // rainbow w/ glitter
      break;
    case 8:
      confetti();
      break;
    case 9:
      bpm(25);
      break;
    case 10:
      bpm(60);
      break;
    case 11:
      bpm(80);
      break;
    case 12:
      sinelon(10);
      break;
    case 13:
      sinelon(20);
      break;
    case 14:
      sinelon(40);
      break;
    case 15:
      inoise8_fire(20,3);
      break;
    case 16:
      inoise8_fire(50,2);
      break;
    case 17:
      juggle(8,4);
      break;
    case 18:
      juggle(16,4);
      break;
    case 19:
      juggle(32,4);
      break;
    case 20:
      juggle(8,8);
      break;
    case 21:
      juggle(16,8);
      break;
    case 22:
      juggle(32,8);
      break;
  }
  FastLED.show();

}

void beatwave(uint8_t beatsPerMin ) {

  TBlendType    currentBlending = LINEARBLEND;
  CRGBPalette16 currentPalette = RainbowColors_p;
  CRGBPalette16 targetPalette;

  uint8_t wave1 = beatsin8(beatsPerMin, 0, 255);
  uint8_t wave2 = beatsin8(beatsPerMin-1, 0, 255);
  uint8_t wave3 = beatsin8(beatsPerMin-2, 0, 255);
  uint8_t wave4 = beatsin8(beatsPerMin-3, 0, 255);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, i + wave1 + wave2 + wave3 + wave4, 255, currentBlending);
  }

  if (millis() - millisTimer1 > 100) {
    millisTimer1 = millis();
    uint8_t maxChanges = 24;
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);
  }
  if (millis() - millisTimer2 > 5000) {
    millisTimer2 = millis();
    targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)), CHSV(random8(), 192, random8(128, 255)), CHSV(random8(), 255, random8(128, 255)));

  }
  FastLED.show();

}

void rainbow_march(uint8_t thisdelay, uint8_t deltahue) {     // The fill_rainbow call doesn't support brightness levels.

  uint8_t thishue = millis() * (255 - thisdelay) / 255;       // To change the rate, add a beat or something to the result. 'thisdelay' must be a fixed value.


  fill_rainbow(leds, NUM_LEDS, thishue, deltahue);            // Use FastLED's fill_rainbow routine.

} // rainbow_march()


void rainbow() {
  fill_rainbow(leds, NUM_LEDS, gHue, 7);                      // FastLED's built-in rainbow generator.

} // rainbow()



void rainbowWithGlitter() {

  rainbow();                                                  // Built-in FastLED rainbow, plus some random sparkly glitter.
  addGlitter(80);

} // rainbowWithGlitter()



void addGlitter(fract8 chanceOfGlitter) {

  if (random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }

} // addGlitter()



void confetti() {
  // Random colored speckles that blink in and fade smoothly.
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(gHue + random8(64), 200, 255);

}


void sinelon(uint8_t beatsPerMin ) {
  // A colored dot sweeping back and forth, with fading trails.
  // beatsPerMin designates speed at which it moves
  fadeToBlackBy(leds, NUM_LEDS, 20);
  int pos = beatsin16(beatsPerMin, 0, NUM_LEDS - 1);
  leds[pos] += CHSV(gHue, 255, 192);

}



void bpm(uint8_t beatsPerMinute) {
  // colored sections sweeping back and forth
  // beatsPerMin designates speed at which it sweeps
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(beatsPerMinute, 64, 255);

  for (int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }

}

void juggle(int hueChange, int juggleSpeed) {
  // pattern where the colors appear to juggle
  // hueChange designates the difference in color beteween 'balls'
  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 0;

  for (int i = 0; i < 8; i++) {
    leds[beatsin16(i + juggleSpeed, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
    dothue += hueChange;
  }

}

void inoise8_fire(uint32_t xscale, uint32_t yscale) {                
  uint8_t index = 0;
  CRGBPalette16 currentPalette = CRGBPalette16(
                                   CRGB::Black, CRGB::Black, CRGB::Black, CHSV(0, 255, 4),
                                   CHSV(0, 255, 8), CRGB::Red, CRGB::Red, CRGB::Red,
                                   CRGB::DarkOrange, CRGB::Orange, CRGB::Orange, CRGB::Orange,
                                   CRGB::Yellow, CRGB::Yellow, CRGB::Gray, CRGB::Gray);

  for (int i = 0; i < NUM_LEDS; i++) {
    index = inoise8(i * xscale, millis() * yscale * NUM_LEDS / 255);
    leds[i] = ColorFromPalette(currentPalette, min(i * (index) >> 6, 255), i * 255 / NUM_LEDS, LINEARBLEND);
  }
}


void sunrise() {

  // total sunrise length, in minutes
  static const uint8_t sunriseLength = 30;

  // how often (in seconds) should the heat color increase
  // for the default of 30 minutes, this should be about every 7 seconds
  // 7 seconds x 256 gradient steps = 1,792 seconds = ~30 minutes
  static const uint8_t interval = (sunriseLength * 60) / 256;

  // current gradient palette color index
  static uint8_t heatIndex = 0; // start out at 0

  // HeatColors_p is a gradient palette built in to FastLED
  // that fades from black to red, orange, yellow, white
  // feel free to use another palette or define your own custom one
  CRGB color = ColorFromPalette(HeatColors_p, heatIndex);

  // fill the entire strip with the current color
  fill_solid(leds, NUM_LEDS, color);

  // slowly increase the heat
  EVERY_N_SECONDS(interval) {
    // stop incrementing at 255, we don't want to overflow back to 0
    if (heatIndex < 255) {
      heatIndex++;
    }
  }
}
