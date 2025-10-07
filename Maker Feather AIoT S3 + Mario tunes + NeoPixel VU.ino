/* Maker Feather AIoT S3 + Mario tunes + NeoPixel VU
   - Uses your ARDUTECH-style note tables and buzzer toggling technique.
   - Buzzer on GPIO12 (D12).
   - NeoPixel stick (8 LEDs) DIN on GPIO5 (D5).
*/

#include <Adafruit_NeoPixel.h>

// -------------------- NeoPixel setup --------------------
#define LED_PIN     5          // D5 on Maker Feather
#define LED_COUNT   8          // Cytron stick has 8 pixels
#define BRIGHTNESS  40
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// -------------------- Buzzer pins -----------------------
#define melodyPin   12         // use GPIO12 (D12) for buzzer on Maker Feather
#define LED_INDICATOR_PIN 13   // optional indicator LED (if available)


// ===================== Your note table =====================
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

// -------------------- Mario main theme --------------------
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};

int tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

// -------------------- Underworld theme --------------------
int underworld_melody[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,
  NOTE_AS3, NOTE_AS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,
  NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4,
  NOTE_DS4, NOTE_GS3,
  NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,
  NOTE_AS3, NOTE_A3, NOTE_GS3,
  0, 0, 0
};

int underworld_tempo[] = {
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  3,
  12, 12, 12, 12,
  12, 12, 6,
  6, 18, 18, 18,
  6, 6,
  6, 6,
  6, 6,
  18, 18, 18, 18, 18, 18,
  10, 10, 10,
  10, 10, 10,
  3, 3, 3
};

// ==========================================================
// ============== NeoPixel VU helper functions ==============
// ==========================================================
uint32_t barColor(uint16_t idx, uint16_t n) {
  // gradient red -> yellow -> green
  float t = n <= 1 ? 1.0f : (float)idx / (float)(n - 1);
  uint8_t r = (uint8_t)(255 * (1.0f - t));
  uint8_t g = (uint8_t)(255 * t);
  return strip.Color(r, g, 0);
}

void showVU(float level01) {         // 0..1
  int lit = (int)(level01 * LED_COUNT + 0.5f);
  if (lit > LED_COUNT) lit = LED_COUNT;
  strip.clear();
  for (int i = 0; i < lit; i++) strip.setPixelColor(i, barColor(i, LED_COUNT));
  strip.setBrightness(BRIGHTNESS);
  strip.show();
}

// ==========================================================
// ===== Your buzzer toggling algorithm (sliced window) =====
// ==========================================================
// Plays a short window of tone using the same math/loop as your buzz().
// We call this repeatedly (10ms slices) so we can refresh the VU meter.
void buzzSlice(int targetPin, long frequency, int length_ms) {
  if (length_ms <= 0) return;
  if (frequency <= 0) { delay(length_ms); return; }  // rest safeguard

  digitalWrite(LED_INDICATOR_PIN, HIGH);

  unsigned long end_us   = micros() + (unsigned long)length_ms * 1000UL;
  unsigned long half_us  = (unsigned long)(500000.0 / (double)frequency);  // half period
  if (half_us == 0) half_us = 1;  // safety for very high freqs

  while ((long)(end_us - micros()) > 0) {
    digitalWrite(targetPin, HIGH);
    delayMicroseconds(half_us);
    digitalWrite(targetPin, LOW);
    delayMicroseconds(half_us);
  }

  digitalWrite(LED_INDICATOR_PIN, LOW);
}

// Play one musical note for 'length_ms', with a simple attack/decay envelope.
// The VU meter follows the envelope. Internally it calls buzzSlice()
// so the buzzer code remains your original toggling style.
void playNoteWithVU(int freq, int length_ms) {
  const int step_ms = 10;          // update every 10 ms
  const float atk = 0.15f;         // 15% attack
  const float dec = 0.25f;         // 25% decay
  int elapsed = 0;

  while (elapsed < length_ms) {
    float t = (float)elapsed / (float)length_ms;
    float env;
    if (t < atk)               env = t / atk;           // 0..1
    else if (t > 1.0f - dec)   env = (1.0f - t) / dec;  // 1..0
    else                       env = 1.0f;

    // Drive VU first
    showVU(env);

    // Gate the tone inside this window according to envelope
    int on_ms  = (int)(step_ms * env + 0.5f);
    int off_ms = step_ms - on_ms;

    buzzSlice(melodyPin, freq, on_ms);
    if (off_ms > 0) delay(off_ms);

    elapsed += step_ms;
  }
}

// ==========================================================
// ===================== Player routines ====================
// ==========================================================
int song = 0;

void sing(int s) {
  song = s;
  if (song == 2) {
    int size = sizeof(underworld_melody) / sizeof(int);
    for (int i = 0; i < size; i++) {
      int noteDuration = 1000 / underworld_tempo[i];
      playNoteWithVU(underworld_melody[i], noteDuration);

      // pause between notes (hold VU at zero)
      int pauseBetweenNotes = (int)(noteDuration * 1.30f);
      showVU(0.0f);
      delay(pauseBetweenNotes);
    }
  } else {
    int size = sizeof(melody) / sizeof(int);
    for (int i = 0; i < size; i++) {
      int noteDuration = 1000 / tempo[i];
      playNoteWithVU(melody[i], noteDuration);

      int pauseBetweenNotes = (int)(noteDuration * 1.30f);
      showVU(0.0f);
      delay(pauseBetweenNotes);
    }
  }
}

// ============================ Setup / Loop ============================
void setup() {
  pinMode(melodyPin, OUTPUT);
  pinMode(LED_INDICATOR_PIN, OUTPUT);
  digitalWrite(melodyPin, LOW);
  digitalWrite(LED_INDICATOR_PIN, LOW);

  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();
}

void loop() {
  sing(1);   // Mario
  sing(1);   // Mario again
  sing(2);   // Underworld
}
