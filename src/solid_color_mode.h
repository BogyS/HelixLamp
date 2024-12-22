#ifndef SOLID_COLOR_MODE_H_
#define SOLID_COLOR_MODE_H_

#include "mode.h"

class SolidColor : public Mode
{
    void solidColorFill()
    {
        switch (this->counter)
        {
        case 0:
        case 1:
            fill_solid(leds, NUM_LEDS, CHSV(0, 255, 192));
            break;
        case 2:
        case 3:
            fill_solid(leds, NUM_LEDS, CHSV(32, 255, 255));
            break;
        case 4:
        case 5:
            fill_solid(leds, NUM_LEDS, CHSV(64, 255, 255));
            break;
        case 6:
        case 7:
            fill_solid(leds, NUM_LEDS, CHSV(96, 255, 255));
            break;
        case 8:
        case 9:
            fill_solid(leds, NUM_LEDS, CHSV(138, 255, 255));
            break;
        case 10:
        case 11:
            fill_solid(leds, NUM_LEDS, CHSV(160, 255, 255));
            break;
        case 12:
        case 13:
            fill_solid(leds, NUM_LEDS, CHSV(180, 255, 255));
            break;
        case 14:
        case 15:
            fill_solid(leds, NUM_LEDS, CHSV(214, 255, 255));
            break;
        // case 16:
        // case 17:
        default:
            fill_solid(leds, NUM_LEDS, CHSV(0, 0, 192));
            break;
        }
    }
public:
    void Update()
    {
        solidColorFill();
        if (this->counter % 2 == 0)
        {
            float breath = (exp(sin(millis() / 2000.0 * PI)) - 0.36787944) * 108.0;
            FastLED.setBrightness(breath);
        }
        else
        {
            FastLED.setBrightness(BRIGHTNESS);
        }
        FastLED.show();
        FastLED.delay(2000 / UPDATES_PER_SECOND);
    }

    uint8_t Max() const { return 18; }
    const __FlashStringHelper* Name() const
    {
        return F("*");
    }
};

#endif /* SOLID_COLOR_MODE_H_ */
