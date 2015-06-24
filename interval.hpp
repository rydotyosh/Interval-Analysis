#ifndef INTERVAL_H
#define INTERVAL_H
#include "exception.hpp"
#include <limits>
#include <cmath>
#include <typeinfo>
#include <type_traits>
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <initializer_list>
#include <algorithm>

using std::nextafter;

namespace Cranberries
{
	/*  function default for meta programming  */
	extern std::nullptr_t enabler;
	
	enum class Version_Tag {
		Version1_0_00 = 0x01000000,
		Version1_0_01 = 0x01000001,
		Version1_0_02 = 0x01000002,
		Version2_0_00 = 0x02000000,
		Version3_0_00 = 0x03000000,
		now_ver = Version1_0_02,
	};
	//---------------------//
	/*   Ordering Symbol   */
	//---------------------//

	/*  3 Values of Ordering  */
	enum class partial_ordering { less, unordered, greater };
	enum class weak_ordering { less, equivalent, greater };
	enum class total_ordering { less, equal, greater };
	enum class interval_ordering { less, unordered, greater };

	/*  Advanced Interval Ordering  */
	enum class Cranberries_Relation
	{
		equal,
		interval_less,
		interval_greater,
		partial_less,
		partial_greater,
		weak_less,
		weak_greater,
		total_less,
		total_greater,
		contain,
		part_of,
		niether,
	};



	//----------------------------------//
	/*                                  */
	/*       Class Declaration          */
	/*                                  */
	/*        Interval Class            */
	/*        (Body)                    */
	/*                                  */
	//----------------------------------//



	template<typename T>
	class interval
	{
	public:

		/*  ctor  */
		constexpr interval();
		constexpr interval(T&, T&);
		constexpr interval(T&&, T&&);

		/*  dtor  */
		~interval();

		/*  copy ctor  */
		interval(const interval&);
		/*  move ctor  */
		interval(interval&&);
		/*  Copy Assignment Op  */
		interval operator=(interval&);
		/*  Move assignment Op  */
		interval operator=(interval&&);

		/*  compound assignment Op  */
		const interval operator +=(const interval&);
		const interval operator -=(const interval&);
		const interval operator *=(const interval&);
		const interval operator /=(const interval&);

		/*  increment/decrement Op  */
		const interval operator ++();
		const interval operator ++(int);
		const interval operator --();
		const interval operator --(int);

		/*  numeric function  */
		const interval sin() const;
		const interval cos() const;
		const interval tan() const;
		const interval asin() const;
		const interval acos() const;
		const interval atan() const;
		const interval sinh() const;
		const interval cosh() const;
		const interval tanh() const;
		const interval asinh() const;
		const interval acosh() const;
		const interval atanh() const;
		const interval pow(int n) const;
		const interval log() const;
		const interval log10() const;
		const interval sqrt() const;
		const interval exp() const;
		const interval abs() const;
		const interval erf() const;
		constexpr T mid() const;
		constexpr T wid() const;
		constexpr bool is_singleton() const;

		/*  accessors  */
		constexpr T low() const;
		constexpr T up() const;
		void set_up(T&);
		void set_low(T&);
		void set_up(T&&);
		void set_low(T&&);

		/*  Advanced Relational Op  */
		Cranberries_Relation relational(interval const&) const;
		bool is_contain(interval const&) const;
		bool is_part_of(interval const&) const;

		/*  Relational Op  */
		bool operator<(interval const&) const;
		bool operator>(interval const&) const;
		bool operator<=(interval const&) const;
		bool operator>=(interval const&) const;
		bool operator==(interval const&) const;
		bool operator!=(interval const&) const;

		/*  output stream function  */
		std::ostream& print(std::ostream&) const;
		/*  For printf  */
		const char* c_str() const;
		typedef T value_type;
	private:
		/*  Internal Class  */
		class impl;
		/*  Keep lower & upper bound Value   */
		std::unique_ptr<impl> pimpl;
	};


	/*PI*/
	template<typename T>
	constexpr T PI(){ return static_cast<T>(3.141592653589793238462643383279); }


	/*maximum*/
	template<typename T>
	constexpr T max(){ return std::numeric_limits<T>::max(); }

	/*minimum*/
	template<typename T>
	constexpr T min(){ return std::numeric_limits<T>::min(); }



	//----------------------------------//
	/*                                  */
	/*       class declaration          */
	/*                                  */
	/*   interval implementation class  */
	/*   (internal class of interval)   */
	/*                                  */
	//----------------------------------//


	template<typename T>
	class interval<T>::impl
	{
	public:
		/*  Set Zero Value  */
		void do_internal_work()
		{
			lower_bound = T{};
			upper_bound = T{};
		}
		/*  Set Initial Value */
		void do_internal_work(T& low, T& up)
		{
			lower_bound = low;
			upper_bound = up;
		}
		/*  Compound Assignment Op Implementation */
		const impl operator +=(const impl&);
		const impl operator -=(const impl&);
		const impl operator *=(const impl&);
		const impl operator /=(const impl&);

		/*  Increment/decrement Op Implementation */
		const impl operator ++();
		const impl operator ++(int);
		const impl operator --();
		const impl operator --(int);

		/*  getter Implementation  */
		constexpr T low() const;
		constexpr T up() const;

		/*  Output Stream Function  */
		std::ostream& print(std::ostream&) const;

		/*  setter Implementation */
		void set_low(T& x)
		{
			if (x > upper_bound){
				throw invalid_argument("lower_bound must be less than upper_bound!");
			}
			lower_bound = x;
		}
		void set_low(T&& x)
		{
			if (x > upper_bound){
				throw invalid_argument("lower_bound must be less than upper_bound!");
			}
			lower_bound = std::move(x);
		}
		void set_up(T& x)
		{
			if (x < lower_bound){
				throw invalid_argument("upper_bound must be greater than lower_bound!");
			}
			upper_bound = x;
		}
		void set_up(T&& x)
		{
			if (x < lower_bound){
				throw invalid_argument("upper_bound must be greater than lower_bound!");
			}
			upper_bound = std::move(x);
		}
		void deep_copy(impl& x)
		{
			lower_bound = x.lower_bound;
			upper_bound = x.upper_bound;
		}
	private:
		/*  Lower Bound  */
		T lower_bound = T{};

		/*  Upper Bound  */
		T upper_bound = T{};
	};




	//------------------------------//
	/*                              */
	/*  Interval Member Functions   */
	/*                              */
	//------------------------------//


	/*  Interval Compound Assignment Op Definition  */

	template<typename T>
	const interval<T> interval<T>::operator +=(const interval& x)
	{
		pimpl->operator+=(*(x.pimpl));
		if (this->up() == max<T>() || this->low() == -max<T>())
		{
			throw Cranberries::over_flow("overflow occurred.");
		}
		return *this;
	}
	template<typename T>
	const interval<T> interval<T>::operator -=(const interval& x)
	{
		pimpl->operator-=(*(x.pimpl));
		if (this->up() == max<T>() || this->low() == -max<T>())
		{
			throw Cranberries::over_flow("overflow occurred.");
		}
		return *this;
	}
	template<typename T>
	const interval<T> interval<T>::operator *=(const interval& x)
	{
		pimpl->operator*=(*(x.pimpl));
		if (this->up() == max<T>() || this->low() == -max<T>())
		{
			throw Cranberries::over_flow("overflow occurred.");
		}
		return *this;
	}
	template<typename T>
	const interval<T> interval<T>::operator /=(const interval& x)
	{
		pimpl->operator/=(*(x.pimpl));
		if (this->up() == max<T>() || this->low() == -max<T>())
		{
			throw Cranberries::over_flow("overflow occurred.");
		}
		return *this;
	}

	/*  Interval Increment/Decrement Op Definition  */

	//prefix
	template<typename T>
	const interval<T> interval<T>::operator ++()
	{
		(*pimpl)++;
		if (this->up() == max<T>() || this->low() == -max<T>())
		{
			throw Cranberries::over_flow("overflow occurred.");
		}
		return *this;
	}
	//postfix
	template<typename T>
	const interval<T> interval<T>::operator ++(int)
	{
		auto tmp(*this);
		(*pimpl)++;
		if (this->up() == max<T>() || this->low() == -max<T>())
		{
			throw Cranberries::over_flow("overflow occurred.");
		}
		return tmp;

	}
	//prefix
	template<typename T>
	const interval<T> interval<T>::operator --()
	{
		(*pimpl)--;
		return *this;
	}
	//postfix
	template<typename T>
	const interval<T> interval<T>::operator --(int)
	{
		auto tmp(*this);
		(*pimpl)--;
		return tmp;
	}

	

	//--------------------------------------------//
	//							                  //
	/*    Interval Numeric Function Definition    */
	//                                            //
	//--------------------------------------------//


	/*  Interval Cos  */
	template<typename T>
	const interval<T> interval<T>::cos() const
	{
		int c;
		auto a = pimpl->low(), b = pimpl->up();
		if (b - a >= 2.0*PI<T>())
		{
			return interval<T>(static_cast<T>(-1.0), static_cast<T>(1.0));
		}
		/*  base point set  */
		if (static_cast<int>(nextafter((a / 2.0 / PI<T>()), max<T>())) > a)
			c = static_cast<int>(nextafter((a / 2.0 / PI<T>()), max<T>()));
		else
			c = static_cast<int>(nextafter((a / 2.0 / PI<T>()), max<T>())) + 1;
		/*  checking phase  */
		if (nextafter(PI<T>()*2.0*c, max<T>()) >= a &&
			nextafter(PI<T>()*2.0*c, -max<T>()) <= b)
		{
			if (nextafter(PI<T>()*(1.0 + 2.0*c), max<T>()) >= a &&
				nextafter(PI<T>()*(1.0 + 2.0*c), -max<T>()) <= b)
			{
				return interval<T>(static_cast<T>(-1.0), static_cast<T>(1.0));
			}
			else if (nextafter((c*2.0 - 1.0)*PI<T>(), max<T>()) >= a &&
				nextafter((c*2.0 - 1.0)*PI<T>(), -max<T>()) <= b)
			{
				return interval<T>(static_cast<T>(-1.0), static_cast<T>(1.0));
			}
			else
			{
				return interval<T>(
					std::fmin(nextafter(std::cos(a), -max<T>()),
					nextafter(std::cos(b), -max<T>())), static_cast<T>(1.0));
			}
		}
		else if (nextafter(PI<T>()*(2.0*c + 1.0), max<T>()) >= a &&
			nextafter(PI<T>()*(2.0*c + 1.0), -max<T>()) <= b)
		{
			return interval<T>(static_cast<T>(-1.0),
				std::fmax(nextafter(std::cos(a), max<T>()),
				nextafter(std::cos(b), max<T>())));
		}
		else if (nextafter((c*2.0 - 1.0)*PI<T>(), max<T>()) >= a &&
			nextafter((c*2.0 - 1.0)*PI<T>(), -max<T>()) <= b)
		{
			return interval<T>(static_cast<T>(-1.0),
				std::fmax(nextafter(std::cos(a), max<T>()),
				nextafter(std::cos(b), max<T>())));
		}
		else
		{
			return interval<T>(
				std::fmin(nextafter(std::cos(a), -max<T>()),
				nextafter(std::cos(b), -max<T>())),
				std::fmax(nextafter(std::cos(a), max<T>()),
				nextafter(std::cos(b), max<T>())));
		}
	}


	/*  Interval Sin  */
	template<typename T>
	const interval<T> interval<T>::sin() const
	{
		int c;
		auto a = pimpl->low(), b = pimpl->up();
		if (b - a >= 2.0*PI<T>())
		{
			return interval<T>(static_cast<T>(-1.0), static_cast<T>(1.0));
		}
		/*  base point set  */
		if (static_cast<int>(nextafter((a * 2.0 / PI<T>() - 1.0) / 4.0, max<T>())) > a)
			c = static_cast<int>(nextafter((a * 2.0 / PI<T>() - 1.0) / 4.0, max<T>()));
		else
			c = static_cast<int>(nextafter((a * 2.0 / PI<T>() - 1.0) / 4.0, max<T>())) + 1;
		/*  checking phase  */
		if (nextafter(PI<T>() / 2.0*(1.0 + c*4.0), max<T>()) >= a &&
			nextafter(PI<T>() / 2.0*(1.0 + c*4.0), -max<T>()) <= b)
		{
			if (nextafter(PI<T>() / 2.0*(3.0 + c*4.0), max<T>()) >= a &&
				nextafter(PI<T>() / 2.0*(3.0 + c*4.0), -max<T>()) <= b)
			{
				return interval<T>(static_cast<T>(-1.0), static_cast<T>(1.0));
			}
			else if (nextafter((c*4.0 - 1.0)*PI<T>() / 2.0, max<T>()) >= a &&
				nextafter((c*4.0 - 1.0)*PI<T>() / 2.0, -max<T>()) <= b)
			{
				return interval<T>(static_cast<T>(-1.0), static_cast<T>(1.0));
			}
			else
			{
				return interval<T>(
					std::fmin(nextafter(std::sin(a), -max<T>()),
					nextafter(std::sin(b), -max<T>())), static_cast<T>(1.0));
			}
		}
		else if (nextafter(PI<T>() / 2.0*(3.0 + c*4.0), max<T>()) >= a &&
			nextafter(PI<T>() / 2.0*(3.0 + c*4.0), -max<T>()) <= b)
		{
			return interval<T>(static_cast<T>(-1.0),
				std::fmax(nextafter(std::sin(a), max<T>()),
				nextafter(std::sin(b), max<T>())));
		}
		else if (nextafter((c*4.0 - 1.0)*PI<T>() / 2.0, max<T>()) >= a &&
			nextafter((c*4.0 - 1.0)*PI<T>() / 2.0, -max<T>()) <= b)
		{
			return interval<T>(static_cast<T>(-1.0),
				std::fmax(nextafter(std::sin(a), max<T>()),
				nextafter(std::sin(b), max<T>())));
		}
		else
		{
			return interval<T>(
				std::fmin(nextafter(std::sin(a), -max<T>()),
				nextafter(std::sin(b), -max<T>())),
				std::fmax(nextafter(std::sin(a), max<T>()),
				nextafter(std::sin(b), max<T>())));
		}
	}


	/*  Interval Tan  */
	template<typename T>
	const interval<T> interval<T>::tan() const
	{
		int c;
		auto a = pimpl->low(), b = pimpl->up();
		if (b - a >= PI<T>())
		{
			throw Cranberries::over_flow("tan");
		}
		/*  base point set  */
		if (static_cast<int>(nextafter(a * 2.0 / PI<T>(), max<T>())) < a)
			c = static_cast<int>(nextafter(a * 2.0 / PI<T>(), max<T>()));
		else
			c = static_cast<int>(nextafter(a * 2.0 / PI<T>(), max<T>())) + 1 ;
		/*  checking phase  */
		if (a < (PI<T>() / static_cast<T>(2.0) * c) && (PI<T>() / static_cast<T>(2.0) * c) < b) {
			throw Cranberries::over_flow("tan");
		}
		else if (a < (PI<T>() / static_cast<T>(2.0) * c - PI<T>()) && (PI<T>() / static_cast<T>(2.0) * c - PI<T>()) < b) {
			throw Cranberries::over_flow("tan");
		}
		else if (a < PI<T>() / static_cast<T>(2.0) * c + PI<T>() && PI<T>() / static_cast<T>(2.0) * c + PI<T>() < b) {
			throw Cranberries::over_flow("tan");
		}
		else if (nextafter(std::tan(a), -max<T>()) == -max<T>() || nextafter(std::tan(b), -max<T>()) == max<T>()) {
			throw Cranberries::over_flow("tan");
		}
		else {
			return interval<T>(nextafter(std::tan(a),-max<T>()), nextafter(std::tan(b),max<T>()));
		}
	}


	/*  Interval ArcCos  */
	template<typename T>
	const interval<T> interval<T>::acos() const
	{
		auto a = pimpl->low(), b = pimpl->up();
		if (a < -1.0 || b > 1.0) {
			throw Cranberries::invalid_argument("acos");
		}
		else {
			return interval<T>(nextafter(std::acos(a), -max<T>()), nextafter(std::acos(b), max<T>()));
		}
	}


	/*  Interval ArcSin  */
	template<typename T>
	const interval<T> interval<T>::asin() const
	{
		auto a = pimpl->low(), b = pimpl->up();
		if (a < -1.0 || b > 1.0) {
			throw Cranberries::invalid_argument("asin");
		}
		else {
			return interval<T>(nextafter(std::asin(b), -max<T>()), nextafter(std::asin(a), max<T>()));
		}
	}


	/*  Interval ArcTan  */
	template<typename T>
	const interval<T> interval<T>::atan() const
	{
		auto a = pimpl->low(), b = pimpl->up();
		if (a < -1.0 || b > 1.0) {
			throw Cranberries::invalid_argument("asin");
		}
		else {
			return interval<T>(nextafter(std::atan(a), -max<T>()), nextafter(std::atan(b), max<T>()));
		}
	}


	/*  Interval Cosh  */
	template<typename T>
	const interval<T> interval<T>::cosh() const
	{
		auto a = pimpl->low(), b = pimpl->up();
		if (nextafter(std::cosh(a), max<T>()) == max<T>() || nextafter(std::cosh(b), max<T>()) == max<T>()) {
			throw Cranberries::over_flow("cosh");
		}
		if (a < 0.0 && b > 0.0) {
			return interval<T>(1.0, std::fmax(nextafter(std::cosh(a), max<T>()), nextafter(std::cosh(b), max<T>())));
		}
		else if (b < 0.0) {
			return interval<T>(nextafter(std::cosh(b), -max<T>()), nextafter(std::cosh(a), max<T>()));
		}
		else if(a > 0.0){
			return interval<T>(nextafter(std::cosh(a), -max<T>()), nextafter(std::cosh(b), max<T>()));
		}
		else {
			throw Cranberries::domain_error("cosh");
		}
	}


	/*  Interval Sinh  */
	template<typename T>
	const interval<T> interval<T>::sinh() const
	{
		auto a = pimpl->low(), b = pimpl->up();
		auto l = nextafter(std::sinh(a), -max<T>()), r = nextafter(std::sinh(b),max<T>());
		if (a == -max<T>() || b == max<T>()) {
			throw Cranberries::over_flow("sinh");
		}
		else {
			return interval<T>(l,r);
		}
	}


	/*  Interval Tanh  */
	template<typename T>
	const interval<T> interval<T>::tanh() const
	{
		return interval<T>(nextafter(std::tanh(pimpl->low()), -max<T>()), nextafter(std::tanh(pimpl->up()), -max<T>()));
	}


	/*  Interval ArcCosh  */
	template<typename T>
	const interval<T> interval<T>::acosh() const
	{
		auto a = pimpl->low();
		auto b = pimpl->up();
		if (a < static_cast<T>(1.0)) { 
			throw Cranberries::invalid_argument("acosh"); 
		}
		else {
			return interval<T>(nextafter(std::tanh(a), -max<T>()), nextafter(std::tanh(b), -max<T>()));
		}
	}
	/*  Interval ArcSinh  */
	template<typename T>
	const interval<T> interval<T>::asinh() const
	{
		return interval<T>(nextafter(std::asinh(pimpl->low()), -max<T>()), nextafter(std::asinh(pimpl->up()), -max<T>()));
	}

	/*  Interval ArcTanh  */
	template<typename T>
	const interval<T> interval<T>::atanh() const
	{
		return interval<T>(nextafter(std::atanh(pimpl->low()), -max<T>()), nextafter(std::atanh(pimpl->up()), -max<T>()));
	}

	/*  Interval Pow  */
	template<typename T>
	const interval<T> interval<T>::pow(int n) const
	{
		if (n < 0){
			auto tmp = 1 / (*this);
			return tmp.pow(-1 * n);
		}
		else if (n == 0){
			return interval<T>(static_cast<T>(1.0), static_cast<T>(1.0));
		}
		else if (n % 2 == 0){
			if (pimpl->up() <= 0.0){
				if (nextafter(std::pow(pimpl->low(), n), max<T>()) == max<T>()
					|| nextafter(std::pow(pimpl->up(), n), -max<T>()) == -max<T>())
				{
					throw Cranberries::over_flow("overflow occurred.");
				}
				return interval<T>(
					nextafter(std::pow(pimpl->up(), n), -max<T>()),
					nextafter(std::pow(pimpl->low(), n), max<T>()));
			}
			else if (pimpl->low() <= T{} && pimpl->up() >= T{}){
				if (std::fmax(
					nextafter(std::pow(pimpl->low(), n), -max<T>()),
					nextafter(std::pow(pimpl->up(), n), max<T>())) == max<T>())
				{
					throw Cranberries::over_flow("overflow occurred.");
				}
				return interval<T>(T{}, std::fmax(
					nextafter(std::pow(pimpl->low(), n), -max<T>()),
					nextafter(std::pow(pimpl->up(), n), max<T>())));
			}
		}
		if (nextafter(std::pow(pimpl->up(), n), max<T>()) == max<T>() || 
			nextafter(std::pow(pimpl->low(), n), -max<T>()) == max<T>())
		{
			throw Cranberries::over_flow("overflow occurred.");
		}
		return interval<T>(
			nextafter(std::pow(pimpl->low(), n), -max<T>()),
			nextafter(std::pow(pimpl->up(), n), max<T>()));
	}


	/*  Interval Sqrt  */
	template<typename T>
	const interval<T> interval<T>::sqrt() const
	{
		if (pimpl->low() < T{}){ throw Cranberries::logic_error("sqrt arg requires positive number"); }
		return interval<T>(
			nextafter(std::sqrt(pimpl->low()), -max<T>()),
			nextafter(std::sqrt(pimpl->up()), max<T>()));
	}


	/*  Interval Exp  */
	template<typename T>
	const interval<T> interval<T>::exp() const
	{
		if (nextafter(std::exp(pimpl->up()), max<T>()) == max<T>() || nextafter(std::exp(pimpl->low()), -max<T>()) == -max<T>())
		{
			throw Cranberries::over_flow("overflow occurred.");
		}
		return interval<T>(
			nextafter(std::exp(pimpl->low()), -max<T>()),
			nextafter(std::exp(pimpl->up()), max<T>()));
	}


	/*  Interval Log  */
	template<typename T>
	const interval<T> interval<T>::log() const
	{
		if (pimpl->low() <= 0.0){ throw Cranberries::logic_error("anti-logarithm less than or equal to zero"); }
		return interval<T>(
			nextafter(std::log(pimpl->low()), -max<T>()),
			nextafter(std::log(pimpl->up()), max<T>()));
	}


	/*  Interval Log10  */
	template<typename T>
	const interval<T> interval<T>::log10() const
	{
		if (pimpl->low() <= 0.0){ throw Cranberries::logic_error("anti-logarithm less than or equal to zero"); }
		return interval<T>(
			nextafter(std::log10(pimpl->low()), -max<T>()),
			nextafter(std::log10(pimpl->up()), max<T>()));
	}


	/*  Interval Abs  */
	template<typename T>
	const interval<T> interval<T>::abs() const
	{
		if (pimpl->low() < T{} && pimpl->up() > T{})
		{
			return interval<T>(T{}, nextafter(std::fmax(std::abs(pimpl->low()), std::abs(pimpl->up())), max<T>()));
		}
		if (pimpl->up() < T{})
		{
			return interval<T>(-pimpl->up(), -pimpl->low());
		}
		else{
			return interval<T>(*this);
		}
	}

	/*  Interval Error Functiontion  */
	template<typename T>
	const interval<T> interval<T>::erf() const
	{
		return interval<T>(std::erf(pimpl->low()), std::erf(pimpl->up()));
	}

	/*  is_singleton  */
	template<typename T>
	constexpr bool interval<T>::is_singleton() const
	{
		return (pimpl->low() == pimpl->up()) ? true : false;
	}

	/*  middle point  */
	template<typename T>
	constexpr T interval<T>::mid() const
	{
		return (pimpl->up() + pimpl->low()) / static_cast<T>(2.0);
	}


	/*  width  */
	template<typename T>
	constexpr T interval<T>::wid() const
	{
		return pimpl->up() - pimpl->low();
	}

	/*  Interval Getter and Setter  */

	template<typename T>
	constexpr T interval<T>::low() const
	{
		return pimpl->low();
	}
	template<typename T>
	constexpr T interval<T>::up() const
	{
		return pimpl->up();
	}
	template<typename T>
	void interval<T>::set_up(T& x)
	{
		pimpl->set_up(x);
	}
	template<typename T>
	void interval<T>::set_up(T&& x)
	{
		pimpl->set_up(x);
	}
	template<typename T>
	void interval<T>::set_low(T& x)
	{
		pimpl->set_low(x);
	}
	template<typename T>
	void interval<T>::set_low(T&& x)
	{
		pimpl->set_low(x);
	}

	/*  Interval Advanced Relational Discriminator Function  */

	template<typename T>
	Cranberries_Relation interval<T>::relational(interval<T> const& x) const
	{
		if (total_equal(*this, x))
		{
			return Cranberries_Relation::equal;
		}
		else if (pimpl->low() == (x.pimpl)->low() && pimpl->up() < (x.pimpl)->up())
		{
			return Cranberries_Relation::total_less;
		}
		else if (pimpl->low() == (x.pimpl)->low() && pimpl->up() > (x.pimpl)->up())
		{
			return Cranberries_Relation::total_greater;
		}
		else if (pimpl->low() < (x.pimpl)->low() && pimpl->up() == (x.pimpl)->up())
		{
			return Cranberries_Relation::weak_less;
		}
		else if (pimpl->low() > (x.pimpl)->low() && pimpl->up() == (x.pimpl)->up())
		{
			return Cranberries_Relation::weak_greater;
		}
		else if (pimpl->low() < (x.pimpl)->low() && pimpl->up() < (x.pimpl)->up())
		{
			return Cranberries_Relation::partial_less;
		}
		else if (pimpl->low() > (x.pimpl)->low() && pimpl->up() > (x.pimpl)->up())
		{
			return Cranberries_Relation::partial_less;
		}
		else if (pimpl->low() <= (x.pimpl)->low() && pimpl->up() >= (x.pimpl)->up())
		{
			return Cranberries_Relation::contain;
		}
		else if (pimpl->low() >= (x.pimpl)->low() && pimpl->up() <= (x.pimpl)->up())
		{
			return Cranberries_Relation::part_of;
		}
		else
		{
			return Cranberries_Relation::niether;
		}
	}


	/*  Interval Relational Op Definition  */

	template<typename T>
	bool interval<T>::operator<(interval<T> const& x) const
	{
		return total_less(*this, x);
	}
	template<typename T>
	bool interval<T>::operator>(interval<T> const& x) const
	{
		return total_greater(*this, x);
	}

	template<typename T>
	bool interval<T>::operator<=(interval<T> const& x) const
	{
		return pimpl->low() <= (x.pimpl)->low();
	}
	template<typename T>
	bool interval<T>::operator>=(interval<T> const& x) const
	{
		return pimpl->low() >= (x.pimpl)->low();
	}

	template<typename T>
	bool interval<T>::operator==(interval<T> const& x) const
	{
		return total_equal(*this, x);
	}
	template<typename T>
	bool interval<T>::operator!=(interval<T> const& x) const
	{
		return !total_equal(*this, x);
	}


	//-----------------------------//
	/*                             */
	/*      Interval Ctor/Dtor     */
	/*                             */
	//-----------------------------//


	/*  Default Ctor  */

	template<typename T>
	constexpr interval<T>::interval()
		: pimpl{ std::make_unique<impl>() }
	{
		pimpl->do_internal_work();
	}

	/*  Two Lvalue Arg Ctor  */

	template<typename T>
	constexpr interval<T>::interval(T& low, T& up)
		: pimpl{ std::make_unique<impl>() }
	{
		if (low > up){ throw invalid_argument("upper_bound less than lower_bound!"); }
		pimpl->do_internal_work(low, up);
	}

	/*  Two Rvalue Arg Ctor  */

	template<typename T>
	constexpr interval<T>::interval(T&& low, T&& up)
		: pimpl{ std::make_unique<impl>() }
	{
		if (low > up){ throw invalid_argument("upper_bound less than lower_bound!"); }
		pimpl->do_internal_work(low, up);
	}

	/*  Default Dtor  */

	template<typename T>
	interval<T>::~interval() = default;

	/*  Move Ctor  */

	template<typename T>
	interval<T>::interval(interval&& x)
		:pimpl{ std::make_unique<impl>() }
	{
		std::swap(this->pimpl,x.pimpl);
		x.pimpl = nullptr;
	}

	/*  Copy Ctor  */

	template<typename T>
	interval<T>::interval(const interval& x)
		: pimpl{ std::make_unique<impl>() }
	{
		pimpl->deep_copy(*(x.pimpl));
	}

	/*  Copy Assignment Op  */

	template<typename T>
	interval<T> interval<T>::operator=(interval<T>& x)
	{
		(this->pimpl).swap( (x.pimpl) ) ;
		pimpl->deep_copy(*(x.pimpl));
		return *this;
	}

	/*  Move Assignment Op  */

	template<typename T>
	interval<T> interval<T>::operator=(interval<T>&& x)
	{
		std::swap(this->pimpl,x.pimpl);
		x.pimpl = nullptr;
		return *this;
	}

	/*  Interval Output Function  */

	template<typename T>
	std::ostream& interval<T>::print(std::ostream& s) const
	{
		return pimpl->print(s);
	}


	//--------------------------//
	/*                          */
	/*     Impl Member Function     */
	/*                          */
	//--------------------------//


	/*  Compound Assignment Op Definition  */

	template<typename T>
	const typename interval<T>::impl interval<T>::impl::operator+=(const typename interval<T>::impl& x)
	{
		lower_bound = nextafter(lower_bound + x.lower_bound, -max<T>());
		upper_bound = nextafter(upper_bound + x.upper_bound, max<T>());
		return *this;
	}
	template<typename T>
	const typename interval<T>::impl interval<T>::impl::operator-=(const typename interval<T>::impl& x)
	{
		if (this == &x)
			return impl();
		lower_bound = nextafter(lower_bound - x.upper_bound, -max<T>());
		upper_bound = nextafter(upper_bound - x.lower_bound, max<T>());
		return *this;
	}
	template<typename T>
	const typename interval<T>::impl interval<T>::impl::operator*=(const typename interval<T>::impl& x)
	{
		auto l = lower_bound;
		auto r = upper_bound;

		if (this == &x)
		{
			if (r <= T{})
			{
				lower_bound = nextafter(r * r, -max<T>());
				upper_bound = nextafter(l * l, max<T>());
				return *this;
			}
			else if (lower_bound <= T{} && upper_bound >= T{})
			{
				lower_bound = T{};
				upper_bound = std::fmax(nextafter(l * l, max<T>()), nextafter(r * r, max<T>()));
				return *this;
			}
			else
			{
				lower_bound = nextafter(l * l, -max<T>());
				upper_bound = nextafter(r * r, max<T>());
				return *this;
			}
		}
		else if (l >= T{} && x.lower_bound >= T{}){
			lower_bound = nextafter(l * x.lower_bound, -max<T>());
			upper_bound = nextafter(r * x.upper_bound, max<T>());
			return *this;
		}
		else if (l >= T{} && x.lower_bound < T{} && x.upper_bound > T{})
		{
			lower_bound = nextafter(upper_bound * x.lower_bound, -max<T>());
			upper_bound = nextafter(upper_bound * x.upper_bound, max<T>());
			return *this;
		}
		else if (l >= T{} && x.upper_bound <= T{})
		{
			lower_bound = nextafter(r * x.lower_bound, -max<T>());
			upper_bound = nextafter(l * x.upper_bound, max<T>());
			return *this;
		}
		else if (l < T{} && r > T{} && x.lower_bound >= T{})
		{
			lower_bound = nextafter(l * x.upper_bound, -max<T>());
			upper_bound = nextafter(r * x.upper_bound, max<T>());
			return *this;
		}
		else if (l < T{} && r > T{} && x.upper_bound <= T{})
		{
			lower_bound = nextafter(r * x.lower_bound, -max<T>());
			upper_bound = nextafter(l * x.lower_bound, max<T>());
			return *this;
		}
		else if (r <= T{} && x.lower_bound >= T{})
		{
			lower_bound = nextafter(l * x.upper_bound, -max<T>());
			upper_bound = nextafter(r * x.lower_bound, max<T>());
			return *this;
		}
		else if (r <= T{} && x.lower_bound < T{} && x.upper_bound > T{})
		{
			lower_bound = nextafter(l * x.upper_bound, -max<T>());
			upper_bound = nextafter(l * x.lower_bound, max<T>());
			return *this;
		}
		else if (r <= T{} && x.upper_bound <= T{})
		{
			lower_bound = nextafter(r * x.upper_bound, -max<T>());
			upper_bound = nextafter(l * x.lower_bound, max<T>());
			return *this;
		}
		else{
			lower_bound = std::fmin(nextafter(r * x.lower_bound, -max<T>()),nextafter(l * x.upper_bound, -max<T>()));
			upper_bound = std::fmax(nextafter(r * (x.upper_bound), max<T>()), nextafter(l * (x.lower_bound), max<T>()));
			return *this;
		}
	}
	template<typename T>
	const typename interval<T>::impl interval<T>::impl::operator/=(const typename interval<T>::impl& x)
	{
		if (x.lower_bound <= T{} && x.upper_bound >= T{})
		{
			throw logic_error("Divided by Cranberries which contains Zero!");
		}
		else
		{
			if (this == &x){
				lower_bound = static_cast<T>(1.0);
				upper_bound = static_cast<T>(1.0);
				return *this;
			}
			else
			{
				interval tmp(nextafter(static_cast<T>(1.0) / x.upper_bound, -max<T>()),
					nextafter(static_cast<T>(1.0) / x.lower_bound, max<T>()));
				*this *= *(tmp.pimpl);
				return *this;
			}
		}
	}

	/*  Increment/Decrement Op  */

	template<typename T>
	const typename interval<T>::impl interval<T>::impl::operator ++()
	{
		lower_bound++;
		upper_bound++;
		return *this;
	}
	template<typename T>
	const typename interval<T>::impl interval<T>::impl::operator ++(int)
	{
		auto tmp(*this);
		lower_bound++;
		upper_bound++;
		return tmp;
	}
	template<typename T>
	const typename interval<T>::impl interval<T>::impl::operator --()
	{
		lower_bound--;
		upper_bound--;
		return *this;
	}
	template<typename T>
	const typename interval<T>::impl interval<T>::impl::operator --(int)
	{
		auto tmp(*this);
		lower_bound--;
		upper_bound--;
		return tmp;
	}



	/*  Getter and Setter  */

	template<typename T>
	constexpr T interval<T>::impl::low() const
	{
		return lower_bound;
	}
	template<typename T>
	constexpr T interval<T>::impl::up() const
	{
		return upper_bound;
	}

	/*  Interval Output Function  */

	template<typename T>
	std::ostream& interval<T>::impl::print(std::ostream& s) const
	{
		return s << "[ " << lower_bound << " , " << upper_bound << " ]";
	}
	template<typename T>
	using interval_list = std::initializer_list<interval<T>>;

	/*  Interval Max  */
	template<typename T>
	const interval<T> max(interval<T>& a, interval<T>& b)
	{
		return a > b ? a : b;
	}
	template<typename T>
	const interval<T> max(std::initializer_list<interval<T>> list)
	{
		std::vector<interval<T>> tmp(list);
		std::nth_element(tmp.begin(), tmp.begin(), tmp.end(), [&](interval<T>& a, interval<T>& b) {return a > b;});
		return *(tmp.begin());
	}
	template<typename T, class Pred>
	const interval<T> max(std::initializer_list<interval<T>> list, Pred pred)
	{
		std::vector<interval<T>> tmp(list);
		std::nth_element(tmp.begin(), tmp.begin(), tmp.end(), [&](interval<T>& a, interval<T>& b) {return pred(a,b);});
		return *(tmp.begin());
	}
	template < typename T, typename Pred>
	const interval<T> max(interval<T>& a, interval<T>& b, Pred pred)
	{
		return pred(a, b) ? a : b;
	}
	template <typename T>
	constexpr T max(T a, T b)
	{
		return std::forward<T>(a) > std::forward<T>(b) ? std::forward<T>(a) : std::forward<T>(b);
	}

	template <typename T, typename ... Args>
	constexpr T max(T a, T b, Args ... args)
	{
		return max(max(std::forward<T>(a), std::forward<T>(b)), std::forward<T>(args)...);
	}
	/*  Interval Min  */
	template<typename T>
	const interval<T> min(interval<T>& a, interval<T>& b)
	{
		return a < b ? a : b;
	}
	template<typename T>
	const interval<T> min(interval_list<T> list)
	{
		std::vector<interval<T>> tmp(list);
		std::nth_element(tmp.begin(), tmp.begin(), tmp.end(), [&](interval<T>& a, interval<T>& b) {return a < b;});
		return *(tmp.begin());
	}
	template<typename T, class Pred>
	const interval<T> min(std::initializer_list<interval<T>> list, Pred pred)
	{
		std::vector<interval<T>> tmp(list);
		std::nth_element(tmp.begin(), tmp.begin(), tmp.end(), [&](interval<T>& a, interval<T>& b) {return pred(a, b);});
		return *(tmp.begin());
	}
	template<typename T, class Pred>
	const interval<T> min(interval<T>& a, interval<T>& b, Pred pred)
	{
		return pred(a, b) ? a : b;
	}
	template <typename T>
	constexpr T min(T a, T b)
	{
		return std::forward<T>(a) < std::forward<T>(b) ? std::forward<T>(a) : std::forward<T>(b);
	}

	template <typename T, typename ... Args>
	constexpr T min(T a, T b, Args ... args)
	{
		return min(min(std::forward<T>(a), std::forward<T>(b)), std::forward<T>(args)...);
	}

	//---------------------------------------------------------//
	/*                                                         */
	/*  Generic Formed Interval Numeric Function Overloading   */
	/*                                                         */
	//---------------------------------------------------------//


	/*  Generic Formed Interval Numeric Function  */
	template<typename T>
	interval<T> sin(const interval<T>& a) { return a.sin(); }

	template<typename T>
	interval<T> cos(const interval<T>& a) { return a.cos(); }

	template<typename T>
	interval<T> tan(const interval<T>& a) { return a.tan(); }

	template<typename T>
	interval<T> asin(const interval<T>& a) { return a.asin(); }

	template<typename T>
	interval<T> acos(const interval<T>& a) { return a.acos(); }

	template<typename T>
	interval<T> atan(const interval<T>& a) { return a.atan(); }

	template<typename T>
	interval<T> sinh(const interval<T>& a) { return a.sinh(); }

	template<typename T>
	interval<T> cosh(const interval<T>& a) { return a.cosh(); }

	template<typename T>
	interval<T> tanh(const interval<T>& a) { return a.tanh(); }

	template<typename T>
	interval<T> asinh(const interval<T>& a) { return a.asinh(); }

	template<typename T>
	interval<T> acosh(const interval<T>& a) { return a.acosh(); }

	template<typename T>
	interval<T> atanh(const interval<T>& a) { return a.atanh(); }

	template<typename T>
	interval<T> exp(const interval<T>& a) { return a.exp(); }

	template<typename T>
	interval<T> pow(const interval<T>& a, int const& n) { return a.pow(n); }

	template<typename T>
	interval<T> abs(const interval<T>& a) { return a.abs(); }

	template<typename T>
	interval<T> log(const interval<T>& a) { return a.log(); }

	template<typename T>
	interval<T> log10(const interval<T>& a) { return a.log10(); }

	template<typename T>
	interval<T> sqrt(const interval<T>& a) { return a.sqrt(); }

	template<typename T>
	interval<T> erf(const interval<T>& a) { return a.erf(); }

	template<typename T>
	interval<T> wid(const interval<T>& a) { return a.wid(); }

	template<typename T>
	interval<T> mid(const interval<T>& a) { return a.mid(); }

	template<typename T>
	interval<T> low(const interval<T>& a) { return a.low(); }

	template<typename T>
	interval<T> up(const interval<T>& a) { return a.up(); }

	template<typename T>
	bool is_singleton(const interval<T>& a) { return a.is_singleton(); }

	/*  Generic numeric function for primitive dispatch  */

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	auto sin(T&& x)->decltype(auto) { return std::sin(std::forward<T>(x)); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	auto cos(T&& x)->decltype(auto) { return std::cos(std::forward<T>(x)); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	auto tan(T&& x)->decltype(auto) { return std::tan(std::forward<T>(x)); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	auto asin(T&& x)->decltype(auto) { return std::asin(std::forward<T>(x)); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	auto acos(T&& x)->decltype(auto) { return std::acos(std::forward<T>(x)); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	auto atan(T&& x)->decltype(auto) { return std::atan(std::forward<T>(x)); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	auto sinh(T&& x)->decltype(auto) { return std::sinh(std::forward<T>(x)); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	auto cosh(T&& x)->decltype(auto) { return std::cosh(std::forward<T>(x)); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	auto tanh(T&& x)->decltype(auto) { return std::tanh(std::forward<T>(x)); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	auto asinh(T&& x)->decltype(auto) { return std::asinh(std::forward<T>(x)); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	auto acosh(T&& x)->decltype(auto) { return std::acosh(std::forward<T>(x)); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	auto atanh(T&& x)->decltype(auto) { return std::atanh(std::forward<T>(x)); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	auto exp(T&& x)->decltype(auto) { return std::exp(std::forward<T>(x)); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	auto pow(T&& x, int n)->decltype(auto) { return std::pow(std::forward<T>(x), n); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	auto abs(T&& x)->decltype(auto) { return std::abs(std::forward<T>(x)); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	auto sqrt(T&& x)->decltype(auto) { return std::sqrt(std::forward<T>(x)); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	auto log(T&& x)->decltype(auto) { return std::log(std::forward<T>(x)); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	auto log10(T&& x)->decltype(auto) { return std::log10(std::forward<T>(x)); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	auto erf(T&& x)->decltype(auto) { return std::erf(std::forward<T>(x)); }



	template <typename T, std::enable_if_t<std::is_same<Cranberries::interval_ordering, std::decay_t<T>>::value>*& = enabler >
	std::string enum2string(T&& x)
	{
		switch (x)
		{
		case Cranberries::interval_ordering::less:
			return std::string("less");
		case Cranberries::interval_ordering::greater:
			return std::string("greater");
		case Cranberries::interval_ordering::unordered:
			return std::string("unordered");
		}
	}
	template <typename T, std::enable_if_t<std::is_same<Cranberries::partial_ordering, std::decay_t<T>>::value>*& = enabler >
	std::string enum2string(T&& x)
	{
		switch (x)
		{
		case Cranberries::partial_ordering::less:
			return std::string("less");
		case Cranberries::partial_ordering::greater:
			return std::string("greater");
		case Cranberries::partial_ordering::unordered:
			return std::string("unordered");
		}
	}
	template <typename T, std::enable_if_t<std::is_same<Cranberries::weak_ordering, std::decay_t<T>>::value>*& = enabler >
	std::string enum2string(T&& x)
	{
		switch (x)
		{
		case Cranberries::weak_ordering::less:
			return std::string("less");
		case Cranberries::weak_ordering::greater:
			return std::string("greater");
		case Cranberries::weak_ordering::equivalent:
			return std::string("equivalent");
		}
	}
	template <typename T, std::enable_if_t<std::is_same<Cranberries::total_ordering, std::decay_t<T>>::value>*& = enabler >
	std::string enum2string(T&& x)
	{
		switch (x)
		{
		case Cranberries::total_ordering::less:
			return std::string("less");
		case Cranberries::total_ordering::greater:
			return std::string("greater");
		case Cranberries::total_ordering::equal:
			return std::string("equal");
		}
	}
	template <typename T, std::enable_if_t<std::is_same<Cranberries::Cranberries_Relation, std::decay_t<T>>::value>*& = enabler >
	std::string enum2string(T&& x)
	{
		switch (x)
		{
		case Cranberries::Cranberries_Relation::interval_less:
			return std::string("interval_less");
		case Cranberries::Cranberries_Relation::interval_greater:
			return std::string("interval_greater");
		case Cranberries::Cranberries_Relation::partial_less:
			return std::string("partial_less");
		case Cranberries::Cranberries_Relation::partial_greater:
			return std::string("partial_greater");
		case Cranberries::Cranberries_Relation::weak_less:
			return std::string("weak_less");
		case Cranberries::Cranberries_Relation::weak_greater:
			return std::string("weak_greater");
		case Cranberries::Cranberries_Relation::contain:
			return std::string("contain");
		case Cranberries::Cranberries_Relation::part_of:
			return std::string("part_of");
		case Cranberries::Cranberries_Relation::niether:
			return std::string("niether");
		}
	}

	template <typename T, std::enable_if_t<std::is_same<Cranberries::interval_ordering, std::decay_t<T>>::value>*& = enabler >
	std::ostream& operator<<(std::ostream& s, T&& x)
	{
		return (s << enum2string(x));
	}
	template <typename T, std::enable_if_t<std::is_same<Cranberries::partial_ordering, std::decay_t<T>>::value>*& = enabler >
	std::ostream& operator<<(std::ostream& s, T&& x)
	{
		return (s << enum2string(x));
	}
	template <typename T, std::enable_if_t<std::is_same<Cranberries::weak_ordering, std::decay_t<T>>::value>*& = enabler >
	std::ostream& operator<<(std::ostream& s, T&& x)
	{
		return (s << enum2string(x));
	}
	template <typename T, std::enable_if_t<std::is_same<Cranberries::total_ordering, std::decay_t<T>>::value>*& = enabler >
	std::ostream& operator<<(std::ostream& s, T&& x)
	{
		return (s << enum2string(x));
	}
	template <typename T, std::enable_if_t<std::is_same<Cranberries::Cranberries_Relation, std::decay_t<T>>::value>*& = enabler >
	std::ostream& operator<<(std::ostream& s, T&& x)
	{
		return (s << enum2string(x));
	}

	/*  Generic operator<< for class has menber function print( ostream& )  */

	template <typename T>
	auto operator<<(std::ostream& s, T&& x)->decltype(x.print(s))
	{
		return x.print(s);
	}
	
	std::ostream& operator<<(std::ostream& s, Version_Tag& v)
	{
		return s << (static_cast<unsigned>(v) / 0x01000000) 
			<< "." << ((static_cast<unsigned>(v) % 0x01000000) / 0x00010000) 
			<< "." << (static_cast<unsigned>(v) % 0x00010000);
	}

	//-------------------------------//
	/*                               */
	/*     Four Arithmetic Op        */
	/*     3 Types Overloading       */
	/*                               */
	/*     T Op interval<T> ,        */
	/*     interval<T> Op T , and    */
	/*   interval<T> Op interval<T>  */
	/*                               */
	//-------------------------------//


	/*  Interval Addition Op  */

	template <typename T>
	interval<T> operator +(T&& x, const interval<T>& y)
	{
		return (interval<T>(y.low() + x, y.up() + x));
	}

	template <typename T>
	interval<T> operator +(const interval<T>& x, T&& y)
	{
		return (interval<T>(x.low() + y, x.up() + y));
	}

	template<typename T>
	const interval<T> operator +(const interval<T>& x, const interval<T>& y)
	{
		return interval<T>(x) += y;
	}

	/*  Interval Subtraction Op */

	template <typename T>
	interval<T> operator -(T&& x, const interval<T>& y)
	{
		return (interval<T>(y.low() - x, y.up() - x));
	}

	template <typename T>
	interval<T> operator -(const interval<T>& x, T&& y)
	{
		return (interval<T>(x.low() - y, x.up() - y));
	}

	template<typename T>
	const interval<T> operator -(const interval<T>& x, const interval<T>& y)
	{
		return interval<T>(x) -= y;
	}

	/*  Interval Multiplication Op  */

	template <typename T>
	interval<T> operator *(T&& x, const interval<T>& y)
	{
		if (x >= interval<T>())
			return (interval<T>(y.low() * x, y.up() * x));
		else
			return (interval<T>(y.up() * x, y.low() * x));
	}

	template <typename T>
	interval<T> operator *(const interval<T>& x, T&& y)
	{
		if (y >= T{})
			return (interval<T>(x.low() * y, x.up() * y));
		else
			return (interval<T>(x.up() * y, x.low() * y));
	}

	template<typename T>
	const interval<T> operator *(const interval<T>& x, const interval<T>& y)
	{
		return interval<T>(x) *= y;
	}

	/*  Interval Division Op  */

	template <typename T>
	interval<T> operator /(T&& x, const interval<T>& y)
	{
		if (y.low() <= T{} && T{} <= y.up())
			throw Cranberries::logic_error("Divided by Cranberries which contains zero!");
		else if (y.low() > T{})
			return (interval<T>(y.low() / x, y.up() / x));
		else
			return (interval<T>(y.up() / x, y.low() / x));
	}


	template <typename T>
	interval<T> operator /(const interval<T>& x, T&& y)
	{
		if (y == T{})
			throw Cranberries::logic_error("Divided by Zero!");
		else if (y > T{})
			return (interval<T>(x.low() / y, x.up() / y));
		else
			return (interval<T>(x.up() / y, x.low() / y));
	}

	template<typename T>
	const interval<T> operator /(const interval<T>& x, const interval<T>& y)
	{
		return interval<T>(x) /= y;
	}

	/*  Interval Compound Assignment Op  */

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value>*& = enabler>
	interval<U> operator +=(interval<U>& x, T&& y)
	{
		return x.operator+=(interval<U>(y, y));
	}

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value>*& = enabler>
	interval<U> operator -=(interval<U>& x, T&& y)
	{
		return x.operator-=(interval<U>(y, y));
	}

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value>*& = enabler>
	interval<U> operator *=(interval<U>& x, T&& y)
	{
		return x.operator*=(interval<U>(y, y));
	}

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value>*& = enabler>
	interval<U> operator /=(interval<U>& x, T&& y)
	{
		return x.operator/=(interval<U>(y, y));
	}



	//-------------------//
	/*  interval output  */
	//-------------------//

	/*  Generic Output Stream  */

	template<typename T>
	std::ostream& operator<<(std::ostream& s, std::unique_ptr<T> x)
	{
		s << (*x);
		return (s);
	}

	/*  C string  */

	template<typename T>
	const char* Cranberries::interval<T>::c_str() const
	{
		auto s = new std::string("[ " + std::to_string(this->low()) + " , " + std::to_string(this->up()) + " ]");
		return (s->c_str());
	}

	//------------------------------//
	/*  argument dependent factory  */
	//------------------------------//

	template<typename T>
	interval<T> hull(T& low, T& up)
	{
		if (low > up){ throw invalid_argument("upper_bound less than lower_bound!"); }
		return interval<T>(low, up);
	}

	template<typename T>
	interval<T> hull(T&& low, T&& up)
	{
		if (low > up){ throw invalid_argument("upper_bound less than lower_bound!"); }
		return interval<T>(std::move(low), std::move(up));
	}

	/*  Unique_Ptr  */
	template<typename T>
	std::unique_ptr<interval<T>> hull_ptr(T& low, T& up)
	{
		if (low > up){ throw invalid_argument("upper_bound less than lower_bound!"); }
		return std::unique_ptr<interval<T>>(new interval<T>(low, up));
	}

	template<typename T>
	std::unique_ptr<interval<T>> hull_ptr(T&& low, T&& up)
	{
		if (low > up){ throw invalid_argument("upper_bound less than lower_bound!"); }
		return std::unique_ptr<interval<T>>(new interval<T>(low, up));
	}


	//-----------------------------------//
	/*   interval relational functions   */
	//-----------------------------------//



	/*  Interval Ordering  */

	template<typename T>
	bool interval_less(interval<T> const& x, interval<T> const& y)
	{
		return (x.up() < y.low());
	}
	template<typename T>
	bool interval_greater(interval<T> const& x, interval<T> const& y)
	{
		return (x.low() > y.up());
	}
	template<typename T>
	bool interval_unordered(interval<T> const& x, interval<T> const& y)
	{
		return (interval_less(x, y) == false && interval_greater(x, y) == false);
	}


	/*  Partial Ordering  */

	template<typename T>
	bool partial_less(interval<T> const& x, interval<T> const& y)
	{
		return (x.low() < y.low());
	}

	template<typename T>
	bool partial_greater(interval<T> const& x, interval<T> const& y)
	{
		return (x.low() > y.low());
	}

	template<typename T>
	bool partial_unordered(interval<T> const& x, interval<T> const& y)
	{
		return (partial_less(x, y) == false && partial_greater(x, y) == false);
	}


	/*  Weak Ordering  */

	template<typename T>
	bool weak_less(interval<T> const& x, interval<T> const& y)
	{
		return (x.low() < y.low());
	}

	template<typename T>
	bool weak_greater(interval<T> const& x, interval<T> const& y)
	{
		return (x.low() > y.low());
	}

	template<typename T>
	bool weak_equal(interval<T> const& x, interval<T> const& y)
	{
		return (weak_less(x, y) == false && weak_less(y, x) == false);
	}


	/*  Total Ordering  */

	template<typename T>
	bool total_less(interval<T> const& x, interval<T> const& y)
	{
		if (x.low() == y.low()) {
			return x.up() < y.up();
		}
		else {
			return x.low() < y.low();
		}
	}

	template<typename T>
	bool total_greater(interval<T> const& x, interval<T> const& y)
	{
		if (x.low() == y.low()) {
			return x.up() > y.up();
		}
		else {
			return x.low() > y.low();
		}
	}

	template<typename T>
	bool total_equal(interval<T> const& x, interval<T> const& y)
	{
		return (x.low() == y.low() && x.up() == y.up());
	}


	//--------------------------//
	/*  Ordering Discriminator  */
	//--------------------------//

	/*  Interval Ordering  */

	template<typename T>
	Cranberries::interval_ordering interval_order(Cranberries::interval<T>& x, Cranberries::interval<T>& y)
	{
		if (interval_less(x, y)){ return Cranberries::interval_ordering::less; }
		else if (interval_greater(x, y)){ return Cranberries::interval_ordering::greater; }
		else{ return Cranberries::interval_ordering::unordered; }
	}

	/*  Partial Ordering  */

	template<typename T>
	Cranberries::partial_ordering partial_order(Cranberries::interval<T>& x, Cranberries::interval<T>& y)
	{
		if (partial_less(x, y)){ return partial_ordering::less; }
		else if (partial_greater(x, y)){ return partial_ordering::greater; }
		else{ return partial_ordering::unordered; }
	}

	/*  Weak ordering  */

	template<typename T>
	Cranberries::weak_ordering weak_order(Cranberries::interval<T>& x, Cranberries::interval<T>& y)
	{
		if (weak_less(x, y)){ return weak_ordering::less; }
		else if (weak_greater(x, y)){ return weak_ordering::greater; }
		else{ return weak_ordering::equivalent; }
	}

	/*  Total Ordering  */

	template<typename T>
	Cranberries::total_ordering total_order(Cranberries::interval<T>& x, Cranberries::interval<T>& y)
	{
		if (total_less(x, y)){ return total_ordering::less; }
		else if (total_greater(x, y)){ return total_ordering::greater; }
		else{ return total_ordering::equal; }
	}
	template<typename T>
	bool interval<T>::is_contain(interval const& x) const
	{
		return (this->relational(x) == Cranberries_Relation::contain);
	}
	template<typename T>
	bool interval<T>::is_part_of(interval const& x) const
	{
		return (this->relational(x) == Cranberries_Relation::part_of);
	}


	/*  This function returns two interval subparts just split middle point as vector<interval<T>> */
	template<class T>
	std::vector<interval<T>>  subpart(interval<T>& x)
	{
		std::vector<interval<T>> val{};
		val.push_back(interval<T>(x.low(), x.mid()));
		val.push_back(interval<T>(x.mid(), x.up()));
		return val;
	}

}//end namespace Cranberries

#endif //!INTERVAL_H