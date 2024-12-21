#ifndef all_mode_h
#define all_mode_h

#include <Arduino.h>
#include <FastLED.h>

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
    virtual void Update() {}
    virtual uint8_t Get() const { return counter; }
    virtual uint8_t Max() const { return 0; }
    virtual void Set(uint8_t val)
    {
        counter = val % this->Max();
    }
    virtual String Name() const { return ""; }
    virtual void Init() { counter = 0; }
};

class Effect
{
public:
    virtual ~Effect() {}

    virtual void Init() {}
    virtual void Update() {}
    virtual String Name() const { return ""; }
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
    String Name() const
    {
        return this->effects[this->counter]->Name();
    }
    void Init()
    {
        Mode::Init();
        for (uint8_t i = 0; i < this->noEffects; i++)
            this->effects[i]->Init();
    }
};

#endif // all_mode_h