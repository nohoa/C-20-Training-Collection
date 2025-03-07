//This class Implement fixed point floating number 
#include <cassert>
#include <iostream>
#include <cstdint>
#include <ostream>

class FixedPoint2 {
    private :

    std:: int16_t integer{} ;
    std :: int8_t fractional{} ;

    public : 
     FixedPoint2(std::int16_t base = 0, std::int8_t decimal = 0)
		: integer { base }, fractional { decimal }
	{
		// If either (or both) of the non-fractional and fractional part of the number are negative, the number should be treated as negative
		if (integer < 0 || fractional < 0)
		{
			// Make sure base is negative
			if (integer > 0)
				integer = -integer;
			// Make sure decimal is negative
			if (fractional > 0)
				fractional = -fractional;
		}
	}

	explicit operator double() const { return integer + (static_cast<double>(fractional)/100); }
    
    friend std::ostream& operator<<(std::ostream& os, const FixedPoint2& f2 ) ;
};

std::ostream& operator<<(std::ostream& os, const FixedPoint2& f2 ){
    os << static_cast<double>(f2); 
    return os;
}
int main()
{
	FixedPoint2 a{ 34, 56 };
	std::cout << a << '\n';
	 std::cout << static_cast<double>(a) << '\n';
	 assert(static_cast<double>(a) == 34.56);

	// FixedPoint2 b{ -2, 8 };
	// assert(static_cast<double>(b) == -2.08);

	// FixedPoint2 c{ 2, -8 };
	// assert(static_cast<double>(c) == -2.08);

	// FixedPoint2 d{ -2, -8 };
	// assert(static_cast<double>(d) == -2.08);

	// FixedPoint2 e{ 0, -5 };
	// assert(static_cast<double>(e) == -0.05);

	// FixedPoint2 f{ 0, 10 };
	// assert(static_cast<double>(f) == 0.1);

	return 0;
}