#ifndef INTERVAL_H
#define INTERVAL_H
#include <memory>

namespace Interval{

	enum class flag
	{
		on,
		off,
	};

	enum class Comp_Policy
	{
		absolute,
		weak,
		partial,
	};

	enum class Interval_Relation
	{
		equality,
		equivalence,
		absolute_less,
		absolute_greater,
		absolute_less_or_equal,
		absolute_greater_or_equal,
		weak_less,
		weak_greater,
		weak_less_or_equal,
		weak_greater_or_equal,
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

		bool is_equal_to(interval const&) const;
		bool is_weak_greater(interval const&,flag const& f=flag::off) const;
		bool is_weak_less(interval const&, flag const& f = flag::off) const;
		bool is_absolute_greater(interval const&, flag const& f = flag::off) const;
		bool is_absolute_less(interval const&, flag const& f = flag::off) const;
		bool partial_greater(interval const&, flag const& f = flag::off) const;
		bool partial_less(interval const&, flag const& f = flag::off) const;
		bool is_contain(interval const&) const;
		bool is_part_of(interval const&) const;
		Interval_Relation relation(interval const&) const;

		bool operator<(interval const&) const;
		bool operator>(interval const&) const;
		bool operator<=(interval const&) const;
		bool operator>=(interval const&) const;
		bool operator==(interval const&) const;
		bool operator!=(interval const&) const;

		std::ostream& print(std::ostream&) const;

	private:
		class impl;
		std::unique_ptr<impl> pimpl;
	};
}//end namespace Interval
#endif //!INTERVAL_H