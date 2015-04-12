#ifndef INTERVAL_CPP
#define INTERVAL_CPP

#include "Interval.h"
#include <exception>
#include <cmath>
#include <typeinfo>
#include <type_traits>
#include <iostream>

using std::nextafter;

namespace Interval{

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
	int min(){ return 1; }

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
	class interval_except : public interval_error
	{
	public:
		interval_except(std::string& cause)
			:interval_error("Error massage : " + cause)
		{}
		interval_except(const char *cause)
			:interval_error(std::string("Error massage : ") + std::string(cause))
		{}
	};

	class range_error : public interval_except
	{
	public:
		range_error(std::string& cause)
			:interval_except(cause)
		{}
		range_error(const char *cause)
			:interval_except(cause)
		{}
	};

	class domain_error : public interval_except
	{
	public:
		domain_error(std::string& cause)
			:interval_except(cause)
		{}
		domain_error(const char *cause)
			:interval_except(cause)
		{}
	};

	class logic_error : public interval_except
	{
	public:
		logic_error(std::string& cause)
			:interval_except(cause)
		{}
		logic_error(const char *cause)
			:interval_except(cause)
		{}
	};

	class invalid_argument : public interval_except
	{
	public:
		invalid_argument(std::string& cause)
			:interval_except(cause)
		{}
		invalid_argument(const char *cause)
			:interval_except(cause)
		{}
	};

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

		bool is_equal_to(impl const&) const;
		bool is_weak_greater(impl const&, flag const& f = flag::off) const;
		bool is_weak_less(impl const&, flag const& f = flag::off) const;
		bool is_absolute_greater(impl const&, flag const& f = flag::off) const;
		bool is_absolute_less(impl const&, flag const& f = flag::off) const;
		bool partial_greater(impl const&, flag const& f = flag::off) const;
		bool partial_less(impl const&, flag const& f = flag::off) const;
		bool is_contain(impl const&) const;
		bool is_part_of(impl const&) const;
		Interval_Relation relation(impl const&) const;

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

	/*interval*/
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

	template<typename T>
	const interval<T> interval<T>::sin() const
	{
		return pimpl->sin();
	}
	template<typename T>
	const interval<T> interval<T>::cos() const
	{
		return pimpl->cos();
	}
	template<typename T>
	const interval<T> interval<T>::pow(int n) const
	{
		return interval<T>(pimpl->pow(n));
	}
	template<typename T>
	const interval<T> interval<T>::exp() const
	{
		return interval<T>(pimpl->exp());
	}
	template<typename T>
	const interval<T> interval<T>::abs() const
	{
		return interval<T>(pimpl->abs());
	}
	template<typename T>
	const T interval<T>::mid() const
	{
		return pimpl->mid();
	}
	template<typename T>
	const T interval<T>::wid() const
	{
		return pimpl->wid();
	}
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

	template<typename T>
	bool interval<T>::is_equal_to(interval<T> const& x) const
	{
		return pimpl->is_equal_to(*(x.pimpl));
	}
	template<typename T>
	bool interval<T>::is_weak_greater(interval<T> const& x, flag const& f = flag::off) const
	{
		return pimpl->is_weak_greater(*(x.pimpl),f);
	}
	template<typename T>
	bool interval<T>::is_weak_less(interval<T> const& x, flag const& f = flag::off) const
	{
		return pimpl->is_weak_less(*(x.pimpl),f);
	}
	template<typename T>
	bool interval<T>::is_absolute_greater(interval<T> const& x, flag const& f = flag::off) const
	{
		return pimpl->is_absolute_greater(*(x.pimpl),f);
	}
	template<typename T>
	bool interval<T>::is_absolute_less(interval<T> const& x, flag const& f = flag::off) const
	{
		return pimpl->is_absolute_less(*(x.pimpl),f);
	}
	template<typename T>
	bool interval<T>::partial_greater(interval<T> const& x, flag const& f = flag::off) const
	{
		return pimpl->partial_greater(*(x.pimpl),f);
	}
	template<typename T>
	bool interval<T>::partial_less(interval<T> const& x, flag const& f = flag::off) const
	{
		return pimpl->partial_less(*(x.pimpl),f);
	}
	template<typename T>
	bool interval<T>::is_contain(interval<T> const& x) const
	{
		return pimpl->is_contain(*(x.pimpl));
	}
	template<typename T>
	bool interval<T>::is_part_of(interval<T> const& x) const
	{
		return pimpl->is_part_of(*(x.pimpl));
	}
	template<typename T>
	Interval_Relation interval<T>::relation(interval<T> const& x) const
	{
		return pimpl->relation(*(x.pimpl));
	}

	template<typename T>
	bool interval<T>::operator<(interval<T> const& x) const
	{
		return pimpl->operator<(*(x.pimpl));
	}
	template<typename T>
	bool interval<T>::operator>(interval<T> const& x) const
	{
		return pimpl->operator>(*(x.pimpl));
	}
	template<typename T>
	bool interval<T>::operator<=(interval<T> const& x) const
	{
		return pimpl->operator<=(*(x.pimpl));
	}
	template<typename T>
	bool interval<T>::operator>=(interval<T> const& x) const
	{
		return pimpl->operator>=(*(x.pimpl));
	}
	template<typename T>
	bool interval<T>::operator==(interval<T> const& x) const
	{
		return pimpl->operator==(*(x.pimpl));
	}
	template<typename T>
	bool interval<T>::operator!=(interval<T> const& x) const
	{
		return pimpl->operator==(*(x.pimpl));
	}
	template<typename T>
	interval<T>::interval()
		: pimpl{ std::make_unique<impl>() }
	{
		pimpl->do_internal_work();
	}
	template<typename T>
	interval<T>::interval(T& low, T& up)
		: pimpl{ std::make_unique<impl>() }
	{
		if (low > up){ throw invalid_argument("upper_bound less than lower_bound!"); }
		pimpl->do_internal_work(low, up);
	}
	template<typename T>
	interval<T>::interval(T&& low, T&& up)
		: pimpl{ std::make_unique<impl>() }
	{
		if (low > up){ throw invalid_argument("upper_bound less than lower_bound!"); }
		pimpl->do_internal_work(low, up);
	}
	template<typename T>
	interval<T>::~interval() = default;

	template<typename T>
	interval<T>::interval(interval&& x)
		: pimpl{ std::make_unique<impl>() }
	{
		pimpl->deep_copy(*(x.pimpl));
	}
	template<typename T>
	interval<T>::interval(const interval& x)
		: pimpl{ std::make_unique<impl>() }
	{
		pimpl->deep_copy(*(x.pimpl));
	}

	template<typename T>
	interval<T>& interval<T>::operator=(interval&&) = default;

	template<typename T>
	std::ostream& interval<T>::print(std::ostream& s) const
	{
		return pimpl->print(s);
	}

	/*impl*/

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
				upper_bound = std::fmax(nextafter(lower_bound * lower_bound , max<T>()), nextafter(upper_bound * upper_bound, max<T>()));
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
			lower_bound = nextafter(upper_bound * x.lower_bound , -max<T>());
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
			lower_bound = nextafter(upper_bound * x.lower_bound , -max<T>());
			upper_bound = nextafter(lower_bound * x.lower_bound , max<T>());
			return *this;
		}
		else if (upper_bound <= T() && x.lower_bound >= T())
		{
			lower_bound = nextafter(lower_bound * x.upper_bound, -max<T>());
			upper_bound = nextafter(upper_bound * x.lower_bound , max<T>());
			return *this;
		}
		else if (upper_bound <= T() && x.lower_bound < T() && x.upper_bound > T())
		{
			lower_bound = nextafter(lower_bound * x.lower_bound , -max<T>());
			upper_bound = nextafter(lower_bound * x.upper_bound, max<T>());
			return *this;
		}
		else if (upper_bound <= T() && x.upper_bound <= T())
		{
			lower_bound = nextafter(upper_bound * x.upper_bound, -max<T>());
			upper_bound = nextafter(lower_bound * x.lower_bound , max<T>());
			return *this;
		}
		else{
			lower_bound = std::fmin(nextafter(upper_bound * x.lower_bound , -max<T>()),
				nextafter(lower_bound * x.upper_bound, -max<T>()));
			upper_bound = std::fmax(nextafter(upper_bound * x.upper_bound, max<T>()),
				nextafter(lower_bound * x.lower_bound , max<T>()));
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

	template<typename T>
	const typename interval<T> interval<T>::impl::cos() const
	{
		int c;
		auto a = lower_bound, b = upper_bound;
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
	const typename interval<T> interval<T>::impl::sin() const
	{
		int c;
		auto a = lower_bound, b = upper_bound;
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
	const typename interval<T> interval<T>::impl::pow(int n) const
	{
		if (n < 0){
			return this->pow(-1 * n);
		}
		else if (n == 0){
			return interval<T>(static_cast<T>(1.0), static_cast<T>(1.0));
		}
		else if (n % 2 == 0){
			if (upper_bound <= 0.0){
				return interval<T>(
					nextafter(std::pow(upper_bound, n), -max<T>()),
					nextafter(std::pow(lower_bound, n), max<T>()));
			}
			else if (lower_bound <= T() && upper_bound >= T()){
				return interval<T>(T(), std::fmax(
					nextafter(std::pow(lower_bound, n), max<T>()),
					nextafter(std::pow(upper_bound, n), max<T>())));
			}
		}
		return interval<T>(
			nextafter(std::pow(lower_bound, n), -max<T>()),
			nextafter(std::pow(upper_bound, n), max<T>()));
	}	
	template<typename T>
	const interval<T> interval<T>::impl::exp() const
	{
		return interval<T>(
			nextafter(std::exp(lower_bound), -max<T>()),
			nextafter(std::exp(upper_bound), max<T>()));
	}
	template<typename T>
	const interval<T> interval<T>::impl::abs() const
	{
		if (lower_bound < T())
		{
			return interval<T>(T(), std::fmax(std::abs(lower_bound), std::abs(upper_bound)));
		}
		else{
			return interval<T>(static_cast<T>(lower_bound),static_cast<T>(upper_bound));
		}
	}	
	template<typename T>
	const T interval<T>::impl::mid() const
	{
		return ( upper_bound + lower_bound ) / static_cast<T>(2.0);
	}
	template<typename T>
	const T interval<T>::impl::wid() const
	{
		return upper_bound - lower_bound;
	}
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

	template<typename T>
	bool interval<T>::impl::is_equal_to(typename interval<T>::impl const& x) const
	{
		return (upper_bound == x.get_up() && lower_bound == x.get_low());
	}
	template<typename T>
	bool interval<T>::impl::is_weak_greater(typename interval<T>::impl const& x, flag const& f = flag::off) const
	{
		if (f==flag::off)
			return (upper_bound > x.get_up() && lower_bound > x.get_low());
		else
			return (upper_bound >= x.get_up() && lower_bound >= x.get_low());
	}
	template<typename T>
	bool interval<T>::impl::is_weak_less(typename interval<T>::impl const& x, flag const& f = flag::off) const
	{
		if (f==flag::off)
			return (upper_bound < x.get_up() && lower_bound < x.get_low());
		else
			return (upper_bound <= x.get_up() && lower_bound <= x.get_low());
	}
	template<typename T>
	bool interval<T>::impl::is_absolute_greater(typename interval<T>::impl const& x, flag const& f = flag::off) const
	{
		if (f==flag::off)
			return lower_bound > x.get_up();
		else
			return lower_bound >= x.get_up();
	}
	template<typename T>
	bool interval<T>::impl::is_absolute_less(typename interval<T>::impl const& x, flag const& f = flag::off) const
	{
		if (f==flag::off)
			return upper_bound < x.get_low();
		else
			return upper_bound <= x.get_low();
	}
	template<typename T>
	bool interval<T>::impl::partial_greater(typename interval<T>::impl const& x, flag const& f = flag::off) const
	{
		if (f==flag::off)
			return upper_bound > x.get_up();
		else
			return upper_bound >= x.get_up();
	}
	template<typename T>
	bool interval<T>::impl::partial_less(typename interval<T>::impl const& x, flag const& f = flag::off) const
	{
		if (f==flag::off)
			return lower_bound < x.get_low();
		else
			return lower_bound <= x.get_low();
	}
	template<typename T>
	bool interval<T>::impl::is_contain(typename interval<T>::impl const& x) const
	{
		return (upper_bound >= x.get_up() && lower_bound <= x.get_low());
	}
	template<typename T>
	bool interval<T>::impl::is_part_of(typename interval<T>::impl const& x) const
	{
		return (upper_bound <= x.get_up() && lower_bound >= x.get_low());
	}
	template<typename T>
	Interval_Relation interval<T>::impl::relation(typename interval<T>::impl const& x) const
	{
		if (this == &x)
			return Interval_Relation::equality;
		else if (this->is_equal_to(x))
			return Interval_Relation::equivalence;
		else if (this->is_absolute_greater(x))
			return Interval_Relation::absolute_greater;
		else if (this->is_absolute_less(x))
			return Interval_Relation::absolute_less;
		else if (this->is_absolute_greater(x, flag::on))
			return Interval_Relation::absolute_greater_or_equal;
		else if (this->is_absolute_less(x, flag::on))
			return Interval_Relation::absolute_less_or_equal;
		else if (this->is_weak_greater(x))
			return Interval_Relation::weak_greater;
		else if (this->is_weak_less(x))
			return Interval_Relation::weak_less;
		else if (this->is_contain(x))
			return Interval_Relation::contain;
		else if (x.is_contain(*this))
			return Interval_Relation::part_of;
		else if (this->is_weak_greater(x, flag::on))
			return Interval_Relation::weak_greater_or_equal;
		else if (this->is_weak_less(x, flag::on))
			return Interval_Relation::weak_less_or_equal;
		else
			return Interval_Relation::niether;
	}

	template<typename T>
	bool interval<T>::impl::operator<(typename interval<T>::impl const& x) const
	{
		return this->partial_less(x);
	}
	template<typename T>
	bool interval<T>::impl::operator>(typename interval<T>::impl const& x) const
	{
		return this->partial_greater(x);
	}
	template<typename T>
	bool interval<T>::impl::operator<=(typename interval<T>::impl const& x) const
	{
		return this->partial_less(x,flag::on);
	}
	template<typename T>
	bool interval<T>::impl::operator>=(typename interval<T>::impl const& x) const
	{
		return this->partial_greater(x,flag::on);
	}
	template<typename T>
	bool interval<T>::impl::operator==(typename interval<T>::impl const& x) const
	{
		return this->is_equal_to(x);
	}
	template<typename T>
	bool interval<T>::impl::operator!=(typename interval<T>::impl const& x) const
	{
		return !(this->is_equal_to(x));
	}

	template<typename T>
	std::ostream& interval<T>::impl::print(std::ostream& s) const
	{
		return s << "[ " << lower_bound << " , " << upper_bound << " ]";
	}


	/*generalized*/
	/*generic ver interval numeric functions overload*/
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

	/*generic numeric functions for primitive dispach*/
	/*Using Forwarding Reference & type_traits*/
	template <typename T, typename = typename std::enable_if <
		!std::is_same<interval, typename std::decay<T>::type>::value>::type>
		double sin(T&& x) { return std::sin(std::forward<T>(x)); }

	template <typename T, typename = typename std::enable_if <
		!std::is_same<interval, typename std::decay<T>::type>::value>::type>
		double cos(T&& x) { return std::cos(std::forward<T>(x)); }

	template <typename T, typename = typename std::enable_if <
		!std::is_same<interval, typename std::decay<T>::type>::value>::type>
		double exp(T&& x) { return std::exp(std::forward<T>(x)); }

	template <typename T, typename = typename std::enable_if <
		!std::is_same<interval, typename std::decay<T>::type>::value>::type>
		double pow(T&& x, int n) { return std::pow(std::forward<T>(x), n); }

	template <typename T, typename = typename std::enable_if <
		!std::is_same<interval, typename std::decay<T>::type>::value>::type>
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
	template <typename T,typename U, typename = typename std::enable_if <
		!std::is_same<interval<U>, typename std::decay<T>::type>::value>::type>
		interval<U> operator +(T&& x, const interval<U>& y) { return (interval<U>(y.get_low() + x, y.get_up() + x)); }

	template <typename T,typename U, typename = typename std::enable_if <
		!std::is_same<interval<U>, typename std::decay<T>::type>::value>::type>
		interval<U> operator +(const interval<U>& x, T&& y) { return (interval<U>(x.get_low() + y, x.get_up() + y)); }

	template<typename T>
	const interval<T> operator +(const interval<T>& x, const interval<T>& y)
	{
		return interval<T>(x) += y;
	}
	//interval subtraction operator
	template <typename T,typename U, typename = typename std::enable_if <
		!std::is_same<interval<U>, typename std::decay<T>::type>::value>::type>
		interval<U> operator -(T&& x, const interval<U>& y) { return (interval<U>(y.get_low() - x, y.get_up() - x)); }

	template <typename T,typename U, typename = typename std::enable_if <
		!std::is_same<interval<U>, typename std::decay<T>::type>::value>::type>
		interval<U> operator -(const interval<U>& x, T&& y) { return (interval<U>(x.get_low() - y, x.get_up() - y)); }

	template<typename T>
	const interval<T> operator -(const interval<T>& x, const interval<T>& y)
	{
		return interval<T>(x) -= y;
	}
	//interval multiplication operator
	template <typename T,typename U, typename = typename std::enable_if <
		!std::is_same<interval<U>, typename std::decay<T>::type>::value>::type>
		interval<U> operator *(T&& x, const interval<U>& y)
	{
		if (x >= T())
			return (interval<U>(y.get_low() * x, y.get_up() * x));
		else
			return (interval<U>(y.get_up() * x, y.get_low() * x));
	}

	template <typename T,typename U, typename = typename std::enable_if <
		!std::is_same<interval<U>, typename std::decay<T>::type>::value>::type>
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
	//interval division operator
	template <typename T,typename U, typename = typename std::enable_if <
		!std::is_same<interval<U>, typename std::decay<T>::type>::value>::type>
		interval<U> operator /(T&& x, const interval<U>& y)
	{
		if (y.get_low() <= T() && T() <= y.get_up())
			throw Interval::logic_error("Divided by Interval whitch contains zero!");
		else if (y.get_low() > T())
			return (interval<U>(y.get_low() / x, y.get_up() / x));
		else
			return (interval<U>(y.get_up() / x, y.get_low() / x));
	}


	template <typename T,typename U, typename = typename std::enable_if <
		!std::is_same<interval<U>, typename std::decay<T>::type>::value>::type>
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

	//compound assignment operator
	template <typename T,typename U, typename = typename std::enable_if <
		!std::is_same<interval<U>, typename std::decay<T>::type>::value>::type>
		interval<U> operator +=(interval<U>& x, T&& y)
	{
		return x.operator+=(interval<U>(y, y));
	}

	template <typename T,typename U, typename = typename std::enable_if <
		!std::is_same<interval<U>, typename std::decay<T>::type>::value>::type>
		interval<U> operator -=(interval<U>& x, T&& y)
	{
		return x.operator-=(interval<U>(y, y));
	}

	template <typename T,typename U, typename = typename std::enable_if <
		!std::is_same<interval<U>, typename std::decay<T>::type>::value>::type>
		interval<U> operator *=(interval<U>& x, T&& y)
	{
		return x.operator*=(interval<U>(y, y));
	}

	template <typename T,typename U, typename = typename std::enable_if <
		!std::is_same<interval<U>, typename std::decay<T>::type>::value>::type>
		interval<U> operator /=(interval<U>& x, T&& y)
	{
		return x.operator/=(interval<U>(y, y));
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
	bool GT(interval<T> const& x, interval<T> const& y, Comp_Policy const& policy = Comp_Policy::weak, flag const& f = flag::off)
	{
		if (policy == Comp_Policy::absolute)
			return x.is_absolute_greater(y, f);
		else if (policy == Comp_Policy::weak)
			return x.is_weak_greater(y, f);
		else
			return x.partial_greater(y, f);
	}
	template<typename T>
	bool LT(interval<T> const& x, interval<T> const& y, Comp_Policy const& policy = Comp_Policy::weak, flag const& f = flag::off)
	{
		if (policy == Comp_Policy::absolute)
			return x.is_absolute_less(y, f);
		else if (policy == Comp_Policy::weak)
			return x.is_weak_less(y, f);
		else
			return x.partial_less(y, f);
	}
}//end namespace Interval
#endif // !INTERVAL_CPP