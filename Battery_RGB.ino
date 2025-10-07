#include <WiFi.h>                 // not used; keeps core happy on some setups
#include <Adafruit_NeoPixel.h>

// ---------- NeoPixel config ----------
#define LED_PIN     5            // DIN of the Cytron NeoPixel stick
#define LED_COUNT   8
#define BRIGHTNESS  32            // 0..255, keep modest if USB-powered

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// ---------- Battery sense (Maker Feather AIoT S3) ----------
#define VIN_SENSE_PIN A12         // "VIN SENSE" (0.6 * Vin). Higher of VUSB or VBATT.
// NOTE: When USB is connected Vin ≈ 5V; when on battery Vin ≈ 3.2..4.2V.

// Smooth the ADC a little
float readVinVolts(uint8_t samples = 8) {
  analogSetPinAttenuation(VIN_SENSE_PIN, ADC_11db);   // up to ~3.6V at ADC input
  uint32_t mv = 0;
  for (uint8_t i = 0; i < samples; i++) {
    mv += analogReadMilliVolts(VIN_SENSE_PIN);        // factory-calibrated mV
    delay(3);
  }
  float v_adc = (mv / (float)samples) / 1000.0f;      // volts at ADC pin
  float v_in  = v_adc / 0.6f;                         // undo 0.6× divider from board
  return v_in;                                        // Vin = VUSB or VBATT (whichever is higher)
}

// Map LiPo voltage (~3.2–4.2V) to % with a simple piecewise curve
int lipoPercent(float vbat) {
  // Clamp typical 1S LiPo range
  if (vbat <= 3.20f) return 0;
  if (vbat >= 4.20f) return 100;

  struct Pt { float v; int p; };
  static const Pt curve[] = {
    {3.20f,  0}, {3.50f, 10}, {3.70f, 50},
    {3.85f, 75}, {4.00f, 90}, {4.20f, 100}
  };
  // Linear interpolate
  for (int i = 0; i < (int)(sizeof(curve)/sizeof(curve[0])) - 1; i++) {
    if (vbat <= curve[i+1].v) {
      float t = (vbat - curve[i].v) / (curve[i+1].v - curve[i].v);
      return curve[i].p + (int)((curve[i+1].p - curve[i].p) * t + 0.5f);
    }
  }
  return 100;
}

// Color helper: red->yellow->green gradient across index 0..(n-1)
uint32_t barColor(uint16_t idx, uint16_t n) {
  float t = n <= 1 ? 1.0f : (float)idx / (float)(n - 1);
  uint8_t r = 255 * (1.0f - t);
  uint8_t g = 255 * t;
  return strip.Color(r, g, 0);
}

void showBatteryBar(int percent) {
  strip.clear();
  int lit = (percent * LED_COUNT + 99) / 100;   // round up slightly
  for (int i = 0; i < LED_COUNT; i++) {
    if (i < lit) strip.setPixelColor(i, barColor(i, LED_COUNT));
  }
  strip.setBrightness(BRIGHTNESS);
  strip.show();
}

void showLowBatteryBlink(bool on) {
  strip.clear();
  if (on) {
    for (int i = 0; i < LED_COUNT; i++) strip.setPixelColor(i, strip.Color(255, 0, 0));
  }
  strip.setBrightness(BRIGHTNESS);
  strip.show();
}

void showUsbAnimation(uint16_t step) {
  // Simple "charging" cyan wipe
  strip.clear();
  int pos = step % LED_COUNT;
  for (int i = 0; i <= pos; i++) strip.setPixelColor(i, strip.Color(0, 150, 150));
  strip.setBrightness(BRIGHTNESS);
  strip.show();
}

void setup() {
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();
}

void loop() {
  static uint32_t t0 = 0;
  static bool blinkOn = false;
  static uint16_t step = 0;

  float vin = readVinVolts();  // higher of VUSB/VBATT

  if (vin > 4.5f) {
    // USB plugged: show charging/plugged animation
    showUsbAnimation(step++);
    delay(120);
    return;
  }

  // On battery: estimate SoC and show bar
  int pct = lipoPercent(vin);
  if (pct < 20) {
    // blink red at ~1 Hz
    uint32_t now = millis();
    if (now - t0 > 500) { t0 = now; blinkOn = !blinkOn; }
    showLowBatteryBlink(blinkOn);
    delay(50);
  } else {
    showBatteryBar(pct);
    delay(250);  // refresh rate
  }
}
