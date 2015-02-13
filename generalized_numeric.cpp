#include <exception>
#include <cmath>
#include <type_traits>
#include <iostream>
#include <limits>


using std::cout ;
using std::endl ;

/*PI*/
template<typename T>
constexpr T PI = static_cast<T>( 3.141592653589793238462643383279 ) ;

/*maximum*/
template<typename T>
constexpr T max = std::numeric_limits<T>::max( ) ;

/*minimum*/
template<typename T>
constexpr T min = std::numeric_limits<T>::min( ) ;

  class interval_error
  {
  public:
    interval_error(char* string)
    :mes(std::string(string))
    {}
    interval_error(std::string string)
    :mes(string)
    {}

    std::string what()
    {
      return mes;
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

/*new Interval class prototype*/
class interval
{
public:
    /*constexpr means it does not have the side effects*/
    constexpr interval( )
    :low( double( ) )
    , up( double( ) )
    {}
    constexpr interval( double Initializer )
    :low( Initializer )
    , up( Initializer )
    {}
    interval( double Lower_Bound, double Upper_Bound )
    {
      if ( Lower_Bound > Upper_Bound )
      {
          throw IntervalDomainExcep( "Lower Bound needs to less than Upper Bound!" ) ;
      }
      else
      {
          low = Lower_Bound ;
          up = Upper_Bound ;
      }
    }

    /*  interval compound assignment operator */

    //addition assignment
    const interval operator +=( const interval& rhs )
    {
      low = std::nextafter( low + rhs.low,-max<double> ) ;
      up = std::nextafter( up + rhs.up,max<double> ) ;
      return *this ;
    }

    //subtraction assignment
    const interval operator -=( const interval& rhs )
    {
      if ( this == &rhs )
      {
        low = 0.0 ;
        up = 0.0 ;
        return *this ;
      }
      low = std::nextafter( low - rhs.up, -max<double> ) ;
      up = std::nextafter( up - rhs.low, max<double> ) ;
      return *this ;
    }

    //multiplication assignment
    const interval operator *=( const interval& rhs )
    {
      if ( this == &rhs )
      {
        if ( up <= 0.0 )
        {
          low = std::pow( up, 2 ) ;
          up = std::pow( up, 2 ) ;
          return *this ;
        }
        else if ( low <= 0.0 && up >= 0.0 )
        {
          low = 0.0 ;
          up = std::max( std::pow( low, 2 ), std::pow( up, 2 ) ) ;
          return *this ;
        }
        else
        {
          low = std::pow( low, 2 ) ;
          up = std::pow( up, 2 ) ;
          return *this ;
        }
      }
      else if (low >= 0.0 && low >= 0.0){
        low = std::nextafter( low * rhs.low, -max<double> ) ;
        up = std::nextafter( up * rhs.up, max<double> ) ;
        return *this ;
      }
      else if ( low >= 0.0 && rhs.low < 0.0 && rhs.up > 0.0 )
      {
        low = std::nextafter( up * rhs.low, -max<double> ) ;
        up = std::nextafter( up * rhs.up, max<double> ) ;
        return *this ;
      }
      else if ( rhs.low >= 0.0 && rhs.up <= 0.0 )
      {
        low = std::nextafter( up * rhs.low, -max<double> ) ;
        up = std::nextafter( low * rhs.up, max<double> ) ;
        return *this ;
      }
      else if ( low < 0.0 && up > 0.0 && rhs.low >= 0.0 )
      {
        low = std::nextafter( low * rhs.up, -max<double> ) ;
        up = std::nextafter( up * rhs.up, max<double> ) ;
        return *this ;
      }
      else if ( low < 0.0 && up > 0.0 && rhs.up <= 0.0 )
      {
        low = std::nextafter( up * rhs.low, -max<double> ) ;
        up = std::nextafter( low * rhs.low, max<double> ) ;
        return *this ;
      }
      else if ( up <= 0.0 && rhs.low >= 0.0 )
      {
        low = std::nextafter( low * rhs.up, -max<double> ) ;
        up = std::nextafter( up * rhs.low, max<double> ) ;
        return *this ;
      }
      else if ( up <= 0.0 && rhs.low < 0.0 && rhs.up > 0.0 )
      {
        low = std::nextafter( low * rhs.low, -max<double> ) ;
        up = std::nextafter( low * rhs.up, max<double> ) ;
        return *this ;
      }
      else if ( up <= 0.0 && rhs.up <= 0.0 )
      {
        low = std::nextafter( up * rhs.up, -max<double> ) ;
        up = std::nextafter( low * rhs.low, max<double> ) ;
        return *this ;
      }
      else{
        low = std::min( std::nextafter( up * rhs.low, -max<double> ),
         std::nextafter( low * rhs.up, -max<double> ) ) ;
        up = std::max( std::nextafter( up * rhs.up, max<double> ),
         std::nextafter( low * rhs.low, max<double> ) ) ;
        return *this ;
      }
    }

    //division assignment
    const interval operator /=(const interval& rhs)
    {
      if ( rhs.low <= 0.0 && rhs.up >= 0.0 )
      {
        throw IntervalZeroDivideExcep( "Divided by Zero!" ) ;
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
          interval tmp(std::nextafter( 1.0 / rhs.up, -max<double> ),
                      std::nextafter( 1.0 / rhs.low, max<double> ) ) ;
          *this *= tmp ;
          return *this ;
        }
      }
    }

    //interval postfix increment operator
    const interval operator ++( int )
    {
      interval tmp( *this ) ;
      low = std::nextafter( low + 1.0, -max<double> ) ;
      up = std::nextafter( up + 1.0, max<double> ) ;
      return tmp ;
    }

    //interval prefix increment operator
    const interval operator ++( )
    {
      low = std::nextafter( low + 1.0, -max<double> ) ;
      up = std::nextafter( up + 1.0, max<double> ) ;
      return *this ;
    }

    //interval postfix decrement operator
    const interval operator --( int )
    {
      interval tmp( *this ) ;
      low = std::nextafter( low - 1.0, -max<double> ) ;
      up = std::nextafter( up - 1.0, max<double> ) ;
      return tmp ;
    }

    //interval prefix decrement operator
    const interval operator --( ){
      low = std::nextafter( low - 1.0, -max<double> ) ;
      up = std::nextafter( up - 1.0, max<double> ) ;
      return *this ;
    }
    /*  Logical operator  */

    //less than operator
    bool operator <( const interval& rhs )
    {
      if ( low == rhs.low )
      {
        if ( up < rhs.up )
        {
          return true ;
        }
      }
      else
      {
        if ( low < rhs.low )
        {
          return true ;
        }
      }
      return false ;
    }

    //greater than operator
    bool operator >( const interval& rhs )
    {
      if ( up > rhs.up )
      {
        return true ;
      }
      return false ;
    }

    //equivalence operator
    bool operator ==( const interval& rhs )
    {
      if (low == rhs.low && up == rhs.up){
        return true ;
      }
      return false ;
    }

    //not equivalence operator
    bool operator !=( const interval& rhs )
    {
      if ( low == rhs.low && up == rhs.up )
      {
        return false ;
      }
      return true ;
    }

    //less than or equal operator
    bool operator <=( const interval& rhs )
    {
      if (low <= rhs.low && up <= rhs.up)
      {
        return true ;
      }
      return false ;
    }

    //greater than or equal operator
    bool operator >=( const interval& rhs )
    {
      if ( up >= rhs.up && low >= rhs.low )
      {
        return true ;
      }
      return false ;
    }

    /*interval pow( int )*/
    const interval pow( int n ) const
    {
      if ( n < 0 ){
        return this->pow( -1 * n ) ;
      }
      else if ( n == 0 ){
        return interval( 1.0, 1.0 ) ;
      }
      else if ( n % 2 == 0 ){
        if ( up <= 0.0 ){
          return interval(
          std::nextafter( std::pow( up, n ), -max<double> ),
          std::nextafter( std::pow( low, n ), max<double> ) ) ;
        }
        else if ( low <= 0.0 && up >= 0.0 ){
          return interval( 0.0, std::max(
          std::nextafter( std::pow( low, n ), max<double> ),
          std::nextafter( std::pow( up, n ), max<double> ) ) ) ;
        }
      }
      return interval(
      std::nextafter( std::pow( low, n ), -max<double> ),
      std::nextafter( std::pow( up, n ), max<double> ) ) ;
    }
    /*interval sin*/
    const interval sin( ) const
    {
        int c ;
        long double a = low, b = up ;
        if ( b - a >= 2.0*PI<double> )
        {
          return interval( -1.0, 1.0 ) ;
        }
        /*  base point set  */
        c = static_cast<int>( std::nextafter( ( b * 2.0 / PI<double> - 1.0 ) / 4.0, max<double> ) ) ;
        /* checking  */
        if ( std::nextafter( PI<double>/2.0*( 1.0 + c*4.0 ), max<double> ) >= a &&
           std::nextafter( PI<double>/2.0*( 1 + c*4.0 ), -max<double> ) <= b )
        {
          if ( std::nextafter(PI<double>/2.0*( 3.0 + c*4.0 ), max<double> ) >= a &&
             std::nextafter( PI<double>/2.0*( 3.0 + c*4.0 ),-max<double> ) <= b )
          {
            return interval( -1.0, 1.0 ) ;
          }
          else if ( std::nextafter( ( c*4.0 - 1.0 )*PI<double>/2.0, max<double> ) >= a &&
             std::nextafter( ( c*4.0 - 1.0 )*PI<double>/2.0, -max<double> ) <= b )
          {
              return interval( -1.0, 1.0 ) ;
          }
          else
          {
            return interval(
            std::min( std::nextafter( std::sin( a ), -max<double> ),
            std::nextafter( std::sin( b ), -max<double> ) ), 1.0 ) ;
          }
        }
        else if ( std::nextafter(PI<double>/2.0*( 3.0 + c*4.0 ), max<double> ) >= a &&
            std::nextafter( PI<double>/2.0*( 3.0 + c*4.0 ),-max<double> ) <= b )
        {
            return interval( -1.0,
             std::max( std::nextafter( std::sin( a ), max<double> ),
              std::nextafter( std::sin( b ), max<double> ) ) ) ;
        }
        else if ( std::nextafter( ( c*4.0 - 1.0 )*PI<double>/2.0, max<double> ) >= a &&
                std::nextafter( ( c*4.0 - 1.0 )*PI<double>/2.0, -max<double> ) <= b )
        {
            return interval( -1.0,
             std::max( std::nextafter( std::sin( a ), max<double> ),
              std::nextafter( std::sin( b ), max<double> ) ) ) ;
        }
        else
        {
          return interval(
          std::min( std::nextafter( std::sin( a ), -max<double> ),
           std::nextafter( std::sin( b ), -max<double> ) ),
          std::max( std::nextafter( std::sin( a ), max<double> ),
           std::nextafter( std::sin( b ), max<double> ) ) ) ;
        }
    }
    /*interval cos*/
    const interval cos( ) const
    {
        int c ;
        long double a = low, b = up ;
        if ( b - a >= 2.0*PI<double> )
        {
          return interval( -1.0, 1.0 ) ;
        }
        /*  base point set  */
        c = static_cast<int>( std::nextafter( b / 2.0 / PI<double>, max<double> ) ) ;
        /* checking  */
        if ( std::nextafter( PI<double>*2.0*c, max<double> ) >= a &&
           std::nextafter( PI<double>*2.0*c, -max<double> ) <= b )
        {
          if ( std::nextafter(PI<double>*( 1.0 + 2.0*c ), max<double> ) >= a &&
             std::nextafter( PI<double>*( 1.0 + 2.0*c ),-max<double> ) <= b )
          {
            return interval( -1.0, 1.0 ) ;
          }
          else if ( std::nextafter( ( c*2.0 - 1.0 )*PI<double>, max<double> ) >= a &&
             std::nextafter( ( c*2.0 - 1.0 )*PI<double>, -max<double> ) <= b )
          {
              return interval( -1.0, 1.0 ) ;
          }
          else
          {
            return interval(
            std::min( std::nextafter( std::cos( a ), -max<double> ),
            std::nextafter( std::cos( b ), -max<double> ) ), 1.0 ) ;
          }
        }
        else if ( std::nextafter( PI<double>*( 2.0*c + 1.0 ), max<double> ) >= a &&
            std::nextafter( PI<double>*( 2.0*c + 1.0 ),-max<double> ) <= b )
        {
            return interval( -1.0,
             std::max( std::nextafter( std::cos( a ), max<double> ),
              std::nextafter( std::cos( b ), max<double> ) ) ) ;
        }
        else if ( std::nextafter( ( c*2.0 - 1.0 )*PI<double>, max<double> ) >= a &&
                std::nextafter( ( c*2.0 - 1.0 )*PI<double>, -max<double> ) <= b )
        {
            return interval( -1.0,
             std::max( std::nextafter( std::cos( a ), max<double> ),
              std::nextafter( std::cos( b ), max<double> ) ) ) ;
        }
        else
        {
          return interval(
          std::min( std::nextafter( std::cos( a ), -max<double> ),
           std::nextafter( std::cos( b ), -max<double> ) ),
          std::max( std::nextafter( std::cos( a ), max<double> ),
           std::nextafter( std::cos( b ), max<double> ) ) ) ;
        }
    }

    /*  interval exp  */
    const interval exp( ) const
    {
      return interval(
      std::nextafter( std::exp( low ), -max<double> ),
      std::nextafter( std::exp( up ), max<double> ) ) ;
    }

    /*  interval absolute */
    const interval abs( ) const
    {
      if ( low < 0 )
      {
        return interval( 0.0, std::max( std::abs( low ), std::abs( up ) ) ) ;
      }
      else{
        return *this ;
      }
    }
    /*  interval wid  */
    double wid( ) const
    {
      return up - low ;
    }
    /*interval middle point*/
    double mid( ) const
    {
      return ( low + up )/2.0 ;
    }
    /*low bound getter*/
    double low_bound( ) const
    {
      return low ;
    }
    /*upper bound getter*/
    double up_bound( ) const
    {
       return up ;
    }
    /*out put function*/
    std::ostream& print( std::ostream& s ) const
    {
      s << "[ " << low << " , " << up << " ]" ;
      return s ;
    }
private:
    double low ;//lower bound value
    double up ;//upper bound value
} ;
/*generic ver interval numeric functions overload*/
interval sin( const interval& a )  { return a.sin( ) ; }
interval cos( const interval& a )  { return a.cos( ) ; }
interval exp( const interval& a )  { return a.exp( ) ; }
interval pow( const interval& a, int const& n )  { return a.pow( n ) ; }
interval abs( const interval& a )  { return a.abs( ) ; }

/*generic numeric functions for primitive dispach*/
/*Using Forwarding Reference & type_traits*/
template <typename T, typename = typename std::enable_if<
  !std::is_same<interval, typename std::decay<T>::type>::value>::type>
    double sin( T&& x )  { return std::sin( std::forward<T>( x ) ) ; }

template <typename T, typename = typename std::enable_if<
  !std::is_same<interval, typename std::decay<T>::type>::value>::type>
    double cos( T&& x )  { return std::cos( std::forward<T>( x ) ) ; }

template <typename T, typename = typename std::enable_if<
  !std::is_same<interval, typename std::decay<T>::type>::value>::type>
    double exp( T&& x )  { return std::exp( std::forward<T>( x ) ) ; }

template <typename T, typename = typename std::enable_if<
  !std::is_same<interval, typename std::decay<T>::type>::value>::type>
    double pow( T&& x, int n )  { return std::pow( std::forward<T>( x ), n ) ; }

template <typename T, typename = typename std::enable_if<
  !std::is_same<interval, typename std::decay<T>::type>::value>::type>
    double abs( T&& x )  { return std::abs( std::forward<T>( x ) ) ; }

/*generic operator<< for classes have menber func print( ostream )*/
/*Using SFINAE rule*/
template<typename T>
auto operator<<( std::ostream& s, T&& x )->decltype( x.print( s ) )
{
  return ( x.print( s ) ) ;
}

/*generic wid*/
template< typename T >
auto wid( T&& x )->decltype( x.wid() )
{
  return x.wid() ;
}
/*generic mid*/
template< typename T >
auto mid( T&& x )->decltype( x.mid() )
{
  return x.mid() ;
}

//interval addition operator
template< typename T >
const interval operator +( T&& lhs, interval& rhs)
{
  return interval(lhs) += rhs ;
}
template< typename T >
const interval operator +( interval& rhs, T&& lhs )
{
  return interval(lhs) += interval(rhs) ;
}

//interval subtraction operator
template< typename T >
const interval operator -( T&& lhs, interval& rhs)
{
  return interval(lhs) -= rhs ;
}
template< typename T >
const interval operator -( interval& rhs, T&& lhs )
{
  return interval(lhs) -= interval(rhs) ;
}

//interval multiplication operator
template< typename T >
const interval operator *( T&& lhs, interval& rhs)
{
  return interval(lhs) *= rhs ;
}
template< typename T >
const interval operator *( interval& rhs, T&& lhs )
{
  return interval(lhs) *= interval(rhs) ;
}

//interval division operator
template< typename T >
const interval operator /( T&& lhs, interval& rhs)
{
  return interval(lhs) /= rhs ;
}
template< typename T >
const interval operator /( interval& rhs, T&& lhs )
{
  return interval(lhs) /= interval(rhs) ;
}



int main( )
{
  try{
    interval v( -3.1415926 , 0 ) ;
    /*test code*/
    cout << "Let v = " << v << endl ;
    cout << "middle(v)" << endl ; 
    cout << "= " << mid( v ) << endl ;
    cout << "width(v)" << endl ;
    cout << "= " <<  wid( v ) << endl ;
    cout << "2 + v" << endl ;
    cout << "= " <<  2 + v << endl ;
    auto d = v + 2 ;
    cout << "v + 2" << endl ;
    cout << "= " <<  d << endl;
    cout << "sin(v)" << endl ;
    cout << "= " <<  sin( v ) << endl ;
    cout << "cos(v)" << endl ;
    cout << "= " <<  cos( v ) << endl ;
    cout << "exp(v)" << endl ;
    cout << "= " <<  exp( v ) << std::endl ;
    cout << "pow(v,2)" << endl ;
    cout << "= " <<  pow( v, 2 ) << std::endl ;
    cout << "abs(v)" << endl ;
    cout << "= " <<  abs( v ) << endl ;

    return 0 ;
  }
  catch( IntervalDomainExcep& e )
  {
    cout << e.what() << endl ;
  }
  catch( IntervalZeroDivideExcep& e )
  {
    cout << e.what() << endl ;
  }
  catch( IntervalExcep& e )
  {
    cout << e.what() << endl ;
  }
  return 1 ;
}
