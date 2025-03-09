#include <cassert>
#include <cmath>
#include <cstdint> // for fixed width integers
#include <iostream>

class FixedPoint2
{
private:
	std::int16_t m_base{}; // here's our non-fractional part
	std::int8_t m_decimal{}; // here's our factional part

public:
	FixedPoint2(std::int16_t base = 0, std::int8_t decimal = 0)
		: m_base{ base }, m_decimal{ decimal }
	{
		// If either (or both) of the non-fractional and fractional part of the number are negative, the number should be treated as negative
		if (m_base < 0 || m_decimal < 0)
		{
			// Make sure base is negative
			if (m_base > 0)
				m_base = -m_base;
			// Make sure decimal is negative
			if (m_decimal > 0)
				m_decimal = -m_decimal;
		}

		// If decimal is out of bounds (in either direction),
		// adjust the decimal so it's in bounds,
		// and adjust base to account for the number of units removed from the decimal
		// h/t to reader David Pinheiro for simplifying this
		m_base += m_decimal / 100;    // integer division
		m_decimal = m_decimal % 100;  // integer remainder
	}

	// We'll delegate to the prior constructor so we don't have to duplicate the negative number and overflow handling logic
	FixedPoint2(double d) :
		FixedPoint2(
			static_cast<std::int16_t>(std::trunc(d)),
			static_cast<std::int8_t>(std::round(d * 100) - std::trunc(d) * 100)
		)
	{
	}

	explicit operator double() const
	{
		return m_base + (static_cast<double>(m_decimal) / 100);
	}
};

// This doesn't require access to the internals of the class, so it can be defined outside the class
std::ostream& operator<<(std::ostream& out, const FixedPoint2& fp)
{
	out << static_cast<double>(fp);
	return out;
}

int main()
{
	FixedPoint2 a{ 0.01 };
	std::cout << a << '\n';
	assert(static_cast<double>(a) == 0.01);

	FixedPoint2 b{ -0.01 };
	assert(static_cast<double>(b) == -0.01);

	FixedPoint2 c{ 1.9 }; // make sure we handle single digit decimal
	assert(static_cast<double>(c) == 1.9);

	FixedPoint2 d{ 5.01 }; // stored as 5.0099999... so we'll need to round this
	assert(static_cast<double>(d) == 5.01);

	FixedPoint2 e{ -5.01 }; // stored as -5.0099999... so we'll need to round this
	assert(static_cast<double>(e) == -5.01);

	// Handle case where the argument's decimal rounds to 100 (need to increase base by 1)
	FixedPoint2 f { 106.9978 }; // should be stored with base 107 and decimal 0
	assert(static_cast<double>(f) == 107.0);

	// Handle case where the argument's decimal rounds to -100 (need to decrease base by 1)
	FixedPoint2 g { -106.9978 }; // should be stored with base -107 and decimal 0
	assert(static_cast<double>(g) == -107.0);

	return 0;
}