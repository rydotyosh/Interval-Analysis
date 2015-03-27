#ifndef INTERVAL
#define INTERVAL
#include <exception>
#include <cmath>
#include <typeinfo>
#include <type_traits>
#include <iostream>
#include <limits>
#include <memory>

using std::cout;
using std::endl;

namespace Interval
{

	/*PI*/
	template<typename T>
	T PI(){ return static_cast<T>(3.141592653589793238462643383279); }//skelton


	/*maximum*/
	template<typename T>
	T max(){ return T(1.7976931348623158e+308); }//skelton

	/*minimum*/
	template<typename T>
	T min(){ return T(2.2250738585072014e-308); }//skelton

	class interval_error
	{
	public:
		interval_error(char* string)
			:mes(std::string(string))
		{}
		interval_error(std::string string)
			:mes(string)
		{}

		const char* what()
		{
			return mes.c_str();
		}
	private:
		std::string mes;

	};
	class IntervalExcep : public interval_error
	{
	public:
		IntervalExcep(std::string& cause)
			:interval_error("Error massage : " + cause)
		{}
		IntervalExcep(const char *cause)
			:interval_error(std::string("Error massage : ") + std::string(cause))
		{}
	};

	class IntervalZeroDivideExcep : public IntervalExcep
	{
	public:
		IntervalZeroDivideExcep(std::string& cause)
			:IntervalExcep(cause)
		{}
		IntervalZeroDivideExcep(const char *cause)
			:IntervalExcep(cause)
		{}
	};

	class IntervalDomainExcep : public IntervalExcep
	{
	public:
		IntervalDomainExcep(std::string& cause)
			:IntervalExcep(cause)
		{}
		IntervalDomainExcep(const char *cause)
			:IntervalExcep(cause)
		{}
	};

	enum class Interval_Relation
	{
		absolute_less,
		absolute_greater,
		weak_less,
		weak_greater,
		contain,
		niether,
	};

	/*new Interval class prototype*/
	class interval
	{
	public:
		/*constexpr means it does not have the side effects*/
		interval()
			:low(double())
			, up(double())
		{}
		/*interval(double Initializer)
			: low(Initializer)
			, up(Initializer)
		{}*/
		interval(double Lower_Bound, double Upper_Bound)
		{
			if (Lower_Bound > Upper_Bound)
			{
				throw IntervalDomainExcep("Lower Bound must be less than Upper Bound!");
			}
			else
			{
				low = Lower_Bound;
				up = Upper_Bound;
			}
		}

		/*  interval compound assignment operator */

		//addition assignment
		const interval operator +=(const interval& rhs)
		{
			low = std::nextafter(low + rhs.low, -max<double>());
			up = std::nextafter(up + rhs.up, max<double>());
			return *this;
		}

		//subtraction assignment
		const interval operator -=(const interval& rhs)
		{
			if (this == &rhs)
			{
				low = 0.0;
				up = 0.0;
				return *this;
			}
			low = std::nextafter(low - rhs.up, -max<double>());
			up = std::nextafter(up - rhs.low, max<double>());
			return *this;
		}

		//multiplication assignment
		const interval operator *=(const interval& rhs)
		{
			if (this == &rhs)
			{
				if (up <= 0.0)
				{
					low = std::pow(up, 2);
					up = std::pow(up, 2);
					return *this;
				}
				else if (low <= 0.0 && up >= 0.0)
				{
					low = 0.0;
					up = std::fmax(std::pow(low, 2), std::pow(up, 2));
					return *this;
				}
				else
				{
					low = std::pow(low, 2);
					up = std::pow(up, 2);
					return *this;
				}
			}
			else if (low >= 0.0 && low >= 0.0){
				low = std::nextafter(low * rhs.low, -max<double>());
				up = std::nextafter(up * rhs.up, max<double>());
				return *this;
			}
			else if (low >= 0.0 && rhs.low < 0.0 && rhs.up > 0.0)
			{
				low = std::nextafter(up * rhs.low, -max<double>());
				up = std::nextafter(up * rhs.up, max<double>());
				return *this;
			}
			else if (rhs.low >= 0.0 && rhs.up <= 0.0)
			{
				low = std::nextafter(up * rhs.low, -max<double>());
				up = std::nextafter(low * rhs.up, max<double>());
				return *this;
			}
			else if (low < 0.0 && up > 0.0 && rhs.low >= 0.0)
			{
				low = std::nextafter(low * rhs.up, -max<double>());
				up = std::nextafter(up * rhs.up, max<double>());
				return *this;
			}
			else if (low < 0.0 && up > 0.0 && rhs.up <= 0.0)
			{
				low = std::nextafter(up * rhs.low, -max<double>());
				up = std::nextafter(low * rhs.low, max<double>());
				return *this;
			}
			else if (up <= 0.0 && rhs.low >= 0.0)
			{
				low = std::nextafter(low * rhs.up, -max<double>());
				up = std::nextafter(up * rhs.low, max<double>());
				return *this;
			}
			else if (up <= 0.0 && rhs.low < 0.0 && rhs.up > 0.0)
			{
				low = std::nextafter(low * rhs.low, -max<double>());
				up = std::nextafter(low * rhs.up, max<double>());
				return *this;
			}
			else if (up <= 0.0 && rhs.up <= 0.0)
			{
				low = std::nextafter(up * rhs.up, -max<double>());
				up = std::nextafter(low * rhs.low, max<double>());
				return *this;
			}
			else{
				low = std::fmin(std::nextafter(up * rhs.low, -max<double>()),
					std::nextafter(low * rhs.up, -max<double>()));
				up = std::fmax(std::nextafter(up * rhs.up, max<double>()),
					std::nextafter(low * rhs.low, max<double>()));
				return *this;
			}
		}

		//division assignment
		const interval operator /=(const interval& rhs)
		{
			if (rhs.low <= 0.0 && rhs.up >= 0.0)
			{
				throw IntervalZeroDivideExcep("Divided by Zero!");
			}
			else
			{
				if (this == &rhs){
					low = 1.0;
					up = 1.0;
					return *this;
				}
				else
				{
					interval tmp(std::nextafter(1.0 / rhs.up, -max<double>()),
						std::nextafter(1.0 / rhs.low, max<double>()));
					*this *= tmp;
					return *this;
				}
			}
		}

		//interval postfix increment operator
		const interval operator ++(int)
		{
			interval tmp(*this);
			low = std::nextafter(low + 1.0, -max<double>());
			up = std::nextafter(up + 1.0, max<double>());
			return tmp;
		}

		//interval prefix increment operator
		const interval operator ++()
		{
			low = std::nextafter(low + 1.0, -max<double>());
			up = std::nextafter(up + 1.0, max<double>());
			return *this;
		}

		//interval postfix decrement operator
		const interval operator --(int)
		{
			interval tmp(*this);
			low = std::nextafter(low - 1.0, -max<double>());
			up = std::nextafter(up - 1.0, max<double>());
			return tmp;
		}

		//interval prefix decrement operator
		const interval operator --(){
			low = std::nextafter(low - 1.0, -max<double>());
			up = std::nextafter(up - 1.0, max<double>());
			return *this;
		}
		/*	relation functions	*/

		bool is_absolute_greater(interval const& x)
		{
			return this->low > x.up_bound();
		}

		bool is_absolute_less(interval const& x)
		{
			return this->up < x.low_bound();
		}

		bool is_weak_greater(interval const& x)
		{
			return (this->up > x.up_bound() && this->low > x.low_bound());
		}

		bool is_weak_less(interval const& x)
		{
			return (this->up < x.up_bound() && this->low < x.low_bound());
		}

		bool is_contain(interval const& x)
		{
			return (this->up > x.up_bound() && this->low < x.low_bound());
		}

		Interval_Relation relation(interval const& x)
		{
			if (this->is_absolute_greater(x))
				return Interval_Relation::absolute_greater;
			else if (this->is_absolute_less(x))
				return Interval_Relation::absolute_less;
			else if (this->is_weak_greater(x))
				return Interval_Relation::weak_greater;
			else if (this->is_weak_less(x))
				return Interval_Relation::weak_less;
			else if (this->is_contain(x))
				return Interval_Relation::contain;
			else
				return Interval_Relation::niether;
		}
		/*  Logical operator  */

		//less than operator
		bool operator <(const interval& rhs)
		{
			if (low == rhs.low)
			{
				if (up < rhs.up)
				{
					return true;
				}
			}
			else
			{
				if (low < rhs.low)
				{
					return true;
				}
			}
			return false;
		}

		//greater than operator
		bool operator >(const interval& rhs)
		{
			if (up > rhs.up)
			{
				return true;
			}
			return false;
		}

		//equivalence operator
		bool operator ==(const interval& rhs)
		{
			if (low == rhs.low && up == rhs.up){
				return true;
			}
			return false;
		}

		//not equivalence operator
		bool operator !=(const interval& rhs)
		{
			if (low == rhs.low && up == rhs.up)
			{
				return false;
			}
			return true;
		}

		//less than or equal operator
		bool operator <=(const interval& rhs)
		{
			if (low <= rhs.low && up <= rhs.up)
			{
				return true;
			}
			return false;
		}

		//greater than or equal operator
		bool operator >=(const interval& rhs)
		{
			if (up >= rhs.up && low >= rhs.low)
			{
				return true;
			}
			return false;
		}

		/*interval pow( int )*/
		const interval pow(int n) const
		{
			if (n < 0){
				return this->pow(-1 * n);
			}
			else if (n == 0){
				return interval(1.0, 1.0);
			}
			else if (n % 2 == 0){
				if (up <= 0.0){
					return interval(
						std::nextafter(std::pow(up, n), -max<double>()),
						std::nextafter(std::pow(low, n), max<double>()));
				}
				else if (low <= 0.0 && up >= 0.0){
					return interval(0.0, std::fmax(
						std::nextafter(std::pow(low, n), max<double>()),
						std::nextafter(std::pow(up, n), max<double>())));
				}
			}
			return interval(
				std::nextafter(std::pow(low, n), -max<double>()),
				std::nextafter(std::pow(up, n), max<double>()));
		}
		/*interval sin*/
		const interval sin() const
		{
			int c;
			long double a = low, b = up;
			if (b - a >= 2.0*PI<double>())
			{
				return interval(-1.0, 1.0);
			}
			/*  base point set  */
			c = static_cast<int>(std::nextafter((b * 2.0 / PI<double>() - 1.0) / 4.0, max<double>()));
			/* checking  */
			if (std::nextafter(PI<double>() / 2.0*(1.0 + c*4.0), max<double>()) >= a &&
				std::nextafter(PI<double>() / 2.0*(1 + c*4.0), -max<double>()) <= b)
			{
				if (std::nextafter(PI<double>() / 2.0*(3.0 + c*4.0), max<double>()) >= a &&
					std::nextafter(PI<double>() / 2.0*(3.0 + c*4.0), -max<double>()) <= b)
				{
					return interval(-1.0, 1.0);
				}
				else if (std::nextafter((c*4.0 - 1.0)*PI<double>() / 2.0, max<double>()) >= a &&
					std::nextafter((c*4.0 - 1.0)*PI<double>() / 2.0, -max<double>()) <= b)
				{
					return interval(-1.0, 1.0);
				}
				else
				{
					return interval(
						std::fmin(std::nextafter(std::sin(a), -max<double>()),
						std::nextafter(std::sin(b), -max<double>())), 1.0);
				}
			}
			else if (std::nextafter(PI<double>() / 2.0*(3.0 + c*4.0), max<double>()) >= a &&
				std::nextafter(PI<double>() / 2.0*(3.0 + c*4.0), -max<double>()) <= b)
			{
				return interval(-1.0,
					std::fmax(std::nextafter(std::sin(a), max<double>()),
					std::nextafter(std::sin(b), max<double>())));
			}
			else if (std::nextafter((c*4.0 - 1.0)*PI<double>() / 2.0, max<double>()) >= a &&
				std::nextafter((c*4.0 - 1.0)*PI<double>() / 2.0, -max<double>()) <= b)
			{
				return interval(-1.0,
					std::fmax(std::nextafter(std::sin(a), max<double>()),
					std::nextafter(std::sin(b), max<double>())));
			}
			else
			{
				return interval(
					std::fmin(std::nextafter(std::sin(a), -max<double>()),
					std::nextafter(std::sin(b), -max<double>())),
					std::fmax(std::nextafter(std::sin(a), max<double>()),
					std::nextafter(std::sin(b), max<double>())));
			}
		}
		/*interval cos*/
		const interval cos() const
		{
			int c;
			long double a = low, b = up;
			if (b - a >= 2.0*PI<double>())
			{
				return interval(-1.0, 1.0);
			}
			/*  base point set  */
			c = static_cast<int>(std::nextafter(b / 2.0 / PI<double>(), max<double>()));
			/* checking  */
			if (std::nextafter(PI<double>()*2.0*c, max<double>()) >= a &&
				std::nextafter(PI<double>()*2.0*c, -max<double>()) <= b)
			{
				if (std::nextafter(PI<double>()*(1.0 + 2.0*c), max<double>()) >= a &&
					std::nextafter(PI<double>()*(1.0 + 2.0*c), -max<double>()) <= b)
				{
					return interval(-1.0, 1.0);
				}
				else if (std::nextafter((c*2.0 - 1.0)*PI<double>(), max<double>()) >= a &&
					std::nextafter((c*2.0 - 1.0)*PI<double>(), -max<double>()) <= b)
				{
					return interval(-1.0, 1.0);
				}
				else
				{
					return interval(
						std::fmin(std::nextafter(std::cos(a), -max<double>()),
						std::nextafter(std::cos(b), -max<double>())), 1.0);
				}
			}
			else if (std::nextafter(PI<double>()*(2.0*c + 1.0), max<double>()) >= a &&
				std::nextafter(PI<double>()*(2.0*c + 1.0), -max<double>()) <= b)
			{
				return interval(-1.0,
					std::fmax(std::nextafter(std::cos(a), max<double>()),
					std::nextafter(std::cos(b), max<double>())));
			}
			else if (std::nextafter((c*2.0 - 1.0)*PI<double>(), max<double>()) >= a &&
				std::nextafter((c*2.0 - 1.0)*PI<double>(), -max<double>()) <= b)
			{
				return interval(-1.0,
					std::fmax(std::nextafter(std::cos(a), max<double>()),
					std::nextafter(std::cos(b), max<double>())));
			}
			else
			{
				return interval(
					std::fmin(std::nextafter(std::cos(a), -max<double>()),
					std::nextafter(std::cos(b), -max<double>())),
					std::fmax(std::nextafter(std::cos(a), max<double>()),
					std::nextafter(std::cos(b), max<double>())));
			}
		}

		/*  interval exp  */
		const interval exp() const
		{
			return interval(
				std::nextafter(std::exp(low), -max<double>()),
				std::nextafter(std::exp(up), max<double>()));
		}

		/*  interval absolute */
		const interval abs() const
		{
			if (low < 0)
			{
				return interval(0.0, std::fmax(std::abs(low), std::abs(up)));
			}
			else{
				return *this;
			}
		}
		/*  interval wid  */
		double wid() const
		{
			return up - low;
		}
		/*interval middle point*/
		double mid() const
		{
			return (low + up) / 2.0;
		}
		/*low bound getter*/
		double low_bound() const
		{
			return low;
		}
		/*upper bound getter*/
		double up_bound() const
		{
			return up;
		}
		/*out put function*/
		std::ostream& print(std::ostream& s) const
		{
			s << "[ " << low << " , " << up << " ]";
			return s;
		}
	private:
		double low;//lower bound value
		double up;//upper bound value
	};

}//namespace end

using Interval::interval;
typedef std::shared_ptr<interval> pinterval;

/*generic ver interval numeric functions overload*/
interval sin(const interval& a) { return a.sin(); }
interval cos(const interval& a) { return a.cos(); }
interval exp(const interval& a) { return a.exp(); }
interval pow(const interval& a, int const& n) { return a.pow(n); }
interval abs(const interval& a) { return a.abs(); }

/*generic numeric functions for primitive dispach*/
/*Using Forwarding Reference & type_traits*/
template <typename T, typename = typename std::enable_if <
	!std::is_same<interval, typename std::decay<T>::type>::value > ::type>
	double sin(T&& x) { return std::sin(std::forward<T>(x)); }

template <typename T, typename = typename std::enable_if <
	!std::is_same<interval, typename std::decay<T>::type>::value > ::type>
	double cos(T&& x) { return std::cos(std::forward<T>(x)); }

template <typename T, typename = typename std::enable_if <
	!std::is_same<interval, typename std::decay<T>::type>::value > ::type>
	double exp(T&& x) { return std::exp(std::forward<T>(x)); }

template <typename T, typename = typename std::enable_if <
	!std::is_same<interval, typename std::decay<T>::type>::value > ::type>
	double pow(T&& x, int n) { return std::pow(std::forward<T>(x), n); }

template <typename T, typename = typename std::enable_if <
	!std::is_same<interval, typename std::decay<T>::type>::value > ::type>
	double abs(T&& x) { return std::abs(std::forward<T>(x)); }

/*generic operator<< for classes have menber func print( ostream )*/
/*Using SFINAE rule*/
template<typename T>
auto operator<<(std::ostream& s, T&& x)->decltype(x.print(s))
{
	return (x.print(s));
}

/*generic wid*/
template< typename T >
auto wid(T&& x)->decltype(x.wid())
{
	return x.wid();
}
/*generic mid*/
template< typename T >
auto mid(T&& x)->decltype(x.mid())
{
	return x.mid();
}

//interval addition operator
template <typename T, typename = typename std::enable_if <
	!std::is_same<interval, typename std::decay<T>::type>::value > ::type>
	interval operator +(T&& x, const interval& y) { return (interval(y.low_bound() + x, y.up_bound() + x)); }

template <typename T, typename = typename std::enable_if <
	!std::is_same<interval, typename std::decay<T>::type>::value > ::type>
	interval operator +(const interval& x, T&& y) { return (interval(x.low_bound() + y, x.up_bound() + y)); }

const interval operator +(const interval& x, const interval& y)
{
	return interval(x) += y;
}
//interval subtraction operator
template <typename T, typename = typename std::enable_if <
	!std::is_same<interval, typename std::decay<T>::type>::value > ::type>
	interval operator -(T&& x, const interval& y) { return (interval(y.low_bound() - x, y.up_bound() - x)); }

template <typename T, typename = typename std::enable_if <
	!std::is_same<interval, typename std::decay<T>::type>::value > ::type>
	interval operator -(const interval& x, T&& y) { return (interval(x.low_bound - y, x.up_bound() - y)); }

const interval operator -(const interval& x, const interval& y)
{
	return interval(x) -= y;
}
//interval multiplication operator
template <typename T, typename = typename std::enable_if <
	!std::is_same<interval, typename std::decay<T>::type>::value > ::type>
	interval operator *(T&& x, const interval& y)
{
	if (x >= 0)
		return (interval(y.low_bound() * x, y.up_bound() * x));
	else
		return (interval(y.up_bound() * x, y.low_bound() * x));
}

template <typename T, typename = typename std::enable_if <
	!std::is_same<interval, typename std::decay<T>::type>::value > ::type>
	interval operator *(const interval& x, T&& y) 
{
	if (y >= 0)
		return (interval(x.low_bound() * y, x.up_bound() * y));
	else
		return (interval(x.up_bound() * y, x.low_bound() * y));
}

const interval operator *(const interval& x, const interval& y)
{
	return interval(x) *= y;
}
//interval division operator
template <typename T, typename = typename std::enable_if <
	!std::is_same<interval, typename std::decay<T>::type>::value > ::type>
	interval operator /(T&& x, const interval& y)
{
	if (y.low_bound() <= 0 && 0 <= y.up_bound())
		return IntervalZeroDivideExcep("");
	else if (y > 0)
		return (interval(y.low_bound() / x, y.up_bound() / x));
	else
		return (interval(y.up_bound() / x, y.low_bound() / x));
}


template <typename T, typename = typename std::enable_if <
	!std::is_same<interval, typename std::decay<T>::type>::value > ::type>
	interval operator /(const interval& x, T&& y)
{
	if (y == 0)
		throw IntervalZeroDivideExcep("");
	else if (y > 0)
		return (interval(x.low_bound() / y, x.up_bound() / y));
	else
		return (interval(x.up_bound() / y, x.low_bound() / y));
}

const interval operator /(const interval& x, const interval& y)
{
	return interval(x) /= y;
}

//compound assignment operator
template <typename T, typename = typename std::enable_if <
	!std::is_same<interval, typename std::decay<T>::type>::value > ::type>
	interval operator +=(interval& x, T&& y) 
{
	return x.operator+=(interval(y,y));
}

template <typename T, typename = typename std::enable_if <
	!std::is_same<interval, typename std::decay<T>::type>::value > ::type>
	interval operator -=(interval& x, T&& y)
{
	return x.operator-=(interval(y, y));
}

template <typename T, typename = typename std::enable_if <
	!std::is_same<interval, typename std::decay<T>::type>::value > ::type>
	interval operator *=(interval& x, T&& y)
{
	return x.operator*=(interval(y, y));
}

template <typename T, typename = typename std::enable_if <
	!std::is_same<interval, typename std::decay<T>::type>::value > ::type>
	interval operator /=(interval& x, T&& y)
{
	return x.operator/=(interval(y, y));
}

//shared_ptr ver. functions
template<typename T>
std::shared_ptr<T> operator +(std::shared_ptr<T> x, std::shared_ptr<T> y)
{
	return std::shared_ptr<T>(new T(std::forward<T>(*x) + std::forward<T>(*y)));
}

template<typename T>
std::shared_ptr<T> operator -(std::shared_ptr<T> x, std::shared_ptr<T> y)
{
	return std::shared_ptr<T>(new T(std::forward<T>(*x) - std::forward<T>(*y)));
}

template<typename T>
std::shared_ptr<T> operator *(std::shared_ptr<T> x, std::shared_ptr<T> y)
{
	return std::shared_ptr<T>(new T(std::forward<T>(*x) * std::forward<T>(*y)));
}

template<typename T>
std::shared_ptr<T> operator /(std::shared_ptr<T> x, std::shared_ptr<T> y)
{
	return std::shared_ptr<T>(new T(std::forward<T>(*x) / std::forward<T>(*y)));
}

template<typename T>
std::ostream& operator<<(std::ostream& s, std::shared_ptr<T> x)
{
	s << (*x);
	return (s);
}


#endif