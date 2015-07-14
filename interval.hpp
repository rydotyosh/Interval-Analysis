#ifndef CRANBERRIES_INTERVAL
#define CRANBERRIES_INTERVAL
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

	/*  version 0x|aa|.|bb|.|cccc|  */
	enum class Version_Tag {
		Version1_0_0 = 0x01000000,
		Version1_0_1,
		Version1_0_2,
		Version1_0_3,
		Version1_0_4,
		Version1_0_5,
		Version1_0_6,
		Version1_0_7,
		Version1_0_8,
		Version1_1_0 = 0x01010000,
		Version2_0_0 = 0x02000000,
		Version3_0_0 = 0x03000000,
		now_ver = Version1_0_8,
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
	enum class Interval_Relation
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
		constexpr interval(std::initializer_list<T>);

		/*  dtor  */
		~interval();

		/*  copy ctor  */
		interval(const interval&);
		/*  move ctor  */
		interval(interval&&);
		/*  Copy Assignment Op  */
		interval operator=(interval const&);
		/*  Move assignment Op  */
		interval operator=(interval&&);

		interval operator=(std::initializer_list<T>);

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
		void set_up(T const&);
		void set_low(T const&);
		void set_up(T&&);
		void set_low(T&&);

		/*  Advanced Relational Op  */
		Interval_Relation relational(interval const&) const;
		bool is_contain(interval const&) const;
		bool is_contain(T const& x) const;
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
		/*  Member value type  */
		typedef T value_type;
	private:
		/*  Inner Class  */
		class impl;
		/*  Keep lower & upper bound Value   */
		std::unique_ptr<impl> pimpl;
	};


	/*PI*/
	template<typename T>
	constexpr T PI() { return static_cast<T>(3.141592653589793238462643383279); }


	/*maximum*/
	template<typename T>
	constexpr T max() { return std::numeric_limits<T>::max(); }

	/*minimum*/
	template<typename T>
	constexpr T min() { return std::numeric_limits<T>::min(); }

	/*  Zero  */
	template<typename T>
	constexpr T zero() { return static_cast<T>(0.0); }

	template<typename T>
	constexpr T one() { return static_cast<T>(1.0); }

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
			lower_bound = zero<T>();
			upper_bound = zero<T>();
		}
		/*  Set Initial Value */
		void do_internal_work(T const& low, T const& up)
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
		void set_low(T const& x)
		{
			if (x > upper_bound) {
				throw invalid_argument("lower_bound must be less than upper_bound!");
			}
			lower_bound = x;
		}
		void set_low(T&& x)
		{
			if (x > upper_bound) {
				throw invalid_argument("lower_bound must be less than upper_bound!");
			}
			lower_bound = std::move(x);
		}
		void set_up(T const& x)
		{
			if (x < lower_bound) {
				throw invalid_argument("upper_bound must be greater than lower_bound!");
			}
			upper_bound = x;
		}
		void set_up(T&& x)
		{
			if (x < lower_bound) {
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
		T lower_bound = zero<T>();

		/*  Upper Bound  */
		T upper_bound = zero<T>();
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

	/*  Interval Increment/Decrement Operator Definition  */

	/*  Interval Increment Operator  */

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

	/*  Interval Increment Operator  */

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
		if (b - a >= static_cast<T>(2.0)*PI<T>())
		{
			return interval<T>(-one<T>(), one<T>());
		}
		/*  base point set  */
		if (static_cast<int>(nextafter((a / static_cast<T>(2.0) / PI<T>()), max<T>())) > a)
			c = static_cast<int>(nextafter((a / static_cast<T>(2.0) / PI<T>()), max<T>()));
		else
			c = static_cast<int>(nextafter((a / static_cast<T>(2.0) / PI<T>()), max<T>())) + 1;
		/*  checking phase  */
		if (nextafter(PI<T>()*static_cast<T>(2.0)*c, max<T>()) >= a &&
			nextafter(PI<T>()*static_cast<T>(2.0)*c, -max<T>()) <= b)
		{
			if (nextafter(PI<T>()*(one<T>() + static_cast<T>(2.0)*c), max<T>()) >= a &&
				nextafter(PI<T>()*(one<T>() + static_cast<T>(2.0)*c), -max<T>()) <= b)
			{
				return interval<T>(-one<T>(), one<T>());
			}
			else if (nextafter((c*static_cast<T>(2.0) - one<T>())*PI<T>(), max<T>()) >= a &&
				nextafter((c*static_cast<T>(2.0) - one<T>())*PI<T>(), -max<T>()) <= b)
			{
				return interval<T>(-one<T>(), one<T>());
			}
			else
			{
				return interval<T>(
					std::fmin(nextafter(std::cos(a), -max<T>()),
						nextafter(std::cos(b), -max<T>())), one<T>());
			}
		}
		else if (nextafter(PI<T>()*(static_cast<T>(2.0)*c + one<T>()), max<T>()) >= a &&
			nextafter(PI<T>()*(static_cast<T>(2.0)*c + one<T>()), -max<T>()) <= b)
		{
			return interval<T>(-one<T>(),
				std::fmax(nextafter(std::cos(a), max<T>()),
					nextafter(std::cos(b), max<T>())));
		}
		else if (nextafter((c*static_cast<T>(2.0) - one<T>())*PI<T>(), max<T>()) >= a &&
			nextafter((c*static_cast<T>(2.0) - one<T>())*PI<T>(), -max<T>()) <= b)
		{
			return interval<T>(-one<T>(),
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
		if (b - a >= static_cast<T>(2.0)*PI<T>())
		{
			return interval<T>(-one<T>(), one<T>());
		}
		/*  base point set  */
		if (static_cast<int>(nextafter((a * static_cast<T>(2.0) / PI<T>() - one<T>()) / static_cast<T>(4.0), max<T>())) > a)
			c = static_cast<int>(nextafter((a * static_cast<T>(2.0) / PI<T>() - one<T>()) / static_cast<T>(4.0), max<T>()));
		else
			c = static_cast<int>(nextafter((a * static_cast<T>(2.0) / PI<T>() - one<T>()) / static_cast<T>(4.0), max<T>())) + 1;
		/*  checking phase  */
		if (nextafter(PI<T>() / static_cast<T>(2.0)*(one<T>() + c*static_cast<T>(4.0)), max<T>()) >= a &&
			nextafter(PI<T>() / static_cast<T>(2.0)*(one<T>() + c*static_cast<T>(4.0)), -max<T>()) <= b)
		{
			if (nextafter(PI<T>() / static_cast<T>(2.0)*(static_cast<T>(3.0) + c*static_cast<T>(4.0)), max<T>()) >= a &&
				nextafter(PI<T>() / static_cast<T>(2.0)*(static_cast<T>(3.0) + c*static_cast<T>(4.0)), -max<T>()) <= b)
			{
				return interval<T>(-one<T>(), one<T>());
			}
			else if (nextafter((c*static_cast<T>(4.0) - one<T>())*PI<T>() / static_cast<T>(2.0), max<T>()) >= a &&
				nextafter((c*static_cast<T>(4.0) - one<T>())*PI<T>() / static_cast<T>(2.0), -max<T>()) <= b)
			{
				return interval<T>(-one<T>(), one<T>());
			}
			else
			{
				return interval<T>(
					std::fmin(nextafter(std::sin(a), -max<T>()),
						nextafter(std::sin(b), -max<T>())), one<T>());
			}
		}
		else if (nextafter(PI<T>() / static_cast<T>(2.0)*(static_cast<T>(3.0) + c*static_cast<T>(4.0)), max<T>()) >= a &&
			nextafter(PI<T>() / static_cast<T>(2.0)*(static_cast<T>(3.0) + c*static_cast<T>(4.0)), -max<T>()) <= b)
		{
			return interval<T>(-one<T>(),
				std::fmax(nextafter(std::sin(a), max<T>()),
					nextafter(std::sin(b), max<T>())));
		}
		else if (nextafter((c*static_cast<T>(4.0) - one<T>())*PI<T>() / static_cast<T>(2.0), max<T>()) >= a &&
			nextafter((c*static_cast<T>(4.0) - one<T>())*PI<T>() / static_cast<T>(2.0), -max<T>()) <= b)
		{
			return interval<T>(-one<T>(),
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
		if (static_cast<int>(nextafter(a * static_cast<T>(2.0) / PI<T>(), max<T>())) < a)
			c = static_cast<int>(nextafter(a * static_cast<T>(2.0) / PI<T>(), max<T>()));
		else
			c = static_cast<int>(nextafter(a * static_cast<T>(2.0) / PI<T>(), max<T>())) + 1;
		/*  checking phase  */
		if (a < (PI<T>() / static_cast<T>(static_cast<T>(2.0)) * c) && (PI<T>() / static_cast<T>(static_cast<T>(2.0)) * c) < b) {
			throw Cranberries::over_flow("tan");
		}
		else if (a < (PI<T>() / static_cast<T>(static_cast<T>(2.0)) * c - PI<T>()) && (PI<T>() / static_cast<T>(static_cast<T>(2.0)) * c - PI<T>()) < b) {
			throw Cranberries::over_flow("tan");
		}
		else if (a < PI<T>() / static_cast<T>(static_cast<T>(2.0)) * c + PI<T>() && PI<T>() / static_cast<T>(static_cast<T>(2.0)) * c + PI<T>() < b) {
			throw Cranberries::over_flow("tan");
		}
		else if (nextafter(std::tan(a), -max<T>()) == -max<T>() || nextafter(std::tan(b), -max<T>()) == max<T>()) {
			throw Cranberries::over_flow("tan");
		}
		else {
			return interval<T>(nextafter(std::tan(a), -max<T>()), nextafter(std::tan(b), max<T>()));
		}
	}


	/*  Interval ArcCos  */

	template<typename T>
	const interval<T> interval<T>::acos() const
	{
		auto a = pimpl->low(), b = pimpl->up();
		if (a < -one<T>() || b > one<T>()) {
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
		if (a < -one<T>() || b > one<T>()) {
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
		if (a < -one<T>() || b > one<T>()) {
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
		if (a < zero<T>() && b > zero<T>()) {
			return interval<T>(one<T>(), std::fmax(nextafter(std::cosh(a), max<T>()), nextafter(std::cosh(b), max<T>())));
		}
		else if (b < zero<T>()) {
			return interval<T>(nextafter(std::cosh(b), -max<T>()), nextafter(std::cosh(a), max<T>()));
		}
		else if (a > zero<T>()) {
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
		auto l = nextafter(std::sinh(a), -max<T>()), r = nextafter(std::sinh(b), max<T>());
		if (a == -max<T>() || b == max<T>()) {
			throw Cranberries::over_flow("sinh");
		}
		else {
			return interval<T>(l, r);
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
		if (a < one<T>()) {
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
		if (n < 0) {
			auto tmp = one<T>() / (*this);
			return tmp.pow(-one<T>() * n);
		}
		else if (n == 0) {
			return interval<T>(one<T>(), one<T>());
		}
		else if (n % 2 == 0) {
			if (pimpl->up() <= zero<T>()) {
				if (nextafter(std::pow(pimpl->low(), n), max<T>()) == max<T>()
					|| nextafter(std::pow(pimpl->up(), n), -max<T>()) == -max<T>())
				{
					throw Cranberries::over_flow("overflow occurred.");
				}
				return interval<T>(
					nextafter(std::pow(pimpl->up(), n), -max<T>()),
					nextafter(std::pow(pimpl->low(), n), max<T>()));
			}
			else if (pimpl->low() <= zero<T>() && pimpl->up() >= zero<T>()) {
				if (std::fmax(
					nextafter(std::pow(pimpl->low(), n), -max<T>()),
					nextafter(std::pow(pimpl->up(), n), max<T>())) == max<T>())
				{
					throw Cranberries::over_flow("overflow occurred.");
				}
				return interval<T>(zero<T>(), std::fmax(
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
		if (pimpl->low() < zero<T>()) { throw Cranberries::logic_error("sqrt arg requires positive number"); }
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
		if (pimpl->low() <= zero<T>()) { throw Cranberries::logic_error("anti-logarithm less than or equal to zero"); }
		return interval<T>(
			nextafter(std::log(pimpl->low()), -max<T>()),
			nextafter(std::log(pimpl->up()), max<T>()));
	}


	/*  Interval Log10  */

	template<typename T>
	const interval<T> interval<T>::log10() const
	{
		if (pimpl->low() <= zero<T>()) { throw Cranberries::logic_error("anti-logarithm less than or equal to zero"); }
		return interval<T>(
			nextafter(std::log10(pimpl->low()), -max<T>()),
			nextafter(std::log10(pimpl->up()), max<T>()));
	}


	/*  Interval Abs  */

	template<typename T>
	const interval<T> interval<T>::abs() const
	{
		if (pimpl->low() < zero<T>() && pimpl->up() >= zero<T>())
		{
			return interval<T>(zero<T>(), nextafter(std::fmax(std::abs(pimpl->low()), std::abs(pimpl->up())), max<T>()));
		}
		if (pimpl->up() < zero<T>())
		{
			return interval<T>(nextafter(std::abs(pimpl->up()),-max<T>()), nextafter(std::abs(pimpl->low()),max<T>()));
		}
		else {
			return interval<T>(*this);
		}
	}

	/*  Interval Error Function  */

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
		return (pimpl->up() + pimpl->low()) / static_cast<T>(static_cast<T>(2.0));
	}


	/*  width  */

	template<typename T>
	constexpr T interval<T>::wid() const
	{
		return pimpl->up() - pimpl->low();
	}

	/*  Interval Accessors  */

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
	void interval<T>::set_up(T const& x)
	{
		pimpl->set_up(x);
	}

	template<typename T>
	void interval<T>::set_up(T&& x)
	{
		pimpl->set_up(x);
	}

	template<typename T>
	void interval<T>::set_low(T const& x)
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
	Interval_Relation interval<T>::relational(interval<T> const& x) const
	{
		if (total_equal(*this, x))
		{
			return Interval_Relation::equal;
		}
		else if (pimpl->low() == (x.pimpl)->low() && pimpl->up() < (x.pimpl)->up())
		{
			return Interval_Relation::total_less;
		}
		else if (pimpl->low() == (x.pimpl)->low() && pimpl->up() > (x.pimpl)->up())
		{
			return Interval_Relation::total_greater;
		}
		else if (pimpl->low() < (x.pimpl)->low() && pimpl->up() == (x.pimpl)->up())
		{
			return Interval_Relation::weak_less;
		}
		else if (pimpl->low() > (x.pimpl)->low() && pimpl->up() == (x.pimpl)->up())
		{
			return Interval_Relation::weak_greater;
		}
		else if (pimpl->low() < (x.pimpl)->low() && pimpl->up() < (x.pimpl)->up())
		{
			return Interval_Relation::partial_less;
		}
		else if (pimpl->low() > (x.pimpl)->low() && pimpl->up() > (x.pimpl)->up())
		{
			return Interval_Relation::partial_less;
		}
		else if (pimpl->low() <= (x.pimpl)->low() && pimpl->up() >= (x.pimpl)->up())
		{
			return Interval_Relation::contain;
		}
		else if (pimpl->low() >= (x.pimpl)->low() && pimpl->up() <= (x.pimpl)->up())
		{
			return Interval_Relation::part_of;
		}
		else
		{
			return Interval_Relation::niether;
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

	/*  Two Lvalue Arguments Ctor  */

	template<typename T>
	constexpr interval<T>::interval(T& low, T& up)
		: pimpl{ std::make_unique<impl>() }
	{
		if (low > up) { throw invalid_argument("upper_bound less than lower_bound!"); }
		pimpl->do_internal_work(low, up);
	}

	/*  Two Rvalue Arguments Ctor  */

	template<typename T>
	constexpr interval<T>::interval(T&& low, T&& up)
		: pimpl{ std::make_unique<impl>() }
	{
		if (low > up) { throw invalid_argument("upper_bound less than lower_bound!"); }
		pimpl->do_internal_work(low, up);
	}

	/*  Default Dtor  */

	template<typename T>
	interval<T>::~interval() = default;

	/*  Move Ctor  */

	template<typename T>
	interval<T>::interval(interval&& x) = default;

	/*  Copy Ctor  */

	template<typename T>
	interval<T>::interval(const interval& x)
		: pimpl{ std::make_unique<impl>() }
	{
		pimpl->deep_copy(*(x.pimpl));
	}

	template<typename T>
	constexpr interval<T>::interval(std::initializer_list<T> list)
		: pimpl{ std::make_unique<impl>() } 
	{
		if (list.size() == 0) {
			pimpl->do_internal_work();
		}
		else if (list.size() != 2)
			throw Cranberries::invalid_argument("Interval Ctor needs only two values.");
		pimpl->do_internal_work(*(list.begin()), *(list.begin() + 1));
	}

	/*  Copy Assignment Op  */

	template<typename T>
	interval<T> interval<T>::operator=(interval<T> const& x)
	{
		pimpl->deep_copy(*(x.pimpl));
		return *this;
	}

	/*  Move Assignment Op  */

	template<typename T>
	interval<T> interval<T>::operator=(interval<T>&& x)
	{
		std::swap(this->pimpl, x.pimpl);
		x.pimpl = nullptr;
		return *this;
	}

	template<typename T>
	interval<T> interval<T>::operator=(std::initializer_list<T> list)
	{
		if (list.size() == 0) {
			this->set_low(zero<T>());
			this->set_up(zero<T>());
		}
		else if (list.size() == 2) {
			if (*(list.begin()) > *(list.begin() + 1)) {
				this->set_low(*(list.begin()));
				this->set_up(*(list.begin() + 1));
			}
			else {
				throw Cranberries::invalid_argument("");
			}
		}
		else {
			throw Cranberries::invalid_argument("");
		}
		return *this;
	}

	/*  Interval Output Function  */

	template<typename T>
	std::ostream& interval<T>::print(std::ostream& s) const
	{
		return pimpl->print(s);
	}


	//------------------------------//
	/*                              */
	/*     impl Member Function     */
	/*                              */
	//------------------------------//


	/*  Compound Assignment Op Definition  */

	/*  impl Compound Assignment Operator  */

	template<typename T>
	const typename interval<T>::impl interval<T>::impl::operator+=(const typename interval<T>::impl& x)
	{
		lower_bound = nextafter(lower_bound + x.lower_bound, -max<T>());
		upper_bound = nextafter(upper_bound + x.upper_bound, max<T>());
		return *this;
	}

	/*  impl Compound Subtraction Operator   */

	template<typename T>
	const typename interval<T>::impl interval<T>::impl::operator-=(const typename interval<T>::impl& x)
	{
		lower_bound = nextafter(lower_bound - x.upper_bound, -max<T>());
		upper_bound = nextafter(upper_bound - x.lower_bound, max<T>());
		return *this;
	}

	/*  impl operator Compound Multiplication Operator  */

	template<typename T>
	const typename interval<T>::impl interval<T>::impl::operator*=(const typename interval<T>::impl& x)
	{
		auto l = lower_bound;
		auto r = upper_bound;

		if (l >= zero<T>() && x.lower_bound >= zero<T>()) {
			lower_bound = nextafter(l * x.lower_bound, -max<T>());
			upper_bound = nextafter(r * x.upper_bound, max<T>());
			return *this;
		}
		else if (l >= zero<T>() && x.lower_bound < zero<T>() && x.upper_bound > zero<T>())
		{
			lower_bound = nextafter(upper_bound * x.lower_bound, -max<T>());
			upper_bound = nextafter(upper_bound * x.upper_bound, max<T>());
			return *this;
		}
		else if (l >= zero<T>() && x.upper_bound <= zero<T>())
		{
			lower_bound = nextafter(r * x.lower_bound, -max<T>());
			upper_bound = nextafter(l * x.upper_bound, max<T>());
			return *this;
		}
		else if (l < zero<T>() && r > zero<T>() && x.lower_bound >= zero<T>())
		{
			lower_bound = nextafter(l * x.upper_bound, -max<T>());
			upper_bound = nextafter(r * x.upper_bound, max<T>());
			return *this;
		}
		else if (l < zero<T>() && r > zero<T>() && x.upper_bound <= zero<T>())
		{
			lower_bound = nextafter(r * x.lower_bound, -max<T>());
			upper_bound = nextafter(l * x.lower_bound, max<T>());
			return *this;
		}
		else if (r <= zero<T>() && x.lower_bound >= zero<T>())
		{
			lower_bound = nextafter(l * x.upper_bound, -max<T>());
			upper_bound = nextafter(r * x.lower_bound, max<T>());
			return *this;
		}
		else if (r <= zero<T>() && x.lower_bound < zero<T>() && x.upper_bound > zero<T>())
		{
			lower_bound = nextafter(l * x.upper_bound, -max<T>());
			upper_bound = nextafter(l * x.lower_bound, max<T>());
			return *this;
		}
		else if (r <= zero<T>() && x.upper_bound <= zero<T>())
		{
			lower_bound = nextafter(r * x.upper_bound, -max<T>());
			upper_bound = nextafter(l * x.lower_bound, max<T>());
			return *this;
		}
		else {
			lower_bound = std::fmin(nextafter(r * x.lower_bound, -max<T>()), nextafter(l * x.upper_bound, -max<T>()));
			upper_bound = std::fmax(nextafter(r * (x.upper_bound), max<T>()), nextafter(l * (x.lower_bound), max<T>()));
			return *this;
		}
	}

	/*  impl Compound Division Operator  */

	template<typename T>
	const typename interval<T>::impl interval<T>::impl::operator/=(const typename interval<T>::impl& x)
	{
		if (x.lower_bound <= zero<T>() && x.upper_bound >= zero<T>())
		{
			throw logic_error("Divided by Cranberries which contains Zero!");
		}
		else
		{
			interval tmp(nextafter(one<T>() / x.upper_bound, -max<T>()),
				nextafter(one<T>() / x.lower_bound, max<T>()));
			*this *= *(tmp.pimpl);
			return *this;
		}
	}

	/*  Increment/Decrement Operators  */

	/*  impl Prefix Increment Operator  */

	template<typename T>
	const typename interval<T>::impl interval<T>::impl::operator ++()
	{
		lower_bound++;
		upper_bound++;
		return *this;
	}

	/*  impl Postfix Increment Operator  */

	template<typename T>
	const typename interval<T>::impl interval<T>::impl::operator ++(int)
	{
		auto tmp(*this);
		lower_bound++;
		upper_bound++;
		return tmp;
	}

	/*  impl Prefix Decrement Operator  */

	template<typename T>
	const typename interval<T>::impl interval<T>::impl::operator --()
	{
		lower_bound--;
		upper_bound--;
		return *this;
	}

	/*  impl Postfix Increment Operator  */

	template<typename T>
	const typename interval<T>::impl interval<T>::impl::operator --(int)
	{
		auto tmp(*this);
		lower_bound--;
		upper_bound--;
		return tmp;
	}



	/*  Accessors  */

	/*  Interval Low_Bound Getter  */

	template<typename T>
	constexpr T interval<T>::impl::low() const
	{
		return lower_bound;
	}

	/*  Interval Upper_Bound Getter  */

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


	/*  Interval Max  */

	/*  Two interval Argument Max  */

	template<typename T>
	const interval<T> max(interval<T>& a, interval<T>& b)
	{
		return a > b ? a : b;
	}

	/*  iitializer_list<interval> Argument Max  */

	template<typename T>
	const interval<T> max(std::initializer_list<interval<T>> list)
	{
		std::vector<interval<T>> tmp(list);
		std::nth_element(tmp.begin(), tmp.begin(), tmp.end(), [&](interval<T>& a, interval<T>& b) {return a > b;});
		return *(tmp.begin());
	}

	/*  initializer_list<interval> and Predicate Argument Max   */

	template<typename T, class Pred>
	const interval<T> max(std::initializer_list<interval<T>> list, Pred pred)
	{
		std::vector<interval<T>> tmp(list);
		std::nth_element(tmp.begin(), tmp.begin(), tmp.end(), [&](interval<T>& a, interval<T>& b) {return pred(a, b);});
		return *(tmp.begin());
	}

	/*  Tow interval and Predicate Argument Max   */

	template < typename T, typename Pred, std::enable_if_t<std::is_same<Pred, interval<T>>::value>*& = enabler>
	const interval<T> max(interval<T>& a, interval<T>& b, Pred pred)
	{
		return pred(a, b) ? a : b;
	}

	/*  Variadic arguments Max overloading (finish)  */

	template <typename T>
	constexpr T max(T a, T b)
	{
		return std::forward<T>(a) > std::forward<T>(b) ? std::forward<T>(a) : std::forward<T>(b);
	}

	/*  Variadic arguments Max  */

	template <typename T, typename ... Args>
	constexpr T max(T a, T b, Args ... args)
	{
		return max(max(std::forward<T>(a), std::forward<T>(b)), std::forward<T>(args)...);
	}


	/*  Interval Min  */

	/*  Two interval Argument Min  */

	template<typename T>
	const interval<T> min(interval<T>& a, interval<T>& b)
	{
		return a < b ? a : b;
	}

	/*  iitializer_list<interval> Argument Min  */

	template<typename T>
	const interval<T> min(std::initializer_list<interval<T>> list)
	{
		std::vector<interval<T>> tmp(list);
		std::nth_element(tmp.begin(), tmp.begin(), tmp.end(), [&](interval<T>& a, interval<T>& b) {return a < b;});
		return *(tmp.begin());
	}

	/*  initializer_list<interval> and Predicate Argument Min   */

	template<typename T, class Pred>
	const interval<T> min(std::initializer_list<interval<T>> list, Pred pred)
	{
		std::vector<interval<T>> tmp(list);
		std::nth_element(tmp.begin(), tmp.begin(), tmp.end(), [&](interval<T>& a, interval<T>& b) {return pred(a, b);});
		return *(tmp.begin());
	}

	/*  Tow interval and Predicate Argument Max   */

	template<typename T, class Pred, std::enable_if_t<std::is_same<Pred, interval<T>>::value>*& = enabler>
	const interval<T> min(interval<T>& a, interval<T>& b, Pred pred)
	{
		return pred(a, b) ? a : b;
	}

	/*  Variadic arguments Max overloading (finish)  */

	template <typename T>
	constexpr T min(T a, T b)
	{
		return std::forward<T>(a) < std::forward<T>(b) ? std::forward<T>(a) : std::forward<T>(b);
	}

	/*  Variadic arguments Max  */

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
	T wid(const interval<T>& a) { return a.wid(); }

	template<typename T>
	T mid(const interval<T>& a) { return a.mid(); }

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


	/*  for enum output with string  */

	//interval_ordering to string
	template <typename T, std::enable_if_t<std::is_same<Cranberries::interval_ordering, std::decay_t<T>>::value>*& = enabler >
	std::string enum2str(T&& x)
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

	//partial_ordering to string
	template <typename T, std::enable_if_t<std::is_same<Cranberries::partial_ordering, std::decay_t<T>>::value>*& = enabler >
	std::string enum2str(T&& x)
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

	//weak_ordering to string
	template <typename T, std::enable_if_t<std::is_same<Cranberries::weak_ordering, std::decay_t<T>>::value>*& = enabler >
	std::string enum2str(T&& x)
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

	//total_ordering to string
	template <typename T, std::enable_if_t<std::is_same<Cranberries::total_ordering, std::decay_t<T>>::value>*& = enabler >
	std::string enum2str(T&& x)
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

	//Interval_Relation to string to string
	template <typename T, std::enable_if_t<std::is_same<Cranberries::Interval_Relation, std::decay_t<T>>::value>*& = enabler >
	std::string enum2str(T&& x)
	{
		switch (x)
		{
		case Cranberries::Interval_Relation::interval_less:
			return std::string("interval_less");
		case Cranberries::Interval_Relation::interval_greater:
			return std::string("interval_greater");
		case Cranberries::Interval_Relation::partial_less:
			return std::string("partial_less");
		case Cranberries::Interval_Relation::partial_greater:
			return std::string("partial_greater");
		case Cranberries::Interval_Relation::weak_less:
			return std::string("weak_less");
		case Cranberries::Interval_Relation::weak_greater:
			return std::string("weak_greater");
		case Cranberries::Interval_Relation::contain:
			return std::string("contain");
		case Cranberries::Interval_Relation::part_of:
			return std::string("part_of");
		case Cranberries::Interval_Relation::niether:
			return std::string("niether");
		}
	}

	/*  ostream << enum overload  */

	//ostream << interval_ordering
	template <typename T, std::enable_if_t<std::is_same<Cranberries::interval_ordering, std::decay_t<T>>::value>*& = enabler >
	std::ostream& operator<<(std::ostream& s, T&& x)
	{
		return (s << enum2str(x));
	}

	//ostream << partial_ordering
	template <typename T, std::enable_if_t<std::is_same<Cranberries::partial_ordering, std::decay_t<T>>::value>*& = enabler >
	std::ostream& operator<<(std::ostream& s, T&& x)
	{
		return (s << enum2str(x));
	}

	//ostream << weak_ordering
	template <typename T, std::enable_if_t<std::is_same<Cranberries::weak_ordering, std::decay_t<T>>::value>*& = enabler >
	std::ostream& operator<<(std::ostream& s, T&& x)
	{
		return (s << enum2str(x));
	}

	//ostream << total_ordering
	template <typename T, std::enable_if_t<std::is_same<Cranberries::total_ordering, std::decay_t<T>>::value>*& = enabler >
	std::ostream& operator<<(std::ostream& s, T&& x)
	{
		return (s << enum2str(x));
	}

	//ostream << Interval_Relation
	template <typename T, std::enable_if_t<std::is_same<Cranberries::Interval_Relation, std::decay_t<T>>::value>*& = enabler >
	std::ostream& operator<<(std::ostream& s, T&& x)
	{
		return (s << enum2str(x));
	}

	/*  Generic operator<< for class whitch has menber function print( ostream& )  */

	template <typename T>
	auto operator<<(std::ostream& s, T&& x)->decltype(x.print(s))
	{
		return x.print(s);
	}

	/*  output for version  */

	std::ostream& operator<<(std::ostream& s, Version_Tag const& v)
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

	// T + interval<T>
	template <typename T>
	interval<T> operator +(T&& x, const interval<T>& y)
	{
		return (interval<T>(y.low() + x, y.up() + x));
	}

	// interval<T> + T
	template <typename T>
	interval<T> operator +(const interval<T>& x, T&& y)
	{
		return (interval<T>(x.low() + y, x.up() + y));
	}

	// interval<T> + interval<T>
	template<typename T>
	const interval<T> operator +(const interval<T>& x, const interval<T>& y)
	{
		return interval<T>(x) += y;
	}

	/*  Interval Subtraction Op */

	// T - interval<T>
	template <typename T>
	interval<T> operator -(T&& x, interval<T> y)
	{
		return interval<T>(x - y.up() , x - y.low());
	}

	// interval<T> - T
	template <typename T>
	interval<T> operator -(const interval<T>& x, T&& y)
	{
		return (interval<T>(x.low() - y, x.up() - y));
	}

	// interval<T> - interval<T>
	template<typename T>
	const interval<T> operator -(const interval<T>& x, const interval<T>& y)
	{
		if (&x == &y)
			return interval<T>{};
		return interval<T>(x) -= y;
	}

	/*  Interval Multiplication Op  */

	// T * interval<T>
	template <typename T>
	interval<T> operator *(T&& x, const interval<T>& y)
	{
		if (x > zero<T>())
			return (interval<T>(y.low() * x, y.up() * x));
		else
			return (interval<T>(y.up() * x, y.low() * x));
	}

	// interval<T> * T
	template <typename T>
	interval<T> operator *(const interval<T>& x, T&& y)
	{
		if (y > zero<T>())
			return (interval<T>(x.low() * y, x.up() * y));
		else
			return (interval<T>(x.up() * y, x.low() * y));
	}

	// interval<T> * interval<T>
	template<typename T>
	const interval<T> operator *(const interval<T>& x, const interval<T>& y)
	{
		if (&x == &y)
			return x.pow(2);

		return interval<T>(x) *= y;
	}

	/*  Interval Division Op  */

	// T / interval<T>
	template <typename T>
	interval<T> operator /(T&& x, const interval<T>& y)
	{
		if (y.low() <= zero<T>() && zero<T>() <= y.up())
			throw Cranberries::logic_error("Divided by Cranberries which contains zero!");
		else if (y.low() > zero<T>())
			return (interval<T>(x / y.up(), x / y.low()));
		else
			return (interval<T>(x / y.low(), x / y.up()));
	}

	// interval<T> / T
	template <typename T>
	interval<T> operator /(const interval<T>& x, T&& y)
	{
		if (y == zero<T>())
			throw Cranberries::logic_error("Divided by Zero!");
		else if (y > zero<T>())
			return (interval<T>(x.low() / y, x.up() / y));
		else
			return (interval<T>(x.up() / y, x.low() / y));
	}

	// interval<T> / interval<T>
	template<typename T>
	const interval<T> operator /(const interval<T>& x, const interval<T>& y)
	{
		if (y.is_contain(zero<T>()))
			throw Cranberries::invalid_argument("");
		else if (&x == &y)
			return interval<T>{one<T>(), one<T>()};
		return interval<T>(x) /= y;
	}

	/*  Interval Compound Assignment Op  */

	// interval<T> += T
	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value>*& = enabler>
	interval<U> operator +=(interval<U> const& x, T&& y)
	{
		return x.operator+=(interval<U>(y, y));
	}

	// interval<T> -= T
	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value>*& = enabler>
	interval<U> operator -=(interval<U> const& x, T&& y)
	{
		return x.operator-=(interval<U>(y, y));
	}

	// interval<T> *= T
	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value>*& = enabler>
	interval<U> operator *=(interval<U> const& x, T&& y)
	{
		return x.operator*=(interval<U>(y, y));
	}

	// interval<T> /= T
	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value>*& = enabler>
	interval<U> operator /=(interval<U> const& x, T&& y)
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

	/*  function returns interval into C style string  */

	template<typename T>
	const char* Cranberries::interval<T>::c_str() const
	{
		auto s = new std::string("[ " + std::to_string(this->low()) + " , " + std::to_string(this->up()) + " ]");
		return (s->c_str());
	}

	//------------------------------------------------------------------------//
	/*  It is cumbersome to write the type to every declaration.              */
	/*  Like ' auto x = interval<double>(one<T>(), static_cast<T>(3.0))'.                          */
	/*  And it becomes a source of mistake.                                   */
	/*  Then, you can use argument dependent factory 'hull(Low,Up)'.          */
	/*  It allows infer to type from arguments and returns interval object.   */
	//------------------------------------------------------------------------//

	// for Lvalue
	template<typename T>
	interval<T> hull(T& low, T& up)
	{
		if (low > up) { throw invalid_argument("upper_bound less than lower_bound!"); }
		return interval<T>(low, up);
	}

	// for Rvalue
	template<typename T>
	interval<T> hull(T&& low, T&& up)
	{
		if (low > up) { throw invalid_argument("upper_bound less than lower_bound!"); }
		return interval<T>(std::move(low), std::move(up));
	}

	/*  Unique_Ptr  */
	template<typename T>
	std::unique_ptr<interval<T>> hull_ptr(T& low, T& up)
	{
		if (low > up) { throw invalid_argument("upper_bound less than lower_bound!"); }
		return std::unique_ptr<interval<T>>(new interval<T>(low, up));
	}

	template<typename T>
	std::unique_ptr<interval<T>> hull_ptr(T&& low, T&& up)
	{
		if (low > up) { throw invalid_argument("upper_bound less than lower_bound!"); }
		return std::unique_ptr<interval<T>>(new interval<T>(low, up));
	}


	//-----------------------------------//
	/*   interval relational functions   */
	//-----------------------------------//



	/*  Interval Ordering  */

	// interval less
	template<typename T>
	bool interval_less(interval<T> const& x, interval<T> const& y)
	{
		return (x.up() < y.low());
	}

	// interval greater
	template<typename T>
	bool interval_greater(interval<T> const& x, interval<T> const& y)
	{
		return (x.low() > y.up());
	}

	// interval unordered
	template<typename T>
	bool interval_unordered(interval<T> const& x, interval<T> const& y)
	{
		return (interval_less(x, y) == false && interval_greater(x, y) == false);
	}


	/*  Partial Ordering  */

	// partial less
	template<typename T>
	bool partial_less(interval<T> const& x, interval<T> const& y)
	{
		return (x.low() < y.low());
	}

	// partial greater
	template<typename T>
	bool partial_greater(interval<T> const& x, interval<T> const& y)
	{
		return (x.low() > y.low());
	}

	// partial unordered
	template<typename T>
	bool partial_unordered(interval<T> const& x, interval<T> const& y)
	{
		return (partial_less(x, y) == false && partial_greater(x, y) == false);
	}


	/*  Weak Ordering  */

	// weak less
	template<typename T>
	bool weak_less(interval<T> const& x, interval<T> const& y)
	{
		return (x.low() < y.low());
	}

	// weak greater
	template<typename T>
	bool weak_greater(interval<T> const& x, interval<T> const& y)
	{
		return (x.low() > y.low());
	}

	// weak equal
	template<typename T>
	bool weak_equal(interval<T> const& x, interval<T> const& y)
	{
		return (weak_less(x, y) == false && weak_less(y, x) == false);
	}


	/*  Total Ordering  */

	// total less
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

	// total greater
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

	// total equal
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
		if (interval_less(x, y)) { return Cranberries::interval_ordering::less; }
		else if (interval_greater(x, y)) { return Cranberries::interval_ordering::greater; }
		else { return Cranberries::interval_ordering::unordered; }
	}

	/*  Partial Ordering  */

	template<typename T>
	Cranberries::partial_ordering partial_order(Cranberries::interval<T>& x, Cranberries::interval<T>& y)
	{
		if (partial_less(x, y)) { return partial_ordering::less; }
		else if (partial_greater(x, y)) { return partial_ordering::greater; }
		else { return partial_ordering::unordered; }
	}

	/*  Weak ordering  */

	template<typename T>
	Cranberries::weak_ordering weak_order(Cranberries::interval<T>& x, Cranberries::interval<T>& y)
	{
		if (weak_less(x, y)) { return weak_ordering::less; }
		else if (weak_greater(x, y)) { return weak_ordering::greater; }
		else { return weak_ordering::equivalent; }
	}

	/*  Total Ordering  */

	template<typename T>
	Cranberries::total_ordering total_order(Cranberries::interval<T>& x, Cranberries::interval<T>& y)
	{
		if (total_less(x, y)) { return total_ordering::less; }
		else if (total_greater(x, y)) { return total_ordering::greater; }
		else { return total_ordering::equal; }
	}

	/*  is contain  */

	template<typename T>
	bool interval<T>::is_contain(interval<T> const& x) const
	{
		return (this->relational(x) == Interval_Relation::contain);
	}

	template<typename T>
	bool interval<T>::is_contain(T const& x) const
	{
		return (this->low() < x && this->up() > x);
	}

	/*  is part of  */

	template<typename T>
	bool interval<T>::is_part_of(interval const& x) const
	{
		return (this->relational(x) == Interval_Relation::part_of);
	}


	/*  This function returns two interval subparts just split middle point as vector<interval<T>> */
	template<class T>
	std::pair<interval<T>,interval<T>>  subpart(interval<T>& x)
	{
		return std::make_pair(interval<T>(x.low(), x.mid()), interval<T>(x.mid(), x.up()));
	}

	template <class InputRange, class BinaryFunction>
	void adjacent_for_each(InputRange&& range, BinaryFunction f)
	{
		using std::begin;
		using std::end;

		auto first = begin(range);
		auto last = end(range);

		if (first == last)
			return;

		while (std::next(first) != last) {
			auto&& a = *first;
			++first;
			auto&& b = *first;
			f(a, b);
		}
	}


}//end namespace Cranberries

#endif //!CRANBERRIES_INTERVAL