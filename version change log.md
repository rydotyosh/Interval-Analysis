*Cranberries Interval Version Change Log*  

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
functino object ( less, greater, equal, unordered )  
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
