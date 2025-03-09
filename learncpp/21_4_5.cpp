#include <cassert>
#include <cmath>
#include <cstdint> // for fixed width integers
#include <fstream>
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
        //std :: cout << m_base << std :: endl;
	}

	explicit operator double() const
	{
		return m_base + (static_cast<double>(m_decimal) / 100);
	}

    friend bool operator==(const FixedPoint2& value, const FixedPoint2& value1){
      //  std :: cout << value.m_base <<" " << value.m_decimal << std :: endl;
       // std :: cout << value1.m_base <<" " << value1.m_decimal << std :: endl;
        if(value.m_base != value1.m_base) return false;
         if(value.m_decimal != value1.m_decimal) return false;
        return true;
    }
    friend std::ostream& operator<<(std::ostream& out, const FixedPoint2& fp);
    //friend std :: ifstream& operator>>(std::)
    friend std::istream& operator>>(std::istream& is, const FixedPoint2& fp);
    FixedPoint2 operator-() const {
    return FixedPoint2(-static_cast<double>(*this));
    }
};

// This doesn't require access to the internals of the class, so it can be defined outside the class
std::ostream& operator<<(std::ostream& out, const FixedPoint2& fp)
{
	out << static_cast<double>(fp);
	return out;
}

std::istream& operator>>(std::istream& is,  FixedPoint2& fp){
    double d{};
    is >> d ;
    fp =   FixedPoint2{d};
    return is;
}

FixedPoint2 operator+(const FixedPoint2& fp1, const FixedPoint2& fp2)
{
	return FixedPoint2{ static_cast<double>(fp1) + static_cast<double>(fp2) };
}


int main()
{
	assert(FixedPoint2{ 0.75 } == FixedPoint2{ 0.75 });    // Test equality true
	assert(!(FixedPoint2{ 0.75 } == FixedPoint2{ 0.76 })); // Test equality false

	// Test additional cases -- h/t to reader Sharjeel Safdar for these test cases
	assert(FixedPoint2{ 0.75 } + FixedPoint2{ 1.23 } == FixedPoint2{ 1.98 });    // both positive, no decimal overflow
	assert(FixedPoint2{ 0.75 } + FixedPoint2{ 1.50 } == FixedPoint2{ 2.25 });    // both positive, with decimal overflow
	assert(FixedPoint2{ -0.75 } + FixedPoint2{ -1.23 } == FixedPoint2{ -1.98 }); // both negative, no decimal overflow
	assert(FixedPoint2{ -0.75 } + FixedPoint2{ -1.50 } == FixedPoint2{ -2.25 }); // both negative, with decimal overflow
	assert(FixedPoint2{ 0.75 } + FixedPoint2{ -1.23 } == FixedPoint2{ -0.48 });  // second negative, no decimal overflow
	assert(FixedPoint2{ 0.75 } + FixedPoint2{ -1.50 } == FixedPoint2{ -0.75 });  // second negative, possible decimal overflow
	assert(FixedPoint2{ -0.75 } + FixedPoint2{ 1.23 } == FixedPoint2{ 0.48 });   // first negative, no decimal overflow
	assert(FixedPoint2{ -0.75 } + FixedPoint2{ 1.50 } == FixedPoint2{ 0.75 });   // first negative, possible decimal overflow

	FixedPoint2 a{ -0.48 };
	assert(static_cast<double>(a) == -0.48);
	assert(static_cast<double>(-a) == 0.48);

	std::cout << "Enter a number: "; // enter 5.678
	std::cin >> a;
	std::cout << "You entered: " << a << '\n';
	assert(static_cast<double>(a) == 5.68);

	return 0;
}