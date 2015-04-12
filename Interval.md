#Systax and Features of Interval Library
---
##はじめに
記載されたコードはファイルインクルードや名前空間の修飾が省略されており、そのままでは動作しません.  
了承ください.
##index

1. このライブラリでできること
2. クラス宣言
2. 区間の四則演算
3. メンバ関数
4. 例外
5. koi


##このライブラリでできること

このライブラリは区間数を扱うためのライブラリである.  
区間を用いた数値計算をおこなえる.  
区間の四則演算、数学関数、区間の関係性判定など.  

Interval.h に宣言、Interval.cpp に定義が記述されている.  
すべてのコードはInterval名前空間にはいっている.

##クラス宣言

intervalは次のように宣言されている.  
```cpp
namespace Interval
{

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

	//メンバ関数...

	private:
		class impl;
		std::unique_ptr<impl> pimpl;
	};
}//end namespace Interval
```
第1にinterval classは内部クラスに上限（upper bound）と下限（lower bound）を保持します.  
interval classは内部クラスimplのインスタンスをスマートポインタで保持する.  

第2にinterval class はクラステンプレートになっており、宣言や定義にはテンプレート仮引数が必要である.  
区間の上下限をどの型で保持するのかを指定しなければならない.
```cpp
auto x = interval<double>( ) ; // OK
auto x = interval( ) ; // error! needs to template argument.

```
とはいえ、いちいちテンプレート引数を明示的に指定するのは甚だ面倒である.  
そこで、hullをつかう. すると初期の上下限から型が推測されinterval classが返される.
```cpp

auto x = hull( 1.0 , 2.0 ) ; // x は interval<double>

```
デフォルトコンストラクタ、2引数（左辺値）コンストラクタ、2引数（右辺値）コンストラクタ  
コピーコンストラクタ、ムーブコンストラクタ、コピー代入演算子が使えます.  
```cpp
// デフォルトコンストラクタ 上下限はdouble()で初期化される
auto v = interval<double>( ) ;

// 2引数コンストラクタ 上下限は double(1.0), double(2.0)で初期化される
// つまり暗黙の型変換が起こるため注意されたし
auto w = interval<double>( 1.0 , 2.0 ) ;

// コピーコンストラクタ 上下限が v.low_bound v.upper_bound で初期化される
// つまり深いコピーが行われ、内部クラスは共有しません
auto x(v) ;

// コピー代入演算子 まずデフォルトコンストラクタが呼ばれ
// y.low_bound = x.low_bound , y.upper_bound = x.upper_bound となる
auto y = x ;
```
区間の定義の方法とコンストラクタ等のポリシーの解説は以上である.  
次に区間演算について解説する.  


##区間の四則演算

区間演算の定義について確認する.  

区間 X は連続する数の集合であると定義され. 上限と下限から
```
X = [ a , b ]
```
と表す.  

ある実数の演算子●を考える. 区間 X , Y についてこの演算子は
```
X ● Y = { x ● y |　x ∈ X , y ∈ Y }
```
と定義される. これを区間拡張という.  
このライブラリには区間拡張された演算子や関数が含まれる.  

###区間のオーバーロードされた演算子

interval class で再定義された演算子はこちら.
```cpp
const interval operator +=(const interval&);
const interval operator -=(const interval&);
const interval operator *=(const interval&);
const interval operator /=(const interval&);

const interval operator ++();
const interval operator ++(int);
const interval operator --();
const interval operator --(int);

bool operator<(interval const&) const;
bool operator>(interval const&) const;
bool operator<=(interval const&) const;
bool operator>=(interval const&) const;
bool operator==(interval const&) const;
bool operator!=(interval const&) const;
```
四則演算については区間同士の演算でしかも複合代入演算子のみ再定義される.  
その他はすべてグローバル関数で定義されている.  
`operator+`を例にとる以下のようなコードだ  
```cpp
//interval addition operator
template <typename T,typename U, typename = typename std::enable_if <
!std::is_same<interval<U>, typename std::decay<T>::type>::value>::type>
interval<U> operator +(T&& x, const interval<U>& y)
{ return (interval<U>(y.get_low() + x, y.get_up() + x)); }

template <typename T,typename U, typename = typename std::enable_if <
!std::is_same<interval<U>, typename std::decay<T>::type>::value>::type>
interval<U> operator +(const interval<U>& x, T&& y)
{ return (interval<U>(x.get_low() + y, x.get_up() + y)); }

template<typename T>
const interval<T> operator +(const interval<T>& x, const interval<T>& y)
{
return interval<T>(x) += y;
}
```
つまり、`interval<U> + T` や `T + interval<U>` が可能である.  
返り値は`interval<U>`なので`T`は`U`に型変換されることに注意が必要だ.  
また、パフォーマンスを優先して複合代入演算子を呼び出すか、直感的にわかりやすいコードか、いずれかを選択することができる.  
つまり、こういうことだ.  
```cpp
interval<double> a , b , c ;

// pattern 1
auto x = a + b + c ;

// pattern 2
auto y(a) ;
y+=b ;
y+=c ;
```
パターン1はわかりやすいし数学ではこう書く.  
このとき、コードはどう解釈されるか？
答えはこうだ
```cpp
x = interval<double>( ) ;
interval<double> tmp1 = a + b ;
interval<double> tmp2 = tmp1 + c ;
x = tmp2 ;
```
デフォルトコンストラクタがxを初期化する.  
次に左結合性に従い１つづつオブジェクトがたされ一時オブジェクトがスタックに入る.  
つxに代入したあとにスコープを抜けると一時オブジェクトは２つ破棄される.  
実にたくさんの無駄がある.  

パターン2はどうか
```cpp
y = interval<double>( a ) ;
y += b ;
y += c ;
```
まずコピーコンストラクタが呼び出されている.  
次に複合代入演算子を使うので新しいオブジェクトはできない.  
yだけが新しいオブジェクトなのでこれが最小のコストだといえる.  

ただし、コードは3行にわたっているし、わかりにくい.  

四則演算についてまとめると以下のようになる.  
```cpp
x++ ;			// postfix increment operator
++x ;			// prefix increment operator
x-- ;			// postfix decrement operator
--x ;			// prefix decrement operator
x + y ;			// interval<T> + interval<T>
x - y ;			// interval<T> - interval<T>
x * y ;			// interval<T> * interval<T>
x / y ;			// interval<T> / interval<T>
x += y ;		// interval<T> += interval<T>
x -= y ;		// interval<T> -= interval<T>
x *= y ;		// interval<T> *= interval<T>
x /= y ;		// interval<T> /= interval<T>

x += 2.0 ;		// interval<T> += T
x -= 2.0 ;		// interval<T> -= T
x *= 2.0 ;		// interval<T> *= T
x /= 2.0 ;		// interval<T> /= T
2.0 += a ;		// compile error!
2.0 -= a ;		// compile error!
2.0 *= a ;		// compile error!
2.0 /= a ;		// compile error!
2.0 + x ;		// T + interval<T>
2.0 - x ;		// T + interval<T>
2.0 * x ;		// T + interval<T>
2.0 / x ;		// T + interval<T>
x * 2.0 ;		// interval<T> + T
x / 2.0 ;		// interval<T> + T
x + 2.0 ;		// interval<T> + T
x - 2.0 ;		// interval<T> + T
```
注意が必要なのは複合代入演算子である.  
```cpp
T += interval<T>
```
はコンパイルエラーだ. 理由は少し考えればわかる.  
複合代入演算子は呼び出し元のオブジェクトを返すので.  
`T += interva<T>`の返り値は`T`である.  
しかし区間演算の結果は区間でなければならないので`interval<T>`である.  
矛盾している. したがってこのようなよびだしはしてはならない.

四則演算の解説は以上である.  
次に、メンバ関数について解説する.  

##メンバ関数
区間のメンバ関数は大きく分けて3種類ある.
数学関数、setter / getter、関係性判定だ.  
そのすべてを以下に列挙する.  

```cpp
//interval numeric
const interval sin() const;
const interval cos() const;
const interval pow(int n) const;
const interval exp() const;
const interval abs() const;
const T mid() const;
const T wid() const;

// getter and setter
const T get_low() const;
const T get_up() const;
void set_up(T&);
void set_low(T&);
void set_up(T&&);
void set_low(T&&);

//interval relation functions
bool is_equal_to(interval const&) const;
bool is_weak_greater(interval const&,flag const& f = flag::off) const;
bool is_weak_less(interval const&, flag const& f = flag::off) const;
bool is_absolute_greater(interval const&, flag const& f = flag::off) const;
bool is_absolute_less(interval const&, flag const& f = flag::off) const;
bool partial_greater(interval const&, flag const& f = flag::off) const;
bool partial_less(interval const&, flag const& f = flag::off) const;
bool is_contain(interval const&) const;
bool is_part_of(interval const&) const;
Interval_Relation relation(interval const&) const;
```
###数学関数
この章では説明に際して
```
X = [ a , b ]
```
を用いる.  

はじめに、区間に特有の関数2つを紹介する.  
####wid
`wid`はwidthの意味であり区間の幅を返す関数.  
```
wid( X ) = b - a
```
である.

####mid
`mid`はmiddleの意味であり、区間の中間を返す関数.  
```
mid( X ) = ( a + b ) / 2
```
である.

ほかは区間拡張された関数である.  
####sin
sinの区間拡張. 区間Xにおけるsin(x)の上下限を返す関数.
```
sin( X ) = {sin( x ) |  x ∈ X }
```
であり、最大値は
```
π/2 + 2πn ∈ X {n ∈ N}
```
のとき
```
sin( x ) = 1
```
最小値は
```
3π/2 + 2πn ∈ X {n ∈ N}
```
のとき
```
sin( x ) = -1
```
となる.
####cos
cosの区間拡張. 区間Xにおけるcos(x)の上下限を返す関数.
```
cos( X ) = {cos( x ) |  x ∈ X }
```
であり、最大値は
```
2πn ∈ X {n ∈ N}
```
のとき
```
cos( x ) = 1
```
最小値は
```
π + 2πn ∈ X {n ∈ N}
```
のとき
```
cos( x ) = -1
```
となる.
####pow
powの区間拡張. 区間Xにおけるpow(x,n)の上下限を返す関数.  
整数乗のみの実装です.  
実数乗は実装されていない.  
```
pow( X ) = {pow( x , n ) |  x ∈ X , n ∈ N}
```
注意が必要なのは
```
n < 0
```
のときである.  

```
n < 0 のとき
pow( X , n ) => pow( 1/X , -n )
```
と再帰呼び出しされる.  
このとき、Xが0を含んでいた場合は0割となり  
Interval::logic_errorがthrowされる.

####exp
expの区間拡張. 区間Xにおけるexp(x)の上下限を返す関数.
```
exp( X ) = {exp( x , n ) |  x ∈ X }
```
expは単調増加の関数なので
```
exp( X ) = [ exp(a) , exp(b) ]
```
である.

####abs
absの区間拡張. 区間Xにおけるabs(x)の上下限を返す関数.
```
abs( X ) = {abs( x , n ) |  x ∈ X }
```

```
if a > 0 -> exp( X ) = [ a , b ]
if b < 0 -> exp( X ) = [ -b , -a ]
if a < 0 < b -> exp( X ) = [ 0 , max( -a , b ) ]
```
となる.  

###setterとgetter
####setter
区間の上下限は`set_up()`と`set_low()`を使って再設定できる.  
このとき上限が下限を下回る、または下限が上限を上回るような呼び出しをした場合には  
Interval::invalid_errorがthrowされる.  

```cpp
auto x = hull( 1.0 , 2.0 ) ;
x.set_low( 0.0 ) ; // OK
x.set_up( 0.0 ) ; // throw Interval::invalid_error!
```
####getter
区間の上下限は`get_up()`と`get_low()`を使って取得できる.  
```cpp
auto x = hull( 0.0 , 1.0 ) ;
auto up = x.get_up() ; // up = x.upper_bound
auto low = x.get_low() ; // low = x.low_bound
```

###関係性判定関数
区間の関係は大小関係だけでなく包含関係もあり複雑である.  
大小比較にはそもそもweak、partial、totalの3種類があり  
イコールにはequalityとequivalentがある.  
加えて区間には包含関係がある.  
