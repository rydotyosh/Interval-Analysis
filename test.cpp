#include <iostream>
#include "Interval.h"
#include "Interval.cpp"

using Interval::interval;
using Interval::hull;

int main()
{
	/*	Syntax and features of interval	*/
	
	try
	{
		auto x = interval<double>(-1.0, 5.0);	// interval decralation needs template argument
		auto y = hull(0.0,1.0);					// To use hull template argument is deduced
		auto z(y);								// interval has copy constructor and move constructor
												// both of them do deep copy
		std::cout << (x != y) << std::endl;
		std::cout << static_cast<int>(y.relation(x)) << std::endl;

		x++;			// postfix increment operator
		++x;			// prefix increment operator
		x--;			// postfix decrement operator
		--x;			// prefix decrement operator
		x + y;			// interval<T> + interval<T>
		x - y;			// interval<T> - interval<T>
		x * y;			// interval<T> * interval<T>
		x / y;			// interval<T> / interval<T>
		x += y;			// interval<T> += interval<T>
		x -= y;			// interval<T> -= interval<T>
		x *= y;			// interval<T> *= interval<T>
		x /= y;			// interval<T> /= interval<T>

		x.set_low(1.0);
		x += 2.0;		// interval<T> += T
		x -= 2.0;		// interval<T> -= T
		x *= 2.0;		// interval<T> *= T
		x /= 2.0;		// interval<T> /= T
		//!2.0 += a;	// compile error! 
		//!2.0 -= a;	// compile error!
		//!2.0 *= a;	// compile error!
		//!2.0 /= a;	// compile error!
		2.0 + x;		// T + interval<T>
		2.0 - x;		// T + interval<T>
		2.0 * x;		// T + interval<T>
		2.0 / x;		// T + interval<T>
		x * 2.0;		// interval<T> + T
		x / 2.0;		// interval<T> + T
		x + 2.0;		// interval<T> + T
		x - 2.0;		// interval<T> + T
		z.get_low();	// getter(return lower_bound)
		z.get_up();		// getter(return upper_bound)


		z.abs();		// interval absolute function
		z.sin();		// interval sin function
		z.cos();		// interval cos function
		z.exp();		// interval exponential function
		z.pow(3);		// interval power(int) function
		z.mid();		// interval middle point function
		z.wid();		// interval width function
	
		/*	interval has relation function	*/
		/*	Let x=[a,b] , y=[c,d]			*/

		x.is_absolute_greater(y);
		// x is absolute greater than y
		// It means a > d

		x.is_absolute_less(y);
		// x is absolute less than y
		// It means b < c

		x.is_weak_greater(y);
		// x is weak greater than y
		// It means a > c & b > d

		x.is_weak_less(y);
		// x is weak less than y
		// It means a < b & c < d

		x.is_contain(y);
		// x is contain y
		// It means a < c < d < b

		x.relation(y);
		// This function returns relation between x and y
		//
		//	Return values are provided 
		//	as strong typed enum class below.
		//
		//	enum class Interval_Relation
		//	{
		//		absolute_less,
		//		absolute_greater,
		//		weak_less,
		//		weak_greater,
		//		contain,
		//		niether,
		//	};
		//
	}
	catch (Interval::logic_error& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	catch (Interval::domain_error& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	catch (Interval::invalid_argument& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	catch (Interval::range_error& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	catch (Interval::interval_error& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}


	return 0;
}