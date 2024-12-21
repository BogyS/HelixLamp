// 1st. List (palettcrossfade): 1 effect
// 2nd. List (palette mode): 12 effects
// 3rd. List (effect mode): 11 effects
// 4th. List (solid color mode): 18 effects

#include <avr/sleep.h>
#include <FastLED.h>
#include <OneButton.h>
#include <EEPROM.h>

#define LED_PIN 4       // Data pin to connect to the strip.
#define NUM_LEDS 28     // Number of LED's.
#define BRIGHTNESS 250  // maximum brightness (250 on 28LEDs = 560mA max current)
#define LED_TYPE WS2812 // Using APA102, WS2812, WS2801.
#define COLOR_ORDER GRB // It's GRB for WS2812 and BGR for APA102.
#define UPDATES_PER_SECOND 100
struct CRGB leds[NUM_LEDS];

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
typedef void (*fnPtrVoid)();

#include "solid_color_mode.h"
#include "effect_mode.h"
#include "palette_mode.h"
#include "pacifica.h"
#include "pride.h"
#include "fire.h"
#include "twinkle.h"

OneButton btn(2); // create button object that attach to pin 2
static uint8_t intNum = digitalPinToInterrupt(2);

bool ledOn = false;
volatile bool justWokeUp = false;

Pacifica pacifica;
Pride pride;
Fire fire;
Test test;
TwinkleFox twinkle;
Effect* effectsMisc[] = {&pacifica, &pride, &fire, &test, &twinkle};

EffectList misc(effectsMisc, ARRAY_SIZE(effectsMisc));
Palette palette;
Efects efects;
SolidColor solidColor;

uint8_t listIdx = 0;
Mode* list[] = {&misc, &palette, &efects, &solidColor};
const char* listNames[] = {"Misc", "Palette", "Effects", "Solid color"};

void wakeUp()
{
    justWokeUp = true;
}

void fallAsleep()
{
    sleep_enable();                       // Enabling sleep mode
    attachInterrupt(intNum, wakeUp, LOW); // attaching a interrupt to pin D2
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);  // Setting the sleep mode, in our case full sleep
    sleep_cpu();                          // activating sleep mode
    Serial.println(F("Just Woke Up!"));   // next line of code executed after the interrupt
    sleep_disable();
    detachInterrupt(intNum);
}

bool randomList = true;

void listPosInfo()
{
    Serial.print(F(" - Pos idx: "));
    Serial.print(list[listIdx]->Get());
    Serial.print(F(" - name: "));
    Serial.println(list[listIdx]->Name());
}

void listInfo()
{
    Serial.print(F("- List idx: "));
    Serial.print(listIdx);
    Serial.print(F(" - max: "));
    Serial.print(list[listIdx]->Max());
    Serial.print(F(" - name: "));
    Serial.println(listNames[listIdx]);
    listPosInfo();
}

void listInfoLeds()
{
    FastLED.clear(); // Set all LEDs to black (off)
    uint8_t i;
    for (i = 0; i <= listIdx; i++)
        leds[i % NUM_LEDS] = CRGB::Green;
    uint8_t m = list[listIdx]->Max();
    for (i = 1; i <= m; i++)
        leds[(NUM_LEDS - i) % NUM_LEDS] = CRGB::Blue;
    FastLED.show(); // Apply the changes
    delay(2000);
    FastLED.clear(); // Set all LEDs to black (off)
    FastLED.show(); // Apply the changes
}

void listInfoPosLeds()
{
    FastLED.clear(); // Set all LEDs to black (off)
    uint8_t i;
    for (i = 0; i <= listIdx; i++)
        leds[i % NUM_LEDS] = CRGB::Green;
    i = list[listIdx]->Get() + 1;
    leds[(NUM_LEDS - i) % NUM_LEDS] = CRGB::Blue;
    FastLED.show(); // Apply the changes
    delay(1500);
    FastLED.clear(); // Set all LEDs to black (off)
    FastLED.show(); // Apply the changes
}

void setRandomList()
{
    listIdx = random8(ARRAY_SIZE(list));
    list[listIdx]->Rnd();
    listInfo();
    listInfoPosLeds();
    ledOn = !ledOn;
    digitalWrite(LED_BUILTIN, ledOn);
}

void resetRandomList()
{
    if (!randomList)
        return;
    randomList = false;
    for (uint8_t i = 0; i < ARRAY_SIZE(list); i++)
        list[i]->Init();
    listIdx = 0;
    listInfo();
    ledOn = false;
    digitalWrite(LED_BUILTIN, ledOn);
}

// Handler function for button:
void handleClick()
{
    resetRandomList();
    Serial.println(F("- Click!"));
    ledOn = !ledOn;
    digitalWrite(LED_BUILTIN, ledOn);
    list[listIdx]->Next();
    listPosInfo();
    listInfoPosLeds();
}

void handleDoubleClick()
{
    resetRandomList();
    Serial.println(F("- DoubleClick!"));
    listIdx = (listIdx + 1) % ARRAY_SIZE(list);
    listInfo();
    listInfoLeds();
    list[listIdx]->Init();
}

void handleLongPressStart()
{
    Serial.println(F("- LongPressStart!"));
    Serial.println(F("Entering sleep mode..."));
    FastLED.clear();
    FastLED.show();
    delay(500);
    ledOn = false;
    digitalWrite(LED_BUILTIN, ledOn);
    //save data
    EEPROM.update(0, 0);        // random
    EEPROM.update(1, listIdx);
    EEPROM.update(2, list[listIdx]->Get());

    fallAsleep();
}

void setup()
{
    delay(1000); // power-up safety delay
    Serial.begin(115200);

    btn.attachClick(handleClick);
    btn.attachDoubleClick(handleDoubleClick);
    btn.attachLongPressStart(handleLongPressStart);
    btn.setClickMs(500);

    pinMode(LED_BUILTIN, OUTPUT);

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();

    for (uint8_t i = 0; i < ARRAY_SIZE(list); i++)
        list[i]->Init();

    // at first run uncomment this for lines for initializing the eeprom data
    //EEPROM.update(0, 24); // random
    //EEPROM.update(1, 0);  // list
    //EEPROM.update(2, 0);  // pos

    //read data
    randomList = (EEPROM.read(0) == 24);
    listIdx = EEPROM.read(1) % ARRAY_SIZE(list);
    list[listIdx]->Set( EEPROM.read(2) );

    Serial.println(F("FastLED started"));
    Serial.println(F("Ver 24.12.22.00"));
    listInfo();
    listInfoLeds();
}

void loop()
{
    if (justWokeUp)
    {
        Serial.println(F("loop: Just Woke Up!"));
        listInfo();
        justWokeUp = false;
    }
    btn.tick();
    if (randomList)
    {
        EVERY_N_MILLISECONDS(5000)
        {
            setRandomList();
        }
    }
    list[listIdx]->Update();
}