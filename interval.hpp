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
		Version1_1_1,
		Version2_0_0 = 0x02000000,
		Version3_0_0 = 0x03000000,
		now_ver = Version1_1_1,
	};

	/*  function default for meta programming  */
	extern std::nullptr_t enabler ;

	/*  PI  */
	template < typename T >
	constexpr T PI() { return static_cast<T>(3.141592653589793238462643383279); }


	/*  maximum  */
	template < typename T >
	constexpr T max() { return std::numeric_limits<T>::max(); }

	/*  Zero  */
	template < typename T >
	constexpr T zero() { return static_cast<T>(0.0); }

	/*  One  */
	template < typename T >
	constexpr T one() { return static_cast<T>(1.0); }

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
		interval operator=( interval const& ) ;
		/*  Move assignment Op  */
		interval operator=( interval&& ) ;

		interval operator=( std::initializer_list<T> ) ;

		/*  compound assignment Op  */
		const interval operator +=( const interval& ) ;
		const interval operator -=( const interval& ) ;
		const interval operator *=( const interval& ) ;
		const interval operator /=( const interval& ) ;

		const interval operator +=( T&& ) ;
		const interval operator -=( T&& ) ;
		const interval operator *=( T&& ) ;
		const interval operator /=( T&& ) ;

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
		constexpr T low() const ;
		constexpr T up() const ;
		// setter
		void set_up( T const& ) ;
		void set_low( T const& ) ;
		void set_up( T&& ) ;
		void set_low( T&& ) ;
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
		/*  Compound Assignment Op Implementation */
		const impl operator +=( const impl& ) ;
		const impl operator -=( const impl& ) ;
		const impl operator *=( const impl& ) ;
		const impl operator /=( const impl& ) ;

		/*  Increment/decrement Op Implementation */
		const impl operator ++() ;
		const impl operator ++(int) ;
		const impl operator --() ;
		const impl operator --(int) ;

		/*  getter Implementation  */
		constexpr T low() const ;
		constexpr T up() const ;

		/*  Output Stream Function  */
		std::ostream& print( std::ostream& ) const ;

		/*  setter Implementation */
		void set_low( T const& x )
		{
			if ( x > upper_bound ) {
				throw invalid_argument( "lower_bound must be less than upper_bound!" ) ;
			}
			lower_bound = x ;
		}
		void set_low( T&& x )
		{
			if ( x > upper_bound ) {
				throw invalid_argument( "lower_bound must be less than upper_bound!" ) ;
			}
			lower_bound = std::move( x ) ;
		}
		void set_up( T const& x )
		{
			if ( x < lower_bound ) {
				throw invalid_argument( "upper_bound must be greater than lower_bound!" ) ;
			}
			upper_bound = x ;
		}
		void set_up( T&& x )
		{
			if ( x < lower_bound ) {
				throw invalid_argument( "upper_bound must be greater than lower_bound!" ) ;
			}
			upper_bound = std::move( x ) ;
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
	/*                              */
	/*  Interval Member Functions   */
	/*                              */
	//------------------------------//


	/*  Interval Compound Assignment Op Definition  */

	//  addition assignment
	template < typename T >
	const interval<T> interval<T>::operator +=( const interval& x )
	{
		pimpl->operator+=( *( x.pimpl ) ) ;
		if ( this->up() == max<T>() || this->low() == -max<T>() ) 
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return *this ;
	}

	//  subtranction sssignment operator
	template < typename T >
	const interval<T> interval<T>::operator -=( const interval& x )
	{
		pimpl->operator-=( *( x.pimpl ) ) ;
		if ( this->up() == max<T>() || this->low() == -max<T>() )
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return *this ;
	}

	//  multiprecation assignmetn operator
	template < typename T >
	const interval<T> interval<T>::operator *=( const interval& x )
	{
		pimpl->operator*=( *( x.pimpl ) ) ;
		if ( this->up() == max<T>() || this->low() == -max<T>() )
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return *this ;
	}

	//  division assignment operator
	template < typename T >
	const interval<T> interval<T>::operator /=( const interval& x )
	{
		pimpl->operator/=( *( x.pimpl ) ) ;
		if ( this->up() == max<T>() || this->low() == -max<T>() )
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return *this ;
	}

	//  addition assignment operator(T&&)
	template< typename T >
	const interval<T> interval<T>::operator +=( T&& x )
	{
		this->set_low( downward( this->low() + std::forward<T>( x ) ) ) ;
		this->set_up( upward( this->up() + std::forward<T>( x ) ) ) ;
		if ( this->up() == max<T>() || this->low() == -max<T>() )
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return *this ;
	}

	//  subtraction assignment operator(T&&)
	template< typename T >
	const interval<T> interval<T>::operator -=( T&& x )
	{
		this->set_low( downward( this->low() - std::forward<T>( x ) ) ) ;
		this->set_up( upward( this->up() - std::forward<T>( x ) ) ) ;
		if ( this->up() == max<T>() || this->low() == -max<T>() )
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return *this ;
	}

	//  multiprecation assignement operator(T&&)
	template< typename T >
	const interval<T> interval<T>::operator *=( T&& x )
	{
		if ( x < zero<T>() )
		{
			this->set_low( downward( this->up() * std::forward<T>( x ) ) ) ;
			this->set_up( upward( this->low() * std::forward<T>( x ) ) ) ;
		}
		else {
			this->set_low( downward( this->low() * std::forward<T>( x ) ) ) ;
			this->set_up( upward( this->up() * std::forward<T>( x ) ) ) ;
		}
		if ( this->up() == max<T>() || this->low() == -max<T>() )
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return *this ;
	}

	//  devisoio assignement operator(T&&)
	template< typename T >
	const interval<T> interval<T>::operator /=( T&& x )
	{
		if ( x < zero<T>() )
		{
			this->set_low( downward( this->up() / std::forward<T>( x ) ) ) ;
			this->set_up( upward( this->low() / std::forward<T>( x ) ) ) ;
		}
		else {
			this->set_low( downward( this->low() / std::forward<T>( x ) ) ) ;
			this->set_up( upward( this->up() / std::forward<T>( x ) ) ) ;
		}
		if ( this->up() == max<T>() || this->low() == -max<T>() )
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return *this ;
	}


	/*  Interval Increment/Decrement Operator Definition  */

	/*  Interval Increment Operator  */

	//  prefix increment
	template < typename T >
	const interval<T> interval<T>::operator ++()
	{
		++( *pimpl ) ;
		if ( this->up() == max<T>() || this->low() == -max<T>() )
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
		if ( this->up() == max<T>() || this->low() == -max<T>() )
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
		if ( this->up() == max<T>() || this->low() == -max<T>() )
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
		if ( this->up() == max<T>() || this->low() == -max<T>() )
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return tmp ;
	}



	//--------------------------------------------//
	//							                  //
	/*    Interval Numeric Function Definition    */
	//                                            //
	//--------------------------------------------//


	/*  interval cosine  */

	template < typename T >
	const interval<T> interval<T>::cos() const
	{
		int c ;
		auto a = pimpl->low() ;
		auto b = pimpl->up() ;

		if ( b - a >= static_cast<T>( 2.0 ) * PI<T>() )
		{
			return interval<T>{ -one<T>(), one<T>() } ;
		}
		/*  base point set  */
		if ( static_cast<int>( upward( ( a / static_cast<T>( 2.0 ) / PI<T>() ) ) ) > a )
		{
			c = static_cast<int> ( upward( ( a / static_cast<T>( 2.0 ) / PI<T>() ) ) ) ;
		}
		else {
			c = static_cast<int>( upward( ( a / static_cast<T>( 2.0 ) / PI<T>() ) ) ) + 1 ;
		}
		/*  checking phase  */
		if ( upward( PI<T>() * static_cast<T>( 2.0 ) * c ) >= a &&
			 downward( PI<T>() * static_cast<T>( 2.0 ) * c ) <= b )
		{
			if ( upward( PI<T>() * ( one<T>() + static_cast<T>( 2.0 ) * c ) ) >= a &&
				 downward( PI<T>() * (one<T>() + static_cast<T>( 2.0 ) * c ) ) <= b )
			{
				return interval<T>{ -one<T>(), one<T>() } ;
			}
			else if ( upward( ( c * static_cast<T>( 2.0 ) - one<T>() ) * PI<T>() ) >= a &&
					  downward( ( c * static_cast<T>( 2.0 ) - one<T>() ) * PI<T>() ) <= b )
			{
				return interval<T>{ -one<T>(), one<T>() } ;
			}
			else
			{
				return interval<T>{ std::fmin( downward( std::cos( a ) ), upward( std::cos( b ) ) ), one<T>() } ;
			}
		}
		else if ( upward( PI<T>() * ( static_cast<T>( 2.0 ) * c + one<T>() ) ) >= a &&
				  downward( PI<T>() * ( static_cast<T>( 2.0 ) * c + one<T>() ) ) <= b )
		{
			return interval<T>( -one<T>(), std::fmax( upward( std::cos( a ) ),	upward( std::cos( b ) ) ) ) ;
		}
		else if ( upward( ( c * static_cast<T>( 2.0 ) - one<T>() ) * PI<T>() ) >= a &&
				  downward( ( c * static_cast<T>( 2.0 ) - one<T>() ) * PI<T>() ) <= b )
		{
			return interval<T>( -one<T>(), std::fmax( upward( std::cos( a ) ), upward( std::cos( b ) ) ) ) ;
		}
		else {
			return interval<T>{ std::fmin( downward( std::cos( a ) ), downward( std::cos( b ) ) ), std::fmax( upward( std::cos( a ) ), upward( std::cos( b ) ) ) };
		}
	}


	/*  interval sine  */

	template < typename T >
	const interval<T> interval<T>::sin() const
	{
		int c ;
		auto a = pimpl->low() ;
		auto b = pimpl->up() ;

		if ( b - a >= static_cast<T>( 2.0 ) * PI<T>() )
		{
			return interval<T>{ -one<T>(), one<T>() };
		}
		/*  base point set  */
		if ( static_cast<int>( upward( ( a * static_cast<T>( 2.0 ) / PI<T>() - one<T>() ) / static_cast<T>( 4.0 ) ) ) > a ) {
			c = static_cast<int>( downward( ( a * static_cast<T>( 2.0 ) / PI<T>() - one<T>() ) / static_cast<T>( 4.0 ) ) ) ;
		}
		else{
			c = static_cast<int>( upward( ( a * static_cast<T>( 2.0 ) / PI<T>() - one<T>() ) / static_cast<T>( 4.0 ) ) ) + 1 ;
		}
		/*  checking phase  */
		if ( upward( PI<T>() / static_cast<T>( 2.0 ) * ( one<T>() + c * static_cast<T>( 4.0 ) ) ) >= a &&
			 downward( PI<T>() / static_cast<T>( 2.0 ) * ( one<T>() + c * static_cast<T>( 4.0 ) ) ) <= b )
		{
			if ( upward( PI<T>() / static_cast<T>( 2.0 ) * ( static_cast<T>( 3.0 ) + c * static_cast<T>( 4.0 ) ) ) >= a &&
				 downward( PI<T>() / static_cast<T>( 2.0 ) * ( static_cast<T>( 3.0 ) + c * static_cast<T>( 4.0 ) ) ) <= b )
			{
				return interval<T>{ -one<T>(), one<T>() } ;
			}
			else if ( upward( ( c * static_cast<T>( 4.0 ) - one<T>() ) * PI<T>() / static_cast<T>( 2.0 ) ) >= a &&
					  downward( ( c * static_cast<T>( 4.0 ) - one<T>() ) * PI<T>() / static_cast<T>( 2.0 ) ) <= b )
			{
				return interval<T>{ -one<T>(), one<T>() } ;
			}
			else
			{
				return interval<T>{ std::fmin( downward( std::sin( a ) ), downward( std::sin( b ) ) ), one<T>() } ;
			}
		}
		else if ( upward( PI<T>() / static_cast<T>( 2.0 ) * ( static_cast<T>( 3.0 ) + c * static_cast<T>( 4.0 ) ) ) >= a &&
				  downward( PI<T>() / static_cast<T>( 2.0 ) * ( static_cast<T>( 3.0 ) + c * static_cast<T>( 4.0 ) ) ) <= b )
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
		auto a = pimpl->low() ;
		auto b = pimpl->up() ;

		if ( b - a >= PI<T>() )
		{
			throw Cranberries::over_flow( "tan" ) ;
		}
		/*  base point set  */
		if ( static_cast<int>( upward( a * static_cast<T>( 2.0 ) / PI<T>() ) ) < a )
		{
			c = static_cast<int>( upward( a * static_cast<T>( 2.0 ) / PI<T>() ) );
		}
		else {
			c = static_cast<int>( upward( a * static_cast<T>( 2.0 ) / PI<T>() ) ) + 1 ;
		}
		/*  checking phase  */
		if ( a < PI<T>() / static_cast<T>( static_cast<T>( 2.0 ) ) * c && 
			 PI<T>() / static_cast<T>( static_cast<T>( 2.0 ) ) * c < b )
		{
			throw Cranberries::over_flow( "tan" ) ;
		}
		else if ( a < PI<T>() / static_cast<T>( static_cast<T>( 2.0 ) ) * c - PI<T>() &&
				  PI<T>() / static_cast<T>( static_cast<T>( 2.0 ) ) * c - PI<T>() < b )
		{
			throw Cranberries::over_flow( "tan" ) ;
		}
		else if ( a < PI<T>() / static_cast<T>( static_cast<T>( 2.0 ) ) * c + PI<T>() &&
				  PI<T>() / static_cast<T>( static_cast<T>( 2.0 ) ) * c + PI<T>() < b )
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
		auto a = pimpl->low() ;
		auto b = pimpl->up() ;

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
		auto a = pimpl->low() ;
		auto b = pimpl->up() ;

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
		auto a = pimpl->low() ;
		auto b = pimpl->up() ;

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
		auto a = pimpl->low() ;
		auto b = pimpl->up() ;

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
		auto a = pimpl->low() ;
		auto b = pimpl->up() ;

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
		return interval<T>{ downward( std::tanh( pimpl->low() ) ), upward( std::tanh( pimpl->up() ) ) } ;
	}


	/*  interval arc hyperbolic cosine  */

	template < typename T >
	const interval<T> interval<T>::acosh() const
	{
		auto a = pimpl->low() ;
		auto b = pimpl->up() ;

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
		return interval<T>{ downward( std::asinh( pimpl->low() ) ), upward( std::asinh( pimpl->up() ) ) } ;
	}

	/*  interval arc hyperbolic tangent  */

	template < typename T >
	const interval<T> interval<T>::atanh() const
	{
		return interval<T>{ downward( std::atanh( pimpl->low() ) ), upward( std::atanh( pimpl->up() ) ) } ;
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
			if ( pimpl->up() <= zero<T>() )
			{
				if ( upward( std::pow( pimpl->low(), n ) ) == max<T>() ||
					 downward( std::pow( pimpl->up(), n ) ) == -max<T>() )
				{
					throw Cranberries::over_flow( "overflow occurred." ) ;
				}
				return interval<T>{ downward( std::pow( pimpl->up(), n ) ),	upward( std::pow( pimpl->low(), n ) ) } ;
			}
			else if ( pimpl->low() <= zero<T>() && pimpl->up() >= zero<T>() )
			{
				if ( std::fmax( downward( std::pow( pimpl->low(), n ) ), upward( std::pow( pimpl->up(), n ) ) ) == max<T>() )
				{
					throw Cranberries::over_flow( "overflow occurred." ) ;
				}
				return interval<T>{ zero<T>(), std::fmax( downward( std::pow( pimpl->low(), n ) ), upward( std::pow( pimpl->up(), n ) ) ) } ;
			}
		}
		if ( upward( std::pow( pimpl->up(), n ) ) == max<T>() ||
			 downward( std::pow( pimpl->low(), n ) ) == max<T>() )
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return interval<T>{ downward( std::pow( pimpl->low(), n ) ), upward( std::pow( pimpl->up(), n ) ) };
	}


	/*  interval square root function  */

	template < typename T >
	const interval<T> interval<T>::sqrt() const
	{
		if ( pimpl->low() < zero<T>() )
		{
			throw Cranberries::logic_error( "sqrt arg requires positive number" ) ;
		}
		return interval<T>{ downward( std::sqrt( pimpl->low() ) ), upward( std::sqrt( pimpl->up() ) ) } ;
	}


	/*  interval exponential function ( base = e )  */

	template < typename T >
	const interval<T> interval<T>::exp() const
	{
		if ( upward( std::exp( pimpl->up() ) ) == max<T>() || downward( std::exp( pimpl->low() ) ) == -max<T>() )
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return interval<T>{ downward( std::exp( pimpl->low() ) ), upward( std::exp( pimpl->up() ) ) } ;
	}

	/*  interval exponential function ( base = 2 )  */

	template < typename T >
	const interval<T> interval<T>::exp2() const
	{
		if ( upward( std::exp2( pimpl->up() ) ) == max<T>() || downward( std::exp2( pimpl->low() ) ) == -max<T>() )
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return interval<T>{ downward( std::exp2( pimpl->low() ) ), upward( std::exp2( pimpl->up() ) ) } ;
	}

	/*  interval exp( x ) - 1  */

	template < typename T >
	const interval<T> interval<T>::expm1() const
	{
		if ( upward( std::expm1( pimpl->up() ) ) == max<T>() || downward( std::expm1( pimpl->low() ) ) == -max<T>() )
		{
			throw Cranberries::over_flow( "overflow occurred." ) ;
		}
		return interval<T>{ downward( std::expm1( pimpl->low() ) ), upward( std::expm1( pimpl->up() ) ) } ;
	}


	/*  interval logarithmic function ( base = e ) */

	template < typename T >
	const interval<T> interval<T>::log() const
	{
		if ( pimpl->low() <= zero<T>() )
		{
			throw Cranberries::logic_error( "anti-logarithm less than or equal to zero" ) ;
		}
		return interval<T>{ downward( std::log( pimpl->low() ) ), upward( std::log( pimpl->up() ) ) } ;
	}

	/*  interval log( x ) + 1  */
	template < typename T >
	const interval<T> interval<T>::log1p() const
	{
		if ( pimpl->low() <= zero<T>() ) { throw Cranberries::logic_error( "anti-logarithm less than or equal to zero" ) ; }
		return interval<T>{ downward( std::log1p( pimpl->low() ) ),	upward( std::log1p( pimpl->up() ) ) } ;
	}


	/*  interval logarithmic function ( base = 10 )  */

	template < typename T >
	const interval<T> interval<T>::log10() const
	{
		if ( pimpl->low() <= zero<T>() ) { throw Cranberries::logic_error( "anti-logarithm less than or equal to zero" ) ; }
		return interval<T>{ downward( std::log10( pimpl->low() ) ), upward( std::log10( pimpl->up() ) ) } ;
	}

	/*  interval logarithmic function ( base = 2 )  */
	template < typename T >
	const interval<T> interval<T>::log2() const
	{
		if ( pimpl->low() <= zero<T>() )
		{
			throw Cranberries::logic_error( "anti-logarithm less than or equal to zero" ) ;
		}
		return interval<T>{ downward( std::log2( pimpl->low() ) ), upward( std::log2( pimpl->up() ) ) } ;
	}


	/*  interval absolute funtion  */

	template < typename T >
	const interval<T> interval<T>::abs() const
	{
		if ( pimpl->low() < zero<T>() && pimpl->up() >= zero<T>() )
		{
			return interval<T>{ zero<T>(), upward( std::fmax( std::abs( pimpl->low() ), std::abs( pimpl->up() ) ) ) } ;
		}
		if ( pimpl->up() < zero<T>() )
		{
			return interval<T>{ downward( std::abs( pimpl->up() ) ), upward( std::abs( pimpl->low() ) ) } ;
		}
		else {
			return interval<T>( *this ) ;
		}
	}

	/*  interval fused multiply-add function fma(interval<T>, interval<T>, interval<T>)  */
	template < typename T >
	const interval<T> fma( interval<T> const& x, interval<T> const& y, interval<T> const& z )
	{
		auto x_low = x.low() ;
		auto y_low = y.low() ;
		auto z_low = z.low() ;
		auto x_up = x.up() ;
		auto y_up = y.up() ;
		auto z_up = z.up() ;

		if ( &x == &y ) {
			return x.pow(2) + z ;
		}
		else if ( x_low >= zero<T>() && y_low >= zero<T>() ) {
			return interval<T>{ downward( std::fma( x_low,y_low,z_low ) ), upward( std::fma( x_up, y_up, z_up ) ) } ;
		}
		else if ( x_low >= zero<T>() && y_low < zero<T>() && y_up > zero<T>() )
		{
			return interval<T>{ downward( std::fma( x_up, y_low, z_low ) ), upward( std::fma( x_low, y_up, z_up ) ) } ;
		}
		else if ( x_low >= zero<T>() && y_up <= zero<T>() )
		{
			return interval<T>{ downward( std::fma( x_up, y_low, z_low ) ), upward( std::fma( x_low, y_up, z_up ) ) } ;
		}
		else if ( x_low < zero<T>() && x_up > zero<T>() && y_low >= zero<T>() )
		{
			return interval<T>{ downward( std::fma( x_low, y_up, z_low ) ), upward( std::fma( x_up, y_up, z_up ) ) } ;
		}
		else if ( x_low < zero<T>() && x_up > zero<T>() && y_low <= zero<T>() )
		{
			return interval<T>{ downward( std::fma( x_up, y_low, z_low ) ), upward( std::fma( x_low, y_low, z_up ) ) } ;
		}
		else if ( x_up <= zero<T>() && y_low >= zero<T>() )
		{
			return interval<T>{ downward( std::fma( x_low, y_up, z_low ) ), upward( std::fma( x_up, y_low, z_up ) ) } ;
		}
		else if ( x_up <= zero<T>() && y_low < zero<T>() && y_up > zero<T>() )
		{
			return interval<T>{ downward( std::fma( x_low, y_up, z_low ) ), upward( std::fma( x_low, y_low, z_up ) ) } ;
		}
		else if ( x_up <= zero<T>() && y_up <= zero<T>() )
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


	/*  interval fused multiply-add function fma(interval<T>, interval<T>, T)  */

	template < typename T >
	const interval<T> fma( interval<T> const& x, interval<T> const& y, T&& z )
	{
		auto x_low = x.low() ;
		auto y_low = y.low() ;
		auto x_up = x.up() ;
		auto y_up = y.up() ;

		if ( &x == &y )
		{
			return x.pow(2) + std::forward<T>(z) ;
		}
		else if ( x_low >= zero<T>() && y_low >= zero<T>() )
		{
			return interval<T>{ downward( std::fma( x_low, y_low, std::forward<T>(z) ) ), upward( std::fma( x_up, y_up, std::forward<T>(z) ) ) } ;
		}
		else if ( x_low >= zero<T>() && y_low < zero<T>() && y_up > zero<T>() )
		{
			return interval<T>{ downward( std::fma( x_up, y_low, std::forward<T>(z) ) ), upward( std::fma( x_low, y_up, std::forward<T>(z) ) ) } ;
		}
		else if ( x_low >= zero<T>() && y_up <= zero<T>() ) 
		{
			return interval<T>{ downward( std::fma( x_up, y_low, std::forward<T>(z) ) ), upward( std::fma( x_low, y_up, std::forward<T>(z) ) ) } ;
		}
		else if ( x_low < zero<T>() && x_up > zero<T>() && y_low >= zero<T>() )
		{
			return interval<T>{ downward( std::fma( x_low, y_up, std::forward<T>(z) ) ), upward( std::fma( x_up, y_up, std::forward<T>(z) ) ) } ;
		}
		else if ( x_low < zero<T>() && x_up > zero<T>() && y_low <= zero<T>() )
		{
			return interval<T>{ downward( std::fma( x_up, y_low, std::forward<T>(z) ) ), upward( std::fma( x_low, y_low, std::forward<T>(z) ) ) } ;
		}
		else if ( x_up <= zero<T>() && y_low >= zero<T>() )
		{
			return interval<T>{ downward( std::fma( x_low, y_up, std::forward<T>(z) ) ), upward( std::fma( x_up, y_low, std::forward<T>(z) ) ) } ;
		}
		else if ( x_up <= zero<T>() && y_low < zero<T>() && y_up > zero<T>() )
		{
			return interval<T>{ downward( std::fma( x_low, y_up, std::forward<T>(z) ) ), upward( std::fma( x_low, y_low, std::forward<T>(z) ) ) } ;
		}
		else if ( x_up <= zero<T>() && y_up <= zero<T>() ) 
		{
			return interval<T>{ downward( std::fma( x_up, y_up, std::forward<T>(z) ) ), upward( std::fma( x_low, y_low, std::forward<T>(z) ) ) } ;
		}
		else {
			return interval<T>{
				std::fmin( downward( std::fma( x_up, y_low, std::forward<T>(z) ) ), downward( std::fma( x_low, y_up, std::forward<T>(z) ) ) )
				, std::fmax( upward( std::fma( x_up, y_low, std::forward<T>(z) ) ), upward( std::fma( x_low, y_up, std::forward<T>(z) ) ) )
			} ;
		}
	}


	/*  interval fused multiply-add function fma(interval<T>, T, interval<T>)  */

	template < typename T >
	const interval<T> fma( interval<T> const& x, T&& y, interval<T> const& z )
	{
		if ( y < zero<T>() )
		
		{
			return interval<T>{ downward( std::fma( x.up(), std::forward<T>( y ), z.low() ) ), upward( std::fma( x.low(), std::forward<T>( y ), z.up() ) ) } ;
		}
		else {
			return interval<T>{ downward( std::fma( x.low(), std::forward<T>( y ), z.low() ) ), upward( std::fma( x.up(), std::forward<T>( y ), z.up() ) ) } ;
		}
	}


	/*  interval fused multiply-add function fma(T, interval<T>, interval<T>)  */

	template < typename T >
	const interval<T> fma( T&& x, interval<T> const& y, interval<T> const& z)
	{
		if ( x < zero<T>() ) {
			return interval<T>{ downward( std::fma( std::forward<T>( x ), y.up(), z.low() ) ), upward( std::fma( std::forward<T>( x ), y.low(), z.up() ) ) } ;
		}
		else {
			return interval<T>{ downward( std::fma( std::forward<T>( x ), y.low(), z.low() ) ), upward( std::fma( std::forward<T>( x ), y.up(), z.up() ) ) } ;
		}
	}


	/*  interval fused multiply-add function fma(interval<T>, T, T)  */

	template < typename T >
	const interval<T> fma( interval<T> const& x, T&& y, T&& z )
	{
		if ( y < zero<T>() ) {
			return interval<T>{ downward( std::fma( x.up(), std::forward<T>( y ), std::forward<T>(z) ) ), upward( std::fma( x.low(), std::forward<T>( y ), std::forward<T>(z) ) ) } ;
		}
		else {
			return interval<T>{ downward( std::fma( x.low(), std::forward<T>( y ), std::forward<T>(z) ) ), upward( std::fma( x.up(), std::forward<T>( y ), std::forward<T>(z) ) ) } ;
		}
	}


	/*  interval fused multiply-add function fma(T, interval<T>, T)  */

	template < typename T >
	const interval<T> fma( T&& x, interval<T> const& y, T&& z )
	{
		if ( x < zero<T>() ) {
			return interval<T>{ downward( std::fma( std::forward<T>( x ), y.up(), std::forward<T>(z) ) ), upward( std::fma( std::forward<T>( x ), y.low(), std::forward<T>(z) ) ) } ;
		}
		else {
			return interval<T>{ downward( std::fma( std::forward<T>( x ), y.low(), std::forward<T>(z) ) ), upward( std::fma( std::forward<T>( x ), y.up(), std::forward<T>(z) ) ) } ;
		}
	}


	/*  interval fused multiply-add function fma(T, T, interval<T>)  */

	template < typename T >
	const interval<T> fma( T&& x, T&& y, interval<T> const& z )
	{
			return interval<T>{ downward( std::fma( std::forward<T>( x ), std::forward<T>( y ), z.low() ) ), upward( std::fma( std::forward<T>( x ), std::forward<T>( y ), z.up() ) ) } ;
	}


	/*  interval error function  */

	template < typename T >
	const interval<T> interval<T>::erf() const
	{
		return interval<T>{ downward( std::erf( pimpl->low() ) ), upward( std::erf( pimpl->up() ) ) } ;
	}


	/*  interval complementary error function  */

	template < typename T >
	const interval<T> interval<T>::erfc() const
	{
		return interval<T>{ downward( std::erfc( pimpl->up() ) ), upward( std::erfc( pimpl->low() ) ) } ;
	}


	/*  is_singleton  */

	template < typename T >
	constexpr bool interval<T>::is_singleton() const
	{
		return ( pimpl->low() == pimpl->up() ) ? true : false ;
	}


	/*  middle point  */

	template < typename T >
	constexpr T interval<T>::mid() const
	{
		return ( pimpl->up() + pimpl->low() ) / static_cast<T>( 2.0 ) ;
	}


	/*  width  */

	template < typename T >
	constexpr T interval<T>::wid() const
	{
		return pimpl->up() - pimpl->low() ;
	}


	/*  Interval Accessors  */

	template < typename T >
	constexpr T interval<T>::low() const
	{
		return pimpl->low() ;
	}

	template < typename T >
	constexpr T interval<T>::up() const
	{
		return pimpl->up() ;
	}

	template < typename T >
	void interval<T>::set_up( T const& x )
	{
		pimpl->set_up( x ) ;
	}

	template < typename T >
	void interval<T>::set_up( T&& x )
	{
		pimpl->set_up( x ) ;
	}

	template < typename T >
	void interval<T>::set_low( T const& x )
	{
		pimpl->set_low( x ) ;
	}

	template < typename T >
	void interval<T>::set_low( T&& x )
	{
		pimpl->set_low( x ) ;
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
		else if ( pimpl->low() == ( x.pimpl )->low() && pimpl->up() < ( x.pimpl )->up() )
		{
			return interval_relation::total_less ;
		}
		else if ( pimpl->low() == ( x.pimpl )->low() && pimpl->up() > ( x.pimpl )->up() )
		{
			return interval_relation::total_greater ;
		}
		else if ( pimpl->low() < ( x.pimpl )->low() && pimpl->up() == ( x.pimpl )->up() )
		{
			return interval_relation::weak_less ;
		}
		else if ( pimpl->low() > ( x.pimpl )->low() && pimpl->up() == ( x.pimpl )->up() )
		{
			return interval_relation::weak_greater ;
		}
		else if ( pimpl->low() < ( x.pimpl )->low() && pimpl->up() < ( x.pimpl )->up() )
		{
			return interval_relation::partial_less ;
		}
		else if ( pimpl->low() > ( x.pimpl )->low() && pimpl->up() > ( x.pimpl )->up() )
		{
			return interval_relation::partial_less ;
		}
		else if ( pimpl->low() <= ( x.pimpl )->low() && pimpl->up() >= ( x.pimpl )->up() )
		{
			return interval_relation::contain ;
		}
		else if ( pimpl->low() >= ( x.pimpl )->low() && pimpl->up() <= ( x.pimpl )->up() )
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
	Its are total ordering, weak ordering, partial ordering and interval ordering.
	Default ordering is total ordering. - end note ]
	*/

	/*
	[ Example : Let interval<T> a,b ;
	"a < b" equals "total_less( a, b )". - end example ]
	*/

	/*
	[ Note : Ordering Policy can switch	using namespace inside a scape block.
	Three namespace of weak_ordering_policy, partial_ordering and interval_ordering_policy are defined. - end note ]
	*/

	/*
	[ Example : 
	auto x = hull( 1.0, 2.0 );
	auto y = hull( 1.2, 1.8 );
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


	template < typename T, typename U >
	bool operator<( interval<T> const& x, U&& y )
	{
		return total_less( x, y ) ;
	}

	template < typename T, typename U >
	bool operator>( interval<T> const& x, U&& y )
	{
		return total_greater( x, y ) ;
	}

	template < typename T, typename U >
	bool operator<=( interval<T> const& x, U&& y )
	{
		return total_less_or_equal( x, y ) ;
	}

	template < typename T, typename U >
	bool operator>=( interval<T> const& x, U&& y )
	{
		return total_greater_or_equal( x, y ) ;
	}

	template < typename T, typename U >
	bool operator==( interval<T> const& x, U&& y )
	{
		return total_equal( x, y ) ;
	}

	template < typename T, typename U >
	bool operator!=( interval<T> const& x, U&& y )
	{
		return !total_equal( x, y ) ;
	}

	namespace interval_ordering_policy
	{
		template < typename T >
		const bool operator <( interval<T>& x, interval<T>& y )
		{
			return interval_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T>& x, interval<T> const& y )
		{
			return interval_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T>& x, interval<T>&& y )
		{
			return interval_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T>& x, interval<T> const&& y )
		{
			return interval_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T> const& x, interval<T>& y )
		{
			return interval_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T> const& x, interval<T> const& y )
		{
			return interval_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T> const& x, interval<T>&& y )
		{
			return interval_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T> const& x, interval<T> const&& y )
		{
			return interval_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T>&& x, interval<T>& y )
		{
			return interval_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T>&& x, interval<T> const& y )
		{
			return interval_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T>&& x, interval<T>&& y )
		{
			return interval_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T>&& x, interval<T> const&& y )
		{
			return interval_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T> const&& x, interval<T>& y )
		{
			return interval_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T> const&& x, interval<T> const& y )
		{
			return interval_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T> const&& x, interval<T>&& y )
		{
			return interval_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T> const&& x, interval<T> const&& y )
		{
			return interval_less( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T>& x, interval<T>& y )
		{
			return interval_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T>& x, interval<T> const& y )
		{
			return interval_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T>& x, interval<T>&& y )
		{
			return interval_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T>& x, interval<T> const&& y )
		{
			return interval_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T> const& x, interval<T>& y )
		{
			return interval_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T> const& x, interval<T> const& y )
		{
			return interval_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T> const& x, interval<T>&& y )
		{
			return interval_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T> const& x, interval<T> const&& y )
		{
			return interval_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T>&& x, interval<T>& y )
		{
			return interval_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T>&& x, interval<T> const& y )
		{
			return interval_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T>&& x, interval<T>&& y )
		{
			return interval_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T>&& x, interval<T> const&& y )
		{
			return interval_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T> const&& x, interval<T>& y )
		{
			return interval_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T> const&& x, interval<T> const& y )
		{
			return interval_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T> const&& x, interval<T>&& y )
		{
			return interval_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T> const&& x, interval<T> const&& y )
		{
			return interval_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T>& x, interval<T>& y )
		{
			return interval_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T>& x, interval<T> const& y )
		{
			return interval_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T>& x, interval<T>&& y )
		{
			return interval_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T>& x, interval<T> const&& y )
		{
			return interval_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T> const& x, interval<T>& y )
		{
			return interval_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T> const& x, interval<T> const& y )
		{
			return interval_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T> const& x, interval<T>&& y )
		{
			return interval_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T> const& x, interval<T> const&& y )
		{
			return interval_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T>&& x, interval<T>& y )
		{
			return interval_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T>&& x, interval<T> const& y )
		{
			return interval_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T>&& x, interval<T>&& y )
		{
			return interval_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T>&& x, interval<T> const&& y )
		{
			return interval_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T> const&& x, interval<T>& y )
		{
			return interval_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T> const&& x, interval<T> const& y )
		{
			return interval_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T> const&& x, interval<T>&& y )
		{
			return interval_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T> const&& x, interval<T> const&& y )
		{
			return interval_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T>& x, interval<T>& y )
		{
			return interval_greater_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T>& x, interval<T> const& y )
		{
			return interval_greater_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T>& x, interval<T>&& y )
		{
			return interval_greater_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T>& x, interval<T> const&& y )
		{
			return interval_greater_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T> const& x, interval<T>& y )
		{
			return interval_greater_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T> const& x, interval<T> const& y )
		{
			return interval_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const& x, interval<T>&& y )
		{
			return interval_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const& x, interval<T> const&& y )
		{
			return interval_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T>&& x, interval<T>& y )
		{
			return interval_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T>&& x, interval<T> const& y )
		{
			return interval_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T>&& x, interval<T>&& y )
		{
			return interval_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T>&& x, interval<T> const&& y )
		{
			return interval_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const&& x, interval<T>& y )
		{
			return interval_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const&& x, interval<T> const& y )
		{
			return interval_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const&& x, interval<T>&& y )
		{
			return interval_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const&& x, interval<T> const&& y )
		{
			return interval_greater_or_equal( x, y ) ;
		}
	}

	namespace partial_ordering_policy
	{
		template < typename T >
		const bool operator <( interval<T>& x, interval<T>& y )
		{
			return partial_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T>& x, interval<T> const& y )
		{
			return partial_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T>& x, interval<T>&& y )
		{
			return partial_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T>& x, interval<T> const&& y )
		{
			return partial_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T> const& x, interval<T>& y )
		{
			return partial_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T> const& x, interval<T> const& y )
		{
			return partial_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T> const& x, interval<T>&& y )
		{
			return partial_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T> const& x, interval<T> const&& y )
		{
			return partial_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T>&& x, interval<T>& y )
		{
			return partial_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T>&& x, interval<T> const& y )
		{
			return partial_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T>&& x, interval<T>&& y )
		{
			return partial_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T>&& x, interval<T> const&& y )
		{
			return partial_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T> const&& x, interval<T>& y )
		{
			return partial_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T> const&& x, interval<T> const& y )
		{
			return partial_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T> const&& x, interval<T>&& y )
		{
			return partial_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T> const&& x, interval<T> const&& y )
		{
			return partial_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T>& x, interval<T>& y )
		{
			return partial_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T>& x, interval<T> const& y )
		{
			return partial_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T>& x, interval<T>&& y )
		{
			return partial_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T>& x, interval<T> const&& y )
		{
			return partial_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T> const& x, interval<T>& y )
		{
			return partial_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T> const& x, interval<T> const& y )
		{
			return partial_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T> const& x, interval<T>&& y )
		{
			return partial_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T> const& x, interval<T> const&& y )
		{
			return partial_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T>&& x, interval<T>& y )
		{
			return partial_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T>&& x, interval<T> const& y )
		{
			return partial_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T>&& x, interval<T>&& y )
		{
			return partial_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T>&& x, interval<T> const&& y )
		{
			return partial_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T> const&& x, interval<T>& y )
		{
			return partial_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T> const&& x, interval<T> const& y )
		{
			return partial_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T> const&& x, interval<T>&& y )
		{
			return partial_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T> const&& x, interval<T> const&& y )
		{
			return partial_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T>& x, interval<T>& y )
		{
			return partial_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T>& x, interval<T> const& y )
		{
			return partial_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T>& x, interval<T>&& y )
		{
			return partial_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T>& x, interval<T> const&& y )
		{
			return partial_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T> const& x, interval<T>& y )
		{
			return partial_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T> const& x, interval<T> const& y )
		{
			return partial_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T> const& x, interval<T>&& y )
		{
			return partial_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T> const& x, interval<T> const&& y )
		{
			return partial_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T>&& x, interval<T>& y )
		{
			return partial_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T>&& x, interval<T> const& y )
		{
			return partial_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T>&& x, interval<T>&& y )
		{
			return partial_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T>&& x, interval<T> const&& y )
		{
			return partial_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T> const&& x, interval<T>& y )
		{
			return partial_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T> const&& x, interval<T> const& y )
		{
			return partial_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T> const&& x, interval<T>&& y )
		{
			return partial_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T> const&& x, interval<T> const&& y )
		{
			return partial_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T>& x, interval<T>& y )
		{
			return partial_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T>& x, interval<T> const& y )
		{
			return partial_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T>& x, interval<T>&& y )
		{
			return partial_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T>& x, interval<T> const&& y )
		{
			return partial_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const& x, interval<T>& y )
		{
			return partial_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const& x, interval<T> const& y )
		{
			return partial_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const& x, interval<T>&& y )
		{
			return partial_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const& x, interval<T> const&& y )
		{
			return partial_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T>&& x, interval<T>& y )
		{
			return partial_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T>&& x, interval<T> const& y )
		{
			return partial_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T>&& x, interval<T>&& y )
		{
			return partial_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T>&& x, interval<T> const&& y )
		{
			return partial_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const&& x, interval<T>& y )
		{
			return partial_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const&& x, interval<T> const& y )
		{
			return partial_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const&& x, interval<T>&& y )
		{
			return partial_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const&& x, interval<T> const&& y )
		{
			return partial_greater_or_equal( x, y ) ;
		}
	}


	namespace weak_ordering_policy
	{
		template < typename T >
		const bool operator <( interval<T>& x, interval<T>& y )
		{
			return weak_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T>& x, interval<T> const& y )
		{
			return weak_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T>& x, interval<T>&& y )
		{
			return weak_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T>& x, interval<T> const&& y )
		{
			return weak_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T> const& x, interval<T>& y )
		{
			return weak_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T> const& x, interval<T> const& y )
		{
			return weak_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T> const& x, interval<T>&& y )
		{
			return weak_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T> const& x, interval<T> const&& y )
		{
			return weak_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T>&& x, interval<T>& y )
		{
			return weak_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T>&& x, interval<T> const& y )
		{
			return weak_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T>&& x, interval<T>&& y )
		{
			return weak_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T>&& x, interval<T> const&& y )
		{
			return weak_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T> const&& x, interval<T>& y )
		{
			return weak_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T> const&& x, interval<T> const& y )
		{
			return weak_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T> const&& x, interval<T>&& y )
		{
			return weak_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <( interval<T> const&& x, interval<T> const&& y )
		{
			return weak_less( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T>& x, interval<T>& y )
		{
			return weak_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T>& x, interval<T> const& y )
		{
			return weak_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T>& x, interval<T>&& y )
		{
			return weak_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T>& x, interval<T> const&& y )
		{
			return weak_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T> const& x, interval<T>& y )
		{
			return weak_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T> const& x, interval<T> const& y )
		{
			return weak_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T> const& x, interval<T>&& y )
		{
			return weak_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T> const& x, interval<T> const&& y )
		{
			return weak_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T>&& x, interval<T>& y )
		{
			return weak_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T>&& x, interval<T> const& y )
		{
			return weak_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T>&& x, interval<T>&& y )
		{
			return weak_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T>&& x, interval<T> const&& y )
		{
			return weak_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T> const&& x, interval<T>& y )
		{
			return weak_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T> const&& x, interval<T> const& y )
		{
			return weak_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T> const&& x, interval<T>&& y )
		{
			return weak_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator <=( interval<T> const&& x, interval<T> const&& y )
		{
			return weak_less_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T>& x, interval<T>& y )
		{
			return weak_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T>& x, interval<T> const& y )
		{
			return weak_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T>& x, interval<T>&& y )
		{
			return weak_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T>& x, interval<T> const&& y )
		{
			return weak_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T> const& x, interval<T>& y )
		{
			return weak_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T> const& x, interval<T> const& y )
		{
			return weak_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T> const& x, interval<T>&& y )
		{
			return weak_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T> const& x, interval<T> const&& y )
		{
			return weak_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T>&& x, interval<T>& y )
		{
			return weak_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T>&& x, interval<T> const& y )
		{
			return weak_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T>&& x, interval<T>&& y )
		{
			return weak_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T>&& x, interval<T> const&& y )
		{
			return weak_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T> const&& x, interval<T>& y )
		{
			return weak_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T> const&& x, interval<T> const& y )
		{
			return weak_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T> const&& x, interval<T>&& y )
		{
			return weak_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >( interval<T> const&& x, interval<T> const&& y )
		{
			return weak_greater( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T>& x, interval<T>& y )
		{
			return weak_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T>& x, interval<T> const& y )
		{
			return weak_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T>& x, interval<T>&& y )
		{
			return weak_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T>& x, interval<T> const&& y )
		{
			return weak_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const& x, interval<T>& y )
		{
			return weak_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const& x, interval<T> const& y )
		{
			return weak_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const& x, interval<T>&& y )
		{
			return weak_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const& x, interval<T> const&& y )
		{
			return weak_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T>&& x, interval<T>& y )
		{
			return weak_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T>&& x, interval<T> const& y )
		{
			return weak_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T>&& x, interval<T>&& y )
		{
			return weak_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T>&& x, interval<T> const&& y )
		{
			return weak_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const&& x, interval<T>& y )
		{
			return weak_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const&& x, interval<T> const& y )
		{
			return weak_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const&& x, interval<T>&& y )
		{
			return weak_greater_or_equal( x, y ) ;
		}
		
		template < typename T >
		const bool operator >=( interval<T> const&& x, interval<T> const&& y )
		{
			return weak_greater_or_equal( x, y ) ;
		}
	}

	namespace total_ordering_policy
	{
		template < typename T >
		const bool operator <( interval<T>& x, interval<T>& y )
		{
			return total_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T>& x, interval<T> const& y )
		{
			return total_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T>& x, interval<T>&& y )
		{
			return total_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T>& x, interval<T> const&& y )
		{
			return total_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T> const& x, interval<T>& y )
		{
			return total_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T> const& x, interval<T> const& y )
		{
			return total_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T> const& x, interval<T>&& y )
		{
			return total_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T> const& x, interval<T> const&& y )
		{
			return total_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T>&& x, interval<T>& y )
		{
			return total_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T>&& x, interval<T> const& y )
		{
			return total_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T>&& x, interval<T>&& y )
		{
			return total_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T>&& x, interval<T> const&& y )
		{
			return total_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T> const&& x, interval<T>& y )
		{
			return total_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T> const&& x, interval<T> const& y )
		{
			return total_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T> const&& x, interval<T>&& y )
		{
			return total_less( x, y ) ;
		}

		template < typename T >
		const bool operator <( interval<T> const&& x, interval<T> const&& y )
		{
			return total_less( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T>& x, interval<T>& y )
		{
			return total_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T>& x, interval<T> const& y )
		{
			return total_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T>& x, interval<T>&& y )
		{
			return total_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T>& x, interval<T> const&& y )
		{
			return total_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T> const& x, interval<T>& y )
		{
			return total_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T> const& x, interval<T> const& y )
		{
			return total_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T> const& x, interval<T>&& y )
		{
			return total_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T> const& x, interval<T> const&& y )
		{
			return total_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T>&& x, interval<T>& y )
		{
			return total_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T>&& x, interval<T> const& y )
		{
			return total_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T>&& x, interval<T>&& y )
		{
			return total_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T>&& x, interval<T> const&& y )
		{
			return total_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T> const&& x, interval<T>& y )
		{
			return total_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T> const&& x, interval<T> const& y )
		{
			return total_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T> const&& x, interval<T>&& y )
		{
			return total_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator <=( interval<T> const&& x, interval<T> const&& y )
		{
			return total_less_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T>& x, interval<T>& y )
		{
			return total_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T>& x, interval<T> const& y )
		{
			return total_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T>& x, interval<T>&& y )
		{
			return total_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T>& x, interval<T> const&& y )
		{
			return total_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T> const& x, interval<T>& y )
		{
			return total_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T> const& x, interval<T> const& y )
		{
			return total_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T> const& x, interval<T>&& y )
		{
			return total_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T> const& x, interval<T> const&& y )
		{
			return total_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T>&& x, interval<T>& y )
		{
			return total_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T>&& x, interval<T> const& y )
		{
			return total_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T>&& x, interval<T>&& y )
		{
			return total_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T>&& x, interval<T> const&& y )
		{
			return total_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T> const&& x, interval<T>& y )
		{
			return total_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T> const&& x, interval<T> const& y )
		{
			return total_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T> const&& x, interval<T>&& y )
		{
			return total_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >( interval<T> const&& x, interval<T> const&& y )
		{
			return total_greater( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T>& x, interval<T>& y )
		{
			return total_greater_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T>& x, interval<T> const& y )
		{
			return total_greater_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T>& x, interval<T>&& y )
		{
			return total_greater_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T>& x, interval<T> const&& y )
		{
			return total_greater_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T> const& x, interval<T>& y )
		{
			return total_greater_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T> const& x, interval<T> const& y )
		{
			return total_greater_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T> const& x, interval<T>&& y )
		{
			return total_greater_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T> const& x, interval<T> const&& y )
		{
			return total_greater_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T>&& x, interval<T>& y )
		{
			return total_greater_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T>&& x, interval<T> const& y )
		{
			return total_greater_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T>&& x, interval<T>&& y )
		{
			return total_greater_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T>&& x, interval<T> const&& y )
		{
			return total_greater_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T> const&& x, interval<T>& y )
		{
			return total_greater_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T> const&& x, interval<T> const& y )
		{
			return total_greater_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T> const&& x, interval<T>&& y )
		{
			return total_greater_or_equal( x, y ) ;
		}

		template < typename T >
		const bool operator >=( interval<T> const&& x, interval<T> const&& y )
		{
			return total_greater_or_equal( x, y ) ;
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
	interval<T> interval<T>::operator=( interval<T> const& x )
	{
		pimpl->deep_copy( *( x.pimpl ) ) ;
		return *this ;
	}

	/*  Move Assignment Op  */

	template < typename T >
	interval<T> interval<T>::operator=( interval<T>&& x )
	{
		std::swap( this->pimpl, x.pimpl ) ;
		x.pimpl = nullptr ;
		return *this ;
	}

	template < typename T >
	interval<T> interval<T>::operator=( std::initializer_list<T> list )
	{
		if ( list.size() == 0 ) 
		{
			this->set_low( zero<T>() ) ;
			this->set_up( zero<T>() ) ;
		}
		else if ( list.size() == 2 )
		{
			if ( *( list.begin() ) > *( list.begin() + 1 ) )
			{
				this->set_low( *(list.begin() ) ) ;
				this->set_up( *( list.begin() + 1 ) ) ;
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


	/*  Compound Assignment Op Definition  */

	/*  impl Compound Assignment Operator  */

	template < typename T >
	const typename interval<T>::impl interval<T>::impl::operator+=( const typename interval<T>::impl& x )
	{
		lower_bound = downward( lower_bound + x.lower_bound ) ;
		upper_bound = upward( upper_bound + x.upper_bound ) ;
		return *this ;
	}

	/*  impl Compound Subtraction Operator   */

	template < typename T >
	const typename interval<T>::impl interval<T>::impl::operator-=( const typename interval<T>::impl& x )
	{
		lower_bound = downward( lower_bound - x.upper_bound ) ;
		upper_bound = upward( upper_bound - x.lower_bound ) ;
		return *this ;
	}

	/*  impl operator Compound Multiplication Operator  */

	template < typename T >
	const typename interval<T>::impl interval<T>::impl::operator*=( const typename interval<T>::impl& x )
	{
		auto l = lower_bound ;
		auto r = upper_bound ;

		if (l >= zero<T>() && x.lower_bound >= zero<T>() )  {
			lower_bound = downward( l * x.lower_bound ) ;
			upper_bound = upward( r * x.upper_bound ) ;
			return *this ;
		}
		else if (l >= zero<T>() && x.lower_bound < zero<T>() && x.upper_bound > zero<T>() ) 
		{
			lower_bound = downward( upper_bound * x.lower_bound ) ;
			upper_bound = upward( upper_bound * x.upper_bound ) ;
			return *this ;
		}
		else if (l >= zero<T>() && x.upper_bound <= zero<T>() ) 
		{
			lower_bound = downward( r * x.lower_bound ) ;
			upper_bound = upward( l * x.upper_bound ) ;
			return *this ;
		}
		else if (l < zero<T>() && r > zero<T>() && x.lower_bound >= zero<T>() ) 
		{
			lower_bound = downward( l * x.upper_bound ) ;
			upper_bound = upward( r * x.upper_bound ) ;
			return *this ;
		}
		else if (l < zero<T>() && r > zero<T>() && x.upper_bound <= zero<T>() ) 
		{
			lower_bound = downward( r * x.lower_bound ) ;
			upper_bound = upward( l * x.lower_bound ) ;
			return *this ;
		}
		else if (r <= zero<T>() && x.lower_bound >= zero<T>() ) 
		{
			lower_bound = downward( l * x.upper_bound ) ;
			upper_bound = upward( r * x.lower_bound ) ;
			return *this ;
		}
		else if (r <= zero<T>() && x.lower_bound < zero<T>() && x.upper_bound > zero<T>() ) 
		{
			lower_bound = downward( l * x.upper_bound ) ;
			upper_bound = upward( l * x.lower_bound ) ;
			return *this ;
		}
		else if (r <= zero<T>() && x.upper_bound <= zero<T>() ) 
		{
			lower_bound = downward( r * x.upper_bound ) ;
			upper_bound = upward( l * x.lower_bound ) ;
			return *this ;
		}
		else {
			lower_bound = std::fmin( downward( r * x.lower_bound ), downward( l * x.upper_bound ) ) ;
			upper_bound = std::fmax( upward( r * ( x.upper_bound) ), upward( l * ( x.lower_bound) ) ) ;
			return *this ;
		}
	}

	/*  impl Compound Division Operator  */

	template < typename T >
	const typename interval<T>::impl interval<T>::impl::operator/=( const typename interval<T>::impl& x )
	{
		if ( x.lower_bound <= zero<T>() && x.upper_bound >= zero<T>() ) 
		{
			throw logic_error( "Divided by Cranberries which contains Zero!" ) ;
		}
		else
		{
			interval tmp( downward( one<T>() / x.upper_bound ) ,
				upward( one<T>() / x.lower_bound ) ) ;
			*this *= *( tmp.pimpl ) ;
			return *this ;
		}
	}

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
	constexpr T interval<T>::impl::low() const
	{
		return lower_bound ;
	}

	/*  Interval Upper_Bound Getter  */

	template < typename T >
	constexpr T interval<T>::impl::up() const
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
	const interval<T> max( std::initializer_list< interval<T> > list)
	{
		std::vector< interval<T> > tmp( list ) ;
		std::nth_element( tmp.begin(), tmp.begin(), tmp.end(), [&]( interval<T>& a, interval<T>& b ) { reutrn a > b ; } ) ;
		return *( tmp.begin() ) ;
	}

	/*  initializer_list<interval> and Predicate Argument Max   */

	template< typename T, class Compare>
	const interval<T> max( std::initializer_list< interval<T> > list, Compare copm)
	{
		std::vector< interval<T> > tmp( list ) ;
		std::nth_element( tmp.begin(), tmp.begin(), tmp.end(), [&]( interval<T>& a, interval<T>& b ) { reutrn comp( a, b ) ; } ) ;
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
	const interval<T> min( std::initializer_list< interval<T> > list)
	{
		std::vector< interval<T> > tmp( list ) ;
		std::nth_element( tmp.begin(), tmp.begin(), tmp.end(), [&]( interval<T>& a, interval<T>& b ) { reutrn a < b ; } ) ;
		return *( tmp.begin() ) ;
	}

	/*  initializer_list<interval> and Predicate Argument Min   */

	template< typename T, class Compare >
	const interval<T> min( std::initializer_list< interval<T> > list, Compare comp )
	{
		std::vector< interval<T> > tmp( list ) ;
		std::nth_element( tmp.begin(), tmp.begin(), tmp.end(), [&]( interval<T>& a, interval<T>& b ) { reutrn comp( a, b ) ; } ) ;
		return *( tmp.begin() ) ;
	}

	/*  Tow interval and Predicate Argument Max   */

	template< typename T, class Compare, std::enable_if_t< std::is_same< Compare, interval<T> >::value >*& = enabler >
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

	template< typename T, typename Compare>
	constexpr std::pair< interval<T> const&, interval<T> const& > minmax( interval<T> const& a, interval<T> const& b, Compare comp )
	{
		return comp(a,b ) ? std::make_pair( a, b ) : std::make_pair( b, a ) ;
	}

	template < typename T >
	const std::pair< interval<T>, interval<T> > minmax( std::initializer_list< interval<T> > list)
	{
		std::vector< interval<T> > v(list) ;
		auto hold = std::minmax_element( v.begin(), v.end() ) ;
		return std::make_pair( *(hold.first), *(hold.second) ) ;
	}

	template< typename T, typename Compare>
	const std::pair< interval<T>, interval<T> > minmax( std::initializer_list< interval<T> > list, Compare comp )
	{
		std::vector< interval<T> > v(list) ;
		auto hold = std::minmax_element( v.begin(), v.end(), [&]( interval<T> const& a, interval<T> const& b ) { reutrn comp( a, b ) ; } ) ;
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
	interval<T> low( interval<T> const& a) { return a.low() ; }

	template < typename T >
	interval<T> up( interval<T> const& a) { return a.up() ; }

	template < typename T >
	bool is_singleton( interval<T> const& a) { return a.is_singleton() ; }

	template < typename T >
	void swap( interval<T>& x, interval<T>& y ) noexcept
	{
		x.swap( y ) ;
	}
	/*  Generic numeric function for primitive dispatch  */

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto sin( T&& x )->decltype(auto) { return std::sin( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto cos( T&& x )->decltype(auto) { return std::cos( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto tan( T&& x )->decltype(auto) { return std::tan( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto asin( T&& x )->decltype(auto) { return std::asin( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto acos( T&& x )->decltype(auto) { return std::acos( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto atan( T&& x )->decltype(auto) { return std::atan( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto sinh( T&& x )->decltype(auto) { return std::sinh( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto cosh( T&& x )->decltype(auto) { return std::cosh( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto tanh( T&& x )->decltype(auto) { return std::tanh( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto asinh( T&& x )->decltype(auto) { return std::asinh( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto acosh( T&& x )->decltype(auto) { return std::acosh( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto atanh( T&& x )->decltype(auto) { return std::atanh( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto exp( T&& x )->decltype(auto) { return std::exp( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto exp2( T&& x )->decltype(auto) { return std::exp2( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto expm1( T&& x )->decltype(auto) { return std::expm1( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto pow( T&& x, int n)->decltype(auto) { return std::pow( std::forward<T>( x ), n) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto abs( T&& x )->decltype(auto) { return std::abs( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto sqrt( T&& x )->decltype(auto) { return std::sqrt( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto log( T&& x )->decltype(auto) { return std::log( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto log1p( T&& x )->decltype(auto) { return std::log1p( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto log2( T&& x )->decltype(auto) { return std::log2( std::forward<T>( x ) ) ; }
	
	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto log10( T&& x )->decltype(auto) { return std::log10( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto erf( T&& x )->decltype(auto) { return std::erf( std::forward<T>( x ) ) ; }

	template < typename T, typename U, std::enable_if_t< std::is_same< interval<U>, std::decay_t<T> >::value >*& = enabler >
	auto erfc( T&& x )->decltype(auto) { return std::erfc( std::forward<T>( x ) ) ; }


	/*  for enum output with string  */

	//interval_ordering to string
	std::string enum2str( interval_ordering const& x )
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
	std::string enum2str( partial_ordering const& x )
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
	std::string enum2str( weak_ordering const& x )
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
	std::string enum2str( total_ordering const& x )
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
	std::string enum2str( interval_relation const& x )
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
		case Cranberries::interval_relation::contain:
			return std::string( "contain" ) ;
		case Cranberries::interval_relation::part_of:
			return std::string( "part_of" ) ;
		case Cranberries::interval_relation::niether:
			return std::string( "niether" ) ;
		}
		return std::string( "error" ) ;
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

	std::ostream& operator<<( std::ostream& s, Version_Tag const& v)
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
	auto Y = hull( -1.0, 1.0 ) ;
	x - x //  returns [ 0.0, 0.0 ]
	y * y //  returns [ 0.0, 1.0 ]
	x / x //  returns [ 1.0, 1.0 ]
	- end example ]
	*/

	/*
	[ Example : You can switch high accuracy arithmetic into nomal arithmetic using namespace inside a scope block.
	auto _1_2 = hull( 1.0, 2.0 ) ;
	auto _m1_1 = hull( -1.0, 1.0 ) ;
	{
		using namespace Cranberries::nomal_accuracy;
		_1_2 - _1_2 //  returns [ -1.0, 1.0 ]
		_m1_1 * _m1_1 //  returns [ -1.0, 1.0 ]
		_1_2 / _1_2 //  returns [ 0.5, 2.0 ]
	}
	- end example ]
	*/

	/*  Interval Addition Op  */

	// T + interval<T>
	template  < typename T >
	interval<T> operator +( T&& x, interval<T> const& y )
	{
		return ( interval<T>( y.low() + x, y.up() + x ) ) ;
	}

	// interval<T> + T
	template < typename T >
	interval<T> operator +( interval<T> const& x, T&& y )
	{
		return ( interval<T>( x.low() + y, x.up() + y ) ) ;
	}

	// interval<T> + interval<T>
	template < typename T >
	const interval<T> operator +( interval<T> const& x, interval<T> const& y )
	{
		return interval<T>( x ) += y ;
	}

	/*  Interval Subtraction Op */

	// T - interval<T>
	template < typename T >
	interval<T> operator -( T&& x, interval<T> y )
	{
		return interval<T>{ x - y.up(), x - y.low() } ;
	}

	// interval<T> - T
	template  < typename T >
	interval<T> operator -( interval<T> const& x, T&& y )
	{
		return interval<T>{ x.low() - y, x.up() - y } ;
	}

	// interval<T> - interval<T>
	template < typename T, typename U >
	const interval<T> operator -( interval<T> const& x, U&& y )
	{
		if ( &x == &y )
			return interval<T>{ } ;
		return interval<T>( x ) -= y ;
	}

	/*  Interval Multiplication Op  */

	// T * interval<T>
	template  < typename T >
	interval<T> operator *( T&& x, interval<T> const& y )
	{
		if ( x > zero<T>() ) 
			return interval<T>{ y.low() * x, y.up() * x } ;
		else
			return interval<T>{ y.up() * x, y.low() * x } ;
	}

	// interval<T> * T
	template  < typename T >
	interval<T> operator *( interval<T> const& x, T&& y )
	{
		if ( y > zero<T>() ) 
			return interval<T>{ x.low() * y, x.up() * y } ;
		else
			return interval<T>{  x.up() * y, x.low() * y } ;
	}

	// interval<T> * interval<T>
	template < typename T, typename U >
	const interval<T> operator *( interval<T> const& x, U&& y )
	{
		if ( &x == &y )
			return x.pow(2) ;

		return interval<T>( x ) *= y ;
	}

	/*  Interval Division Op  */

	// T / interval<T>
	template  < typename T >
	interval<T> operator /( T&& x, interval<T> const& y )
	{
		if ( y.low() <= zero<T>() && zero<T>() <= y.up() ) 
			throw Cranberries::logic_error( "Divided by Cranberries which contains zero!" ) ;
		else if ( y.low() > zero<T>() ) 
			return interval<T>{ x / y.up(), x / y.low() } ;
		else
			return interval<T>{ x / y.low(), x / y.up() } ;
	}

	// interval<T> / T
	template  < typename T >
	interval<T> operator /( interval<T> const& x, T&& y )
	{
		if ( y == zero<T>() ) 
			throw Cranberries::logic_error( "Divided by Zero!" ) ;
		else if ( y > zero<T>() ) 
			return interval<T>{ x.low() / y, x.up() / y } ;
		else
			return interval<T>{ x.up() / y, x.low() / y } ;
	}

	// interval<T> / interval<T>
	template < typename T, typename U >
	const interval<T> operator /( interval<T> const& x, U&& y )
	{
		if ( y.is_contain(zero<T>() ) )
			throw Cranberries::invalid_argument( "" ) ;
		else if ( &x == &y )
			return interval<T>{ one<T>(), one<T>() } ;
		return interval<T>( x ) /= y ;
	}

	namespace nomal_accuracy
	{
		template < typename T >
		const interval<T> operator -( interval<T>& x, interval<T>& y )
		{
			return interval<T>( x ) -= y ;
		}
		template < typename T >
		const interval<T> operator -( interval<T>& x, interval<T> const& y )
		{
			return interval<T>( x ) -= y ;
		}
		template < typename T >
		const interval<T> operator -( interval<T>& x, interval<T>&& y )
		{
			return interval<T>( x ) -= y ;
		}
		template < typename T >
		const interval<T> operator -( interval<T>& x, interval<T> const&& y )
		{
			return interval<T>( x ) -= y ;
		}
		template < typename T >
		const interval<T> operator -( interval<T> const& x, interval<T>& y )
		{
			return interval<T>( x ) -= y ;
		}
		template < typename T >
		const interval<T> operator -( interval<T> const& x, interval<T> const& y )
		{
			return interval<T>( x ) -= y ;
		}
		template < typename T >
		const interval<T> operator -( interval<T> const& x, interval<T>&& y )
		{
			return interval<T>( x ) -= y ;
		}
		template < typename T >
		const interval<T> operator -( interval<T> const& x, interval<T> const&& y )
		{
			return interval<T>( x ) -= y ;
		}
		template < typename T >
		const interval<T> operator -( interval<T>&& x, interval<T>& y )
		{
			return interval<T>( x ) -= y ;
		}
		template < typename T >
		const interval<T> operator -( interval<T>&& x, interval<T> const& y )
		{
			return interval<T>( x ) -= y ;
		}
		template < typename T >
		const interval<T> operator -( interval<T>&& x, interval<T>&& y )
		{
			return interval<T>( x ) -= y ;
		}
		template < typename T >
		const interval<T> operator -( interval<T>&& x, interval<T> const&& y )
		{
			return interval<T>( x ) -= y ;
		}
		template < typename T >
		const interval<T> operator -( interval<T> const&& x, interval<T>& y )
		{
			return interval<T>( x ) -= y ;
		}
		template < typename T >
		const interval<T> operator -( interval<T> const&& x, interval<T> const& y )
		{
			return interval<T>( x ) -= y ;
		}
		template < typename T >
		const interval<T> operator -( interval<T> const&& x, interval<T>&& y )
		{
			return interval<T>( x ) -= y ;
		}
		template < typename T >
		const interval<T> operator -( interval<T> const&& x, interval<T> const&& y )
		{
			return interval<T>( x ) -= y ;
		}
		template < typename T >
		const interval<T> operator *( interval<T>& x, interval<T>& y )
		{
			return interval<T>( x ) *= y ;
		}
		template < typename T >
		const interval<T> operator *( interval<T>& x, interval<T> const& y )
		{
			return interval<T>( x ) *= y ;
		}
		template < typename T >
		const interval<T> operator *( interval<T>& x, interval<T>&& y )
		{
			return interval<T>( x ) *= y ;
		}
		template < typename T >
		const interval<T> operator *( interval<T>& x, interval<T> const&& y )
		{
			return interval<T>( x ) *= y ;
		}
		template < typename T >
		const interval<T> operator *( interval<T> const& x, interval<T>& y )
		{
			return interval<T>( x ) *= y ;
		}
		template < typename T >
		const interval<T> operator *( interval<T> const& x, interval<T> const& y )
		{
			return interval<T>( x ) *= y ;
		}
		template < typename T >
		const interval<T> operator *( interval<T> const& x, interval<T>&& y )
		{
			return interval<T>( x ) *= y ;
		}
		template < typename T >
		const interval<T> operator *( interval<T> const& x, interval<T> const&& y )
		{
			return interval<T>( x ) *= y ;
		}
		template < typename T >
		const interval<T> operator *( interval<T>&& x, interval<T>& y )
		{
			return interval<T>( x ) *= y ;
		}
		template < typename T >
		const interval<T> operator *( interval<T>&& x, interval<T> const& y )
		{
			return interval<T>( x ) *= y ;
		}
		template < typename T >
		const interval<T> operator *( interval<T>&& x, interval<T>&& y )
		{
			return interval<T>( x ) *= y ;
		}
		template < typename T >
		const interval<T> operator *( interval<T>&& x, interval<T> const&& y )
		{
			return interval<T>( x ) *= y ;
		}
		template < typename T >
		const interval<T> operator *( interval<T> const&& x, interval<T>& y )
		{
			return interval<T>( x ) *= y ;
		}
		template < typename T >
		const interval<T> operator *( interval<T> const&& x, interval<T> const& y )
		{
			return interval<T>( x ) *= y ;
		}
		template < typename T >
		const interval<T> operator *( interval<T> const&& x, interval<T>&& y )
		{
			return interval<T>( x ) *= y ;
		}
		template < typename T >
		const interval<T> operator *( interval<T> const&& x, interval<T> const&& y )
		{
			return interval<T>( x ) *= y ;
		}
		template < typename T >
		const interval<T> operator /( interval<T>& x, interval<T>& y )
		{
			return interval<T>( x ) /= y ;
		}
		template < typename T >
		const interval<T> operator /( interval<T>& x, interval<T> const& y )
		{
			return interval<T>( x ) /= y ;
		}
		template < typename T >
		const interval<T> operator /( interval<T>& x, interval<T>&& y )
		{
			return interval<T>( x ) /= y ;
		}
		template < typename T >
		const interval<T> operator /( interval<T>& x, interval<T> const&& y )
		{
			return interval<T>( x ) /= y ;
		}
		template < typename T >
		const interval<T> operator /( interval<T> const& x, interval<T>& y )
		{
			return interval<T>( x ) /= y ;
		}
		template < typename T >
		const interval<T> operator /( interval<T> const& x, interval<T> const& y )
		{
			return interval<T>( x ) /= y ;
		}
		template < typename T >
		const interval<T> operator /( interval<T> const& x, interval<T>&& y )
		{
			return interval<T>( x ) /= y ;
		}
		template < typename T >
		const interval<T> operator /( interval<T> const& x, interval<T> const&& y )
		{
			return interval<T>( x ) /= y ;
		}
		template < typename T >
		const interval<T> operator /( interval<T>&& x, interval<T>& y )
		{
			return interval<T>( x ) /= y ;
		}
		template < typename T >
		const interval<T> operator /( interval<T>&& x, interval<T> const& y )
		{
			return interval<T>( x ) /= y ;
		}
		template < typename T >
		const interval<T> operator /( interval<T>&& x, interval<T>&& y )
		{
			return interval<T>( x ) /= y ;
		}
		template < typename T >
		const interval<T> operator /( interval<T>&& x, interval<T> const&& y )
		{
			return interval<T>( x ) /= y ;
		}
		template < typename T >
		const interval<T> operator /( interval<T> const&& x, interval<T>& y )
		{
			return interval<T>( x ) /= y ;
		}
		template < typename T >
		const interval<T> operator /( interval<T> const&& x, interval<T> const& y )
		{
			return interval<T>( x ) /= y ;
		}
		template < typename T >
		const interval<T> operator /( interval<T> const&& x, interval<T>&& y )
		{
			return interval<T>( x ) /= y ;
		}
		template < typename T >
		const interval<T> operator /( interval<T> const&& x, interval<T> const&& y )
		{
			return interval<T>( x ) /= y ;
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
		auto s = new std::string( "[ " + std::to_string( this->low() )  + ", " + std::to_string( this->up() )  + " ]" ) ;
		return s->c_str() ;
	}

	//------------------------------------------------------------------------//
	/*  It is cumbersome to write the type to every declaration.              */
	/*  Like ' auto x = interval<double>(one<T>(), static_cast<T>(3.0) ) '.                          */
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
	template < typename T >
	bool interval_less( interval<T> const& x, interval<T> const& y )
	{
		return ( x.up() < y.low() ) ;
	}
	template < typename T >
	bool interval_less_or_equal( interval<T> const& x, interval<T> const& y )
	{
		return ( x.up() <= y.low() ) ;
	}


	// interval greater
	template < typename T >
	bool interval_greater( interval<T> const& x, interval<T> const& y )
	{
		return ( x.low() > y.up() ) ;
	}
	template < typename T >
	bool interval_greater_or_equal( interval<T> const& x, interval<T> const& y )
	{
		return ( x.low() >= y.up() ) ;
	}

	// interval unordered
	template < typename T >
	bool interval_unordered( interval<T> const& x, interval<T> const& y )
	{
		return ( interval_less( x, y ) == false && interval_greater( x, y ) == false ) ;
	}


	/*  Partial Ordering  */

	// partial less
	template < typename T >
	bool partial_less( interval<T> const& x, interval<T> const& y )
	{
		return ( x.low() < y.low() && x.up() < y.up() ) ;
	}
	template < typename T >
	bool partial_less_or_equal( interval<T> const& x, interval<T> const& y )
	{
		return ( x.low() <= y.low() && x.up() <= y.up() ) ;
	}

	// partial greater
	template < typename T >
	bool partial_greater( interval<T> const& x, interval<T> const& y )
	{
		return ( x.low() > y.low() && x.up() > y.up() ) ;
	}
	template < typename T >
	bool partial_greater_or_equal( interval<T> const& x, interval<T> const& y )
	{
		return ( x.low() >= y.low() && x.up() >= y.up() ) ;
	}

	// partial unordered
	template < typename T >
	bool partial_unordered( interval<T> const& x, interval<T> const& y )
	{
		return ( partial_less( x, y ) == false && partial_greater( x, y ) == false ) ;
	}


	/*  Weak Ordering  */

	// weak less
	template < typename T >
	bool weak_less( interval<T> const& x, interval<T> const& y )
	{
		return ( x.low() < y.low() ) ;
	}
	template < typename T >
	bool weak_less_or_equal( interval<T> const& x, interval<T> const& y )
	{
		return ( x.low() <= y.low() ) ;
	}

	// weak greater
	template < typename T >
	bool weak_greater( interval<T> const& x, interval<T> const& y )
	{
		return ( x.low() > y.low() ) ;
	}
	template < typename T >
	bool weak_greater_or_equal( interval<T> const& x, interval<T> const& y )
	{
		return ( x.low() >= y.low() ) ;
	}

	// weak equal
	template < typename T >
	bool weak_equal( interval<T> const& x, interval<T> const& y )
	{
		return ( weak_less( x, y ) == false && weak_less( y, x ) == false ) ;
	}


	/*  Total Ordering  */

	// total less
	template < typename T >
	bool total_less( interval<T> const& x, interval<T> const& y )
	{
		if ( x.low() == y.low() )
		{
			return x.up() < y.up() ;
		}
		else {
			return x.low() < y.low() ;
		}
	}
	template < typename T >
	bool total_less_or_equal( interval<T> const& x, interval<T> const& y )
	{
		if ( x.low() == y.low() )
		{
			return x.up() <= y.up() ;
		}
		else {
			return x.low() <= y.low() ;
		}
	}

	// total greater
	template < typename T >
	bool total_greater( interval<T> const& x, interval<T> const& y )
	{
		if ( x.low() == y.low() )
		{
			return x.up() > y.up() ;
		}
		else {
			return x.low() > y.low() ;
		}
	}
	template < typename T >
	bool total_greater_or_equal( interval<T> const& x, interval<T> const& y )
	{
		if ( x.low() == y.low() )
		{
			return x.up() >= y.up() ;
		}
		else {
			return x.low() >= y.low() ;
		}
	}

	// total equal
	template < typename T >
	bool total_equal( interval<T> const& x, interval<T> const& y )
	{
		return ( x.low() == y.low() && x.up() == y.up() ) ;
	}


	//--------------------------//
	/*  Ordering Discriminator  */
	//--------------------------//

	/*  Interval Ordering  */

	template < typename T >
	Cranberries::interval_ordering interval_order(Cranberries::interval<T>& x, Cranberries::interval<T>& y )
	{
		if ( interval_less( x, y ) )
		{
			return Cranberries::interval_ordering::less ;
		}
		else if ( interval_greater( x, y ) )
		{
			return Cranberries::interval_ordering::greater ;
		}
		else {
			return Cranberries::interval_ordering::unordered ;
		}
	}

	/*  Partial Ordering  */

	template < typename T >
	Cranberries::partial_ordering partial_order(Cranberries::interval<T>& x, Cranberries::interval<T>& y )
	{
		if ( partial_less( x, y ) )
		{
			return partial_ordering::less ;
		}
		else if ( partial_greater( x, y ) )
		{
			return partial_ordering::greater ;
		}
		else {
			return partial_ordering::unordered ;
		}
	}

	/*  Weak ordering  */

	template < typename T >
	Cranberries::weak_ordering weak_order(Cranberries::interval<T>& x, Cranberries::interval<T>& y )
	{
		if ( weak_less( x, y ) )
		{
			return weak_ordering::less ;
		}
		else if ( weak_greater( x, y ) )
		{
			return weak_ordering::greater ;
		}
		else {
			return weak_ordering::equivalent ;
		}
	}

	/*  Total Ordering  */

	template < typename T >
	Cranberries::total_ordering total_order(Cranberries::interval<T>& x, Cranberries::interval<T>& y )
	{
		if ( total_less( x, y ) )
		{
			return total_ordering::less ;
		}
		else if ( total_greater( x, y ) )
		{
			return total_ordering::greater ;
		}
		else {
			return total_ordering::equal ;
		}
	}

	/*  interval compare function objrct  */
	template < order = order::Total >
	struct less
	{
		template < typename T >
		bool operator()( interval<T> const& a, interval<T> const& b )
		{
			return total_less( a, b ) ;
		}
	} ;
	template < >
	struct less < order::Weak >
	{
		template < typename T >
		bool operator()( interval<T> const& a, interval<T> const& b )
		{
			return weak_less( a, b ) ;
		}
	} ;
	template < >
	struct less < order::Partial >
	{
		template < typename T >
		bool operator()( interval<T> const& a, interval<T> const& b )
		{
			return partial_less( a, b ) ;
		}
	} ;
	template < >
	struct less < order::Interval >
	{
		template < typename T >
		bool operator()( interval<T> const& a, interval<T> const& b )
		{
			return interval_less( a, b ) ;
		}
	} ;
	template < order = order::Total >
	struct greater
	{
		template < typename T >
		bool operator()( interval<T> const& a, interval<T> const& b )
		{
			return total_greater( a, b ) ;
		}
	} ;
	template < >
	struct greater < order::Weak >
	{
		template < typename T >
		bool operator()( interval<T> const& a, interval<T> const& b )
		{
			return weak_greater( a, b ) ;
		}
	} ;
	template < >
	struct greater < order::Partial >
	{
		template < typename T >
		bool operator()( interval<T> const& a, interval<T> const& b )
		{
			return partial_greater( a, b ) ;
		}
	} ;
	template < >
	struct greater < order::Interval >
	{
		template < typename T >
		bool operator()( interval<T> const& a, interval<T> const& b )
		{
			return interval_greater( a, b ) ;
		}
	} ;
	template < order = order::Total >
	struct equal
	{
		template < typename T >
		bool operator()( interval<T> const& a, interval<T> const& b )
		{
			return total_equal( a, b ) ;
		}
	} ;
	template < >
	struct equal < order::Weak >
	{
		template < typename T >
		bool operator()( interval<T> const& a, interval<T> const& b )
		{
			return weak_equal( a, b ) ;
		}
	} ;
	template < order = order::Partial>
	struct undorderesd
	{
		template < typename T >
		bool operator()( interval<T> const& a, interval<T> const& b )
		{
			return partial_unordered( a, b ) ;
		}
	} ;
	template < >
	struct undorderesd < order::Interval >
	{
		template < typename T >
		bool operator()( interval<T> const& a, interval<T> const& b )
		{
			return interval_unordered( a, b ) ;
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
		return ( this->low() < x && this->up() > x ) ;
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
		return std::make_pair( interval<T>( x.low(), x.mid() ), interval<T>( x.mid(), x.up() ) ) ;
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