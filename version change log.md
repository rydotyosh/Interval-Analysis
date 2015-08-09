*Cranberries Interval Version Change Log*  
========================================
- ver 2.0.2
 - added
 interval cbrt function  
- ver 2.0.1
 - fixed
added missing two overload div(L const&,interval<R> const&) and div(interval<L> const&,R const&)  
interval<T> default template parameter designed T = long double  
- ver 2.0.0  
 - fixed  
semantics switching improve( function signeture changed and helper function defined )  
accepted to arithmetic for two interval that have different value type  
compound assignment operator is changed member function to non-member function  
allowed promotion interval and other arithmetic type operation ( interval<L> @ interval<R> , interval<L> @ R and L @ interval<R> ).  
- ver 1.2.0
 - fixed  
ordering switching  
 - added
able to order interval<T> and T
- ver. 1.1.1
 - added  
log1p  
expm1  
fma  
exp2  
ordering switching ( namespace & overloaad trick )  
 - fixed  
wrap nextafteer(T&&, max<T>()) -> upward(T&&)  
wrap nextafteer(T&&, max<T>()) -> upward(T&&)  

- ver. 1.1.0
 - added  
function object class ( less, greater, equal, unordered )  
arithmetic type switching ( namespace & overload trick )  
minmax  

- ver. 1.0.8  
 - fixed  
interval<T> - interval<T>  
interval<T> * interval<T>  
interval<T> / interval<T>  
 - added  
is_contain(T const&)  

- ver. 1.0.7  
 - added  
initializer_list Cotr & operator=  

- ver. 1.0.6  
 - fixed
abs  
T - interval<T>  
T * interval<T>  
T / interval<T> 

- ver. 1.0.5  
 - fixed  
enum2string rename enum2str  
max, min ( SFINAE )  

- ver. 1.0.4  
 - fixed  
critical mistake in compound assignment operator  

- ver. 1.0.3  
 - fixed  
Cranberries_Relation rename into Interval_Relation

- ver. 1.0.2  
 - fixed  
variadic max & min  
- ver. 1.0.1  
  - fixed
T() is repalaced into T{}  

- ver. 1.0.0  
Pre-release
