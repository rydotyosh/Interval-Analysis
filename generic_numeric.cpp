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

/*new Interval class prototype*/
class interval
{
public:
    /*constexpr means it does not have the side effects*/
    constexpr interval( )
    :low( double( ) )
    , up( double( ) )
    {}
    explicit constexpr interval( double Initializer )
    :low( Initializer )
    , up( Initializer )
    {}
    constexpr interval( double Lower_Bound, double Upper_Bound )
    :low( Lower_Bound )
    , up( Upper_Bound )
    {}

    /*interval pow( int )*/
    const interval pow( int n ) const
    {
      if ( n < 0 ){
        return this->pow( -1 * n ) ;
      }
      else if ( n == 0 ){
        return interval( 1.0L, 1.0L ) ;
      }
      else if ( n % 2 == 0 ){
        if ( up <= 0.0L ){
          return interval(
          std::nextafter( std::pow( up, n ), -max<double> ),
          std::nextafter( std::pow( low, n ), max<double> ) ) ;
        }
        else if ( low <= 0.0L && up >= 0.0L ){
          return interval( 0.0L, std::max(
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
        if ( b - a >= 2.0L*PI<double> )
        {
          return interval( -1.0L, 1.0L ) ;
        }
        /*	base point set  */
        if ( a >= 0.0L )
        {
          c = static_cast<int>( std::nextafter( a / 2.0L / PI<double>, max<double> ) ) ;
        }
        else if ( b <= 0.0L )
        {
          c = static_cast<int>( std::nextafter( a / 2.0L / PI<double>, -max<double> ) ) ;
        }
        else
        {
          c = static_cast<int>( -PI<double> / 2.0L ) ;
        }
        /* checking  */
        if ( std::nextafter( PI<double> / 2.0L + c*PI<double>*2.0L, max<double> ) >= a &&
           std::nextafter( PI<double> / 2.0L + c*PI<double>*2.0L, -max<double> ) <= b )
        {
          if ( PI<double> / 2.0L*3.0L + c*PI<double>*2.0L >= a &&
             PI<double> / 2.0L*3.0L + c*PI<double>*2.0L <= b )
          {
            return interval( -1.0, 1.0 ) ;
          }
          else
          {
            return interval(
            std::min( std::nextafter( std::sin( a ), -max<double> ),
            std::nextafter( std::sin( b ), -max<double> ) ), 1.0L ) ;
          }
        }
        else if ( std::nextafter( PI<double> / 2.0L*3.0L + c*PI<double>*2.0L, max<double> ) >= a &&
         std::nextafter( PI<double> / 2.0L*3.0L + c*PI<double>*2.0L, -max<double> ) <= b )
        {
          if ( PI<double> / 2.0L + c*PI<double>*2.0L >= a &&
             PI<double> / 2.0L + c*PI<double>*2.0L <= b )
          {
            return interval( -1.0L, 1.0L ) ;
          }
          else
          {
            return interval( -1.0L,
             std::max( std::nextafter( std::sin( a ), max<double> ),
              std::nextafter( std::sin( b ), max<double> ) ) ) ;
          }
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
      if ( b - a >= 2.0L*PI<double> )
      {
        return interval( -1.0L, 1.0L ) ;
      }
      /*	base point set  */
      if ( a >= 0.0L )
      {
        c = static_cast<int>( a / 2.0L / PI<double> ) ;
      }
      else if ( b <= 0.0L )
      {
        c = static_cast<int>( a / 2.0L / PI<double> - 1.0L ) ;
      }
      else
      {
        c = static_cast<int>( -PI<double> / 2.0L ) ;
      }
      /* checking  */
      if ( PI<double> + c*PI<double>*2.0L >= a && PI<double> + c*PI<double> * 2 <= b )
      {
        if ( PI<double>*2.0L + c*PI<double>*2.0L >= a &&
           PI<double>*2.0L + c*PI<double>*2.0L <= b )
        {
          return interval( -1.0L, 1.0L ) ;
        }
        else
        {
          return interval( -1.0L,
          std::max( std::nextafter( std::cos( a ), max<double> ),
           std::nextafter( std::cos( b ), max<double> ) ) ) ;
        }
      }
      else if ( PI<double>*2.0L + c*PI<double>*2.0L >= a &&
       PI<double>*2.0L + c*PI<double>*2.0L <= b )
      {
        if ( PI<double> + c*PI<double>*2.0L >= a &&
           PI<double> + c*PI<double>*2.0L <= b )
        {
          return interval( -1.0L, 1.0L ) ;
        }
        else
        {
          return interval(
          std::min( std::nextafter( std::cos( a ), -max<double> ),
           std::nextafter( std::cos( b ), -max<double> ) ), 1.0L ) ;
        }
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

    /*	interval exp	*/
    const interval exp( ) const
    {
      return interval(
      std::nextafter( std::exp( low ), -max<double> ),
      std::nextafter( std::exp( up ), max<double> ) ) ;
    }

    /*	interval absolute	*/
    const interval abs( ) const
    {
      if ( low < 0 ){
        return interval( 0.0L, std::max( std::abs( low ), std::abs( up ) ) ) ;
      }
      else{
        return *this ;
      }
    }
    /*low bound getter*/
    double low_bound( )
    {
      return low ;
    }
    /*upper bound value*/
    double up_bound( )
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
    double low ;//lower bound
    double up ;//upper bound
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
auto& operator<<( std::ostream& s, T&& x )->decltype( x.print( s ) )
{
  return x.print( s ) ;
}


int main( )
{
    interval v( -3.1415926 / 4, 0 ) ;
    /*test code*/
    cout << "sin" << endl ;
    cout << sin( v ) << endl ;
    cout << sin( 3.1415926 / 3 ) << endl ;
    cout << "cos" << endl ;
    cout << cos( v ) << endl ;
    cout << cos( 3.1415926 / 3 ) << endl ;
    cout << "exp" << endl ;
    cout << exp( v ) << std::endl ;
    cout << exp( 3.1415926 / 3 ) << endl ;
    cout << "pow" << endl ;
    cout << pow( v, 2 ) << std::endl ;
    cout << pow( 3.1415926, 2 ) << endl ;
    cout << "abs" << endl ;
    cout << abs( v ) << endl ;
    cout << abs( 3.1415926 / 3 ) << endl ;
}
