#pragma once

#ifndef LED_CONFIG_H
#define LED_CONFIG_H

#include <FastLED.h>

// Constant definitions
#define LED_PIN 4       // Data pin to connect to the strip
#define NUM_LEDS 28     // Number of LEDs
#define BRIGHTNESS 250  // Maximum brightness
#define LED_TYPE WS2812 // LED type (e.g., WS2812, APA102)
#define COLOR_ORDER GRB // Color order for the LEDs
#define UPDATES_PER_SECOND 100

// Declare LED array
extern struct CRGB leds[NUM_LEDS];

// Macro to calculate array size
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// Define a function pointer type for void-returning functions
typedef void (*fnPtrVoid)();

#endif // LED_CONFIG_H
