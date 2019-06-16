#pragma once

<<<<<<< 355839a92907f7fc8ef0c9cb0b38a8b4f70b8aa4
#include "Arduino.h"
=======
#include <Arduino.h>
>>>>>>> Add chrono lib

class Time
{
public:

    Time();

<<<<<<< 355839a92907f7fc8ef0c9cb0b38a8b4f70b8aa4
=======
    float asHertz() const;

>>>>>>> Add chrono lib
    float asSeconds() const;

    int64_t asMilliseconds() const;

    int64_t asMicroseconds() const;

    static Time now();

private:

<<<<<<< 355839a92907f7fc8ef0c9cb0b38a8b4f70b8aa4
=======
    friend Time hertz(float);
    friend Time hours(float);
    friend Time minutes(float);
>>>>>>> Add chrono lib
    friend Time seconds(float);
    friend Time milliseconds(int64_t);
    friend Time microseconds(int64_t);

    explicit Time(int64_t microseconds);

private:

    int64_t m_microseconds; ///< Time value stored as microseconds
};

<<<<<<< 355839a92907f7fc8ef0c9cb0b38a8b4f70b8aa4
=======
Time hertz(float amount);

Time hours(float);

Time minutes(float);

>>>>>>> Add chrono lib
Time seconds(float amount);

Time milliseconds(int64_t amount);

Time microseconds(int64_t amount);

bool operator ==(Time left, Time right);

bool operator !=(Time left, Time right);

bool operator <(Time left, Time right);

bool operator >(Time left, Time right);

bool operator <=(Time left, Time right);

bool operator >=(Time left, Time right);

Time operator -(Time right);

Time operator +(Time left, Time right);

Time& operator +=(Time& left, Time right);

Time operator -(Time left, Time right);

Time& operator -=(Time& left, Time right);

Time operator *(Time left, float right);

Time operator *(Time left, int64_t right);

Time operator *(float left, Time right);

Time operator *(int64_t left, Time right);

Time& operator *=(Time& left, float right);

Time& operator *=(Time& left, int64_t right);

Time operator /(Time left, float right);

Time operator /(Time left, int64_t right);

Time& operator /=(Time& left, float right);

Time& operator /=(Time& left, int64_t right);

float operator /(Time left, Time right);

Time operator %(Time left, Time right);

Time& operator %=(Time& left, Time right);