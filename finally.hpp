class finally
{
    std::function< void() > f ;
public :
    explicit finally( std::function< void () > f )
        : f(f) { }

    finally( scoped_guard const & ) = delete ;
    void operator = ( scoped_guard const & ) = delete ;


    ~finally()
    { f() ; }

} ;
