#ifndef INTERVAL_H
#define INTERVAL_H
#include <memory>

namespace Interval{

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
		bool is_equal(interval const&) const;
		bool is_interval_less(interval const&) const;
		bool is_interval_greater(interval const&) const;
		bool is_partial_less(interval const&) const;
		bool is_partial_greater(interval const&) const;
		bool is_weak_less(interval const&) const;
		bool is_weak_greater(interval const&) const;
		bool is_total_less(interval const&) const;
		bool is_total_greater(interval const&) const;
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
}//end namespace Interval
#endif //!INTERVAL_H