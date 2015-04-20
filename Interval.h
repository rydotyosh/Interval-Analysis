#ifndef INTERVAL_H
#define INTERVAL_H
#include "exception.h"
#include <exception>
#include <cmath>
#include <typeinfo>
#include <type_traits>
#include <iostream>
#include <string>
#include <memory>

using std::nextafter;

namespace Interval{
	extern void* enabler;

	enum class partial_ordering { less, unordered, greater };
	enum class weak_ordering { less, equivalent, greater };
	enum class total_ordering { less, equal, greater };
	enum class interval_ordering { less, unordered, greater };

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

	template<typename T>
	class interval
	{
	public:
		interval();
		interval(T&, T&);
		interval(T&&, T&&);
		~interval();

		interval(const interval&);
		interval(interval&&);
		interval& operator=(interval&&);

		const interval operator +=(const interval&);
		const interval operator -=(const interval&);
		const interval operator *=(const interval&);
		const interval operator /=(const interval&);

		const interval operator ++();
		const interval operator ++(int);
		const interval operator --();
		const interval operator --(int);

		const interval sin() const;
		const interval cos() const;
		const interval pow(int n) const;
		const interval log() const;
		const interval log10() const;
		const interval sqrt() const;
		const interval exp() const;
		const interval abs() const;
		const T mid() const;
		const T wid() const;
		const T get_low() const;
		const T get_up() const;
		void set_up(T&);
		void set_low(T&);
		void set_up(T&&);
		void set_low(T&&);

		Interval_Relation relational(interval const&) const;
		bool is_contain(interval const&) const;
		bool is_part_of(interval const&) const;


		bool operator<(interval const&) const;
		bool operator>(interval const&) const;
		bool operator<=(interval const&) const;
		bool operator>=(interval const&) const;
		bool operator==(interval const&) const;
		bool operator!=(interval const&) const;

		std::ostream& print(std::ostream&) const;
		const char* c_str() const;

	private:
		class impl;
		std::unique_ptr<impl> pimpl;
	};


	/*PI*/
	template<typename T>
	T PI(){ return static_cast<T>(3.141592653589793238462643383279); }


	/*maximum*/
	template<typename T>
	T max(){ return static_cast<T>(1.7976931348623158e+308); }

	/*minimum*/
	template<typename T>
	T min(){ return static_cast<T>(2.2250738585072014e-308); }

	template<>
	int min<int>(){ return 1; }





	//////////////////////////////////////
	/*									*/
	/*		class declaration			*/
	/*									*/
	/*	interval implementaion class	*/
	/*	(interval internal class)		*/
	/*									*/
	//////////////////////////////////////


	template<typename T>
	class interval<T>::impl
	{
	public:
		void do_internal_work()
		{
			lower_bound = T();
			upper_bound = T();
		}
		void do_internal_work(T& low, T& up)
		{
			lower_bound = low;
			upper_bound = up;
		}
		const impl operator +=(const impl&);
		const impl operator -=(const impl&);
		const impl operator *=(const impl&);
		const impl operator /=(const impl&);

		const impl operator ++();
		const impl operator ++(int);
		const impl operator --();
		const impl operator --(int);

		const interval<T> sin() const;
		const interval<T> cos() const;
		const interval<T> pow(int n) const;
		const interval<T> exp() const;
		const interval<T> abs() const;
		const T mid() const;
		const T wid() const;
		const T get_low() const;
		const T get_up() const;


		bool operator<(impl const&) const;
		bool operator>(impl const&) const;
		bool operator<=(impl const&) const;
		bool operator>=(impl const&) const;
		bool operator==(impl const&) const;
		bool operator!=(impl const&) const;

		std::ostream& print(std::ostream&) const;
		void set_low(T& x)
		{
			if (x > upper_bound){
				throw invalid_argument("lower_bound greater than upper_bound!");
			}
			lower_bound = x;
		}
		void set_low(T&& x)
		{
			if (x > upper_bound){
				throw invalid_argument("lower_bound greater than upper_bound!");
			}
			lower_bound = x;
		}
		void set_up(T& x)
		{
			if (x < lower_bound){
				throw invalid_argument("upper_bound less than lower_bound!");
			}
			upper_bound = x;
		}
		void set_up(T&& x)
		{
			if (x < lower_bound){
				throw invalid_argument("upper_bound less than lower_bound!");
			}
			upper_bound = x;
		}
		void deep_copy(impl& x)
		{
			lower_bound = x.lower_bound;
			upper_bound = x.upper_bound;
		}
	private:
		T lower_bound = T();
		T upper_bound = T();
	};




	//////////////////////////////////
	/*								*/
	/*	interval member functions	*/
	/*								*/
	//////////////////////////////////


	/*	interval four compound assignment operator	*/

	template<typename T>
	const interval<T> interval<T>::operator +=(const interval& x)
	{
		pimpl->operator+=(*(x.pimpl));
		return *this;
	}
	template<typename T>
	const interval<T> interval<T>::operator -=(const interval& x)
	{
		pimpl->operator-=(*(x.pimpl));
		return *this;
	}
	template<typename T>
	const interval<T> interval<T>::operator *=(const interval& x)
	{
		pimpl->operator*=(*(x.pimpl));
		return *this;
	}
	template<typename T>
	const interval<T> interval<T>::operator /=(const interval& x)
	{
		pimpl->operator/=(*(x.pimpl));
		return *this;
	}

	/*	interval increment/decrement operator	*/

	template<typename T>
	const interval<T> interval<T>::operator ++()
	{
		(*pimpl)++;
		return *this;
	}
	template<typename T>
	const interval<T> interval<T>::operator ++(int)
	{
		auto tmp(*this);
		(*pimpl)++;
		return tmp;

	}
	template<typename T>
	const interval<T> interval<T>::operator --()
	{
		(*pimpl)--;
		return *this;
	}
	template<typename T>
	const interval<T> interval<T>::operator --(int)
	{
		auto tmp(*this);
		(*pimpl)--;
		return tmp;
	}

	/*	interval numeric	*/

	template<typename T>
	const interval<T> interval<T>::cos() const
	{
		int c;
		auto a = pimpl->get_low(), b = pimpl->get_up();
		if (b - a >= 2.0*PI<T>())
		{
			return interval<T>(static_cast<T>(-1.0), static_cast<T>(1.0));
		}
		/*  base point set  */
		c = static_cast<int>(nextafter(b / 2.0 / PI<T>(), max<T>()));
		/* checking  */
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
	template<typename T>
	const interval<T> interval<T>::sin() const
	{
		int c;
		auto a = pimpl->get_low(), b = pimpl->get_up();
		if (b - a >= 2.0*PI<T>())
		{
			return interval<T>(static_cast<T>(-1.0), static_cast<T>(1.0));
		}
		/*  base point set  */
		c = static_cast<int>(nextafter((b * 2.0 / PI<T>() - 1.0) / 4.0, max<T>()));
		/* checking  */
		if (nextafter(PI<T>() / 2.0*(1.0 + c*4.0), max<T>()) >= a &&
			nextafter(PI<T>() / 2.0*(1 + c*4.0), -max<T>()) <= b)
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
	template<typename T>
	const interval<T> interval<T>::pow(int n) const
	{
		if (n < 0){
			return this->pow(-1 * n);
		}
		else if (n == 0){
			return interval<T>(static_cast<T>(1.0), static_cast<T>(1.0));
		}
		else if (n % 2 == 0){
			if (pimpl->get_low() <= 0.0){
				return interval<T>(
					nextafter(std::pow(pimpl->get_up(), n), -max<T>()),
					nextafter(std::pow(pimpl->get_low(), n), max<T>()));
			}
			else if (pimpl->get_low() <= T() && pimpl->get_up() >= T()){
				return interval<T>(T(), std::fmax(
					nextafter(std::pow(pimpl->get_low(), n), -max<T>()),
					nextafter(std::pow(pimpl->get_up(), n), max<T>())));
			}
		}
		return interval<T>(
			nextafter(std::pow(pimpl->get_low(), n), -max<T>()),
			nextafter(std::pow(pimpl->get_up(), n), max<T>()));
	}
	template<typename T>
	const interval<T> interval<T>::sqrt() const
	{
		if (pimpl->get_low() < T()){ throw Interval::logic_error("sqrt arg requires positive number"); }
		return interval<T>(
			nextafter(std::sqrt(pimpl->get_low()), -max<T>()),
			nextafter(std::sqrt(pimpl->get_up()), max<T>()));
	}
	template<typename T>
	const interval<T> interval<T>::exp() const
	{
		return interval<T>(
			nextafter(std::exp(pimpl->get_low()), -max<T>()),
			nextafter(std::exp(pimpl->get_up()), max<T>()));
	}
	template<typename T>
	const interval<T> interval<T>::log() const
	{
		if (pimpl->get_low() <= 0.0){ throw Interval::logic_error("anti-logarithm less than or equal to zero"); }
		return interval<T>(
			nextafter(std::log(pimpl->get_low()), -max<T>()),
			nextafter(std::log(pimpl->get_up()), max<T>()));
	}
	template<typename T>
	const interval<T> interval<T>::log10() const
	{
		if (pimpl->get_low() <= 0.0){ throw Interval::logic_error("anti-logarithm less than or equal to zero"); }
		return interval<T>(
			nextafter(std::log(pimpl->get_low()), -max<T>()),
			nextafter(std::log(pimpl->get_up()), max<T>()));
	}
	template<typename T>
	const interval<T> interval<T>::abs() const
	{
		if (pimpl->get_low() < T() && pimpl->get_up() > T())
		{
			return interval<T>(T(), nextafter(std::fmax(std::abs(pimpl->get_low()), std::abs(pimpl->get_up())), max<T>()));
		}
		if (pimpl->get_up() < T())
		{
			return interval<T>(-pimpl->get_up(), -pimpl->get_low());
		}
		else{
			return interval<T>(*this);
		}
	}
	template<typename T>
	const T interval<T>::mid() const
	{
		return (pimpl->get_up() + pimpl->get_low()) / static_cast<T>(2.0);
	}
	template<typename T>
	const T interval<T>::wid() const
	{
		return pimpl->get_up() - pimpl->get_low();
	}

	/*	interval getter and setter	*/

	template<typename T>
	const T interval<T>::get_low() const
	{
		return pimpl->get_low();
	}
	template<typename T>
	const T interval<T>::get_up() const
	{
		return pimpl->get_up();
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

	/*	interval relational discriminator	*/

	template<typename T>
	Interval_Relation interval<T>::relational(interval<T> const& x) const
	{
		if (total_equal(*this, x))
		{
			return Interval_Relation::equal;
		}
		else if (pimpl->get_low() == (x.pimpl)->get_low() && pimpl->get_up() < (x.pimpl)->get_up())
		{
			return Interval_Relation::total_less;
		}
		else if (pimpl->get_low() == (x.pimpl)->get_low() && pimpl->get_up() > (x.pimpl)->get_up())
		{
			return Interval_Relation::total_greater;
		}
		else if (pimpl->get_low() < (x.pimpl)->get_low() && pimpl->get_up() == (x.pimpl)->get_up())
		{
			return Interval_Relation::weak_less;
		}
		else if (pimpl->get_low() > (x.pimpl)->get_low() && pimpl->get_up() == (x.pimpl)->get_up())
		{
			return Interval_Relation::weak_greater;
		}
		else if (pimpl->get_low() < (x.pimpl)->get_low() && pimpl->get_up() < (x.pimpl)->get_up())
		{
			return Interval_Relation::partial_less;
		}
		else if (pimpl->get_low() > (x.pimpl)->get_low() && pimpl->get_up() > (x.pimpl)->get_up())
		{
			return Interval_Relation::partial_less;
		}
		else if (pimpl->get_low() <= (x.pimpl)->get_low() && pimpl->get_up() >= (x.pimpl)->get_up())
		{
			return Interval_Relation::contain;
		}
		else if (pimpl->get_low() >= (x.pimpl)->get_low() && pimpl->get_up() <= (x.pimpl)->get_up())
		{
			return Interval_Relation::part_of;
		}
		else
		{
			return Interval_Relation::niether;
		}
	}


	/*	interval relational operator	*/

	template<typename T>
	bool interval<T>::operator<(interval<T> const& x) const
	{
		return weak_less(*this, x);
	}
	template<typename T>
	bool interval<T>::operator>(interval<T> const& x) const
	{
		return weak_greater(*this, x);
	}

	template<typename T>
	bool interval<T>::operator<=(interval<T> const& x) const
	{
		return pimpl->get_low() <= (x.pimpl)->get_low();
	}
	template<typename T>
	bool interval<T>::operator>=(interval<T> const& x) const
	{
		return pimpl->get_low() >= (x.pimpl)->get_low();
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


	//////////////////////////////////////
	/*									*/
	/*	interval constructor/destructor	*/
	/*									*/
	//////////////////////////////////////


	/*	default	constructor	*/

	template<typename T>
	interval<T>::interval()
		: pimpl{ std::make_unique<impl>() }
	{
		pimpl->do_internal_work();
	}

	/*	two lvalue arg constructor	*/

	template<typename T>
	interval<T>::interval(T& low, T& up)
		: pimpl{ std::make_unique<impl>() }
	{
		if (low > up){ throw invalid_argument("upper_bound less than lower_bound!"); }
		pimpl->do_internal_work(low, up);
	}

	/*	two rvalue arg constructor	*/

	template<typename T>
	interval<T>::interval(T&& low, T&& up)
		: pimpl{ std::make_unique<impl>() }
	{
		if (low > up){ throw invalid_argument("upper_bound less than lower_bound!"); }
		pimpl->do_internal_work(low, up);
	}

	/*	default destructor	*/

	template<typename T>
	interval<T>::~interval() = default;

	/*	move constructor	*/

	template<typename T>
	interval<T>::interval(interval&& x)
		: pimpl{ std::make_unique<impl>() }
	{
		pimpl->deep_copy(*(x.pimpl));
	}

	/*	copy constructor	*/

	template<typename T>
	interval<T>::interval(const interval& x)
		: pimpl{ std::make_unique<impl>() }
	{
		pimpl->deep_copy(*(x.pimpl));
	}

	/*	copy assinment operator	*/

	template<typename T>
	interval<T>& interval<T>::operator=(interval&&) = default;

	/*	interval output	*/

	template<typename T>
	std::ostream& interval<T>::print(std::ostream& s) const
	{
		return pimpl->print(s);
	}


	//////////////////////////////
	/*							*/
	/*	impl member functions	*/
	/*							*/
	//////////////////////////////


	/*	four arithmetic compound assignment operators	*/

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
		if (this == &x)
		{
			if (upper_bound <= T())
			{
				lower_bound = nextafter(upper_bound * upper_bound, -max<T>());
				upper_bound = nextafter(lower_bound * lower_bound, max<T>());
				return *this;
			}
			else if (lower_bound <= T() && upper_bound >= T())
			{
				lower_bound = T();
				upper_bound = std::fmax(nextafter(lower_bound * lower_bound, max<T>()), nextafter(upper_bound * upper_bound, max<T>()));
				return *this;
			}
			else
			{
				lower_bound = nextafter(lower_bound * lower_bound, -max<T>());
				upper_bound = nextafter(upper_bound * upper_bound, max<T>());
				return *this;
			}
		}
		else if (lower_bound >= T() && lower_bound >= T()){
			lower_bound = nextafter(lower_bound * x.lower_bound, -max<T>());
			upper_bound = nextafter(upper_bound * x.upper_bound, max<T>());
			return *this;
		}
		else if (lower_bound >= T() && x.lower_bound < T() && x.upper_bound > T())
		{
			lower_bound = nextafter(upper_bound * x.lower_bound, -max<T>());
			upper_bound = nextafter(upper_bound * x.upper_bound, max<T>());
			return *this;
		}
		else if (x.lower_bound >= T() && x.upper_bound <= T())
		{
			lower_bound = nextafter(upper_bound * x.lower_bound, -max<T>());
			upper_bound = nextafter(lower_bound * x.upper_bound, max<T>());
			return *this;
		}
		else if (lower_bound < T() && upper_bound > T() && x.lower_bound >= T())
		{
			lower_bound = nextafter(lower_bound * x.upper_bound, -max<T>());
			upper_bound = nextafter(upper_bound * x.upper_bound, max<T>());
			return *this;
		}
		else if (lower_bound < T() && upper_bound > T() && x.upper_bound <= T())
		{
			lower_bound = nextafter(upper_bound * x.lower_bound, -max<T>());
			upper_bound = nextafter(lower_bound * x.lower_bound, max<T>());
			return *this;
		}
		else if (upper_bound <= T() && x.lower_bound >= T())
		{
			lower_bound = nextafter(lower_bound * x.upper_bound, -max<T>());
			upper_bound = nextafter(upper_bound * x.lower_bound, max<T>());
			return *this;
		}
		else if (upper_bound <= T() && x.lower_bound < T() && x.upper_bound > T())
		{
			lower_bound = nextafter(lower_bound * x.lower_bound, -max<T>());
			upper_bound = nextafter(lower_bound * x.upper_bound, max<T>());
			return *this;
		}
		else if (upper_bound <= T() && x.upper_bound <= T())
		{
			lower_bound = nextafter(upper_bound * x.upper_bound, -max<T>());
			upper_bound = nextafter(lower_bound * x.lower_bound, max<T>());
			return *this;
		}
		else{
			lower_bound = std::fmin(nextafter(upper_bound * x.lower_bound, -max<T>()),
				nextafter(lower_bound * x.upper_bound, -max<T>()));
			upper_bound = std::fmax(nextafter(upper_bound * x.upper_bound, max<T>()),
				nextafter(lower_bound * x.lower_bound, max<T>()));
			return *this;
		}
	}
	template<typename T>
	const typename interval<T>::impl interval<T>::impl::operator/=(const typename interval<T>::impl& x)
	{
		if (x.lower_bound <= T() && x.upper_bound >= T())
		{
			throw logic_error("Divided by Interval whitch contains Zero!");
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

	/*	increment / decrement operator	*/

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



	/*	getter and setter	*/

	template<typename T>
	const T interval<T>::impl::get_low() const
	{
		return lower_bound;
	}
	template<typename T>
	const T interval<T>::impl::get_up() const
	{
		return upper_bound;
	}

	/*	interval output	*/

	template<typename T>
	std::ostream& interval<T>::impl::print(std::ostream& s) const
	{
		return s << "[ " << lower_bound << " , " << upper_bound << " ]";
	}







	//////////////////////////////////////////////////////
	/*													*/
	/*	generic ver interval numeric functions overload	*/
	/*													*/
	//////////////////////////////////////////////////////





	template<typename T>
	interval<T> sin(const interval<T>& a) { return a.sin(); }
	template<typename T>
	interval<T> cos(const interval<T>& a) { return a.cos(); }
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

	/*	generalized numeric functions for primitive dispach	*/
	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	double sin(T&& x) { return std::sin(std::forward<T>(x)); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	double cos(T&& x) { return std::cos(std::forward<T>(x)); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	double exp(T&& x) { return std::exp(std::forward<T>(x)); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	double pow(T&& x, int n) { return std::pow(std::forward<T>(x), n); }

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value >>
	double abs(T&& x) { return std::abs(std::forward<T>(x)); }

	/*	generalized operator<< for classes have menber func print( ostream& )	*/
	template <typename T, std::enable_if_t<std::is_same<Interval::interval_ordering, std::decay_t<T>>::value>*& = enabler >
	std::string enum2string(T&& x)
	{
		switch (x)
		{
		case Interval::interval_ordering::less:
			return std::string("less");
		case Interval::interval_ordering::greater:
			return std::string("greater");
		case Interval::interval_ordering::unordered:
			return std::string("unordered");
		}
	}
	template <typename T, std::enable_if_t<std::is_same<Interval::partial_ordering, std::decay_t<T>>::value>*& = enabler >
	std::string enum2string(T&& x)
	{
		switch (x)
		{
		case Interval::partial_ordering::less:
			return std::string("less");
		case Interval::partial_ordering::greater:
			return std::string("greater");
		case Interval::partial_ordering::unordered:
			return std::string("unordered");
		}
	}
	template <typename T, std::enable_if_t<std::is_same<Interval::weak_ordering, std::decay_t<T>>::value>*& = enabler >
	std::string enum2string(T&& x)
	{
		switch (x)
		{
		case Interval::weak_ordering::less:
			return std::string("less");
		case Interval::weak_ordering::greater:
			return std::string("greater");
		case Interval::weak_ordering::equivalent:
			return std::string("equivalent");
		}
	}
	template <typename T, std::enable_if_t<std::is_same<Interval::total_ordering, std::decay_t<T>>::value>*& = enabler >
	std::string enum2string(T&& x)
	{
		switch (x)
		{
		case Interval::total_ordering::less:
			return std::string("less");
		case Interval::total_ordering::greater:
			return std::string("greater");
		case Interval::total_ordering::equal:
			return std::string("equal");
		}
	}
	template <typename T, std::enable_if_t<std::is_same<Interval::Interval_Relation, std::decay_t<T>>::value>*& = enabler >
	std::string enum2string(T&& x)
	{
		switch (x)
		{
		case Interval::Interval_Relation::interval_less:
			return std::string("interval_less");
		case Interval::Interval_Relation::interval_greater:
			return std::string("interval_greater");
		case Interval::Interval_Relation::partial_less:
			return std::string("partial_less");
		case Interval::Interval_Relation::partial_greater:
			return std::string("partial_greater");
		case Interval::Interval_Relation::weak_less:
			return std::string("weak_less");
		case Interval::Interval_Relation::weak_greater:
			return std::string("weak_greater");
		case Interval::Interval_Relation::contain:
			return std::string("contain");
		case Interval::Interval_Relation::part_of:
			return std::string("part_of");
		case Interval::Interval_Relation::niether:
			return std::string("niether");
		}
	}

	template <typename T, std::enable_if_t<std::is_same<Interval::interval_ordering, std::decay_t<T>>::value>*& = enabler >
	std::ostream& operator<<(std::ostream& s, T&& x)
	{
		return (s << enum2string(x));
	}
	template <typename T, std::enable_if_t<std::is_same<Interval::partial_ordering, std::decay_t<T>>::value>*& = enabler >
	std::ostream& operator<<(std::ostream& s, T&& x)
	{
		return (s << enum2string(x));
	}
	template <typename T, std::enable_if_t<std::is_same<Interval::weak_ordering, std::decay_t<T>>::value>*& = enabler >
	std::ostream& operator<<(std::ostream& s, T&& x)
	{
		return (s << enum2string(x));
	}
	template <typename T, std::enable_if_t<std::is_same<Interval::total_ordering, std::decay_t<T>>::value>*& = enabler >
	std::ostream& operator<<(std::ostream& s, T&& x)
	{
		return (s << enum2string(x));
	}
	template <typename T, std::enable_if_t<std::is_same<Interval::Interval_Relation, std::decay_t<T>>::value>*& = enabler >
	std::ostream& operator<<(std::ostream& s, T&& x)
	{
		return (s << enum2string(x));
	}

	template <typename T>
	auto operator<<(std::ostream& s, T&& x)->decltype(x.print(s))
	{
		return x.print(s);
	}
	/*	generic wid	*/

	template< typename T >
	auto wid(T&& x)->decltype(x.wid())
	{
		return x.wid();
	}
	/*	generic mid	*/

	template< typename T >
	auto mid(T&& x)->decltype(x.mid())
	{
		return x.mid();
	}

	/*	interval addition operator	*/

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value>*& = enabler>
	interval<U> operator +(T&& x, const interval<U>& y)
	{
		return (interval<U>(y.get_low() + x, y.get_up() + x));
	}

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value>*& = enabler>
	interval<U> operator +(const interval<U>& x, T&& y)
	{
		return (interval<U>(x.get_low() + y, x.get_up() + y));
	}

	template<typename T>
	const interval<T> operator +(const interval<T>& x, const interval<T>& y)
	{
		return interval<T>(x) += y;
	}

	/*	interval subtraction operator	*/

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value>*& = enabler>
	interval<U> operator -(T&& x, const interval<U>& y)
	{
		return (interval<U>(y.get_low() - x, y.get_up() - x));
	}

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value>*& = enabler>
	interval<U> operator -(const interval<U>& x, T&& y)
	{
		return (interval<U>(x.get_low() - y, x.get_up() - y));
	}

	template<typename T>
	const interval<T> operator -(const interval<T>& x, const interval<T>& y)
	{
		return interval<T>(x) -= y;
	}

	/*	interval multiplication operator	*/

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value>*& = enabler>
	interval<U> operator *(T&& x, const interval<U>& y)
	{
		if (x >= T())
			return (interval<U>(y.get_low() * x, y.get_up() * x));
		else
			return (interval<U>(y.get_up() * x, y.get_low() * x));
	}

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value>*& = enabler>
	interval<U> operator *(const interval<U>& x, T&& y)
	{
		if (y >= T())
			return (interval<U>(x.get_low() * y, x.get_up() * y));
		else
			return (interval<U>(x.get_up() * y, x.get_low() * y));
	}

	template<typename T>
	const interval<T> operator *(const interval<T>& x, const interval<T>& y)
	{
		return interval<T>(x) *= y;
	}

	/*	interval division operator	*/

	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value>*& = enabler>
	interval<U> operator /(T&& x, const interval<U>& y)
	{
		if (y.get_low() <= T() && T() <= y.get_up())
			throw Interval::logic_error("Divided by Interval whitch contains zero!");
		else if (y.get_low() > T())
			return (interval<U>(y.get_low() / x, y.get_up() / x));
		else
			return (interval<U>(y.get_up() / x, y.get_low() / x));
	}


	template <typename T, typename U, std::enable_if_t<!std::is_same<interval<U>, std::decay_t<T>>::value>*& = enabler>
	interval<U> operator /(const interval<U>& x, T&& y)
	{
		if (y == T())
			throw Interval::logic_error("Divided by Zero!");
		else if (y > T())
			return (interval<U>(x.get_low() / y, x.get_up() / y));
		else
			return (interval<U>(x.get_up() / y, x.get_low() / y));
	}

	template<typename T>
	const interval<T> operator /(const interval<T>& x, const interval<T>& y)
	{
		return interval<T>(x) /= y;
	}

	/*	compound assignment operator	*/

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





	//////////////////////
	/*	interval output */
	//////////////////////

	/*	iostream	*/

	template<typename T>
	std::ostream& operator<<(std::ostream& s, std::unique_ptr<T> x)
	{
		s << (*x);
		return (s);
	}

	/*	C string	*/

	template<typename T>
	const char* Interval::interval<T>::c_str() const
	{
		auto s = new std::string("[ " + std::to_string(this->get_low()) + " , " + std::to_string(this->get_up()) + " ]");
		return (s->c_str());
	}

	//////////////////////////////////
	/*	argument dependent factory	*/
	//////////////////////////////////

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
		return interval<T>(low, up);
	}
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


	//////////////////////////////////////
	/*	interval relational functions	*/
	//////////////////////////////////////



	/*	interval ordering	*/

	template<typename T>
	bool interval_less(interval<T> const& x, interval<T> const& y)
	{
		return (x.get_up() < y.get_low());
	}
	template<typename T>
	bool interval_greater(interval<T> const& x, interval<T> const& y)
	{
		return (x.get_low() > y.get_up());
	}
	template<typename T>
	bool interval_unordered(interval<T> const& x, interval<T> const& y)
	{
		return (interval_less(x, y) == false && interval_greater(x, y) == false);
	}


	/*	partial ordering	*/

	template<typename T>
	bool partial_less(interval<T> const& x, interval<T> const& y)
	{
		return (x.get_low() < y.get_low());
	}

	template<typename T>
	bool partial_greater(interval<T> const& x, interval<T> const& y)
	{
		return (x.get_low() > y.get_low());
	}

	template<typename T>
	bool partial_unordered(interval<T> const& x, interval<T> const& y)
	{
		return (partial_less(x, y) == false && partial_greater(x, y) == false);
	}


	/*	weak ordering	*/

	template<typename T>
	bool weak_less(interval<T> const& x, interval<T> const& y)
	{
		return (x.get_low() < y.get_low());
	}

	template<typename T>
	bool weak_greater(interval<T> const& x, interval<T> const& y)
	{
		return (x.get_low() > y.get_low());
	}

	template<typename T>
	bool weak_equal(interval<T> const& x, interval<T> const& y)
	{
		return (weak_less(x, y) == false && weak_less(y, x) == false);
	}


	/*	total ordering	*/

	template<typename T>
	bool total_less(interval<T> const& x, interval<T> const& y)
	{
		return (x.get_low() < y.get_low() || x.get_up() < y.get_up());
	}

	template<typename T>
	bool total_greater(interval<T> const& x, interval<T> const& y)
	{
		return (x.get_low() > y.get_low() || x.get_up() > y.get_up());
	}

	template<typename T>
	bool total_equal(interval<T> const& x, interval<T> const& y)
	{
		return (x.get_low() == y.get_low() && x.get_up() == y.get_up());
	}


	//////////////////////////////
	/*	ordering discriminator	*/
	//////////////////////////////

	/*	interval ordering	*/

	template<typename T>
	Interval::interval_ordering interval_order(Interval::interval<T>& x, Interval::interval<T>& y)
	{
		if (interval_less(x, y)){ return Interval::interval_ordering::less; }
		else if (interval_greater(x, y)){ return Interval::interval_ordering::greater; }
		else{ return Interval::interval_ordering::unordered; }
	}

	/*	partial ordering	*/

	template<typename T>
	Interval::partial_ordering partial_order(Interval::interval<T>& x, Interval::interval<T>& y)
	{
		if (partial_less(x, y)){ return partial_ordering::less; }
		else if (partial_greater(x, y)){ return partial_ordering::greater; }
		else{ return partial_ordering::unordered; }
	}

	/*	weak ordering	*/

	template<typename T>
	Interval::weak_ordering weak_order(Interval::interval<T>& x, Interval::interval<T>& y)
	{
		if (weak_less(x, y)){ return weak_ordering::less; }
		else if (weak_greater(x, y)){ return weak_ordering::greater; }
		else{ return weak_ordering::equivalent; }
	}

	/*	total ordering	*/

	template<typename T>
	Interval::total_ordering total_order(Interval::interval<T>& x, Interval::interval<T>& y)
	{
		if (total_less(x, y)){ return total_ordering::less; }
		else if (total_greater(x, y)){ return total_ordering::greater; }
		else{ return total_ordering::equal; }
	}
	template<typename T>
	bool interval<T>::is_contain(interval const& x) const
	{
		return (this->relational(x) == Interval_Relation::contain);
	}
	template<typename T>
	bool interval<T>::is_part_of(interval const& x) const
	{
		return (this->relational(x) == Interval_Relation::part_of);
	}


}//end namespace Interval
#endif //!INTERVAL_H