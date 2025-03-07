#include <cassert>
#include <iostream>

class FixedPoint2 {
    private :

    std:: int16_t integer{} ;
    std :: int8_t fractional{} ;

    public : 
     FixedPoint2(std::int16_t base = 0, std::int8_t decimal = 0)
		: integer { base }, fractional { decimal }
	{
        //std :: cout << decimal << std :: endl;
		// If either (or both) of the non-fractional and fractional part of the number are negative, the number should be treated as negative
		
			// Make sure base is negative
			// if (integer > 0)
			// 	integer = -integer;
			// // Make sure decimal is negative
			// if (fractional > 0)
			// 	fractional = -fractional;
            
        bool negative = false ;
        if(integer < 0 || decimal < 0){
            negative = true ;
        }
        if(integer < 0) {
            integer = - integer;
        }
        if(decimal < 0){
            fractional = -fractional ;
        }

       // std :: cout << static_cast<double>(decimal) << " " << static_cast<double>(fractional) << std :: endl;

        if(integer > 99){
            integer = 99 ;
        }
        if(fractional  > 99){
            fractional -= 100;
            integer ++;
            if(integer > 99) integer = 99;
        }
        if(negative == true) {
            integer = - integer;
            fractional = -fractional;
        }
        //std :: cout << static_cast<double>(decimal) << " " << static_cast<double>(fractional) << std :: endl;
	}

	explicit operator double() const { 
        if(integer < 0){
           // std :: cout << static_cast<double>(integer) << " " << static_cast<double>(fractional) << std :: endl;
            return -((-integer) + (static_cast<double>(-fractional)/100));
        }
        return integer + (static_cast<double>(fractional)/100);
     }
    
    friend std::ostream& operator<<(std::ostream& os, const FixedPoint2& f2 ) ;
    friend bool testDecimal(const FixedPoint2 &fp);
};

std::ostream& operator<<(std::ostream& os, const FixedPoint2& f2 ){
    os << static_cast<double>(f2); 
    return os;
}

// You will need to make testDecimal a friend of FixedPoint2
// so the function can access the private members of FixedPoint2
bool  testDecimal(const FixedPoint2 &fp)
{
    if (fp.integer >= 0)
        return fp.fractional >= 0 && fp.fractional < 100;
    else
        return fp.fractional <= 0 && fp.fractional > -100;
}

int main()
{
	FixedPoint2 a{ 1, 104 };
	std::cout << a << '\n';
	std::cout << static_cast<double>(a) << '\n';
	assert(static_cast<double>(a) == 2.04);
	assert(testDecimal(a));

	FixedPoint2 b{ 1, -104 };
    //std :: cout << static_cast<double>(b) << std :: endl;
	assert(static_cast<double>(b) == -2.04);
    //std :: cout << b << '\n';
	assert(testDecimal(b));

	FixedPoint2 c{ -1, 104 };
    //std :: cout << static_cast<double>(c) << std :: endl;
	assert(static_cast<double>(c) == -2.04);
	assert(testDecimal(c));

	FixedPoint2 d{ -1, -104 };
	assert(static_cast<double>(d) == -2.04);
	assert(testDecimal(d));

	return 0;
}