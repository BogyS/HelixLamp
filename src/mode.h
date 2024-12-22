#pragma once

#ifndef all_mode_h
#define all_mode_h

#include <Arduino.h>
#include <FastLED.h>
#include "serial_config.h"

class Mode
{
protected:
    uint8_t counter = 0;

public:
    virtual ~Mode() {}

    virtual uint8_t Next()
    {
        counter = (counter + 1) % this->Max();
        return counter;
    }
    virtual uint8_t Rnd()
    {
        counter = random8(this->Max());
        return counter;
    }
    virtual void Update() = 0;
    virtual uint8_t Get() const { return counter; }
    virtual uint8_t Max() const = 0;
    virtual void Set(uint8_t val)
    {
        counter = val % this->Max();
    }
#if USE_SERIAL == true
    virtual const __FlashStringHelper* Name() const { return F("!"); };
#endif
    virtual void Init() { counter = 0; }
};

class Effect
{
public:
    virtual ~Effect() {}

    virtual void Init() {};
    virtual void Update() {};
#if USE_SERIAL == true
    virtual const __FlashStringHelper* Name() const { return F("?"); };
#endif
};

class EffectList : public Mode
{
protected:
    Effect** effects;
    uint8_t noEffects;

public:
    EffectList(Effect** effectsArray, uint8_t effectCount)
        : effects(effectsArray), noEffects(effectCount) {}

    virtual uint8_t Next()
    {
        counter = (counter + 1) % this->Max();
        this->effects[this->counter]->Init();
        return counter;
    }
    void Update()
    {
        this->effects[counter]->Update();
    }
    uint8_t Max() const
    {
        return this->noEffects;
    }
    virtual void Set(uint8_t val)
    {
        Mode::Set(val);
        this->effects[counter]->Init();
    }
#if USE_SERIAL == true
    const __FlashStringHelper* Name() const
    {
        return this->effects[this->counter]->Name();
    }
#endif
    void Init()
    {
        Mode::Init();
        for (uint8_t i = 0; i < this->noEffects; i++)
            this->effects[i]->Init();
    }
};

#endif // all_mode_h