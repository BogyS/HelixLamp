#ifndef PALETTE_MODE_H_
#define PALETTE_MODE_H_

#include "led_config.h"
#include "mode.h"

namespace PaletteMode
{
    CRGBPalette16 currentPalette;

    void FillLEDsFromNewPaletteColors(uint8_t colorIndex)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = ColorFromPalette(currentPalette, colorIndex + sin8(i * 16), 255);
            colorIndex += 3;
        }
    }

    void FillLEDsFromPaletteColors(uint8_t colorIndex)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = ColorFromPalette(currentPalette, colorIndex, 255, LINEARBLEND);
            colorIndex += 3;
        }
    }

    DEFINE_GRADIENT_PALETTE(rainbow_gp){
        0, 255, 0, 255,
        51, 0, 0, 255,
        102, 0, 255, 255,
        153, 0, 255, 0,
        204, 255, 255, 0,
        255, 255, 0, 0};

    DEFINE_GRADIENT_PALETTE(es_landscape_24_gp){
        0, 27, 91, 0,
        38, 94, 255, 0,
        63, 142, 255, 21,
        68, 173, 244, 252,
        127, 10, 164, 156,
        255, 5, 68, 66};

    DEFINE_GRADIENT_PALETTE(Gummy_Bears_gp){
        0, 252, 91, 0,
        63, 222, 21, 1,
        127, 194, 1, 1,
        191, 35, 48, 2,
        255, 1, 215, 10};

    DEFINE_GRADIENT_PALETTE(bhw3_01_gp){
        0, 255, 146, 228,
        25, 152, 43, 65,
        48, 36, 17, 22,
        72, 8, 34, 75,
        81, 1, 58, 170,
        104, 42, 49, 245,
        124, 15, 15, 95,
        150, 1, 8, 39,
        175, 98, 32, 35,
        204, 123, 9, 2,
        219, 220, 15, 2,
        237, 255, 93, 6,
        255, 244, 244, 0};

    DEFINE_GRADIENT_PALETTE(BlacK_Magenta_Red_gp){
        0, 0, 0, 0,
        63, 42, 0, 45,
        127, 255, 0, 255,
        191, 255, 0, 45,
        255, 255, 0, 0};

    DEFINE_GRADIENT_PALETTE(BlacK_Blue_Magenta_White_gp){
        0, 0, 0, 0,
        42, 0, 0, 45,
        84, 0, 0, 255,
        127, 42, 0, 255,
        170, 255, 0, 255,
        212, 255, 55, 255,
        255, 255, 255, 255};

    DEFINE_GRADIENT_PALETTE(Sunset_Real_gp){
        0, 120, 0, 0,
        22, 179, 22, 0,
        51, 255, 104, 0,
        85, 167, 22, 18,
        135, 100, 0, 103,
        198, 16, 0, 130,
        255, 0, 0, 160};

    DEFINE_GRADIENT_PALETTE(lava_gp){
        0, 0, 0, 0,
        46, 18, 0, 0,
        96, 113, 0, 0,
        108, 142, 3, 1,
        119, 175, 17, 1,
        146, 213, 44, 2,
        174, 255, 82, 4,
        188, 255, 115, 4,
        202, 255, 156, 4,
        218, 255, 203, 4,
        234, 255, 255, 4,
        244, 255, 255, 71,
        255, 255, 255, 255};

    DEFINE_GRADIENT_PALETTE(GMT_drywet_gp){
        0, 47, 30, 2,
        42, 213, 147, 24,
        84, 103, 219, 52,
        127, 3, 219, 207,
        170, 1, 48, 214,
        212, 1, 1, 111,
        255, 1, 7, 33};

    DEFINE_GRADIENT_PALETTE(gr65_hult_gp){
        0, 247, 176, 247,
        48, 255, 136, 255,
        89, 220, 29, 226,
        160, 7, 82, 178,
        216, 1, 124, 109,
        255, 1, 124, 109};

    DEFINE_GRADIENT_PALETTE(departure_gp){
        0, 8, 3, 0,
        42, 23, 7, 0,
        63, 75, 38, 6,
        84, 169, 99, 38,
        106, 213, 169, 119,
        116, 255, 255, 255,
        138, 135, 255, 138,
        148, 22, 255, 24,
        170, 0, 255, 0,
        191, 0, 136, 0,
        212, 0, 55, 0,
        255, 0, 55, 0};

    DEFINE_GRADIENT_PALETTE(Analogous_1_gp){
        0, 3, 0, 255,
        63, 23, 0, 255,
        127, 67, 0, 255,
        191, 142, 0, 45,
        255, 255, 0, 0};

    // There are several different palettes of colors demonstrated below.
    void ChangePalettePeriodically(uint8_t paletteCounter)
    {
        switch (paletteCounter)
        {
        case 0:
            currentPalette = rainbow_gp;
            break;
        case 1:
            currentPalette = es_landscape_24_gp;
            break;
        case 2:
            currentPalette = Gummy_Bears_gp;
            break;
        case 3:
            currentPalette = bhw3_01_gp;
            break;
        case 4:
            currentPalette = BlacK_Magenta_Red_gp;
            break;
        case 5:
            currentPalette = BlacK_Blue_Magenta_White_gp;
            break;
        case 6:
            currentPalette = Sunset_Real_gp;
            break;
        case 7:
            currentPalette = lava_gp;
            break;
        case 8:
            currentPalette = GMT_drywet_gp;
            break;
        case 9:
            currentPalette = gr65_hult_gp;
            break;
        case 10:
            currentPalette = departure_gp;
            break;
        case 11:
            currentPalette = Analogous_1_gp;
            break;
        default:
            // ...
            break;
        }
    }
} // PacificaMode

class Palette : public Mode
{
public:
    void Update()
    {
        FastLED.setBrightness(BRIGHTNESS);
        PaletteMode::ChangePalettePeriodically(this->counter);
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1;
        PaletteMode::FillLEDsFromPaletteColors(startIndex);
        FastLED.show();
        FastLED.delay(2000 / UPDATES_PER_SECOND);
    }
    uint8_t Max() const { return 12; }
    const __FlashStringHelper* Name() const
    {
        switch (this->counter)
        {
        case 0:
            return F("Rainbow");
        case 1:
            return F("Landscape");
        case 2:
            return F("Gummy Bears");
        case 3:
            return F("BHW3");
        case 4:
            return F("BlacK Magenta Red");
        case 5:
            return F("BlacK Blue Magenta White");
        case 6:
            return F("Sunset Real");
        case 7:
            return F("Lava");
        case 8:
            return F("GMT Dry Wet");
        case 9:
            return F("GR65 Hult");
        case 10:
            return F("Departure");
        case 11:
            return F("Analogous");
        default:
            return F("?");
        }
    }
};

#endif /* PALETTE_MODE_H_ */
