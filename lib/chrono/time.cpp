#include "time.h"

Time Time::now()
{
    return microseconds(micros());
}

Time::Time() 
:
m_microseconds(0)
{
}

float Time::asHertz() const
{
    return 1.0 / asSeconds();
}

float Time::asSeconds() const
{
    return m_microseconds / 1000000.0;
}

int64_t Time::asMilliseconds() const
{
    return static_cast<int64_t>(m_microseconds / 1000);
}

int64_t Time::asMicroseconds() const
{
    return m_microseconds;
}

Time::Time(int64_t microseconds) :
m_microseconds(microseconds)
{
}

Time hertz(float amount)
{
    return Time(static_cast<int64_t>((1.0/amount) * 1000000.0));
}

Time hours(float amount)
{
    return minutes(amount * 60.0);
}

Time minutes(float amount)
{
    return seconds(amount * 60.0);
}

Time seconds(float amount)
{
    return Time(static_cast<int64_t>(amount * 1000000.0));
}

Time milliseconds(int64_t amount)
{
    return Time(static_cast<int64_t>(amount) * 1000.0);
}

Time microseconds(int64_t amount)
{
    return Time(amount);
}

bool operator ==(Time left, Time right)
{
    return left.asMicroseconds() == right.asMicroseconds();
}

bool operator !=(Time left, Time right)
{
    return left.asMicroseconds() != right.asMicroseconds();
}

bool operator <(Time left, Time right)
{
    return left.asMicroseconds() < right.asMicroseconds();
}

bool operator >(Time left, Time right)
{
    return left.asMicroseconds() > right.asMicroseconds();
}

bool operator <=(Time left, Time right)
{
    return left.asMicroseconds() <= right.asMicroseconds();
}

bool operator >=(Time left, Time right)
{
    return left.asMicroseconds() >= right.asMicroseconds();
}

Time operator -(Time right)
{
    return microseconds(-right.asMicroseconds());
}

Time operator +(Time left, Time right)
{
    return microseconds(left.asMicroseconds() + right.asMicroseconds());
}

Time& operator +=(Time& left, Time right)
{
    return left = left + right;
}

Time operator -(Time left, Time right)
{
    return microseconds(left.asMicroseconds() - right.asMicroseconds());
}

Time& operator -=(Time& left, Time right)
{
    return left = left - right;
}

Time operator *(Time left, float right)
{
    return seconds(left.asSeconds() * right);
}

Time operator *(Time left, int64_t right)
{
    return microseconds(left.asMicroseconds() * right);
}

Time operator *(float left, Time right)
{
    return right * left;
}

Time operator *(int64_t left, Time right)
{
    return right * left;
}

Time& operator *=(Time& left, float right)
{
    return left = left * right;
}

Time& operator *=(Time& left, int64_t right)
{
    return left = left * right;
}

Time operator /(Time left, float right)
{
    return seconds(left.asSeconds() / right);
}

Time operator /(Time left, int64_t right)
{
    return microseconds(left.asMicroseconds() / right);
}

Time& operator /=(Time& left, float right)
{
    return left = left / right;
}

Time& operator /=(Time& left, int64_t right)
{
    return left = left / right;
}

float operator /(Time left, Time right)
{
    return left.asSeconds() / right.asSeconds();
}

Time operator %(Time left, Time right)
{
    return microseconds(left.asMicroseconds() % right.asMicroseconds());
}

Time& operator %=(Time& left, Time right)
{
    return left = left % right;
}