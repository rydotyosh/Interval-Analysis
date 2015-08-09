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

using std::nextafter ;

namespace Cranberries
{
	/*  version 0x|aa|.|bb|.|cccc|  */
	enum class version_tag
	{
		v1_0_0 = 0x01000000,
		v1_0_1,
		v1_0_2,
		v1_0_3,
		v1_0_4,
		v1_0_5,
		v1_0_6,
		v1_0_7,
		v1_0_8,
		v1_1_0 = 0x01010000,
		v1_1_1,
		v1_2_0 = 0x01020000,
		v2_0_0 = 0x02000000,
		v3_0_0 = 0x03000000,
		now= v2_0_0,
	} ;

	/*  function default for meta programming  */
	extern std::nullptr_t enabler ;

	template < typename T>
	class is_interval
	{
	private:
		template < typename U>
		static auto check( U v ) -> decltype(v.lower(), v.upper(), std::true_type()) ;
		static auto check( ... ) -> decltype(std::false_type()) ;

	public:
		typedef decltype(check(std::declval<T>())) type;
		static bool const value = type::value;
	} ;

	/*  PI  */
	template < typename T >
	constexpr T PI() { return static_cast<T>(3.141592653589793238462643383279L) ; }


	/*  maximum  */
	template < typename T >
	constexpr T max() { return std::numeric_limits<T>::max() ; }

	/*  Zero  */
	template < typename T >
	constexpr T zero() { return static_cast<T>(0.0L) ; }

	/*  One  */
	template < typename T >
	constexpr T one() { return static_cast<T>(1.0L) ; }

	template < typename T >
	T upward( T&& x )
	{
		return nextafter( std::forward<T>( x ), max<T>() ) ;
	}

	template < typename T >
	T downward( T&& x )
	{
		return nextafter( std::forward<T>( x ), -max<T>() ) ;
	}

	//---------------------//
	/*   Ordering Symbol   */
	//---------------------//

	enum class order {Total,Weak,Partial,Interval} ;
	/*  3 Values of Ordering  */
	enum class partial_ordering { less, unordered, greater } ;
	enum class weak_ordering { less, equivalent, greater } ;
	enum class total_ordering { less, equal, greater } ;
	enum class interval_ordering { less, unordered, greater } ;

	/*  Advanced Interval Ordering  */
	enum class interval_relation
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
	} ;



	//----------------------------------//
	/*                                  */
	/*       Class Declaration          */
	/*                                  */
	/*        Interval Class            */
	/*        (Body )                    */
	/*                                  */
	//----------------------------------//



	template < typename T >
	class interval
	{
	public:

		/*  ctor  */
		constexpr interval() ;
		constexpr interval( T&, T& ) ;
		constexpr interval( T&&, T&& ) ;
		constexpr interval( std::initializer_list<T> ) ;

		/*  dtor  */
		~interval( ) ;

		/*  copy ctor  */
		interval( const interval& ) ;
		/*  move ctor  */
		interval( interval&& ) ;
		/*  Copy Assignment Op  */
		interval& operator=( interval const& ) ;
		/*  Move assignment Op  */
		interval& operator=( interval&& ) ;

		interval& operator=( std::initializer_list<T> ) ;


		/*  increment/decrement Op  */
		const interval operator ++() ;
		const interval operator ++(int) ;
		const interval operator --() ;
		const interval operator --(int) ;

		/*  numeric functions  */

		/*  torigonometric functions  */
		const interval sin() const ;
		const interval cos() const ;
		const interval tan() const ;
		const interval asin() const ;
		const interval acos() const ;
		const interval atan() const ;

		/*  hyperbolic functions  */
		const interval sinh() const ;
		const interval cosh() const ;
		const interval tanh() const ;
		const interval asinh() const ;
		const interval acosh() const ;
		const interval atanh() const ;

		/*  exponential & logarithmic functions  */
		const interval log() const ;
		const interval log10() const ;
		const interval log2() const ;
		const interval log1p() const ;
		const interval exp() const ;
		const interval exp2() const ;
		const interval expm1() const ;

		/*  power math & absolute functions  */
		const interval pow(int n) const ;
		const interval sqrt() const ;
		const interval abs() const ;

		/*  error functions  */
		const interval erf() const ;
		const interval erfc() const ;

		/*  interval functions  */
		constexpr T mid() const ;
		constexpr T wid() const ;
		constexpr bool is_singleton() const ;

		/*  accessors  */
		// getter
		constexpr T lower() const ;
		constexpr T upper() const ;
		// setter
		void set_upper( T&& ) ;
		void set_lower( T&& ) ;
		void set( T&&,T&& ) ;
		// swap
		void swap( interval& ) noexcept ;

		/*  Advanced Relational Op  */
		interval_relation relational( interval const& ) const ;
		bool is_contain( interval const& ) const ;
		bool is_contain( T const& x ) const ;
		bool is_part_of( interval const& ) const ;


		/*  output stream function  */
		std::ostream& print( std::ostream& ) const ;
		/*  For printf  */
		const char* c_str() const ;
		/*  Member value type  */
		typedef T value_type ;
	private:
		/*  Inner Class  */
		class impl ;
		/*  Keep lower & upper bound Value   */
		std::unique_ptr<impl> pimpl ;
	} ;



	//----------------------------------//
	/*                                  */
	/*       class declaration          */
	/*                                  */
	/*   interval implementation class  */
	/*   (internal class of interval)   */
	/*                                  */
	//----------------------------------//


	template < typename T >
	class interval<T>::impl
	{
	public:
		/*  Set Zero Value  */
		void do_internal_work( )
		{
			lower_bound = zero<T>() ;
			upper_bound = zero<T>() ;
		}
		/*  Set Initial Value */
		void do_internal_work( T const& low, T const& up )
		{
			lower_bound = low ;
			upper_bound = up ;
		}
		/*  Increment/decrement Op Implementation */
		const impl operator ++() ;
		const impl operator ++(int) ;
		const impl operator --() ;
		const impl operator --(int) ;

		/*  getter Implementation  */
		constexpr T lower() const ;
		constexpr T upper() const ;

		/*  Output Stream Function  */
		std::ostream& print( std::ostream& ) const ;

		/*  setter Implementation */
		void set_lower( T&& x )
		{
			if ( x > upper_bound ) {
				throw invalid_argument( "lower_bound must be less than upper_bound!" ) ;
			}
			lower_bound = std::forward<T>( x ) ;
		}
		void set_upper( T&& x )
		{
			if ( x < lower_bound ) {
				throw invalid_argument( "upper_bound must be greater than lower_bound!" ) ;
			}
			upper_bound = std::forward<T>( x ) ;
		}

		void set( T&& x, T&& y)
		{
			if (x > y) {
				throw invalid_argument( "upper_bound must be greater than lower_bound!" ) ;
			}
			lower_bound = std::forward<T>(x) ;
			upper_bound = std::forward<T>(y) ;
		}
		void deep_copy( impl& x )
		{
			lower_bound = x.lower_bound ;
			upper_bound = x.upper_bound ;
		}
	private:
		/*  Lower Bound  */
		T lower_bound = zero<T>() ;

		/*  Upper Bound  */
		T upper_bound = zero<T>() ;
	} ;




	//------------------------------//
	//                              //
	/*  Interval Member Functions   */
	//                              //
	//------------------------------//




	/*  Interval Increment/Decrement Operator Definition  */

	/*  Interval Increment Operator  */

	//  prefix increment
	template < typename T >
	const interval<T> interval<T>::operator ++()
	{
		++( *pimpl ) ;
		if ( this->upper() == max<T>() || this->lower() == -max<T>() )
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return *this ;
	}

	//  postfix increment
	template < typename T >
	const interval<T> interval<T>::operator ++(int)
	{
		auto tmp( *this ) ;
		++( *pimpl ) ;
		if ( this->upper() == max<T>() || this->lower() == -max<T>() )
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return tmp ;

	}

	/*  Interval Increment Operator  */

	//  prefix decrement
	template < typename T >
	const interval<T> interval<T>::operator --()
	{
		--( *pimpl ) ;
		if ( this->upper() == max<T>() || this->lower() == -max<T>() )
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return *this ;
	}

	//  postfix decrement
	template < typename T >
	const interval<T> interval<T>::operator --(int)
	{
		auto tmp( *this ) ;
		--( *pimpl ) ;
		if ( this->upper() == max<T>() || this->lower() == -max<T>() )
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return tmp ;
	}



	//--------------------------------------------//
	//                                            //
	/*    Interval Numeric Function Definition    */
	//                                            //
	//--------------------------------------------//


	/*  interval cosine  */

	template < typename T >
	const interval<T> interval<T>::cos() const
	{
		int c ;
		auto a = pimpl->lower() ;
		auto b = pimpl->upper() ;

		if ( b - a >= static_cast<T>( 2.0L ) * PI<T>() )
		{
			return interval<T>{ -one<T>(), one<T>() } ;
		}
		/*  base point set  */
		if ( static_cast<int>( upward( ( a / static_cast<T>( 2.0L ) / PI<T>() ) ) ) > a )
		{
			c = static_cast<int> ( upward( ( a / static_cast<T>( 2.0L ) / PI<T>() ) ) ) ;
		}
		else {
			c = static_cast<int>( upward( ( a / static_cast<T>( 2.0L ) / PI<T>() ) ) ) + 1 ;
		}
		/*  checking phase  */
		if ( upward( PI<T>() * static_cast<T>( 2.0L ) * c ) >= a &&
			 downward( PI<T>() * static_cast<T>( 2.0L ) * c ) <= b )
		{
			if ( upward( PI<T>() * ( one<T>() + static_cast<T>( 2.0L ) * c ) ) >= a &&
				 downward( PI<T>() * (one<T>() + static_cast<T>( 2.0L ) * c ) ) <= b )
			{
				return interval<T>{ -one<T>(), one<T>() } ;
			}
			else if ( upward( ( c * static_cast<T>( 2.0L ) - one<T>() ) * PI<T>() ) >= a &&
					  downward( ( c * static_cast<T>( 2.0L ) - one<T>() ) * PI<T>() ) <= b )
			{
				return interval<T>{ -one<T>(), one<T>() } ;
			}
			else
			{
				return interval<T>{ std::fmin( downward( std::cos( a ) ), upward( std::cos( b ) ) ), one<T>() } ;
			}
		}
		else if ( upward( PI<T>() * ( static_cast<T>( 2.0L ) * c + one<T>() ) ) >= a &&
				  downward( PI<T>() * ( static_cast<T>( 2.0L ) * c + one<T>() ) ) <= b )
		{
			return interval<T>( -one<T>(), std::fmax( upward( std::cos( a ) ),	upward( std::cos( b ) ) ) ) ;
		}
		else if ( upward( ( c * static_cast<T>( 2.0L ) - one<T>() ) * PI<T>() ) >= a &&
				  downward( ( c * static_cast<T>( 2.0L ) - one<T>() ) * PI<T>() ) <= b )
		{
			return interval<T>( -one<T>(), std::fmax( upward( std::cos( a ) ), upward( std::cos( b ) ) ) ) ;
		}
		else {
			return interval<T>{ std::fmin( downward( std::cos( a ) ), downward( std::cos( b ) ) ), std::fmax( upward( std::cos( a ) ), upward( std::cos( b ) ) ) } ;
		}
	}


	/*  interval sine  */

	template < typename T >
	const interval<T> interval<T>::sin() const
	{
		int c ;
		auto a = pimpl->lower() ;
		auto b = pimpl->upper() ;

		if ( b - a >= static_cast<T>( 2.0L ) * PI<T>() )
		{
			return interval<T>{ -one<T>(), one<T>() } ;
		}
		/*  base point set  */
		if ( static_cast<int>( upward( ( a * static_cast<T>( 2.0L ) / PI<T>() - one<T>() ) / static_cast<T>( 4.0 ) ) ) > a ) {
			c = static_cast<int>( downward( ( a * static_cast<T>( 2.0L ) / PI<T>() - one<T>() ) / static_cast<T>( 4.0 ) ) ) ;
		}
		else{
			c = static_cast<int>( upward( ( a * static_cast<T>( 2.0L ) / PI<T>() - one<T>() ) / static_cast<T>( 4.0 ) ) ) + 1 ;
		}
		/*  checking phase  */
		if ( upward( PI<T>() / static_cast<T>( 2.0L ) * ( one<T>() + c * static_cast<T>( 4.0L ) ) ) >= a &&
			 downward( PI<T>() / static_cast<T>( 2.0L ) * ( one<T>() + c * static_cast<T>( 4.0L ) ) ) <= b )
		{
			if ( upward( PI<T>() / static_cast<T>( 2.0L ) * ( static_cast<T>( 3.0 ) + c * static_cast<T>( 4.0L ) ) ) >= a &&
				 downward( PI<T>() / static_cast<T>( 2.0L ) * ( static_cast<T>( 3.0 ) + c * static_cast<T>( 4.0L ) ) ) <= b )
			{
				return interval<T>{ -one<T>(), one<T>() } ;
			}
			else if ( upward( ( c * static_cast<T>( 4.0L ) - one<T>() ) * PI<T>() / static_cast<T>( 2.0L ) ) >= a &&
					  downward( ( c * static_cast<T>( 4.0L ) - one<T>() ) * PI<T>() / static_cast<T>( 2.0L ) ) <= b )
			{
				return interval<T>{ -one<T>(), one<T>() } ;
			}
			else
			{
				return interval<T>{ std::fmin( downward( std::sin( a ) ), downward( std::sin( b ) ) ), one<T>() } ;
			}
		}
		else if ( upward( PI<T>() / static_cast<T>( 2.0L ) * ( static_cast<T>( 3.0L ) + c * static_cast<T>( 4.0L ) ) ) >= a &&
				  downward( PI<T>() / static_cast<T>( 2.0L ) * ( static_cast<T>( 3.0L ) + c * static_cast<T>( 4.0L ) ) ) <= b )
		{
			return interval<T>{ -one<T>(), std::fmax( upward( std::sin( a ) ),	upward( std::sin( b ) ) ) } ;
		}
		else if ( upward( ( c * static_cast<T>( 4.0 ) - one<T>() ) * PI<T>() / static_cast<T>( 2.0 ) ) >= a &&
				  downward( ( c * static_cast<T>( 4.0 ) - one<T>() ) * PI<T>() / static_cast<T>( 2.0 ) ) <= b )
		{
			return interval<T>{ -one<T>(), std::fmax( upward( std::sin( a ) ),	upward( std::sin( b ) ) ) } ;
		}
		else {
			return interval<T>{ std::fmin( downward( std::sin( a ) ), downward( std::sin( b ) ) ), std::fmax( upward( std::sin( a ) ), upward( std::sin( b ) ) ) } ;
		}
	}


	/*  interval tangent  */

	template < typename T >
	const interval<T> interval<T>::tan() const
	{
		int c ;
		auto a = pimpl->lower() ;
		auto b = pimpl->upper() ;

		if ( b - a >= PI<T>() )
		{
			throw Cranberries::over_flow( "tan" ) ;
		}
		/*  base point set  */
		if ( static_cast<int>( upward( a * static_cast<T>( 2.0L ) / PI<T>() ) ) < a )
		{
			c = static_cast<int>( upward( a * static_cast<T>( 2.0L ) / PI<T>() ) ) ;
		}
		else {
			c = static_cast<int>( upward( a * static_cast<T>( 2.0L ) / PI<T>() ) ) + 1 ;
		}
		/*  checking phase  */
		if ( a < PI<T>() / static_cast<T>( static_cast<T>( 2.0L ) ) * c && 
			 PI<T>() / static_cast<T>( static_cast<T>( 2.0L ) ) * c < b )
		{
			throw Cranberries::over_flow( "tan" ) ;
		}
		else if ( a < PI<T>() / static_cast<T>( static_cast<T>( 2.0L ) ) * c - PI<T>() &&
				  PI<T>() / static_cast<T>( static_cast<T>( 2.0L ) ) * c - PI<T>() < b )
		{
			throw Cranberries::over_flow( "tan" ) ;
		}
		else if ( a < PI<T>() / static_cast<T>( static_cast<T>( 2.0L ) ) * c + PI<T>() &&
				  PI<T>() / static_cast<T>( static_cast<T>( 2.0L ) ) * c + PI<T>() < b )
		{
			throw Cranberries::over_flow( "tan" ) ;
		}
		else if ( downward( std::tan( a ) ) == -max<T>() || downward( std::tan( b ) )  == max<T>() )
		{
			throw Cranberries::over_flow( "tan" ) ;
		}
		else {
			return interval<T>{ downward( std::tan( a ) ), upward( std::tan( b ) ) } ;
		}
	}


	/*  interval arc cosine  */

	template < typename T >
	const interval<T> interval<T>::acos() const
	{
		auto a = pimpl->lower() ;
		auto b = pimpl->upper() ;

		if ( a < -one<T>() || b > one<T>() )
		{
			throw Cranberries::invalid_argument( "acos" ) ;
		}
		else {
			return interval<T>{ downward( std::acos( a ) ), upward( std::acos( b ) ) } ;
		}
	}


	/*  interval arc sine  */

	template < typename T >
	const interval<T> interval<T>::asin() const
	{
		auto a = pimpl->lower() ;
		auto b = pimpl->upper() ;

		if ( a < -one<T>() || b > one<T>() )
		{
			throw Cranberries::invalid_argument( "asin" ) ;
		}
		else {
			return interval<T>{ downward( std::asin( b ) ), upward( std::asin( a ) ) } ;
		}
	}


	/*  interval arc tangent  */

	template < typename T >
	const interval<T> interval<T>::atan() const
	{
		auto a = pimpl->lower() ;
		auto b = pimpl->upper() ;

		if ( a < -one<T>() || b > one<T>() )
		{
			throw Cranberries::invalid_argument( "asin" ) ;
		}
		else {
			return interval<T>{ downward( std::atan( a ) ), upward( std::atan( b ) ) } ;
		}
	}


	/*  interval hyperbolic cosine  */

	template < typename T >
	const interval<T> interval<T>::cosh() const
	{
		auto a = pimpl->lower() ;
		auto b = pimpl->upper() ;

		if ( upward( std::cosh( a ) ) == max<T>() || upward( std::cosh( b ) ) == max<T>() )
		{
			throw Cranberries::over_flow( "cosh" ) ;
		}
		if ( a < zero<T>() && b > zero<T>() )
		{
			return interval<T>{ one<T>(), std::fmax( upward( std::cosh( a ) ), upward( std::cosh( b ) ) ) } ;
		}
		else if ( b < zero<T>() )
		{
			return interval<T>{ downward( std::cosh( b ) ), upward( std::cosh( a ) ) } ;
		}
		else if ( a > zero<T>() )
		{
			return interval<T>{ downward( std::cosh( a ) ), upward( std::cosh( b ) ) } ;
		}
		else {
			throw Cranberries::domain_error( "cosh" ) ;
		}
	}


	/*  interval hyperbolic sine  */

	template < typename T >
	const interval<T> interval<T>::sinh() const
	{
		auto a = pimpl->lower() ;
		auto b = pimpl->upper() ;

		auto l = downward( std::sinh( a ) ) ;
		auto r = upward( std::sinh( b ) ) ;

		if ( a == -max<T>() || b == max<T>() )
		{
			throw Cranberries::over_flow( "sinh" ) ;
		}
		else {
			return interval<T>{ l, r } ;
		}
	}


	/*  interval hyperbolic tangent  */

	template < typename T >
	const interval<T> interval<T>::tanh() const
	{
		return interval<T>{ downward( std::tanh( pimpl->lower() ) ), upward( std::tanh( pimpl->upper() ) ) } ;
	}


	/*  interval arc hyperbolic cosine  */

	template < typename T >
	const interval<T> interval<T>::acosh() const
	{
		auto a = pimpl->lower() ;
		auto b = pimpl->upper() ;

		if ( a < one<T>() )
		{
			throw Cranberries::invalid_argument( "acosh" ) ;
		}
		else {
			return interval<T>{ downward( std::tanh( a ) ), upward( std::tanh( b ) ) } ;
		}
	}

	/*  interval arc hyperbolic sine  */

	template < typename T >
	const interval<T> interval<T>::asinh() const
	{
		return interval<T>{ downward( std::asinh( pimpl->lower() ) ), upward( std::asinh( pimpl->upper() ) ) } ;
	}

	/*  interval arc hyperbolic tangent  */

	template < typename T >
	const interval<T> interval<T>::atanh() const
	{
		return interval<T>{ downward( std::atanh( pimpl->lower() ) ), upward( std::atanh( pimpl->upper() ) ) } ;
	}

	/*  interval power function  */

	template < typename T >
	const interval<T> interval<T>::pow(int n) const
	{
		if ( n < 0 )
		{
			auto tmp = one<T>() / ( *this ) ;
			return tmp.pow( -1 * n ) ;
		}
		else if ( n == 0 )
		{
			return interval<T>{ one<T>(), one<T>() } ;
		}
		else if ( n % 2 == 0 )
		{
			if ( pimpl->upper() <= zero<T>() )
			{
				if ( upward( std::pow( pimpl->lower(), n ) ) == max<T>() ||
					 downward( std::pow( pimpl->upper(), n ) ) == -max<T>() )
				{
					throw Cranberries::over_flow( "overflow occurred." ) ;
				}
				return interval<T>{ downward( std::pow( pimpl->upper(), n ) ),	upward( std::pow( pimpl->lower(), n ) ) } ;
			}
			else if ( pimpl->lower() <= zero<T>() && pimpl->upper() >= zero<T>() )
			{
				if ( std::fmax( downward( std::pow( pimpl->lower(), n ) ), upward( std::pow( pimpl->upper(), n ) ) ) == max<T>() )
				{
					throw Cranberries::over_flow( "overflow occurred." ) ;
				}
				return interval<T>{ zero<T>(), std::fmax( downward( std::pow( pimpl->lower(), n ) ), upward( std::pow( pimpl->upper(), n ) ) ) } ;
			}
		}
		if ( upward( std::pow( pimpl->upper(), n ) ) == max<T>() ||
			 downward( std::pow( pimpl->lower(), n ) ) == max<T>() )
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return interval<T>{ downward( std::pow( pimpl->lower(), n ) ), upward( std::pow( pimpl->upper(), n ) ) } ;
	}


	/*  interval square root function  */

	template < typename T >
	const interval<T> interval<T>::sqrt() const
	{
		if ( pimpl->lower() < zero<T>() )
		{
			throw Cranberries::logic_error( "sqrt arg requires positive number" ) ;
		}
		return interval<T>{ downward( std::sqrt( pimpl->lower() ) ), upward( std::sqrt( pimpl->upper() ) ) } ;
	}


	/*  interval exponential function ( base = e )  */

	template < typename T >
	const interval<T> interval<T>::exp() const
	{
		if ( upward( std::exp( pimpl->upper() ) ) == max<T>() || downward( std::exp( pimpl->lower() ) ) == -max<T>() )
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return interval<T>{ downward( std::exp( pimpl->lower() ) ), upward( std::exp( pimpl->upper() ) ) } ;
	}

	/*  interval exponential function ( base = 2 )  */

	template < typename T >
	const interval<T> interval<T>::exp2() const
	{
		if ( upward( std::exp2( pimpl->upper() ) ) == max<T>() || downward( std::exp2( pimpl->lower() ) ) == -max<T>() )
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return interval<T>{ downward( std::exp2( pimpl->lower() ) ), upward( std::exp2( pimpl->upper() ) ) } ;
	}

	/*  interval exp( x ) - 1  */

	template < typename T >
	const interval<T> interval<T>::expm1() const
	{
		if ( upward( std::expm1( pimpl->upper() ) ) == max<T>() || downward( std::expm1( pimpl->lower() ) ) == -max<T>() )
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return interval<T>{ downward( std::expm1( pimpl->lower() ) ), upward( std::expm1( pimpl->upper() ) ) } ;
	}


	/*  interval logarithmic function ( base = e ) */

	template < typename T >
	const interval<T> interval<T>::log() const
	{
		if ( pimpl->lower() <= zero<T>() )
		{
			throw Cranberries::logic_error( "anti-logarithm less than or equal to zero" ) ;
		}
		return interval<T>{ downward( std::log( pimpl->lower() ) ), upward( std::log( pimpl->upper() ) ) } ;
	}

	/*  interval log( x ) + 1  */
	template < typename T >
	const interval<T> interval<T>::log1p() const
	{
		if ( pimpl->lower() <= zero<T>() ) { throw Cranberries::logic_error( "anti-logarithm less than or equal to zero" ) ; }
		return interval<T>{ downward( std::log1p( pimpl->lower() ) ),	upward( std::log1p( pimpl->upper() ) ) } ;
	}


	/*  interval logarithmic function ( base = 10 )  */

	template < typename T >
	const interval<T> interval<T>::log10() const
	{
		if ( pimpl->lower() <= zero<T>() ) { throw Cranberries::logic_error( "anti-logarithm less than or equal to zero" ) ; }
		return interval<T>{ downward( std::log10( pimpl->lower() ) ), upward( std::log10( pimpl->upper() ) ) } ;
	}

	/*  interval logarithmic function ( base = 2 )  */
	template < typename T >
	const interval<T> interval<T>::log2() const
	{
		if ( pimpl->lower() <= zero<T>() )
		{
			throw Cranberries::logic_error( "anti-logarithm less than or equal to zero" ) ;
		}
		return interval<T>{ downward( std::log2( pimpl->lower() ) ), upward( std::log2( pimpl->upper() ) ) } ;
	}


	/*  interval absolute funtion  */

	template < typename T >
	const interval<T> interval<T>::abs() const
	{
		if ( pimpl->lower() < zero<T>() && pimpl->upper() >= zero<T>() )
		{
			return interval<T>{ zero<T>(), upward( std::fmax( std::abs( pimpl->lower() ), std::abs( pimpl->upper() ) ) ) } ;
		}
		if ( pimpl->upper() < zero<T>() )
		{
			return interval<T>{ downward( std::abs( pimpl->upper() ) ), upward( std::abs( pimpl->lower() ) ) } ;
		}
		else {
			return interval<T>( *this ) ;
		}
	}

	template < typename T1, typename T2, typename T3 >
	struct fma_promotion
	{
		typedef decltype( std::fma(T1{}, T2{}, T3{} ) ) value_type;
		typedef interval<value_type> return_type;
	} ;


	/*  interval fused multiply-add function fma( interval<T>, interval<T>, interval<T>)  */
	template < typename T1, typename T2, typename T3 >
	const typename fma_promotion<T1,T2,T3>::return_type fma( interval<T1> const& x, interval<T2> const& y, interval<T3> const& z )
	{
		auto x_low = x.lower() ;
		auto y_low = y.lower() ;
		auto z_low = z.lower() ;
		auto x_up = x.upper() ;
		auto y_up = y.upper() ;
		auto z_up = z.upper() ;
		
		typedef typename fma_promotion<T1,T2,T3>::value_type T;
		
		if ( x_low >= zero<T1>() && y_low >= zero<T2>() ) {
			return interval<T>{ downward( std::fma( x_low,y_low,z_low ) ), upward( std::fma( x_up, y_up, z_up ) ) } ;
		}
		else if ( x_low >= zero<T1>() && y_low < zero<T2>() && y_up > zero<T2>() )
		{
			return interval<T>{ downward( std::fma( x_up, y_low, z_low ) ), upward( std::fma( x_low, y_up, z_up ) ) } ;
		}
		else if ( x_low >= zero<T1>() && y_up <= zero<T2>() )
		{
			return interval<T>{ downward( std::fma( x_up, y_low, z_low ) ), upward( std::fma( x_low, y_up, z_up ) ) } ;
		}
		else if ( x_low < zero<T1>() && x_up > zero<T1>() && y_low >= zero<T2>() )
		{
			return interval<T>{ downward( std::fma( x_low, y_up, z_low ) ), upward( std::fma( x_up, y_up, z_up ) ) } ;
		}
		else if ( x_low < zero<T1>() && x_up > zero<T1>() && y_low <= zero<T2>() )
		{
			return interval<T>{ downward( std::fma( x_up, y_low, z_low ) ), upward( std::fma( x_low, y_low, z_up ) ) } ;
		}
		else if ( x_up <= zero<T1>() && y_low >= zero<T2>() )
		{
			return interval<T>{ downward( std::fma( x_low, y_up, z_low ) ), upward( std::fma( x_up, y_low, z_up ) ) } ;
		}
		else if ( x_up <= zero<T>() && y_low < zero<T>() && y_up > zero<T>() )
		{
			return interval<T>{ downward( std::fma( x_low, y_up, z_low ) ), upward( std::fma( x_low, y_low, z_up ) ) } ;
		}
		else if ( x_up <= zero<T>() && y_up <= zero<T2>() )
		{
			return interval<T>{ downward( std::fma( x_up, y_up, z_low ) ), upward( std::fma( x_low, y_low, z_up ) ) } ;
		}
		else {
			return interval<T>{
				std::fmin( downward( std::fma( x_up, y_low, z_low ) ), downward( std::fma( x_low, y_up, x_low ) ) ),
					std::fmax( upward( std::fma( x_up, y_low, z_low ) ), upward( std::fma( x_low, y_up, x_low ) ) )
			} ;
		}
	}


	/*  interval fused multiply-add function fma( interval<T>, interval<T>, T)  */
	
	template < typename T1, typename T2, typename T3 >
	const typename fma_promotion<T1, T2, T3>::return_type fma(interval<T1> const& x, interval<T2> const& y, T3 const& z)
	{
		auto x_low = x.lower() ;
		auto y_low = y.lower() ;
		auto x_up = x.upper() ;
		auto y_up = y.upper() ;

		typedef typename fma_promotion<T1, T2, T3>::value_type T;

		if ( x_low >= zero<T1>() && y_low >= zero<T2>() )
		{
			return interval<T>{ downward( std::fma( x_low, y_low, z ) ), upward( std::fma( x_up, y_up, z ) ) } ;
		}
		else if ( x_low >= zero<T1>() && y_low < zero<T2>() && y_up > zero<T2>() )
		{
			return interval<T>{ downward( std::fma( x_up, y_low, z ) ), upward( std::fma( x_low, y_up, z ) ) } ;
		}
		else if ( x_low >= zero<T1>() && y_up <= zero<T2>() ) 
		{
			return interval<T>{ downward( std::fma( x_up, y_low, z ) ), upward( std::fma( x_low, y_up, z ) ) } ;
		}
		else if ( x_low < zero<T1>() && x_up > zero<T1>() && y_low >= zero<T2>() )
		{
			return interval<T>{ downward( std::fma( x_low, y_up, z ) ), upward( std::fma( x_up, y_up, z ) ) } ;
		}
		else if ( x_low < zero<T1>() && x_up > zero<T1>() && y_low <= zero<T2>() )
		{
			return interval<T>{ downward( std::fma( x_up, y_low, z ) ), upward( std::fma( x_low, y_low, z ) ) } ;
		}
		else if ( x_up <= zero<T1>() && y_low >= zero<T2>() )
		{
			return interval<T>{ downward( std::fma( x_low, y_up, z ) ), upward( std::fma( x_up, y_low, z ) ) } ;
		}
		else if ( x_up <= zero<T1>() && y_low < zero<T2>() && y_up > zero<T2>() )
		{
			return interval<T>{ downward( std::fma( x_low, y_up, z ) ), upward( std::fma( x_low, y_low, z ) ) } ;
		}
		else if ( x_up <= zero<T1>() && y_up <= zero<T2>() ) 
		{
			return interval<T>{ downward( std::fma( x_up, y_up, z ) ), upward( std::fma( x_low, y_low, z ) ) } ;
		}
		else {
			return interval<T>{
				std::fmin( downward( std::fma( x_up, y_low, z ) ), downward( std::fma( x_low, y_up, z ) ) )
				, std::fmax( upward( std::fma( x_up, y_low, z ) ), upward( std::fma( x_low, y_up, z ) ) )
			} ;
		}
	}


	/*  interval fused multiply-add function fma( interval<T>, T, interval<T>)  */

	template < typename T1, typename T2, typename T3 >
	const typename fma_promotion<T1, T2, T3>::return_type fma( interval<T1> const& x, T2 const& y, interval<T3> const& z )
	{
		typedef typename fma_promotion<T1, T2, T3>::value_type T;

		if ( y < zero<T2>() )
		
		{
			return interval<T>{ downward( std::fma( x.upper(), y, z.lower() ) ), upward( std::fma( x.lower(), y, z.upper() ) ) } ;
		}
		else {
			return interval<T>{ downward( std::fma( x.lower(), y, z.lower() ) ), upward( std::fma( x.upper(), y, z.upper() ) ) } ;
		}
	}


	/*  interval fused multiply-add function fma(T, interval<T>, interval<T>)  */

	template < typename T1, typename T2, typename T3 >
	const typename fma_promotion<T1,T2,T3>::return_type fma( T1 const& x, interval<T2> const& y, interval<T3> const& z)
	{
		typedef typename fma_promotion<T1, T2, T3>::value_type T;

		if ( x < zero<T1>() ) {
			return interval<T>{ downward( std::fma( x, y.upper(), z.lower() ) ), upward( std::fma( x, y.lower(), z.upper() ) ) } ;
		}
		else {
			return interval<T>{ downward( std::fma( x, y.lower(), z.lower() ) ), upward( std::fma( x, y.upper(), z.upper() ) ) } ;
		}
	}


	/*  interval fused multiply-add function fma( interval<T>, T, T)  */

	template < typename T1, typename T2, typename T3 >
	const typename fma_promotion<T1,T2,T3>::return_type fma( interval<T1> const& x, T2 const& y, T3 const& z )
	{
		typedef typename fma_promotion<T1, T2, T3>::value_type T;

		if ( y < zero<T2>() ) {
			return interval<T>{ downward( std::fma( x.upper(), y, z ) ), upward( std::fma( x.lower(), y, z ) ) } ;
		}
		else {
			return interval<T>{ downward( std::fma( x.lower(), y, z ) ), upward( std::fma( x.upper(), y, z ) ) } ;
		}
	}


	/*  interval fused multiply-add function fma(T, interval<T>, T)  */

	template < typename T1, typename T2, typename T3 >
	const typename fma_promotion<T1, T2, T3>::return_type fma( T1 const& x, interval<T2> const& y, T3 const& z )
	{
		typedef typename fma_promotion<T1, T2, T3>::value_type T;

		if ( x < zero<T1>() ) {
			return interval<T>{ downward( std::fma( x, y.upper(), z ) ), upward( std::fma( x, y.lower(), z ) ) } ;
		}
		else {
			return interval<T>{ downward( std::fma( x, y.lower(), z ) ), upward( std::fma( x, y.upper(), z ) ) } ;
		}
	}


	/*  interval fused multiply-add function fma(T, T, interval<T>)  */

	template < typename T1, typename T2, typename T3 >
	const typename fma_promotion<T1,T2,T3>::return_type fma( T1 const& x, T2 const& y, interval<T3> const& z )
	{
		typedef typename fma_promotion<T1, T2, T3>::value_type T;

		return interval<T>{ downward( std::fma( x, y, z.lower() ) ), upward( std::fma( x, y, z.upper() ) ) } ;
	}


	/*  interval error function  */

	template < typename T >
	const interval<T> interval<T>::erf() const
	{
		return interval<T>{ downward( std::erf( pimpl->lower() ) ), upward( std::erf( pimpl->upper() ) ) } ;
	}


	/*  interval complementary error function  */

	template < typename T >
	const interval<T> interval<T>::erfc() const
	{
		return interval<T>{ downward( std::erfc( pimpl->upper() ) ), upward( std::erfc( pimpl->lower() ) ) } ;
	}


	/*  is_singleton  */

	template < typename T >
	constexpr bool interval<T>::is_singleton() const
	{
		return ( pimpl->lower() == pimpl->upper() ) ? true : false ;
	}


	/*  middle point  */

	template < typename T >
	constexpr T interval<T>::mid() const
	{
		return ( pimpl->upper() + pimpl->lower() ) / static_cast<T>( 2.0 ) ;
	}


	/*  width  */

	template < typename T >
	constexpr T interval<T>::wid() const
	{
		return pimpl->upper() - pimpl->lower() ;
	}


	/*  Interval Accessors  */

	template < typename T >
	constexpr T interval<T>::lower() const
	{
		return pimpl->lower() ;
	}

	template < typename T >
	constexpr T interval<T>::upper() const
	{
		return pimpl->upper() ;
	}


	template < typename T >
	void interval<T>::set_upper( T&& x )
	{
		pimpl->set_upper( std::forward<T>( x ) ) ;
	}


	template < typename T >
	void interval<T>::set_lower( T&& x )
	{
		pimpl->set_lower(std::forward<T>(x));
	}

	template < typename T >
	void interval<T>::set( T&& x, T&& y )
	{
		pimpl->set( std::forward<T>( x ), std::forward<T>( y ) ) ;
	}
	template < typename T >
	void interval<T>::swap( interval& x ) noexcept
	{
		std::swap( this->pimpl, x.pimpl ) ;
	}

	/*  Interval Relational Discriminator Function  */

	template < typename T >
	interval_relation interval<T>::relational( interval<T> const& x ) const
	{
		if ( total_equal( *this, x ) )
		{
			return interval_relation::equal ;
		}
		else if ( pimpl->lower() == ( x.pimpl )->lower() && pimpl->upper() < ( x.pimpl )->upper() )
		{
			return interval_relation::total_less ;
		}
		else if ( pimpl->lower() == ( x.pimpl )->lower() && pimpl->upper() > ( x.pimpl )->upper() )
		{
			return interval_relation::total_greater ;
		}
		else if ( pimpl->lower() < ( x.pimpl )->lower() && pimpl->upper() == ( x.pimpl )->upper() )
		{
			return interval_relation::weak_less ;
		}
		else if ( pimpl->lower() > ( x.pimpl )->lower() && pimpl->upper() == ( x.pimpl )->upper() )
		{
			return interval_relation::weak_greater ;
		}
		else if ( pimpl->lower() < ( x.pimpl )->lower() && pimpl->upper() < ( x.pimpl )->upper() )
		{
			return interval_relation::partial_less ;
		}
		else if ( pimpl->lower() > ( x.pimpl )->lower() && pimpl->upper() > ( x.pimpl )->upper() )
		{
			return interval_relation::partial_less ;
		}
		else if ( pimpl->lower() <= ( x.pimpl )->lower() && pimpl->upper() >= ( x.pimpl )->upper() )
		{
			return interval_relation::contain ;
		}
		else if ( pimpl->lower() >= ( x.pimpl )->lower() && pimpl->upper() <= ( x.pimpl )->upper() )
		{
			return interval_relation::part_of ;
		}
		else {
			return interval_relation::niether ;
		}
	}


	/*  Interval Relational function Definition  */

	/*
	[ Note : There are four type ordering oplicy.
	total ordering, weak ordering, partial ordering and interval ordering.
	Default ordering is total ordering. - end note ]
	*/

	/*
	[ Example : Let interval<T> a,b ;
	"a < b" equals "total_less( a, b )".
	- end example ]
	*/

	/*
	[ Note : Ordering Policy can switch using namespace inside a scape block.
	Three namespace are defined ( weak_ordering_policy, partial_ordering and interval_ordering_policy ).
	- end note ]
	*/

	/*
	[ Example : 
	auto x = hull( 1.0, 2.0 ) ;
	auto y = hull( 1.2, 1.8 ) ;
	x < y ; // OK! default ordering_policy is total_ordering_policy.
	{
		using namespace Cranberries::weak_ordering::policy; // valid! only one ordering_policy.
		y < x ; // OK! equals weal_less( y, x ) ;
	}
	{
		using namespace Cranberries::weak_ordering::policy;
		using namespace Cranberries::weak_ordering::policy; // invalid! two ordering_policy.
		x < y ; // overload conflict occur.
	}
	- end example ]
	*/

	inline namespace total_ordering_policy
	{

		template < typename T, typename U >
		inline constexpr bool operator<( interval<T> const& x, interval<U> const& y )
		{
			return total_less( x, y ) ;
		}
		template < typename T, typename U >
		inline constexpr bool operator<( interval<T> const& x, U const& y )
		{
			return total_less( x, y ) ;
		}
		template < typename T, typename U >
		inline constexpr bool operator<( U const& x, interval<T> const& y )
		{
			return total_less( x, y ) ;
		}

		template < typename T, typename U >
		constexpr bool operator<=( interval<T> const& x, interval<U> const& y )
		{
			return total_less_or_equal( x, y ) ;
		}
		template < typename T, typename U >
		constexpr bool operator<=( interval<T> const& x, U const& y )
		{
			return total_less_or_equal( x, y ) ;
		}
		template < typename T, typename U >
		constexpr bool operator<=( U const& x, interval<T> const& y )
		{
			return total_less_or_equal( x, y ) ;
		}

		template < typename T, typename U >
		constexpr bool operator>( interval<T> const& x, interval<U> const& y )
		{
			return total_greater( x, y ) ;
		}
		template < typename T, typename U >
		constexpr bool operator>( interval<T> const& x, U const& y )
		{
			return total_greater( x, y ) ;
		}
		template < typename T, typename U >
		constexpr bool operator>( U const& x, interval<T> const& y )
		{
			return total_greater( x, y ) ;
		}

		template < typename T, typename U >
		constexpr bool operator>=( interval<T> const& x, interval<U> const& y )
		{
			return total_greater_or_equal( x, y ) ;
		}
		template < typename T, typename U >
		constexpr bool operator>=( interval<T> const& x, U const& y )
		{
			return total_greater_or_equal( x, y ) ;
		}
		template < typename T, typename U >
		constexpr bool operator>=( U const& x, interval<T> const& y )
		{
			return total_greater_or_equal( x, y ) ;
		}

		template < typename T, typename U >
		constexpr bool operator==( interval<T> const& x, interval<U> const& y )
		{
			return total_equal( x, y ) ;
		}
		template < typename T, typename U >
		constexpr bool operator==( interval<T> const& x, U const& y )
		{
			return total_equal( x, y ) ;
		}
		template < typename T, typename U >
		constexpr bool operator==( U const& x, interval<T> const& y )
		{
			return total_equal( x, y ) ;
		}

		template < typename T, typename U >
		constexpr bool operator!=( interval<T> const& x, interval<U> const& y )
		{
			return !total_equal( x, y ) ;
		}
		template < typename T, typename U >
		constexpr bool operator!=( interval<T> const& x, U const& y )
		{
			return !total_equal( x, y ) ;
		}
		template < typename T, typename U >
		constexpr bool operator!=( U const& x, interval<T> const& y )
		{
			return !total_equal( x, y ) ;
		}

	}

	namespace weak_ordering_policy
	{
		template < typename T, typename U >
		constexpr bool operator<( T&& x, U&& y )
		{
			return weak_less( std::forward<T>(x), std::forward<U>(y) ) ;
		}
		template < typename T, typename U >
		constexpr bool operator<=( T&& x, U&& y )
		{
			return weak_less_or_equal( std::forward<T>(x), std::forward<U>(y) ) ;
		}
		template < typename T, typename U >
		constexpr bool operator>( T&& x, U&& y )
		{
			return weak_greater( std::forward<T>(x), std::forward<U>(y) ) ;
		}
		template < typename T, typename U >
		constexpr bool operator>=( T&& x, U&& y )
		{
			return weak_greater_or_equalr( std::forward<T>(x), std::forward<U>(y) ) ;
		}
		template < typename T, typename U >
		constexpr bool operator==( T&& x, U&& y )
		{
			return weak_eqaul( std::forward<T>(x), std::forward<U>(y) ) ;
		}
		template < typename T, typename U >
		constexpr bool operator!=( T&& x, U&& y )
		{
			return !weak_equal( std::forward<T>(x), std::forward<U>(y) ) ;
		}
	}

	namespace partial_ordering_policy
	{
		template < typename T, typename U >
		constexpr bool operator<( T&& x, U&& y )
		{
			return partial_less( std::forward<T>(x), std::forward<U>(y) ) ;
		}
		template < typename T, typename U >
		constexpr bool operator<=( T&& x, U&& y )
		{
			return partial_less_or_equal( std::forward<T>(x), std::forward<U>(y) ) ;
		}
		template < typename T, typename U >
		constexpr bool operator>( T&& x, U&& y )
		{
			return partial_greater( std::forward<T>(x), std::forward<U>(y) ) ;
		}
		template < typename T, typename U >
		constexpr bool operator>=( T&& x, U&& y )
		{
			return partial_greater_or_equalr( std::forward<T>(x), std::forward<U>(y) ) ;
		}
		template < typename T, typename U >
		constexpr bool operator!=( T&& x, U&& y )
		{
			return partial_unordered( std::forward<T>(x), std::forward<U>(y) ) ;
		}
	}

	namespace interval_ordering_policy
	{
		template < typename T, typename U>
		constexpr bool operator<( T&& x, U&& y )
		{
			return interval_less( std::forward<T>(x), std::forward<U>(y) ) ;
		}
		template < typename T, typename U>
		constexpr bool operator<=( T&& x, U&& y )
		{
			return interval_less_or_equal( std::forward<T>(x), std::forward<U>(y) ) ;
		}
		template < typename T, typename U>
		constexpr bool operator>( T&& x, U&& y )
		{
			return interval_greater( std::forward<T>(x), std::forward<U>(y) ) ;
		}
		template < typename T, typename U>
		constexpr bool operator>=( T&& x, U&& y )
		{
			return interval_greater_or_equal( std::forward<T>(x), std::forward<U>(y) ) ;
		}
		template < typename T, typename U>
		constexpr bool operator!=( T&& x, U&& y )
		{
			return interval_unordered( std::forward<T>(x), std::forward<U>(y) ) ;
		}
	}


	//-----------------------------//
	/*                             */
	/*      Interval Ctor/Dtor     */
	/*                             */
	//-----------------------------//


	/*  Default Ctor  */

	template < typename T >
	constexpr interval<T>::interval()
		: pimpl{ std::make_unique<impl>() }
	{
		pimpl->do_internal_work() ;
	}

	/*  Two Lvalue Arguments Ctor  */

	template < typename T >
	constexpr interval<T>::interval( T& low, T& up )
		: pimpl{ std::make_unique<impl>() }
	{
		if ( low > up )
		{
			throw invalid_argument( "upper_bound less than lower_bound!" ) ;
		}
		pimpl->do_internal_work( low, up ) ;
	}

	/*  Two Rvalue Arguments Ctor  */

	template < typename T >
	constexpr interval<T>::interval( T&& low, T&& up )
		: pimpl{ std::make_unique<impl>() }
	{
		if ( low > up )
		{
			throw invalid_argument( "upper_bound less than lower_bound!" ) ;
		}
		pimpl->do_internal_work( low, up ) ;
	}

	/*  Default Dtor  */

	template < typename T >
	interval<T>::~interval() = default ;

	/*  Move Ctor  */

	template < typename T >
	interval<T>::interval( interval&& x ) = default ;

	/*  Copy Ctor  */

	template < typename T >
	interval<T>::interval( const interval& x )
		: pimpl{ std::make_unique<impl>() }
	{
		pimpl->deep_copy( *( x.pimpl ) ) ;
	}

	template < typename T >
	constexpr interval<T>::interval( std::initializer_list<T> list)
		: pimpl{ std::make_unique<impl>() }
	{
		if ( list.size() == 0 )
		{
			pimpl->do_internal_work() ;
		}
		else if ( list.size() != 2 )
		{
			throw Cranberries::invalid_argument( "Interval Ctor needs only two values." ) ;
		}
		pimpl->do_internal_work( *( list.begin() ), *( list.begin() + 1 ) ) ;
	}

	/*  Copy Assignment Op  */

	template < typename T >
	interval<T>& interval<T>::operator=( interval<T> const& x )
	{
		pimpl->deep_copy( *( x.pimpl ) ) ;
		return *this ;
	}

	/*  Move Assignment Op  */

	template < typename T >
	interval<T>& interval<T>::operator=( interval<T>&& x )
	{
		std::swap( this->pimpl, x.pimpl ) ;
		x.pimpl = nullptr ;
		return *this ;
	}

	template < typename T >
	interval<T>& interval<T>::operator=( std::initializer_list<T> list )
	{
		if ( list.size() == 0 ) 
		{
			this->set_lower( zero<T>() ) ;
			this->set_upper( zero<T>() ) ;
		}
		else if ( list.size() == 2 )
		{
			if ( *( list.begin() ) > *( list.begin() + 1 ) )
			{
				this->set_lower( *(list.begin() ) ) ;
				this->set_upper( *( list.begin() + 1 ) ) ;
			}
			else {
				throw Cranberries::invalid_argument( "operator=" ) ;
			}
		}
		else {
			throw Cranberries::invalid_argument( "operator=" ) ;
		}
		return *this ;
	}

	/*  Interval Output Function  */

	template < typename T >
	std::ostream& interval<T>::print( std::ostream& s ) const
	{
		return pimpl->print( s ) ;
	}


	//------------------------------//
	/*                              */
	/*     impl Member Function     */
	/*                              */
	//------------------------------//



	/*  Increment/Decrement Operators  */

	/*  impl Prefix Increment Operator  */

	template < typename T >
	const typename interval<T>::impl interval<T>::impl::operator ++()
	{
		++lower_bound ;
		++upper_bound ;
		return *this ;
	}

	/*  impl Postfix Increment Operator  */

	template < typename T >
	const typename interval<T>::impl interval<T>::impl::operator ++(int)
	{
		auto tmp( *this ) ;
		++lower_bound ;
		++upper_bound ;
		return tmp ;
	}

	/*  impl Prefix Decrement Operator  */

	template < typename T >
	const typename interval<T>::impl interval<T>::impl::operator --()
	{
		--lower_bound ;
		--upper_bound ;
		return *this ;
	}

	/*  impl Postfix Increment Operator  */

	template < typename T >
	const typename interval<T>::impl interval<T>::impl::operator --(int)
	{
		auto tmp( *this ) ;
		--lower_bound ;
		--upper_bound ;
		return tmp ;
	}



	/*  Accessors  */

	/*  Interval Low_Bound Getter  */

	template < typename T >
	constexpr T interval<T>::impl::lower() const
	{
		return lower_bound ;
	}

	/*  Interval Upper_Bound Getter  */

	template < typename T >
	constexpr T interval<T>::impl::upper() const
	{
		return upper_bound ;
	}

	/*  Interval Output Function  */

	template < typename T >
	std::ostream& interval<T>::impl::print( std::ostream& s ) const
	{
		return s << "[ " << lower_bound << ", " << upper_bound << " ]" ;
	}


	/*  Interval Max  */

	/*  Two interval Argument Max  */

	template < typename T >
	const interval<T> max( interval<T>& a, interval<T>& b )
	{
		return a > b ? a : b ;
	}

	/*  iitializer_list<interval> Argument Max  */

	template < typename T >
	const interval<T> max( std::initializer_list< interval<T> > list )
	{
		std::vector< interval<T> > tmp( list ) ;
		std::nth_element( tmp.begin(), tmp.begin(), tmp.end(), [&]( interval<T>& a, interval<T>& b ) { return a > b ; } ) ;
		return *( tmp.begin() ) ;
	}

	/*  initializer_list<interval> and Predicate Argument Max   */

	template < typename T, class Compare>
	const interval<T> max( std::initializer_list< interval<T> > list, Compare comp )
	{
		std::vector< interval<T> > tmp( list ) ;
		std::nth_element( tmp.begin(), tmp.begin(), tmp.end(), [&]( interval<T>& a, interval<T>& b ) { return comp( a, b ) ; } ) ;
		return *( tmp.begin() ) ;
	}

	/*  Tow interval and Predicate Argument Max   */

	template < typename T, typename Compare, std::enable_if_t< std::is_same< Compare, interval<T> >::value >*& = enabler >
	const interval<T> max( interval<T>& a, interval<T>& b, Compare comp )
	{
		return comp( a, b ) ? a : b ;
	}

	/*  Variadic arguments Max overloading (finish)  */

	template  < typename T >
	constexpr T max( T a, T b )
	{
		return std::forward<T>( a ) > std::forward<T>( b ) ? std::forward<T>( a ) : std::forward<T>( b ) ;
	}

	/*  Variadic arguments Max  */

	template < typename T, typename ... Args >
	constexpr T max( T a, T b, Args ... args )
	{
		return max(max( std::forward<T>( a ), std::forward<T>( b ) ), std::forward<T>(args )... ) ;
	}


	/*  Interval Min  */

	/*  Two interval Argument Min  */

	template < typename T >
	const interval<T> min( interval<T>& a, interval<T>& b )
	{
		return a < b ? a : b ;
	}

	/*  iitializer_list<interval> Argument Min  */

	template < typename T >
	const interval<T> min( std::initializer_list< interval<T> > list )
	{
		std::vector< interval<T> > tmp( list ) ;
		std::nth_element( tmp.begin(), tmp.begin(), tmp.end(), [&]( interval<T>& a, interval<T>& b ) { return a < b ; } ) ;
		return *( tmp.begin() ) ;
	}

	/*  initializer_list<interval> and Predicate Argument Min   */

	template < typename T, class Compare >
	const interval<T> min( std::initializer_list< interval<T> > list, Compare comp )
	{
		std::vector< interval<T> > tmp( list ) ;
		std::nth_element( tmp.begin(), tmp.begin(), tmp.end(), [&]( interval<T>& a, interval<T>& b ) { return comp( a, b ) ; } ) ;
		return *( tmp.begin() ) ;
	}

	/*  Tow interval and Predicate Argument Max   */

	template < typename T, class Compare, std::enable_if_t< std::is_same< Compare, interval<T> >::value >*& = enabler >
	const interval<T> min( interval<T>& a, interval<T>& b, Compare comp )
	{
		return comp( a, b ) ? a : b ;
	}

	/*  Variadic arguments Max overloading (finish)  */

	template  < typename T >
	constexpr T min( T a, T b )
	{
		return std::forward<T>( a ) < std::forward<T>( b ) ? std::forward<T>( a ) : std::forward<T>( b ) ;
	}

	/*  Variadic arguments Max  */

	template < typename T, typename ... Args >
	constexpr T min( T a, T b, Args ... args )
	{
		return min(min( std::forward<T>( a ), std::forward<T>( b ) ), std::forward<T>(args )... ) ;
	}

	template < typename T >
	constexpr std::pair<interval<T> const&, interval<T> const&> minmax( interval<T> const& a, interval<T> const& b )
	{
		return a < b ? std::make_pair( a, b ) : std::make_pair( b, a ) ;
	}

	template < typename T, typename Compare >
	constexpr std::pair< interval<T> const&, interval<T> const& > minmax( interval<T> const& a, interval<T> const& b, Compare comp )
	{
		return comp(a,b ) ? std::make_pair( a, b ) : std::make_pair( b, a ) ;
	}

	template < typename T >
	const std::pair< interval<T>, interval<T> > minmax( std::initializer_list< interval<T> > list )
	{
		std::vector< interval<T> > v(list) ;
		auto hold = std::minmax_element( v.begin(), v.end() ) ;
		return std::make_pair( *(hold.first), *(hold.second) ) ;
	}

	template < typename T, typename Compare >
	const std::pair< interval<T>, interval<T> > minmax( std::initializer_list< interval<T> > list, Compare comp )
	{
		std::vector< interval<T> > v(list) ;
		auto hold = std::minmax_element( v.begin(), v.end(), [&]( interval<T> const& a, interval<T> const& b ) { return comp( a, b ) ; } ) ;
		return std::make_pair( *(hold.first), *(hold.second) ) ;
	}

	//---------------------------------------------------------//
	/*                                                         */
	/*  Generic Formed Interval Numeric Function Overloading   */
	/*                                                         */
	//---------------------------------------------------------//


	/*  Generic Formed Interval Numeric Function  */
	template < typename T >
	interval<T> sin( interval<T> const& a) { return a.sin() ; }

	template < typename T >
	interval<T> cos( interval<T> const& a) { return a.cos() ; }

	template < typename T >
	interval<T> tan( interval<T> const& a) { return a.tan() ; }

	template < typename T >
	interval<T> asin( interval<T> const& a) { return a.asin() ; }

	template < typename T >
	interval<T> acos( interval<T> const& a) { return a.acos() ; }

	template < typename T >
	interval<T> atan( interval<T> const& a) { return a.atan() ; }

	template < typename T >
	interval<T> sinh( interval<T> const& a) { return a.sinh() ; }

	template < typename T >
	interval<T> cosh( interval<T> const& a) { return a.cosh() ; }

	template < typename T >
	interval<T> tanh( interval<T> const& a) { return a.tanh() ; }

	template < typename T >
	interval<T> asinh( interval<T> const& a) { return a.asinh() ; }

	template < typename T >
	interval<T> acosh( interval<T> const& a) { return a.acosh() ; }

	template < typename T >
	interval<T> atanh( interval<T> const& a) { return a.atanh() ; }

	template < typename T >
	interval<T> exp( interval<T> const& a) { return a.exp() ; }

	template < typename T >
	interval<T> exp2( interval<T> const& a) { return a.exp2() ; }
	
	template < typename T >
	interval<T> expm1( interval<T> const& a) { return a.expm1() ; }

	template < typename T >
	interval<T> pow( interval<T> const& a, int const& n) { return a.pow(n) ; }

	template < typename T >
	interval<T> abs( interval<T> const& a) { return a.abs() ; }

	template < typename T >
	interval<T> log( interval<T> const& a) { return a.log() ; }

	template < typename T >
	interval<T> log10( interval<T> const& a) { return a.log10() ; }

	template < typename T >
	interval<T> log2( interval<T> const& a) { return a.log2() ; }

	template < typename T >
	interval<T> log1p( interval<T> const& a) { return a.log1p() ; }

	template < typename T >
	interval<T> sqrt( interval<T> const& a) { return a.sqrt() ; }

	template < typename T >
	interval<T> erf( interval<T> const& a) { return a.erf() ; }

	template < typename T >
	interval<T> erfc( interval<T> const& a) { return a.erfc() ; }

	template < typename T >
	T wid( interval<T> const& a) { return a.wid() ; }

	template < typename T >
	T mid( interval<T> const& a) { return a.mid() ; }

	template < typename T >
	interval<T> low( interval<T> const& a) { return a.lower() ; }

	template < typename T >
	interval<T> up( interval<T> const& a) { return a.upper() ; }

	template < typename T >
	bool is_singleton( interval<T> const& a) { return a.is_singleton() ; }

	template < typename T >
	void swap( interval<T>& x, interval<T>& y ) noexcept
	{
		x.swap( y ) ;
	}
	/*  Generic numeric function for primitive dispatch  */

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto sin( T&& x ) { return std::sin( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto cos( T&& x ) { return std::cos( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto tan( T&& x ) { return std::tan( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto asin( T&& x ) { return std::asin( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto acos( T&& x ) { return std::acos( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto atan( T&& x ) { return std::atan( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto sinh( T&& x ) { return std::sinh( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto cosh( T&& x ) { return std::cosh( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto tanh( T&& x ) { return std::tanh( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto asinh( T&& x ) { return std::asinh( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto acosh( T&& x ) { return std::acosh( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto atanh( T&& x ) { return std::atanh( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto exp( T&& x ) { return std::exp( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto exp2( T&& x ) { return std::exp2( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto expm1( T&& x ) { return std::expm1( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto pow( T&& x, int n) { return std::pow( std::forward<T>( x ), n) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto abs( T&& x ) { return std::abs( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto sqrt( T&& x ) { return std::sqrt( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto log( T&& x ) { return std::log( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto log1p( T&& x ) { return std::log1p( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto log2( T&& x ) { return std::log2( std::forward<T>( x ) ) ; }
	
	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto log10( T&& x ) { return std::log10( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto erf( T&& x ) { return std::erf( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto erfc( T&& x ) { return std::erfc( std::forward<T>( x ) ) ; }


	/*  for enum output with string  */

	//interval_ordering to string
	const std::string enum2str( interval_ordering const& x )
	{
		switch ( x )
		{
		case Cranberries::interval_ordering::less:
			return std::string( "less" ) ;
		case Cranberries::interval_ordering::greater:
			return std::string( "greater" ) ;
		case Cranberries::interval_ordering::unordered:
			return std::string( "unordered" ) ;
		}
		return std::string( "niether" ) ;
	}

	//partial_ordering to string
	const std::string enum2str( partial_ordering const& x )
	{
		switch ( x )
		{
		case Cranberries::partial_ordering::less:
			return std::string( "less" ) ;
		case Cranberries::partial_ordering::greater:
			return std::string( "greater" ) ;
		case Cranberries::partial_ordering::unordered:
			return std::string( "unordered" ) ;
		}
		return std::string( "niether" ) ;
	}

	//weak_ordering to string
	const std::string enum2str( weak_ordering const& x )
	{
		switch ( x )
		{
		case Cranberries::weak_ordering::less:
			return std::string( "less" ) ;
		case Cranberries::weak_ordering::greater:
			return std::string( "greater" ) ;
		case Cranberries::weak_ordering::equivalent:
			return std::string( "equivalent" ) ;
		}
		return std::string( "niether" ) ;
	}

	//total_ordering to string
	const std::string enum2str( total_ordering const& x )
	{
		switch ( x )
		{
		case Cranberries::total_ordering::less:
			return std::string( "less" ) ;
		case Cranberries::total_ordering::greater:
			return std::string( "greater" ) ;
		case Cranberries::total_ordering::equal:
			return std::string( "equal" ) ;
		}
		return std::string( "niether" ) ;
	}

	//interval_relation to string to string
	const std::string enum2str( interval_relation const& x )
	{
		switch ( x )
		{
		case Cranberries::interval_relation::interval_less:
			return std::string( "interval_less" ) ;
		case Cranberries::interval_relation::interval_greater:
			return std::string( "interval_greater" ) ;
		case Cranberries::interval_relation::partial_less:
			return std::string( "partial_less" ) ;
		case Cranberries::interval_relation::partial_greater:
			return std::string( "partial_greater" ) ;
		case Cranberries::interval_relation::weak_less:
			return std::string( "weak_less" ) ;
		case Cranberries::interval_relation::weak_greater:
			return std::string( "weak_greater" ) ;
		case Cranberries::interval_relation::total_less:
			return std::string( "total_less" ) ;
		case Cranberries::interval_relation::total_greater:
			return std::string( "total_greater" ) ;
		case Cranberries::interval_relation::contain:
			return std::string( "contain" ) ;
		case Cranberries::interval_relation::part_of:
			return std::string( "part_of" ) ;
		case Cranberries::interval_relation::equal:
			return std::string( "equal" ) ;    
		case Cranberries::interval_relation::niether:
			return std::string( "niether" ) ;
		}
		throw Cranberries::logic_error( "error" ) ;
	}

	/*  ostream << enum overload  */

	//ostream << interval_ordering
	std::ostream& operator<<( std::ostream& s, interval_ordering const& x )
	{
		return ( s << enum2str( x ) ) ;
	}

	//ostream << partial_ordering
	std::ostream& operator<<( std::ostream& s, partial_ordering const& x )
	{
		return ( s << enum2str( x ) ) ;
	}

	//ostream << weak_ordering
	std::ostream& operator<<( std::ostream& s, weak_ordering const& x )
	{
		return ( s << enum2str( x ) ) ;
	}

	//ostream << total_ordering
	std::ostream& operator<<( std::ostream& s, total_ordering const& x )
	{
		return ( s << enum2str( x ) ) ;
	}

	//ostream << interval_relation
	std::ostream& operator<<( std::ostream& s, interval_relation const& x )
	{
		return ( s << enum2str( x ) ) ;
	}

	/*  Generic operator<< for class whitch has menber function print( ostream& )  */

	template  < typename T >
	auto operator<<( std::ostream& s, T&& x )->decltype( x.print( s ) ) 
	{
		return x.print( s ) ;
	}

	/*  output for version  */

	std::ostream& operator<<( std::ostream& s, version_tag const& v)
	{
		return s << (static_cast<unsigned>(v) / 0x01000000)
			<< "." << ((static_cast<unsigned>(v) % 0x01000000) / 0x00010000)
			<< "." << (static_cast<unsigned>(v) % 0x00010000) ;
	}

	/*
	[ Note : Four arithmetic operator has three ovreload, 
	interval<T> Op interval<T>, T Op interval<T> and interval<T> Op T .
	High accuracy arithmetic operation is default. - end note ]
	*/

	/*
	[ Example : High accuracy operation is available default operator-, * and / .
	auto x = hull( 1.0, 2.0 ) ;
	auto y = hull( -1.0, 1.0 ) ;
	x - x //  returns [ 0.0, 0.0 ]
	y * y //  returns [ 0.0, 1.0 ]
	x / x //  returns [ 1.0, 1.0 ]
	- end example ]
	*/

	/*
	[ Example : You can switch high accuracy arithmetic into nomal arithmetic using namespace inside a scope block.
	auto x = hull( 1.0, 2.0 ) ;
	auto y = hull( -1.0, 1.0 ) ;
	{
		using namespace Cranberries::nomal_accuracy;
		x - x //  returns [ -1.0, 1.0 ]
		y - y //  returns [ -1.0, 1.0 ]
		x / x //  returns [ 0.5, 2.0 ]
	}
	- end example ]
	*/

	template < typename L, typename R >
	struct promotion
	{
		typedef decltype( L{} +R{} ) value_type ;
		typedef interval<value_type> return_type ;
	} ;

	template < typename L, typename R >
	using promotion_t = typename promotion<L,R>::value_type ;

	/*  Interval Addition Op  */


	inline namespace normal_accuracy
	{

		template < typename L, typename R >
		const interval<promotion_t<L,R>> add( interval<L> const& x, interval<R> const& y )
		{
			typedef promotion_t<L,R> T ;
			return interval<T>{ downward( x.lower() + y.lower() ), upward( x.upper() + y.upper() ) } ;
		}

		template < typename L, typename R >
		const interval<promotion_t<L, R>> sub( interval<L> const& x, interval<R> const& y )
		{
			typedef promotion_t<L, R> T;
			return interval<T>{ downward( x.lower() - y.upper() ), upward( x.upper() - y.lower() ) } ;
		}

		template < typename L, typename R >
		const interval<promotion_t<L, R>> multi( interval<L> const& x, interval<R> const& y )
		{
			typedef promotion_t<L, R> T;

			auto x_lower = x.lower() ;
			auto x_upper = x.upper() ;
			auto y_lower = y.lower() ;
			auto y_upper = y.upper() ;

			if ( x_lower >= zero<T>() && y_lower >= zero<T>() ) 
			{
				return interval<T>{ downward( x_lower * y_lower ), upward( x_upper * y_upper ) } ;
			}
			else if ( x_lower >= zero<T>() && y_lower < zero<T>() && y_upper > zero<T>() ) 
			{
				return interval<T>{ downward( x_upper * y_lower ), upward( x_upper * y_upper ) } ;
			}
			else if ( x_lower >= zero<T>() && y_upper <= zero<T>() ) 
			{
				return interval<T>{ downward( x_upper * y_lower ), upward( x_lower * y_upper ) } ;
			}
			else if ( x_lower < zero<T>() && x_upper > zero<T>() && y_lower >= zero<T>() ) 
			{
				return interval<T>{ downward( x_lower * y_upper ), upward( x_upper * y_upper ) } ;
			}
			else if ( x_lower < zero<T>() && x_upper > zero<T>() && y_upper <= zero<T>() ) 
			{
				return interval<T>{ downward( x_upper * y_lower ), upward( x_lower * y_lower ) } ;
			}
			else if ( x_upper <= zero<T>() && y_lower >= zero<T>() ) 
			{
				return interval<T>{ downward( x_lower * y_upper ), upward( x_upper * y_lower ) } ;
			}
			else if ( x_upper <= zero<T>() && y_lower < zero<T>() && y_upper > zero<T>() ) 
			{
				return interval<T>{ downward( x_lower * y_upper ), upward( x_lower * y_lower ) } ;
			}
			else if ( x_upper <= zero<T>() && y_upper <= zero<T>() ) 
			{
				return interval<T>{ downward( x_upper * y_upper ), upward( x_lower * y_lower ) } ;
			}
			else {
				return interval<T>{
					std::fmin( downward( x_upper * y_lower ), downward( x_lower * y_upper ) )
				   ,std::fmax( upward( x_upper * y_upper ), upward( x_lower * y_lower ) )
				} ;
			}

		}
		template < typename L, typename R>
		const interval<promotion_t<L, R>> div( interval<L> const& x, interval<R> const& y )
		{
			typedef promotion_t<L,R> T ;

			auto y_lower = y.lower() ;
			auto y_upper = y.upper() ;

			if ( y_lower <= zero<T>() && y_upper >= zero<T>() ) 
			{
				throw logic_error( "Divided by Cranberries which contains Zero!" ) ;
			}
			else
			{
				interval<T> tmp( downward( one<T>() / y_upper ) ,
					upward( one<T>() / y_lower ) ) ;
				return normal_accuracy::multi( x, tmp) ;
			}
		}
		template < typename L, typename R>
		interval<L>& add_assign( interval<L>& x, interval<R> const& y )
		{
			L&& x_lower = x.lower() ;
			L&& x_upper = x.upper() ;
			L&& y_lower = y.lower() ;
			L&& y_upper = y.upper() ;

			x.set( downward( x_lower + y_lower ), upward( x_upper + y_upper ) ) ;

			return x ;
		}

		template < typename L, typename R>
		interval<L>& sub_assign( interval<L>& x, interval<R> const& y )
		{
			L&& x_lower = x.lower() ;
			L&& x_upper = x.upper() ;
			L&& y_lower = y.lower() ;
			L&& y_upper = y.upper() ;

			x.set( downward( x_lower - y_upper ), upward( x_upper - y_lower ) ) ;
			return x ;
		}

		template < typename T, typename U>
		interval<T>& multi_assign( interval<T> const& x, interval<U> const& y )
		{
			auto x_lower = x.lower() ;
			auto x_upper = x.upper() ;
			auto y_lower = y.lower() ;
			auto y_upper = y.upper() ;

			if ( x_lower >= zero<T>() && y_lower >= zero<T>() ) 
			{
				x.set_lower( downward( x_lower * y_lower ) ) ;
				x.set_upper( upward( x_upper * y_upper ) ) ;
			}
			else if ( x_lower >= zero<T>() && y_lower < zero<T>() && y_upper > zero<T>() ) 
			{
				x.set_lower( downward( x_upper * y_lower ) ) ;
				x.set_upper( upward( x_upper * y_upper ) ) ;
			}
			else if ( x_lower >= zero<T>() && y_upper <= zero<T>() ) 
			{
				x.set_lower( downward( x_upper * y_lower ) ) ;
				x.set_upper( upward( x_lower * y_upper ) ) ;
			}
			else if ( x_lower < zero<T>() && x_upper > zero<T>() && y_lower >= zero<T>() ) 
			{
				x.set_lower( downward( x_lower * y_upper ) ) ;
				x.set_upper( upward( x_upper * y_upper ) ) ;
			}
			else if ( x_lower < zero<T>() && x_upper > zero<T>() && y_upper <= zero<T>() ) 
			{
				x.set_lower( downward( x_upper * y_lower ) ) ;
				x.set_upper( upward( x_lower * y_lower ) ) ;
			}
			else if ( x_upper <= zero<T>() && y_lower >= zero<T>() ) 
			{
				x.set_lower( downward( x_lower * y_upper ) ) ;
				x.set_upper( upward( x_upper * y_lower ) ) ;
			}
			else if ( x_upper <= zero<T>() && y_lower < zero<T>() && y_upper > zero<T>() ) 
			{
				x.set_lower( downward( x_lower * y_upper ) ) ;
				x.set_upper( upward( x_lower * y_lower ) ) ;
			}
			else if ( x_upper <= zero<T>() && y_upper <= zero<T>() ) 
			{
				x.set_lower( downward( x_upper * y_upper ) ) ;
				x.set_upper( upward( x_lower * y_lower ) ) ;
			}
			else {
				x.set_lower( std::fmin( downward( x_upper * y_lower ), downward( x_lower * y_upper ) ) ) ;
				x.set_upper( std::fmax( upward( x_upper * y_upper ), upward( x_lower * y_lower ) ) ) ;
			}
			return x ;
		}
		template < typename T, typename U>
		interval<T>& div_assign( interval<T>& x, interval<U> const& y )
		{

			auto y_lower = y.lower() ;
			auto y_upper = y.upper() ;

			if ( y_lower <= zero<T>() && y_upper >= zero<T>() ) 
			{
				throw logic_error( "Divided by Cranberries which contains Zero!" ) ;
			}
			else
			{
				interval<T> tmp( downward( one<T>() / y_upper ), upward( one<T>() / y_lower ) ) ;
				return normal_accuracy::multi_assign( x, tmp ) ;
			}
		}

	}

	namespace high_accuracy
	{

		template < typename T >
		const interval<T> add(interval<T> const& x, interval<T> const& y)
		{
			return interval<T>{ downward(x.lower() + y.lower()), upward(x.upper() + y.upper()) };
		}
		template < typename L, typename R >
		const interval<promotion_t<L,R>> add(interval<L> const& x, interval<R> const& y)
		{
			return interval<promotion_t<L,R>>{ downward(x.lower() + y.lower()), upward(x.upper() + y.upper()) };
		}
		template < typename T >
		const interval<T> sub(interval<T> const& x, interval<T> const& y)
		{
			if (&x == &y)
				return interval<T>{};
			return interval<T>{ downward(x.lower() - y.upper()), upward(x.upper() - y.lower()) };
		}
		template < typename L, typename R>
		const interval<promotion_t<L,R>> sub(interval<L> const& x, interval<R> const& y)
		{
			return interval<promotion_t<L,R>>{ downward(x.lower() - y.upper()), upward(x.upper() - y.lower()) };
		}
		template < typename T >
		const interval<T> multi(interval<T> const& x, interval<T> const& y)
		{
			if (&x == &y)
				return pow(x, 2);

			auto x_lower = x.lower();
			auto x_upper = x.upper();
			auto y_lower = y.lower();
			auto y_upper = y.upper();

			if (x_lower >= zero<T>() && y_lower >= zero<T>())
			{
				return interval<T>{ downward(x_lower * y_lower), upward(x_upper * y_upper) };
			}
			else if (x_lower >= zero<T>() && y_lower < zero<T>() && y_upper > zero<T>())
			{
				return interval<T>{ downward(x_upper * y_lower), upward(x_upper * y_upper) };
			}
			else if (x_lower >= zero<T>() && y_upper <= zero<T>())
			{
				return interval<T>{ downward(x_upper * y_lower), upward(x_lower * y_upper) };
			}
			else if (x_lower < zero<T>() && x_upper > zero<T>() && y_lower >= zero<T>())
			{
				return interval<T>{ downward(x_lower * y_upper), upward(x_upper * y_upper) };
			}
			else if (x_lower < zero<T>() && x_upper > zero<T>() && y_upper <= zero<T>())
			{
				return interval<T>{ downward(x_upper * y_lower), upward(x_lower * y_lower) };
			}
			else if (x_upper <= zero<T>() && y_lower >= zero<T>())
			{
				return interval<T>{ downward(x_lower * y_upper), upward(x_upper * y_lower) };
			}
			else if (x_upper <= zero<T>() && y_lower < zero<T>() && y_upper > zero<T>())
			{
				return interval<T>{ downward(x_lower * y_upper), upward(x_lower * y_lower) };
			}
			else if (x_upper <= zero<T>() && y_upper <= zero<T>())
			{
				return interval<T>{ downward(x_upper * y_upper), upward(x_lower * y_lower) };
			}
			else {
				return interval<T>{
					std::fmin(downward(x_upper * y_lower), downward(x_lower * y_upper))
						, std::fmax(upward(x_upper * y_upper), upward(x_lower * y_lower))
				};
			}
		}
		template < typename L, typename R >
		const interval<promotion_t<L,R>> multi( interval<L> const& x, interval<R> const& y )
		{
			typedef promotion_t<L,R> T ;

			auto x_lower = x.lower();
			auto x_upper = x.upper();
			auto y_lower = y.lower();
			auto y_upper = y.upper();

			if (x_lower >= zero<T>() && y_lower >= zero<T>())
			{
				return interval<T>{ downward(x_lower * y_lower), upward(x_upper * y_upper) };
			}
			else if (x_lower >= zero<T>() && y_lower < zero<T>() && y_upper > zero<T>())
			{
				return interval<T>{ downward(x_upper * y_lower), upward(x_upper * y_upper) };
			}
			else if (x_lower >= zero<T>() && y_upper <= zero<T>())
			{
				return interval<T>{ downward(x_upper * y_lower), upward(x_lower * y_upper) };
			}
			else if (x_lower < zero<T>() && x_upper > zero<T>() && y_lower >= zero<T>())
			{
				return interval<T>{ downward(x_lower * y_upper), upward(x_upper * y_upper) };
			}
			else if (x_lower < zero<T>() && x_upper > zero<T>() && y_upper <= zero<T>())
			{
				return interval<T>{ downward(x_upper * y_lower), upward(x_lower * y_lower) };
			}
			else if (x_upper <= zero<T>() && y_lower >= zero<T>())
			{
				return interval<T>{ downward(x_lower * y_upper), upward(x_upper * y_lower) };
			}
			else if (x_upper <= zero<T>() && y_lower < zero<T>() && y_upper > zero<T>())
			{
				return interval<T>{ downward(x_lower * y_upper), upward(x_lower * y_lower) };
			}
			else if (x_upper <= zero<T>() && y_upper <= zero<T>())
			{
				return interval<T>{ downward(x_upper * y_upper), upward(x_lower * y_lower) };
			}
			else {
				return interval<T>{
					std::fmin(downward(x_upper * y_lower), downward(x_lower * y_upper))
						, std::fmax(upward(x_upper * y_upper), upward(x_lower * y_lower))
				};
			}
		}
		template < typename T >
		const interval<T> div(interval<T> const& x, interval<T> const& y)
		{
			if (&x == &y)
				return interval<T>{ one<T>(), one<T>() };
			auto y_lower = y.lower();
			auto y_upper = y.upper();

			if (y_lower <= zero<T>() && y_upper >= zero<T>())
			{
				throw logic_error("Divided by Cranberries which contains Zero!");
			}
			else
			{
				interval<T> tmp(downward(one<T>() / y_upper), upward(one<T>() / y_lower));
				return normal_accuracy::multi(x, tmp);
			}
		}
		template < typename L, typename R >
		const interval<promotion_t<L,R>> div( interval<L> const& x, interval<R> const& y )
		{
			typedef promotion_t<L,R> T ;

			auto y_lower = y.lower();
			auto y_upper = y.upper();

			if (y_lower <= zero<T>() && y_upper >= zero<T>())
			{
				throw logic_error("Divided by Cranberries which contains Zero!");
			}
			else
			{
				interval<T> tmp(downward(one<T>() / y_upper), upward(one<T>() / y_lower));
				return normal_accuracy::multi(x, tmp);
			}
		}

		template < typename L, typename R >
		interval<L>& add_assign( interval<L>& x, interval<R> const& y )
		{
			auto&& x_lower = x.lower();
			auto&& x_upper = x.upper();
			auto&& y_lower = y.lower();
			auto&& y_upper = y.upper();

			x.set( downward( x_lower + y_lower ), upward( x_upper + y_upper ) ) ;

			return x ;
		}

		template < typename L, typename R >
		interval<L>& sub_assign(interval<L>& x, interval<R> const& y)
		{
			auto&& x_lower = x.lower();
			auto&& x_upper = x.upper();
			auto&& y_lower = y.lower();
			auto&& y_upper = y.upper();

			x.set( downward( x_lower - y_upper ), upward( x_upper - y_lower ) ) ;
			return x;
		}

		template < typename T >
		interval<T>& sub_assign( interval<T>& x, interval<T> const& y )
		{
			if (&x == &y)
			{
				x.set(zero<T>(), zero<T>());
			}
			else {
				auto&& x_lower = x.lower();
				auto&& x_upper = x.upper();
				auto&& y_lower = y.lower();
				auto&& y_upper = y.upper();

				x.set(downward(x_lower - y_upper), upward(x_upper - y_lower));
			}
			return x;
		}

		template < typename T >
		interval<T>& multi_assign( interval<T> const& x, interval<T> const& y )
		{
			auto x_lower = x.lower() ;
			auto x_upper = x.upper() ;
			auto y_lower = y.lower() ;
			auto y_upper = y.upper() ;

			if ( &x == &y )
			{
				if ( x_lower > zero<T>() )
				{
					x.set( std::pow( x_lower ), std::pow( x_upper ) ) ;
				}
				else if ( x_upper < zero<T>() )
				{
					x.set( std::pow( x_upper ), std::pow( x_lower ) ) ;
				}
				else {
					x.set( zero<T>(), std::fmax( std::pow( x_lower ), std::pow( x_upper ) ) ) ;
				}
			}
			else if ( x_lower >= zero<T>() && y_lower >= zero<T>() ) 
			{
				x.set( downward( x_lower * y_lower ), upward( x_upper * y_upper ) ) ;
			}
			else if ( x_lower >= zero<T>() && y_lower < zero<T>() && y_upper > zero<T>() ) 
			{
				x.set( downward( x_upper * y_lower ), upward( x_upper * y_upper ) ) ;
			}
			else if ( x_lower >= zero<T>() && y_upper <= zero<T>() ) 
			{
				x.set( downward( x_upper * y_lower ), upward( x_lower * y_upper ) ) ;
			}
			else if ( x_lower < zero<T>() && x_upper > zero<T>() && y_lower >= zero<T>() ) 
			{
				x.set( downward( x_lower * y_upper ), upward( x_upper * y_upper ) ) ;
			}
			else if ( x_lower < zero<T>() && x_upper > zero<T>() && y_upper <= zero<T>() ) 
			{
				x.set( downward( x_upper * y_lower ), upward( x_lower * y_lower ) ) ;
			}
			else if ( x_upper <= zero<T>() && y_lower >= zero<T>() ) 
			{
				x.set( downward( x_lower * y_upper ), upward( x_upper * y_lower ) ) ;
			}
			else if ( x_upper <= zero<T>() && y_lower < zero<T>() && y_upper > zero<T>() ) 
			{
				x.set( downward( x_lower * y_upper ), upward( x_lower * y_lower ) ) ;
			}
			else if ( x_upper <= zero<T>() && y_upper <= zero<T>() ) 
			{
				x.set( downward( x_upper * y_upper ), upward( x_lower * y_lower ) ) ;
			}
			else {
				x.set( std::fmin( downward( x_upper * y_lower ), downward( x_lower * y_upper ) ), std::fmax( upward( x_upper * y_upper ), upward( x_lower * y_lower ) ) ) ;
			}
			return x ;
		}
		template < typename L, typename R >
		interval<L>& multi_assign(interval<L> const& x, interval<R> const& y)
		{
			auto&& x_lower = x.lower();
			auto&& x_upper = x.upper();
			auto&& y_lower = y.lower();
			auto&& y_upper = y.upper();

			if (x_lower >= zero<L>() && y_lower >= zero<R>())
			{
				x.set(downward(x_lower * y_lower), upward(x_upper * y_upper));
			}
			else if (x_lower >= zero<L>() && y_lower < zero<R>() && y_upper > zero<R>())
			{
				x.set(downward(x_upper * y_lower), upward(x_upper * y_upper));
			}
			else if (x_lower >= zero<L>() && y_upper <= zero<R>())
			{
				x.set(downward(x_upper * y_lower), upward(x_lower * y_upper));
			}
			else if (x_lower < zero<L>() && x_upper > zero<L>() && y_lower >= zero<R>())
			{
				x.set(downward(x_lower * y_upper), upward(x_upper * y_upper));
			}
			else if (x_lower < zero<L>() && x_upper > zero<L>() && y_upper <= zero<R>())
			{
				x.set(downward(x_upper * y_lower), upward(x_lower * y_lower));
			}
			else if (x_upper <= zero<L>() && y_lower >= zero<R>())
			{
				x.set(downward(x_lower * y_upper), upward(x_upper * y_lower));
			}
			else if (x_upper <= zero<L>() && y_lower < zero<R>() && y_upper > zero<R>())
			{
				x.set(downward(x_lower * y_upper), upward(x_lower * y_lower));
			}
			else if (x_upper <= zero<L>() && y_upper <= zero<R>())
			{
				x.set(downward(x_upper * y_upper), upward(x_lower * y_lower));
			}
			else {
				x.set(std::fmin(downward(x_upper * y_lower), downward(x_lower * y_upper)), std::fmax(upward(x_upper * y_upper), upward(x_lower * y_lower)));
			}
			return x;
		}

		template < typename L, typename R >
		interval<L>& div_assign( interval<L>& x, interval<R> const& y )
		{

			auto&& y_lower = y.lower();
			auto&& y_upper = y.upper();

			if ( y_lower <= zero<R>() && y_upper >= zero<R>() )
			{
				throw logic_error("Divided by Cranberries which contains Zero!");
			}
			else
			{
				interval<L> tmp( downward( one<L>() / y_upper ), upward( one<L>() / y_lower ) ) ;
				return normal_accuracy::multi_assign( x, tmp ) ;
			}
		}
		template < typename T>
		interval<T>& div_assign(interval<T>& x, interval<T> const& y)
		{

			auto&& y_lower = y.lower();
			auto&& y_upper = y.upper();

			if (y_lower <= zero<T>() && y_upper >= zero<T>())
			{
				throw logic_error("Divided by Cranberries which contains Zero!");
			}
			else
			{
				if (&x == &y)
				{
					x.set_lower(one<T>());
					x.set_upper(one<T>());
					return x;
				}
				interval<T> tmp( downward(one<T>() / y_upper), upward(one<T>() / y_lower) ) ;
				return normal_accuracy::multi_assign(x, tmp);
			}
		}


	}
	
	inline namespace
	{

		template < typename L, typename R>
		const interval<promotion_t<L,R>> sub( L const& x, interval<R> const& y )
		{
			return interval<promotion_t<L,R>>{ downward( y.lower() - x ), upward( y.upper() - x) } ;
		}

		template < typename L, typename R >
		const interval<promotion_t<L,R>> sub( interval<L> const& x, R const& y )
		{
			return interval<promotion_t<L,R>>{ downward( x.lower() - y ), upward( x.upper() - y ) } ;
		}

		template < typename L, typename R >
		const interval<promotion_t<L,R>> add( L const& x, interval<R> const& y )
		{
			return interval<promotion_t<L,R>>{ downward( y.lower() + x), upward( y.upper() + x ) } ;
		}

		template < typename L, typename R >
		const interval<promotion_t<L,R>> add( interval<L> const& x, R const& y )
		{
			return interval<promotion_t<L, R>>{ downward( x.lower() + y ), upward( x.upper() + y ) } ;
		}

		template < typename L, typename R >
		const interval<promotion_t<L, R>> multi( L const& x, interval<R> const& y )
		{
			if ( x < zero<L>() ){
				return interval<promotion_t<L, R>>{ downward( y.upper() * x), upward( y.lower() * x) } ;
			}
			return interval<promotion_t<L, R>>{ downward( y.lower() * x), upward( y.upper() * x) } ;
		}

		template < typename L, typename R >
		const interval<promotion_t<L, R>> multi( interval<L> const& x, R const& y )
		{
			if ( y < zero<R>() ){
				return interval<promotion_t<L, R>>{ downward( x.upper() * y ), upward( x.lower() * y ) } ;
			}
			return interval<promotion_t<L, R>>{ downward( x.lower() * y ), upward( x.upper() * y ) } ;
		}

		template < typename L, typename R >
		interval<L>& add_assign( interval<L>& x, R const& y )
		{
			x.set_lower( downward( x.lower() + y ) ) ;
			x.set_upper( upward( x.upper() + y ) ) ;
			return x ;
		}

		template < typename L, typename R >
		interval<L>& sub_assign( interval<L>& x, R const& y )
		{
			x.set_lower( downward( x.lower() - y ) ) ;
			x.set_upper( upward( x.upper() - y ) ) ;
			return x ;
		}

		template < typename L, typename R >
		interval<L>& multi_assign( interval<L> const& x, R const& y )
		{
			if ( y < zero<R>() )
			{
				x.set_lower( downward( x.upper() * y ) ) ;
				x.set_upper( upward( x.lower() * y ) ) ;
			}
			else{
				x.set_lower( downward( x.lower() * y ) ) ;
				x.set_upper( upward( x.upper() * y ) ) ;
			}
			return x ;
		}
		template < typename L, typename R >
		interval<L>& div_assign( interval<L>& x, R const& y )
		{
			if ( y == zero<R>() ) 
			{
				throw logic_error( "Divided by Cranberries which contains Zero!" ) ;
			}
			else
			{
				if ( y < zero<R>() )
				{
					x.set_lower( downward( x.upper() / y ) ) ;
					x.set_upper( upward( x.lower() / y ) ) ;
				}
				else
				{
					x.set_lower( downward( x.lower() / y ) ) ;
					x.set_upper( upward( x.upper() / y ) ) ;
				}
				return x ; 
			}
		}

	}

	namespace normal_accurate_policy
	{

		template < typename L, typename R >
		constexpr auto operator+( L&& x, R&& y )->decltype( add( x, y ) )
		{
			return add( std::forward<L>(x), std::forward<R>(y) ) ;
		}
		template < typename L, typename R >
		constexpr auto operator-( L&& x, R&& y )->decltype( sub( x, y ) )
		{
			return sub( std::forward<L>(x), std::forward<R>(y) ) ;
		}
		template < typename L, typename R >
		constexpr auto operator*( L&& x, R&& y )->decltype( multi( x, y ) )
		{
			return multi( std::forward<L>(x), std::forward<R>(y) ) ;
		}
		template < typename L, typename R >
		constexpr auto operator/( L&& x, R&& y )->decltype( div( x, y ) )
		{
			return div( std::forward<L>(x), std::forward<R>(y) ) ;
		}
		template < typename L, typename R >
		interval<L>& operator+=( interval<L>& x, R&& y )
		{
			return add_assign( x, std::forward<R>(y) ) ;
		}
		template < typename L, typename R >
		interval<L>& operator-=( interval<L>& x, R&& y )
		{
			return sub_assign( x, std::forward<R>(y) ) ;
		}
		template < typename L, typename R >
		interval<L>& operator*=( interval<L>& x, R&& y )
		{
			return multi_assign( x, std::forward<R>(y) ) ;
		}
		template < typename L, typename R >
		interval<L>& operator/=( interval<L>& x, R&& y )
		{
			return div_assign( x, std::forward<L>(y) ) ;
		}

	}

	inline namespace high_accurate_policy
	{

		template < typename L, typename R >
		constexpr interval<promotion_t<L, R>> operator+( interval<L> const& x, interval<R> const& y )
		{
			return high_accuracy::add( x, y ) ;
		}
		template < typename L, typename R >
		constexpr interval<promotion_t<L, R>> operator+( interval<L> const& x, R const& y )
		{
			return add( x, y ) ;
		}
		template < typename L, typename R >
		constexpr interval<promotion_t<L, R>> operator+( L const& x, interval<R> const& y )
		{
			return add( x, y ) ;
		}

		template < typename L, typename R >
		constexpr interval<promotion_t<L,R>> operator-( interval<L> const& x, interval<R> const& y )
		{
			return high_accuracy::sub( x, y ) ;
		}
		template < typename L, typename R >
		constexpr interval<promotion_t<L, R>> operator-( interval<L> const& x, R const& y )
		{
			return sub( x, y ) ;
		}
		template < typename L, typename R >
		constexpr interval<promotion_t<L, R>> operator-( L const& x, interval<R> const& y )
		{
			return sub( x, y ) ;
		}

		template < typename L, typename R >
		constexpr interval<promotion_t<L, R>> operator*( interval<L> const& x, interval<R> const& y )
		{
			return high_accuracy::multi( x, y ) ;
		}
		template < typename L, typename R >
		constexpr interval<promotion_t<L, R>> operator*( interval<L> const& x, R const& y )
		{
			return multi( x, y ) ;
		}
		template < typename L, typename R >
		constexpr interval<promotion_t<L, R>> operator*( L const& x, interval<R> const& y )
		{
			return multi( x, y ) ;
		}

		template < typename L, typename R >
		constexpr interval<promotion_t<L, R>> operator/( interval<L> const& x, interval<R> const& y )
		{
			return high_accuracy::div( x, y ) ;
		}
		template < typename L, typename R >
		constexpr interval<promotion_t<L, R>> operator/( interval<L> const& x, R const& y )
		{
			return div( x, y ) ;
		}
		template < typename L, typename R >
		constexpr interval<promotion_t<L, R>> operator/( L const& x, interval<R> const& y )
		{
			return div( x, y ) ;
		}
		template < typename L, typename R >
		interval<L>& operator+=( interval<L>& x, interval<R> const& y )
		{
			return high_accuracy::add_assign( x, y ) ;
		}
		template < typename L, typename R >
		interval<L>& operator-=( interval<L>& x, interval<R> const& y )
		{
			return high_accuracy::sub_assign( x, y ) ;
		}
		template < typename L, typename R >
		interval<L>& operator*=( interval<L>& x, interval<R> const& y )
		{
			return high_accuracy::multi_assign( x, y ) ;
		}
		template < typename L, typename R >
		interval<L>& operator/=( interval<L>& x, interval<R> const& y )
		{
			return high_accuracy::div_assign( x, y ) ;
		}

	}



	//-------------------//
	/*  interval output  */
	//-------------------//

	/*  Generic Output Stream  */

	template < typename T >
	std::ostream& operator<<( std::ostream& s, std::unique_ptr<T> x )
	{
		s << ( *x ) ;
		return ( s ) ;
	}

	/*  function returns interval into C style string  */

	template < typename T >
	const char* Cranberries::interval<T>::c_str() const
	{
		auto s = new std::string( "[ " + std::to_string( this->lower() )  + ", " + std::to_string( this->upper() )  + " ]" ) ;
		return s->c_str() ;
	}

	//------------------------------------------------------------------------//
	/*  It is cumbersome to write the type to every declaration.              */
	/*  Like ' auto x = interval<double>(one<T>(), static_cast<T>(3.0) ) '.   */
	/*  And it becomes a source of mistake.                                   */
	/*  Then, you can use argument dependent factory 'hull(Low,Up)'.          */
	/*  It allows infer to type from arguments and returns interval object.   */
	//------------------------------------------------------------------------//

	// for Lvalue
	template < typename T >
	interval<T> hull( T& low, T& up )
	{
		if ( low > up) { throw invalid_argument( "upper_bound less than lower_bound!" ) ; }
		return interval<T>(low, up) ;
	}

	// for Rvalue
	template < typename T >
	interval<T> hull( T&& low, T&& up )
	{
		if ( low > up) { throw invalid_argument( "upper_bound less than lower_bound!" ) ; }
		return interval<T>( std::move(low), std::move(up) ) ;
	}

	/*  Unique_Ptr  */
	template < typename T >
	std::unique_ptr<interval<T>> hull_ptr( T& low, T& up )
	{
		if ( low > up) { throw invalid_argument( "upper_bound less than lower_bound!" ) ; }
		return std::unique_ptr<interval<T>>(new interval<T>(low, up) ) ;
	}

	template < typename T >
	std::unique_ptr<interval<T>> hull_ptr( T&& low, T&& up )
	{
		if ( low > up) { throw invalid_argument( "upper_bound less than lower_bound!" ) ; }
		return std::unique_ptr<interval<T>>(new interval<T>(low, up) ) ;
	}


	//-----------------------------------//
	/*   interval relational functions   */
	//-----------------------------------//



	/*  Interval Ordering  */

	// interval less
	template < typename T, typename U >
	constexpr bool interval_less( interval<T> const& x, interval<U> const& y ) 
	{
		return ( x.upper() < y.lower() ) ;
	}

	template < typename T, typename U >
	constexpr bool interval_less( U const& x, interval<T> const& y )
	{
		return ( x < y.lower() ) ;
	}

	template < typename T, typename U >
	constexpr bool interval_less( interval<T> const& x, U const& y )
	{
		return ( x.upper() < y ) ;
	}

	template < typename T, typename U >
	constexpr bool interval_less_or_equal( interval<T> const& x, interval<U> const& y )
	{
		return ( x.upper() <= y.lower() ) ;
	}
	template < typename T, typename U >
	constexpr bool interval_less_or_equal( U const& x, interval<T> const& y )
	{
		return ( x <= y.lower() ) ;
	}

	template < typename T, typename U >
	constexpr bool interval_less_or_equal( interval<T> const& x, U const& y )
	{
		return ( x.upper() <= y ) ;
	}

	// interval greater
	template < typename T, typename U >
	constexpr bool interval_greater( interval<T> const& x, interval<U> const& y )
	{
		return ( x.lower() > y.upper() ) ;
	}

	template < typename T, typename U >
	constexpr bool interval_greater( U const& x, interval<T> const& y )
	{
		return ( x > y.upper() ) ;
	}

	template < typename T, typename U >
	constexpr bool interval_greater( interval<T> const& x, U const& y )
	{
		return ( x.lower() > y ) ;
	}

	template < typename T, typename U >
	constexpr bool interval_greater_or_equal( interval<T> const& x, interval<U> const& y )
	{
		return ( x.lower() >= y.upper() ) ;
	}

	template < typename T, typename U >
	constexpr bool interval_greater_or_equal( U const& x, interval<T> const& y )
	{
		return ( x >= y.upper() ) ;
	}

	template < typename T, typename U >
	constexpr bool interval_greater_or_equal( interval<T> const& x, U const& y )
	{
		return ( x.lower() >= y ) ;
	}

	// interval unordered
	template < typename T, typename U >
	constexpr bool interval_unordered( interval<T> const& x, interval<U> const& y )
	{
		return ( interval_less( x, y ) == false && interval_greater( x, y ) == false ) ;
	}
	template < typename T, typename U >
	constexpr bool interval_unordered( U const&, interval<T> const& )
	{
		return false ;
	}
	template < typename T, typename U >
	constexpr bool interval_unordered( interval<T> const&, U const& )
	{
		return false ;
	}


	/*  Partial Ordering  */

	// partial less
	template < typename T, typename U >
	constexpr bool partial_less( interval<T> const& x, interval<U> const& y )
	{
		return ( x.lower() < y.lower() && x.upper() < y.upper() ) ;
	}

	template < typename T, typename U >
	constexpr bool partial_less( U const& x, interval<T> const& y )
	{
		return ( x < y.lower() ) ;
	}

	template < typename T, typename U >
	constexpr bool partial_less( interval<T> const& x, U const& y )
	{
		return ( x.upper() < y ) ;
	}

	template < typename T, typename U >
	constexpr bool partial_less_or_equal( interval<T> const& x, interval<U> const& y )
	{
		return ( x.lower() <= y.lower() && x.upper() <= y.upper() ) ;
	}

	template < typename T, typename U >
	constexpr bool partial_less_or_equal( U const& x, interval<T> const& y )
	{
		return ( x <= y.lower() ) ;
	}

	template < typename T, typename U >
	constexpr bool partial_less_or_equal( interval<T> const& x, U const& y )
	{
		return ( x.upper() <= y ) ;
	}

	// partial greater
	template < typename T, typename U >
	constexpr bool partial_greater( interval<T> const& x, interval<U> const& y )
	{
		return ( x.lower() > y.lower() && x.upper() > y.upper() ) ;
	}

	template < typename T, typename U >
	constexpr bool partial_greater( U const& x, interval<T> const& y )
	{
		return ( x > y.upper() ) ;
	}

	template < typename T, typename U >
	constexpr bool partial_greater( interval<T> const& x, U const& y )
	{
		return ( x.lower() > y ) ;
	}

	template < typename T, typename U >
	constexpr bool partial_greater_or_equal( interval<T> const& x, interval<U> const& y )
	{
		return ( x.lower() >= y.lower() && x.upper() >= y.upper() ) ;
	}

	template < typename T, typename U >
	constexpr bool partial_greater_or_equal( U const& x, interval<T> const& y )
	{
		return ( std::forward<T>(x) >= y.upper() ) ;
	}

	template < typename T, typename U >
	constexpr bool partial_greater_or_equal( interval<T> const& x, U const& y )
	{
		return ( x.lower() >= y ) ;
	}

	// partial unordered
	template < typename T, typename U >
	constexpr bool partial_unordered( interval<T> const& x, interval<U> const& y )
	{
		return ( partial_less( x, y ) == false && partial_greater( x, y ) == false ) ;
	}
	template < typename T, typename U >
	constexpr bool partial_unordered( U const&, interval<T> const& )
	{
		return false;
	}
	template < typename T, typename U >
	constexpr bool partial_unordered( interval<T> const&, U const& )
	{
		return false;
	}


	/*  Weak Ordering  */

	// weak less
	template < typename T, typename U >
	constexpr bool weak_less( interval<T> const& x, interval<U> const& y )
	{
		return ( x.lower() < y.lower() ) ;
	}

	template < typename T, typename U >
	constexpr bool weak_less( U const& x, interval<T> const& y )
	{
		return ( x < y.lower() ) ;
	}

	template < typename T, typename U >
	constexpr bool weak_less( interval<T> const& x, U const& y )
	{
		return ( x.lower() > y ) ;
	}

	template < typename T, typename U >
	constexpr bool weak_less_or_equal( interval<T> const& x, interval<U> const& y )
	{
		return ( x.lower() <= y.lower() ) ;
	}

	template < typename T, typename U >
	constexpr bool weak_less_or_equal( U const& x, interval<T> const& y )
	{
		return ( x <= y.lower() ) ;
	}

	template < typename T, typename U >
	constexpr bool weak_less_or_equal( interval<T> const& x, U const& y )
	{
		return ( x.lower() >= std::forward<T>(y ) ) ;
	}

	// weak greater
	template < typename T, typename U >
	constexpr bool weak_greater( interval<T> const& x, interval<U> const& y )
	{
		return ( x.lower() > y.lower() ) ;
	}

	template < typename T, typename U >
	constexpr bool weak_greater( U const& x, interval<T> const& y )
	{
		return ( x > y.lower() ) ;
	}

	template < typename T, typename U >
	constexpr bool weak_greater( interval<T> const& x, U const& y )
	{
		return ( x.lower() > y ) ;
	}

	template < typename T, typename U >
	constexpr bool weak_greater_or_equal( interval<T> const& x, interval<U> const& y )
	{
		return ( x.lower() >= y.lower() ) ;
	}

	template < typename T, typename U >
	constexpr bool weak_greater_or_equal( U const& x, interval<T> const& y )
	{
		return ( x >= y.lower() ) ;
	}

	template < typename T, typename U >
	constexpr bool weak_greater_or_equal( interval<T> const& x, U const& y )
	{
		return ( x.lower() >= y ) ;
	}

	// weak equal
	template < typename T, typename U >
	constexpr bool weak_equal( interval<T> const& x, interval<U> const& y )
	{
		return ( weak_less( x, y ) == false && weak_less( y, x ) == false ) ;
	}

	template < typename T, typename U >
	constexpr bool weak_equal( U const& x, interval<T> const& y )
	{
		return ( x == y.lower() ) ;
	}

	template < typename T, typename U >
	constexpr bool weak_equal( interval<T> const& x, U const& y )
	{
		return ( x.lower() == std::forward<U>(y ) ) ;
	}

	/*  Total Ordering  */

	// total less
	template < typename T, typename U >
	constexpr bool total_less( interval<T> const& x, interval<U> const& y )
	{
		return x.lower() == y.lower() ? x.upper() < y.upper() : x.lower() < y.lower() ;
	}

	template < typename T, typename U >
	constexpr bool total_less( U const& x, interval<T> const& y )
	{
		return ( x < y.upper() ) ;
	}

	template < typename T, typename U >
	constexpr bool total_less( interval<T> const& x, U const& y )
	{
		return ( x.lower() < y ) ;
	}

	template < typename T, typename U >
	constexpr bool total_less_or_equal( interval<T> const& x, interval<U> const& y )
	{
		return x.lower() == y.lower() ? x.upper() <= y.upper() : x.lower() <= y.lower() ;
	}

	template < typename T, typename U >
	constexpr bool total_less_or_equal( U const& x, interval<T> const& y )
	{
		return ( x <= y.upper() ) ;
	}

	template < typename T, typename U >
	constexpr bool total_less_or_equal( interval<T> const& x, U const& y )
	{
		return ( x.lower() <= y ) ;
	}

	// total greater
	template < typename T, typename U >
	constexpr bool total_greater( interval<T> const& x, interval<U> const& y )
	{
		return x.lower() == y.lower() ? x.upper() > y.upper() : x.lower() > y.lower() ;
	}

	template < typename T, typename U >
	constexpr bool total_greater( U const& x, interval<T> const& y )
	{
		return ( x > y.lower () ) ;
	}

	template < typename T, typename U >
	constexpr bool total_greater( interval<T> const& x, U const& y )
	{
		return ( x.upper() > y ) ;
	}

	template < typename T, typename U >
	constexpr bool total_greater_or_equal( interval<T> const& x, interval<U> const& y )
	{
		return x.lower() == y.lower() ? x.upper() >= y.upper() : x.lower() >= y.lower() ;
	}

	template < typename T, typename U >
	constexpr bool total_greater_or_equal( U const& x, interval<T> const& y )
	{
		return ( x >= y.lower() ) ;
	}

	template < typename T, typename U >
	constexpr bool total_greater_or_equal( interval<T> const& x, U const& y )
	{
		return ( x.upper() >= y ) ;
	}

	// total equal
	template < typename T, typename U >
	constexpr bool total_equal( interval<T> const& x, interval<U> const& y )
	{
		return ( x.lower() == y.lower() && x.upper() == y.upper() ) ;
	}

	template < typename T, typename U >
	constexpr bool total_equal( U const& x, interval<T> const& y )
	{
		return ( x == y.lower() && x == y.upper() ) ;
	}

	template < typename T, typename U >
	constexpr bool total_equal( interval<T> const& x, U const& y )
	{
		return ( x.lower() == std::forward<T>(y ) && x.upper() == std::forward<T>(y ) ) ;
	}

	//--------------------------//
	/*  Ordering Discriminator  */
	//--------------------------//

	/*  Interval Ordering  */

	template < typename T, typename U >
	Cranberries::interval_ordering interval_order( interval<T> const& x, interval<U> const& y )
	{
		if ( interval_less( x, y ) )
		{
			return Cranberries::interval_ordering::less ;
		}
		else if ( interval_greater( x, y ) )
		{
			return Cranberries::interval_ordering::greater ;
		}
		return Cranberries::interval_ordering::unordered ;
	}

	template < typename T, typename U >
	Cranberries::interval_ordering interval_order( U const& x, interval<T> const& y )
	{
		if ( interval_less( x, y ) )
		{
			return Cranberries::interval_ordering::less;
		}
		else if ( interval_greater( x, y ) )
		{
			return Cranberries::interval_ordering::greater;
		}
		return Cranberries::interval_ordering::unordered;
	}

	template < typename T, typename U >
	Cranberries::interval_ordering interval_order( interval<T> const& x, U const& y )
	{
		if ( interval_less( x, y ) )
		{
			return Cranberries::interval_ordering::less;
		}
		else if ( interval_greater( x, y ) )
		{
			return Cranberries::interval_ordering::greater;
		}
		return Cranberries::interval_ordering::unordered;
	}

	/*  Partial Ordering  */

	template < typename T, typename U >
	Cranberries::partial_ordering partial_order( interval<T> const& x, interval<U> const& y )
	{
		if ( partial_less( x, y ) )
		{
			return partial_ordering::less ;
		}
		else if ( partial_greater( x, y ) )
		{
			return partial_ordering::greater ;
		}
		return partial_ordering::unordered ;
	}

	template < typename T, typename U >
	Cranberries::partial_ordering partial_order( U const& x, interval<T> const& y )
	{
		if ( partial_less( x, y ) )
		{
			return partial_ordering::less;
		}
		else if ( partial_greater( x, y ) )
		{
			return partial_ordering::greater;
		}
		return partial_ordering::unordered;
	}

	template < typename T, typename U >
	Cranberries::partial_ordering partial_order( interval<T> const& x, U const& y )
	{
		if ( partial_less( x, y ) )
		{
			return partial_ordering::less;
		}
		else if ( partial_greater( x, y ) )
		{
			return partial_ordering::greater;
		}
		return partial_ordering::unordered;
	}

	/*  Weak ordering  */

	template < typename T, typename U >
	Cranberries::weak_ordering weak_order( interval<T> const& x, interval<U> const& y )
	{
		if ( weak_less( x, y ) )
		{
			return weak_ordering::less ;
		}
		else if ( weak_greater( x, y ) )
		{
			return weak_ordering::greater ;
		}
		return weak_ordering::equivalent ;
	}

	template < typename T, typename U >
	Cranberries::weak_ordering weak_order( U const& x, interval<T> const& y )
	{
		if ( weak_less( x, y ) )
		{
			return weak_ordering::less;
		}
		else if ( weak_greater( x, y ) )
		{
			return weak_ordering::greater;
		}
		return weak_ordering::equivalent;
	}

	template < typename T, typename U >
	Cranberries::weak_ordering weak_order( interval<T> const& x, U const& y )
	{
		if ( weak_less( x, y ) )
		{
			return weak_ordering::less;
		}
		else if ( weak_greater( x, y ) )
		{
			return weak_ordering::greater;
		}
		return weak_ordering::equivalent;
	}

	/*  Total Ordering  */

	template < typename T, typename U >
	Cranberries::total_ordering total_order( interval<T> const& x, interval<U> const& y )
	{
		if ( total_less( x, y ) )
		{
			return total_ordering::less ;
		}
		else if ( total_greater( x, y ) )
		{
			return total_ordering::greater ;
		}
		return total_ordering::equal ;
	}


	template < typename T, typename U >
	Cranberries::total_ordering total_order( U const& x, interval<T> const& y )
	{
		if ( total_less( x, y ) )
		{
			return total_ordering::less;
		}
		else if ( total_greater( x, y ) )
		{
			return total_ordering::greater;
		}
		return total_ordering::equal;
	}


	template < typename T, typename U >
	Cranberries::total_ordering total_order( interval<T> const& x, U const& y )
	{
		if ( total_less( x, y ) )
		{
			return total_ordering::less;
		}
		else if ( total_greater( x, y ) )
		{
			return total_ordering::greater;
		}
		return total_ordering::equal;
	}

	/*  interval compare function objrct  */
	template < order = order::Total >
	struct less
	{
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, interval<U> const& b ) const
		{
			return total_less( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( U const a, interval<T> const& b ) const
		{
			return total_less( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, U const& b ) const
		{
			return total_less( a, b ) ;
		}
	} ;


	template < >
	struct less < order::Weak >
	{
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, interval<U> const& b ) const
		{
			return weak_less( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( U const& a, interval<T> const& b ) const
		{
			return weak_less( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, U const& b) const
		{
			return weak_less( a, b ) ;
		}
	} ;


	template < >
	struct less < order::Partial >
	{
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, interval<U> const& b ) const
		{
			return partial_less( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( U const& a, interval<T> const& b ) const
		{
			return partial_less( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, U const& b ) const
		{
			return partial_less( a, b ) ;
		}
	} ;


	template < >
	struct less < order::Interval >
	{
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, interval<T> const& b ) const
		{
			return interval_less( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( U const& a, interval<T> const& b ) const
		{
			return interval_less( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, U const& b ) const
		{
			return interval_less( a, b ) ;
		}
	} ;

	template < order = order::Total >
	struct less_or_equal
	{
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, interval<U> const& b ) const
		{
			return total_less_or_equal( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( U const a, interval<T> const& b ) const
		{
			return total_less_or_equal( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, U const& b ) const
		{
			return total_less_or_equal( a, b ) ;
		}
	} ;


	template < >
	struct less_or_equal < order::Weak >
	{
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, interval<U> const& b ) const
		{
			return weak_less_or_equal( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( U const& a, interval<T> const& b ) const
		{
			return weak_less_or_equal( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, U const& b) const
		{
			return weak_less_or_equal( a, b ) ;
		}
	} ;


	template < >
	struct less_or_equal < order::Partial >
	{
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, interval<U> const& b ) const
		{
			return partial_less_or_equal( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( U const& a, interval<T> const& b ) const
		{
			return partial_less_or_equal( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, U const& b ) const
		{
			return partial_less_or_equal( a, b ) ;
		}
	} ;


	template < >
	struct less_or_equal < order::Interval >
	{
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, interval<T> const& b ) const
		{
			return interval_less_or_equal( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( U const& a, interval<T> const& b ) const
		{
			return interval_less_or_equal( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, U const& b ) const
		{
			return interval_less_or_equal( a, b ) ;
		}
	} ;


	template < order = order::Total >
	struct greater
	{
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, interval<U> const& b ) const
		{
			return total_greater( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( U const& a, interval<T> const& b ) const
		{
			return total_greater( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, U const& b ) const
		{
			return total_greater( a, b ) ;
		}
	} ;


	template < >
	struct greater < order::Weak >
	{
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, interval<U> const& b ) const
		{
			return weak_greater( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( U const& a, interval<T> const& b ) const
		{
			return weak_greater( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, U const& b ) const
		{
			return weak_greater( a, b ) ;
		}
	} ;


	template < >
	struct greater < order::Partial >
	{
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, interval<U> const& b ) const
		{
			return partial_greater( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( U const& a, interval<T> const& b ) const
		{
			return partial_greater( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, U const& b ) const
		{
			return partial_greater( a, b ) ;
		}
	} ;


	template < >
	struct greater < order::Interval >
	{
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, interval<U> const& b ) const
		{
			return interval_greater( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( U const& a, interval<T> const& b ) const
		{
			return interval_greater( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, U const& b ) const
		{
			return interval_greater( a, b ) ;
		}
	} ;

	template < order = order::Total >
	struct greater_or_equal
	{
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, interval<U> const& b ) const
		{
			return total_greater_or_equal( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( U const a, interval<T> const& b ) const
		{
			return total_greater_or_equal( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, U const& b ) const
		{
			return total_greater_or_equal( a, b ) ;
		}
	} ;


	template < >
	struct greater_or_equal < order::Weak >
	{
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, interval<U> const& b ) const
		{
			return weak_greater_or_equal( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( U const& a, interval<T> const& b ) const
		{
			return weak_greater_or_equal( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, U const& b) const
		{
			return weak_greater_or_equal( a, b ) ;
		}
	} ;


	template < >
	struct greater_or_equal < order::Partial >
	{
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, interval<U> const& b ) const
		{
			return partial_greater_or_equal( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( U const& a, interval<T> const& b ) const
		{
			return partial_greater_or_equal( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, U const& b ) const
		{
			return partial_greater_or_equal( a, b ) ;
		}
	} ;


	template < >
	struct greater_or_equal < order::Interval >
	{
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, interval<T> const& b ) const
		{
			return interval_greater_or_equal( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( U const& a, interval<T> const& b ) const
		{
			return interval_greater_or_equal( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, U const& b ) const
		{
			return interval_greater_or_equal( a, b ) ;
		}
	} ;


	template < order = order::Total >
	struct equal
	{
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, interval<U> const& b ) const
		{
			return total_equal( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( U const& a, interval<T> const& b ) const
		{
			return total_equal( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, U const& b ) const
		{
			return total_equal( a, b ) ;
		}
	} ;
	template < >
	struct equal < order::Weak >
	{
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, interval<U> const& b ) const
		{
			return weak_equal( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( U const& a, interval<T> const& b ) const
		{
			return weak_equal( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, U const& b ) const
		{
			return weak_equal( a, b ) ;
		}
	} ;
	template < order = order::Partial>
	struct undorderesd
	{
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, interval<U> const& b ) const
		{
			return partial_unordered( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( U const&, interval<T> const& ) const
		{
			return false ;
		}
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const&, U const& ) const
		{
			return false ;
		}
	} ;
	template < >
	struct undorderesd < order::Interval >
	{
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& a, interval<U> const& b ) const
		{
			return interval_unordered( a, b ) ;
		}
		template < typename T, typename U >
		constexpr bool operator()( U const&, interval<T> const& ) const
		{
			return false ;
		}
		template < typename T, typename U >
		constexpr bool operator()( interval<T> const& , U const& ) const
		{
			return false ;
		}
	} ;


	/*  is contain  */

	template < typename T >
	bool interval<T>::is_contain( interval<T> const& x ) const
	{
		return ( this->relational( x ) == interval_relation::contain ) ;
	}

	template < typename T >
	bool interval<T>::is_contain( T const& x ) const
	{
		return ( this->lower() < x && this->upper() > x ) ;
	}

	/*  is part of  */

	template < typename T >
	bool interval<T>::is_part_of( interval const& x ) const
	{
		return ( this->relational( x ) == interval_relation::part_of ) ;
	}


	/*  This function returns two interval subparts just split middle point as vector<interval<T>> */
	template < class T >
	std::pair< interval<T>, interval<T> >  subpart( interval<T>& x )
	{
		return std::make_pair( interval<T>( x.lower(), x.mid() ), interval<T>( x.mid(), x.upper() ) ) ;
	}

	template <class InputRange, class BinaryFunction>
	void adjacent_for_each( InputRange&& range, BinaryFunction f )
	{
		using std::begin ;
		using std::end ;

		auto const& first = begin( range ) ;
		auto const& last = end( range ) ;

		if ( first == last )
			return ;

		while ( std::next(first) != last )
		{
			auto const& a = *first ;
			++first ;
			auto const& b = *first ;
			f( a, b ) ;
		}
	}


}//end namespace Cranberries

#endif //!CRANBERRIES_INTERVAL