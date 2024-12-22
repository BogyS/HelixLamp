// 1st. List (palettcrossfade): 1 effect
// 2nd. List (palette mode): 12 effects
// 3rd. List (effect mode): 11 effects
// 4th. List (solid color mode): 18 effects

#include <avr/sleep.h>
#include <OneButton.h>
#include <EEPROM.h>

#include "led_config.h"
#include "mode.h"


#include "solid_color_mode.h"
#include "effect_mode.h"
#include "palette_mode.h"
#include "pacifica.h"
#include "pride.h"
#include "fire.h"
#include "twinkle.h"

#define BTN_PIN 2       // Button pin.
OneButton btn(BTN_PIN); // create button object that attach to pin 2

bool ledOn = false;
volatile bool isSleeping = false;
volatile bool isClicking = false;
volatile bool randomList = true;

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
const __FlashStringHelper* listName()
{
    switch (listIdx)
    {
    case 0:
        return F("Misc");
    case 1:
        return F("Palette");
    case 2:
        return F("Effects");
    case 3:
        return F("Solid color");
    default:
        return F("?");
    }
}

void btnISR()
{
    btn.tick();                           // Read pin and update FSM
}

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
    Serial.println(listName());
    listPosInfo();
}

void listInfoLeds()
{
    FastLED.clear(); // Set all LEDs to black (off)
    uint8_t i;
    for (i = 0; i <= listIdx; i++)
        leds[i % NUM_LEDS] = CRGB::Green;
    uint8_t m = list[listIdx]->Max();
    for (i = 0; i < m; i++)
        leds[(NUM_LEDS - 1 - i) % NUM_LEDS] = CRGB::Blue;
    FastLED.show(); // Apply the changes
    delay(2000);
    FastLED.clear(); // Set all LEDs to black (off)
    FastLED.show();  // Apply the changes
}

void listInfoPosLeds()
{
    FastLED.clear(); // Set all LEDs to black (off)
    uint8_t i;
    for (i = 0; i <= listIdx; i++)
        leds[i % NUM_LEDS] = CRGB::Green;
    uint8_t p = list[listIdx]->Get();
    uint8_t m = list[listIdx]->Max();
    for (i = 0; i < m; i++)
        leds[(NUM_LEDS - 1 - i) % NUM_LEDS] = (i == p) ? CRGB::Red : CRGB::Blue;
    FastLED.show(); // Apply the changes
    delay(1500);
    FastLED.clear(); // Set all LEDs to black (off)
    FastLED.show();  // Apply the changes
}

void setRandomList()
{
    listIdx = random8(ARRAY_SIZE(list));
    list[listIdx]->Rnd();
    FastLED.setBrightness(BRIGHTNESS);
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
    FastLED.setBrightness(BRIGHTNESS);
    listInfo();
    ledOn = false;
    digitalWrite(LED_BUILTIN, ledOn);
}

// Handler function for button:
void handleClick()
{
    if( isSleeping || isClicking )
      return;   // do nothing in sleep mode
    isClicking = true;
    resetRandomList();
    Serial.println(F("- Click!"));
    ledOn = !ledOn;
    digitalWrite(LED_BUILTIN, ledOn);
    list[listIdx]->Next();
    listPosInfo();
    listInfoPosLeds();
    isClicking = false;
}

void handleDoubleClick()
{
    if( isSleeping || isClicking )
      return;   // do nothing in sleep mode
    isClicking = true;
    resetRandomList();
    Serial.println(F("- DoubleClick!"));
    listIdx = (listIdx + 1) % ARRAY_SIZE(list);
    FastLED.setBrightness(BRIGHTNESS);
    listInfo();
    listInfoLeds();
    list[listIdx]->Init();
    isClicking = false;
}

void handleMultiClick()
{
    if( isSleeping || isClicking )
      return;           // do nothing in sleep mode
    isClicking = true;
    Serial.println(F("- MultiClick!"));
    randomList = true;  //start random
    isClicking = false;
}

void handleLongPressStart()
{
    if( isSleeping || isClicking )
      return;   // do nothing in sleep mode
     // Disable the current interrupt
    detachInterrupt(digitalPinToInterrupt(BTN_PIN));
    isClicking = true;
    isSleeping = true;
    Serial.println(F("- LongPressStart!"));
    Serial.println(F("Entering sleep mode..."));
    FastLED.clear();
    FastLED.show();
    delay(500);
    ledOn = false;
    digitalWrite(LED_BUILTIN, ledOn);
    // save data
    EEPROM.update(0, randomList ? 1 : 0); // random
    EEPROM.update(1, listIdx);
    EEPROM.update(2, list[listIdx]->Get());

    sleep_enable();                       // Enabling sleep mode
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);  // Setting the sleep mode, in our case full sleep
    // Reconfigure the interrupt for FALLING edge to wake up
    attachInterrupt(digitalPinToInterrupt(BTN_PIN), btnISR, FALLING);
    sleep_cpu();                          // activating sleep mode
    // Restore interrupt to CHANGE mode
    detachInterrupt(digitalPinToInterrupt(BTN_PIN));
    Serial.println(F("Just Woke Up!"));   // next line of code executed after the interrupt
    sleep_disable();
    attachInterrupt(digitalPinToInterrupt(BTN_PIN), btnISR, CHANGE);
    isClicking = false;
}

void setup()
{
    delay(1000); // power-up safety delay
    Serial.begin(9600);

    pinMode(LED_BUILTIN, OUTPUT);
    // pin mode i
    //pinMode(BTN_PIN, INPUT_PULLUP);
    // attaching a interrupt to pin D2
    attachInterrupt(digitalPinToInterrupt(BTN_PIN), btnISR, CHANGE);

    btn.attachClick(handleClick);
    btn.attachDoubleClick(handleDoubleClick);
    btn.attachMultiClick(handleMultiClick);
    btn.setClickMs(500);
    btn.attachLongPressStart(handleLongPressStart);
    

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();

    for (uint8_t i = 0; i < ARRAY_SIZE(list); i++)
        list[i]->Init();

    // read data
    randomList = (EEPROM.read(0) == 1);
    listIdx = EEPROM.read(1) % ARRAY_SIZE(list);
    list[listIdx]->Set(EEPROM.read(2));

    Serial.println(F("FastLED started"));
    Serial.println(F("Ver 24.12.22.12"));
    listInfo();
    listInfoLeds();
}

void loop()
{
    if (isSleeping)
    {
        Serial.println(F("Wake info:"));
        listInfo();
        isSleeping = false;
    }
    btn.tick();
    if (randomList)
    {
        EVERY_N_MILLISECONDS(7000)
        {
            setRandomList();
        }
    }
    list[listIdx]->Update();
}