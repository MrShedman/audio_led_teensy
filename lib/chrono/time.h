#pragma once

#include <Arduino.h>

class Time
{
public:

    Time();

    float asHertz() const;

    float asSeconds() const;

    int64_t asMilliseconds() const;

    int64_t asMicroseconds() const;

    static Time now();

private:

    friend Time hertz(float);
    friend Time hours(float);
    friend Time minutes(float);
    friend Time seconds(float);
    friend Time milliseconds(int64_t);
    friend Time microseconds(int64_t);

    explicit Time(int64_t microseconds);

private:

    int64_t m_microseconds; ///< Time value stored as microseconds
};

Time hertz(float amount);

Time hours(float);

Time minutes(float);

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