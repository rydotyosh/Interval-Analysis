#Systax and Features of Cranberries Interval Library
---
##はじめに[Readme]
本ライブラリの開発はプログラミング言語C++14でおこなわれている.開発環境はVisual Studio 2015 RC Community(compiler=VC14)である.  
記載されたコードはファイルインクルードや名前空間の修飾が省略されており,そのままではほぼ動作しない.  
あらかじめ了承ください.  

##まえがき[Introduce]

'Syntax and Features of Cranberries Interval Library'では  
Cranberries Interval Libraryにおける,  
クラスや関数の文法と機能を解説する.  
プログラミングにおける落とし穴になりえると思われる箇所にはメモを書いてある.  
プログラミングにおける技術的な話は最低限にとどめ,  
代わりに数学的なバックグラウンドについて詳しく解説している.  

---
##目次[Index]

1. このライブラリでできること
2. クラス宣言
2. 区間の四則演算
3. 関数
4. 例外
5. 出力


##このライブラリでできること[What This Library Allows?]

このライブラリは区間数を扱うためのライブラリである.  
区間を用いた数値計算をおこなえる.  
区間の四則演算,数学関数,区間の関係性判定など.  

Interval.hpp にほぼすべてのコードが記述されている.  
例外オブジェクトのみexception.hpp に記述されている.
すべてのコードはCranberries名前空間に所属している.  

ライブラリを使用するためにはinterval.hppをインクルードするだけでよい.  

##クラス宣言[Class Declaration]

intervalは次のように宣言されている.  
```cpp
namespace Cranberries
{

	template<typename T>
	class interval
	{
	public:
		constexpr interval();
		constexpr interval(T&, T&);
		constexpr interval(T&&, T&&);
		~interval();

		interval(const interval&);
		interval(interval&&);
		interval& operator=(const interval&);
		interval& operator=(interval&&);

	//メンバ関数...

	private:
		class impl;
		std::unique_ptr<impl> pimpl;
	};
}//end namespace Cranberries
```
第1にinterval classは内部クラスに上限（upper bound）と下限（lower bound）を保持する.  
interval classは内部クラスimplのインスタンスをスマートポインタで保持する.  

第2にinterval class はクラステンプレートになっており,宣言や定義にはテンプレート仮引数が必要である.  
区間の上下限をどの型で保持するのかを指定しなければならない.
```cpp
auto x = interval<double>( ) ; // OK
auto x = interval( ) ; // error! Ctor needs to template argument.

```
とはいえ,テンプレート引数を明示的に指定するのは甚だ面倒である.  
そこで,hullをつかう. 引数から型が推測されinterval classが返される.
```cpp

auto x = hull( 1.0 , 2.0 ) ; // decltype( x ) は interval<double>

```
デフォルトコンストラクタ,2引数コンストラクタ（左辺値）,2引数コンストラクタ（右辺値）  
コピーコンストラクタ,ムーブコンストラクタ,コピー代入演算子が使える.  
```cpp
// デフォルトコンストラクタ 上下限はdouble{}で初期化される
auto v = interval<double>( ) ;

// 2引数コンストラクタ 上下限は double(1), double(2)で初期化される
// つまり暗黙の型変換が起こるため注意されたし
auto w = interval<double>( 1 , 2 ) ;

// コピーコンストラクタ 上下限が v.low_bound v.upper_bound で初期化される
// つまり深いコピーが行われ,内部クラスは共有しない
auto x(v) ;

// コピー代入演算子 まずデフォルトコンストラクタが呼ばれ
// y.low_bound = x.low_bound , y.upper_bound = x.upper_bound となる
auto y = x ;
```

また,inteval class はムーブコンストラクタとムーブ代入演算子が定義されている.  
ムーブセマンティクスが利用できる.  
MoveはRvalue Referenceと対になるC++11からのコア言語機能である.各自で勉強されること.  


区間の定義の方法とコンストラクタ等のポリシーの解説は以上である.  
次に区間演算について解説する.  


##区間の四則演算[Interval Four Arithmetic Operations]

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

###区間のオーバーロードされた演算子[Overloaded Interval Operations]

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
Relational operator については,関数のrelational functionsの項目で詳細に解説するのでそちらを参照してもらいたい.  
四則演算については区間同士の演算でしかも複合代入演算子のみ再定義される.  
その他はすべてグローバル関数で定義されている.  
`operator+`を例に挙げる.  
```cpp
template <typename T>
interval<T> operator +(T&& x, const interval<T>& y)
{
	return (interval<T>(y.low() + x, y.up() + x));
}

template <typename T>
interval<T> operator +(const interval<T>& x, T&& y)
{
	return (interval<T>(x.low() + y, x.up() + y));
}

template<typename T>
const interval<T> operator +(const interval<T>& x, const interval<T>& y)
{
	return interval<T>(x) += y;
}

```
つまり,`interval<T> + T` や `T + interval<T>` が可能である.  
また,パフォーマンスを優先して複合代入演算子を呼び出すか,直感的にわかりやすいコードか,いずれかを選択することができる.  
つまり,以下のコードを選択できる.  
```cpp
interval<double> a , b , c ;

// pattern 1
auto x = a + b + c ;

// pattern 2
auto y(a) ;
y+=b ;
y+=c ;
```
パターン1はわかりやすいし数学ではこのように書く.  
このとき,コードはどう解釈されるか？
答えは以下のようになる
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

ただし,コードは3行にわたっているし,わかりにくい.  

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
次に,メンバ関数について解説する.  

##区間関数[Interval Expanded Functions]

区間の関数は大きく分けて3種類ある.
数学関数,アクセッサ,関係性だ.  
そのすべてを以下に列挙する.  

```cpp
template<typename T>
class interval
{
	const interval sin() const;
	const interval cos() const;
	const interval tan() const;
	const interval asin() const;
	const interval acos() const;
	const interval atan() const;
	const interval sinh() const;
	const interval cosh() const;
	const interval tanh() const;
	const interval asinh() const;
	const interval acosh() const;
	const interval atanh() const;
	const interval pow(int n) const;
	const interval log() const;
	const interval log10() const;
	const interavl cbrt() const;;
	const interval sqrt() const;
	const interval exp() const;
	const interval abs() const;
	const interval erf() const;
	constexpr T mid() const;
	constexpr T wid() const;
	constexpr bool is_singleton() const;

	constexpr T low() const;
	constexpr T up() const;
	void set_up(T&);
	void set_low(T&);
	void set_up(T&&);
	void set_low(T&&);

	Interval_Relation relational(interval const&) const;
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
	typedef T value_type;
}
template<typename T>
const interval<T> max(interval<T>& a, interval<T>& b);

template<typename T>
const interval<T> max(std::vector<interval<T>> list);

template<typename T, class Pred>
const interval<T> max(std::vector<interval<T>> list, Pred pred);

template < typename T, typename Pred, typename std::enable_if_t<!std::is_same<Pred,interval<T>>::value>*& = enabler>
const interval<T> max(interval<T>& a, interval<T>& b, Pred pred);

template <typename T>
constexpr T max(T a, T b);

template <typename T, typename ... Args>
constexpr T max(T a, T b, Args ... args);


template<typename T>
const interval<T> min(interval<T>& a, interval<T>& b);

template<typename T>
const interval<T> min(std::vector<interval<T>> list);

template<typename T, class Pred>
const interval<T> min(std::vector<interval<T>> list, Pred pred);

template<typename T, class Pred, typename std::enable_if_t<!std::is_same<Pred, interval<T>>::value>*& = enabler>
const interval<T> min(interval<T>& a, interval<T>& b, Pred pred);

template <typename T>
constexpr T min(T a, T b);

template <typename T, typename ... Args>
constexpr T min(T a, T b, Args ... args);


template<typename T>
bool total_less(interval<T> const&,interval<T> const&);
template<typename T>
bool total_greater(interval<T> const&,interval<T> const&);
template<typename T>
bool weak_less(interval<T> const&,interval<T> const&);
template<typename T>
bool weak_greater(interval<T> const&,interval<T> const&);
template<typename T>
bool partial_less(interval<T> const&,interval<T> const&);
template<typename T>
bool partial_greater(interval<T> const&,interval<T> const&);
template<typename T>
bool total_equal(interval<T> const&,interval<T> const&);
template<typename T>
bool weak_equal(interval<T> const&,interval<T> const&);
template<typename T>
bool partial_unordered(interval<T> const&,interval<T> const&);
```
###数学関数[Interval Arithmetic Functions]
この節では説明に際して
```
X = [ a , b ]
```
を用いる.  

はじめに,区間に特有の関数3つを紹介する.  
####wid
`wid`はwidthの意味であり区間の幅を返す関数.  
```
wid( X ) = b - a
```
である.

####mid
`mid`はmiddleの意味であり,区間の中間を返す関数.  
```
mid( X ) = ( a + b ) / 2
```
である.

####is_singleton
`is_singleton`は区間の要素が唯一であるかをboolで返す関数.

```cpp
is_singleton( X )  
true iff a == b
niether false
```



ほかは区間拡張された関数である.  


####sin
sinの区間拡張. 区間Xにおけるsin(x)の上下限を返す関数.
```
sin( X ) = {sin( x ) |  x ∈ X }
```
であり,最大値は
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
であり,最大値は
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
####tan
引数の区間が`πn/2`を含むは  
`Cranberries::over_flow` がthrowされる.  

それ以外の場合は単調増加関数であるので  
```
tan(X) = [tan(a) , tan(b)]
```
となる.  

ただし,tan(a),tan(b)が区間が保持する値の表現できる限界を超えた場合,  
`Cranberries::over_flow`がthrowされる.  


####asin
arcsinは引数が-1以上,1以下でなければならない  
`a < -1 or b > 1`  
の場合は  
`Cranberries::invalid_argument`がthrowされる.  

そうでなければ単調減少関数なので  
```
asin( X ) = [asin(b) , asin(b)]
```
となる.  
####acos
arcsinは引数が-1以上,1以下でなければならない  
a < -1 or b > 1
の場合は  
`Cranberries::invalid_argument`がthrowされる.

そうでなければ単調増加関数なので  
```
acos( X ) = [acos(a) , acos(b)]
```
となる.

####atan
atanは単調増加関数なので  
```
atan( X ) = [atan(a) , atan(b)]
```
となる.  

####sinh
sinhは単調増加関数なので  
```
sinh( X ) = [sinh(a) , sinh(b)]
```
となる.  

ただし,sinh(a),sinh(b)が区間が保持する値の表現できる限界を超えた場合,  
`Cranberries::over_flow`がthrowされる.  

####cosh
coshは懸垂曲線であり.線対称性をもち,軸は0である.  
coshは常に正である,  
引数の区間Xが0より大きければ,局所的な単調増加性をもつ.  
よって,
```
cos( X ) = [cosh(a), cosh(b)]
```
となる.  

引数の区間Xが0より小さければ,局所的な単調減少性をもつ.  
よって,  
```
cos( X ) = [cosh(b), cosh(a)]
```
となる.  

引数の区間Xが0を含む場合は,下限値は1である.  
また、対称性から,  
```
|a| > |b| -> cosh(a) > cosh(b)
```
である.  
よって、  
```
if |a| > |b|
 cosh( X ) = [1, cosh(a)]
else
 cosh( X ) = [1, cosh(b)]
```
となる.  

ただし,cosh(a),cosh(b)が区間が保持する値の表現できる限界を超えた場合,  
`Cranberries::over_flow`がthrowされる.  


####tanh
tanhは単調増加関数なので,  
```
tanh( X ) = [tanh(a), tanh(b)]
```
となる.  


####asinh
asinhは単調増加関数なので,  
```
asinh( X ) = [asinh(a), asinh(b)]
```
となる.  


####acosh
acoshは引数が1以上でなければならないので,  
`a < 1` の場合は,  
`Cranberries::invalid_argument`がthrowされる.  

`1 <= a`の場合  
acoshは単調増加関数なので,
```
acosh( X ) = [acosh(a), acosh(b)]
```
となる.  

####pow
powの区間拡張. 区間Xにおけるpow(x,n)の上下限を返す関数.  
整数乗と実数乗で実装が分かれる.  
nが整数の場合
```
pow( X ) = {pow( x , n ) |  x ∈ X , n ∈ N}
```
```
if n<0 -> pow( X , n) ->pow( X , n) = pow( 1/X , -n )
if n=0 -> pow( X , n) = [ 1 , 1 ]
if a>=0 or if n is odd -> pow( X , n ) = [ a^n , b^n ]
if b<=0 and n is even -> pow( X , n ) = [ b^n , a^n ]
if a <= 0 <= b and n is even -> pow( X , n ) = [ 0 , max( a^n , b^n ) ]
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
このとき,Xが0を含んでいた場合は0割となり  
`Cranberries::logic_error`がthrowされる.  

nが実数の場合
まず,引数の区間が負の数を含む場合は解が複素数となる.  
本ライブラリでは複素数をサポートしていないため  
`Cranberries::logic_error` がthrowされる.  

引数の区間の下限が0.0より大きければ
pow(X) = [pow(a), pow(b)]
となる.  

ただし,両方の場合においてpow(a,n),pow(b,n)が区間が保持する値の表現できる限界を超えた場合,
`Cranberries::over_flow`がthrowされる.  

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

####sqrt
sqrtの区間拡張. 区間Xにおけるsqrt(x)の上下限を返す関数.  
実数範囲の関数なので`X >= 0` でなければならない.  

a < 0 である場合には`Cranberries::logic_error`がthrowされる.  

```
sqrt( X ) = {sqrt(x) | x ∈ X }
```

sqrtは単調増加の関数なので

```
sqrt( X ) = [ sqrt(a) , sqrt(b) ]
```
である.  




####cbrt
sqrtの区間拡張. 区間Xにおけるsqrt(x)の上下限を返す関数.  
実数範囲の関数なので`X >= 0` でなければならない.  

a < 0 である場合には`Cranberries::logic_error`がthrowされる.  

```
cbrt( X ) = {cbrt(x) | x ∈ X }
```

cbrtは単調増加の関数なので

```
cbrt( X ) = [ cbrt(a) , cbrt(b) ]
```
である.  



####log
logの区間拡張. 区間Xにおけるlog(x)の上下限を返す関数.  
`X > 0` でなければならない.  

```
log( X ) = {log(x) | x ∈ X }
```

logは単調増加の関数なので

```
log( X ) = [ log(a) , log(b) ]
```
である.  

a <= 0 である場合には`Cranberries::logic_error`がthrowされる.  

####log10
log10の区間拡張. 区間Xにおけるlog(x)の上下限を返す関数.  
`X > 0` でなければならない.  

```
log10( X ) = {log10(x) | x ∈ X }
```

log10は単調増加の関数なので

```
log10( X ) = [ log10(a) , log10(b) ]
```
である.  

a <= 0 である場合には`Cranberries::logic_error`がthrowされる.  


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

####max
区間を引数にとり,最大の区間を返す関数たち.  
maxには5つのオーバーロードがある.  
```cpp
max(interval,interval)
max(interval,interval,Pred)
max(vector<interval>)
max(vector<interval>,Pred)
max(interval...)
```
1つめは2引数のmaxで大きい方を返す.  
比較にはデフォルトの  
````
operator>
```
つまり,  
```
total_greater
```
を用いる.  

2つめは2引数＋3つめ引数の引数に述語をとるだ.  
任意の述語を用いて区間を比較する.  

3つめは可変数の区間を比較するためのもので  
```cpp
Cranberries::max({a,b,c,d}); /// a,b,c,d is interval class instance
```
のようにリスト初期子を用いて呼び出すことができる.  

4つめは3つめに任意の述語を使って比較できる第2引数を追加したものだ.  

5つめはvariadic templates を用いて実装されている.  
ADLを利用できるという利点がある.  
```cpp
max(a,b,c,d) // a,b,c,d is interval class instance
```
上のように名前空間の修飾を省くことができる.  

####min
実装はmaxとまったく同じであるのでmaxを参照.  
異なるのは比較に
```cpp
operator<
```
を用いることのみである.  

###アクセッサ[Accessors]

####セッター[setter]
区間の上下限は`set_up()`と`set_low()`を使って再設定できる.  
このとき上限が下限を下回る,または下限が上限を上回るような呼び出しをした場合には  
`Cranberries::invalid_argument`がthrowされる.  

```cpp
auto x = hull( 2.0 , 3.0 ) ;
x.set_low( 1.0 ) ; // OK
x.set_up( 0.0 ) ; // error [ 1.0 , 0.0] is invalid!
```
####ゲッター[getter]
区間の上下限は`up()`と`low()`を使って取得できる.  
```cpp
auto x = hull( 0.0 , 1.0 ) ;
auto up = x.up() ; // up = 1.0
auto low = x.low() ; // low = 0.0
```

###区間の関係性関数[Interval Relational Functions]
__この節はとても重要なので必ず一読するべきである__
区間の関係は大小関係だけでなく包含関係もあり複雑である.  
大小比較にはtotal,weak,partialの3種類があり,
イコールにはequalityとequivalentがある.  

これらすべてを  
```cpp
operator>  
operator<  
operator>=  
operator<=
operator==
```
で表すことは不可能なので関数を提供する.  

```cpp
// - interval.h
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

// - interval.cpp

template<typename T>
bool total_less(interval<T> const&,interval<T> const&);
template<typename T>
bool total_greater(interval<T> const&,interval<T> const&);
template<typename T>
bool weak_less(interval<T> const&,interval<T> const&);
template<typename T>
bool weak_greater(interval<T> const&,interval<T> const&);
template<typename T>
bool partial_less(interval<T> const&,interval<T> const&);
template<typename T>
bool partial_greater(interval<T> const&,interval<T> const&);
template<typename T>
bool total_equal(interval<T> const&,interval<T> const&);
template<typename T>
bool weak_equal(interval<T> const&,interval<T> const&);
template<typename T>
bool partial_unordered(interval<T> const&,interval<T> const&);
template<typename T>
bool interval_less(interval<T> const&,interval<T> const&);
template<typename T>
bool interval_greater(interval<T> const&,interval<T> const&);
template<typename T>
bool interval_unordered(interval<T> const&,interval<T> const&);


template<typename T>
Cranberries::partial_ordering partial_order(Cranberries::interval<T>& x, Cranberries::interval<T>& y);
template<typename T>
Cranberries::weak_ordering weak_order(Cranberries:: interval<T>& x, Cranberries::interval<T>& y);
template<typename T>
Cranberries::total_ordering total_order(Cranberries::interval<T>& x, Cranberries::interval<T>& y);
template<typename T>
Cranberries::interval_ordering interval_order(Cranberries::interval<T>& x, Cranberries::interval<T>& y);

```

これらの挙動を解説する前に,なぜこのような種類の関数が用意されているのかを説明せねばならない.  
ここからは集合論とアルゴリズムの少し長い話になる.  
それは区間においてはどう定義されるのかという話だ.  
__これはとても重要なので読みとばすことはおすすめしない__

####順序[Ordering]
まず最初に「値が等しいということ（Equality）」という概念について今一度確認する．C++（に限らず多くのprogramming言語）で普通"=="（及び"!="）で表される関係である．
```cpp
int a = 1;
int b = 1;
int c = 2;
assert( a == b ); // `a' is equal to `b'
assert( a != c ); // `a' is not equal to `c'
```
上の例では，aとbはequalである，と言える．

何のことは無い「値が等しい」という関係を表しているだけだ（詳しく議論するとこれはこれで曲者であるが）．

なぜこれを確認してもらったかというと，後でこのequalityと似たような概念である"equivalence"という概念が出てくるからだ．そして，このequivalenceという概念がSTLにおけるsort（広く言えば順序付け全般）において非常に重要な意味を持つからである．  

####厳密で弱い順序[Strict Weak Ordering]
STLにおいて，順序を定める述語を指定するときにはすべからく「厳密で弱い順序（Strict Weak Order）」という概念がでてくる．
std::sortに渡される述語には厳密で弱い順序であることが要求されるため非常に重要である.  

ある集合の要素を一列に並べるときに「大きい順」や「小さい順」とかいうふうな基準を決めるが，この基準のことを普通は「順序」と呼ぶ．しかしここでは「順序」という言葉を数学的・抽象的な意味，即ち，ある集合の任意の2つの要素の間にある関係が有る（true）か無い(false)か，という意味で使う．この意味で，順序は2つの値を引数に取ってbool値を返す関数の形式を取る．STLで使われる「順序」もこの形式を取っているのはご存知の通り．
```cpp
int a = 1;
int b = 2;
assert( less<int>()(a, b) ); // STLがデフォルトで使う「順序」であるless
```
順序には様々な種類があり，厳密で弱い順序はこの種類の中の一つだ．まず先に代表的な順序の種類である「全順序（Total Order）」を説明した方が良いと思われるので，そちらから説明する．

全順序とは以下の性質を満たしている順序だ（より正確には「厳密な（strict）」全順序の定義）．ここで，f(x, y)は2つの要素xとyの順序を定義する関数とする．
1. 非反射律:f(x, x)は常にfalseである
2. 非対称律:f(x, y)がtrueならば!f(y, x)もtrueである
3. 推移律:f(x, y)がtrueでありf(y, z)がtrueであるならf(x, z)はtrueである
4. 反対称律:f(x, y)がfalseでありf(y, x)もfalseであるならば，xとyは等しい(equal)


よく分からないので具体的な全順序の例としてless<int>に登場してもらって，条件の意味を順に追っていく．

非反射律
```cpp
int i = 3;
assert( !less<int>()(i, i) );
```
要するに同じ値に対して常にfalseを返す，というのが一つ目の条件「（非）反射律」である．

この条件に関して非常に重要な指摘として，less_equal<int>やgreater_equal<int>はこの条件を満たさないということだ．これらの「順序」は同じ値に対してtrueを返してしまう．このような「順序」はSTLにおける比較の条件として絶対に使えない.  


非対称律
```cpp
int i;
int j;
...
if( less<int>()(i, j) ){
  assert( !less<int>()(j, i) );
}
```
これは「順序」という言葉から直感的に分かる条件だ．具体的には例えば「aはbより小さい」なら「bはaより小さくない」と言えるのは当然でしょう，ということだ．

推移律
```cpp
int i;
int j;
int k;
...
if( less<int>()(i, j) && less<int>()(j, k) ){
  assert( !less<int>()(i, k) );
}
```
これまた「順序」という言葉から直感的に理解できる条件だ．言葉で言えば「aはbより小さい」かつ「bはcより小さい」なら「aはcより小さい」ということだ．

反対称律
```cpp
int i, j;
...
if( !less<int>()(i, j) && !less<int>()(j, i) ){
  assert( i == j );
}
```
これも分かりやすい条件だ．「iがjよりも小さいことはない」かつ「jがiよりも小さいことはない」なら「iとjは同じ値である」ということは，通常の順序なら当然でしょう．そもそも，「全」順序という言葉は異なる2つの要素x, yの間全てにf(x, y)またはf(y, x)が成り立つことから来ている名前で，そのどちらも成り立たないということは，そもそもxとyは同じ要素だったのだ，ということが言える．

さて，これを踏まえた上で，次に厳密で弱い順序の定義を見ていく．

厳密で弱い順序では全順序の反対称律だけが緩められている．

1. 非反射律:f(x, x)は常にfalseである
2. 非対称律:xとyが（equalityの意味で）等しくないとき，f(x, y)がtrueならば!f(y, x)もtrueである
3.  推移律:f(x, y)がtrueでありf(y, z)がtrueであるならf(x, z)はtrueである
4. equivalenceに対する推移律:xとyがequivalentでありyとzもequivalentであるとき，xとzもequivalentである

ここで初めてequivalenceという言葉が出てきる．xとyがequivalentであるというのは，f(x, y)がfalseでありかつf(y, x)がfalseである，ということだ．  

具体的な例で見てみる．厳密で弱い順序として一番有名なのが「大文字小文字を無視した文字列の大小比較」だ．この文字列比較の実装は簡単なので省略して，この順序（文字列として小さいほうが前にきたらtrue）を定義した述語をci_lessとする．
```cpp
assert( ci_less("ABC", "acb") );
```
さて問題は"ABC"と"aBc"にこのci_lessで順序をつけるとどうなるか，だ．当然，ci_less("ABC", "aBc")もci_less("aBc", "ABC")も両方falseを返す．

ここで，全順序の定義の最後の条件を思い出してほしい．全順序の定義では上のように双方の比較がfalseだった場合，その値は等しい（equal）ことが必要でした．ところがci_lessによる順序付けができないこの"ABC"と"aBc"は値が等しいとはいえない．要するにci_lessは，異なる値である"ABC"と"aBc"の大小を決定できないのだ．

この，ci_lessにおける"ABC"と"aBc"の比較のように，値として等しくないにもかかわらず，双方の順序関係が定義できないような比較が存在する．つまり，値は違うけれどもある順序付けによっては「等しいとみなされる」値の関係が存在する．これが"Equivalence"だ．例えば上の例で言えば，ci_lessにおいては"ABC"と"aBc"はequivalentである，というふうに言うことができる．

さて，もう一度厳密で弱い順序の定義に戻る．

1. 非反射律:f(x, x)は常にfalseである
2. 非対称律:xとyが等しくないとき，f(x, y)がtrueならば!f(y, x)もtrueである
3. 推移律:f(x, y)がtrueでありf(y, z)がtrueであるならf(x, z)はtrueである
4. equivalenceに対する推移律:xとyがequivalentでありyとzもequivalentであるとき，xとzもequivalentである

実はさっき例に出したci_lessは4番目の条件を満たす．具体的に言うと，ci_lessでは"abc"と"abC"の順序を決められない．同時に，"abC"と"aBc"の順序も決められない．このとき，"abc"と"aBc"の順序も決められない，というのが最後の条件だ．

このweak ordering の最後の条件である.  
4. equivalenceに対する推移律:xとyがequivalentでありyとzもequivalentであるとき，xとzもequivalentである  

という条件はなぜ必要なのか？
1~3の条件だけでは何が困るのか？

結論をいうと,この条件はソートの事後条件として非常に重要である.  
ソートは隣接要素を比較するため  
```
a < b , b と c が equivalent -> a < c
```
とならなければならない.  
このことが実はweak orderingの最後の条件と同値なのである.  

すべての隣接要素を比較するようなソートでは問題ないが,クイックソートのようにすべての隣接要素を比較しないソートではソートが正しく行われなくなってしまうためこの条件が必要となる.  
言い換えればソートの複雑性が`O(NlogN)`であるための必要十分条件がequivalenceに対する推移律ということになる.  

####区間の順序問題[Interval Ordering Problem]

区間の順序について考えることにしよう.  
区間の順序の付け方については3つの候補が考えられる.  
```
X = [ a , b ] , Y = [ c , d ]　について

1. X < Y iff b < c
2. X < Y iff c < a < b < d
3. X < Y iff a < c ∧ b < d
```
1.はInterval Orderingと呼ばれており,区間拡張された順序といえる.  
もう少し厳密に定義すると,
```
X < Y iff ∀x,y (x < y)
```
という定義である.  

2.は包含関係である.  
つまり
```
X < Y iff X∈Y
or
X < Y iff ∃x∀y , (x=y)
```
という定義だ.  

3.は1.を弱めた定義で
```
X < Y iff ∃x∀y, (x<y) ∨　∃x∀y, (x>y)
```
という定義である.  

証明は割愛する(簡単な反例を挙げるだけだが)がこの3つの定義すべてがpartial orderingになっている.  
ソートにはweak orderingが必要なのでこれらを直接用いることはできない.  

そこで,本ライブラリでは3.をpartial orderingとして採用しweak orderingとtotal orderingを定義する.  
1.は重要な概念のためinterval_orderingとして採用する.  
2.の順序が使用したいときはAdvenced Ordering Func が提供される.  


weak orderingでは
条件を
```
X < Y iff a < c
X > Y iff a > c
```
とする.  
厳密にいうと
```
X < Y iff ∃x∀y, x < y
X > Y iff ∃y∀x, y < x
```
と定義する.  

total orderingでは
```
X < Y iff if(a=c) -> b < d else -> a < c  
X > Y iff if(a=c) -> b < d else -> a < c  
```
とする.  

簡単に証明をしておきます.  
厳密な証明をすると集合論の王国へ入国してしまうのでここではしません.
以下の証明では実数の順序
```
x < y
x > y
```

がtotal orderingであることを自明として証明を行います.  
このことを議論するのは楽しいですが,非常に厄介なので避けます.  
また,
XとYがequivalentであることを
```
X EQ Y
```
とあらわし,
XとYがequalityであることは
```
a = b ∧ b = d iff X = Y
```  
と定義します.


weak ordering がequivalenceに対する推移律
```
X EQ Y ∧ Y EQ Z ならば X EQ Z
```
を満たすことの証明を行います.  
```
X < Y iff a < c
X > Y iff a > c
とすると
X EQ Y
であるためには
X < Y = false
X > Y = false
でなければならないので
a < c = false かつ
a > c = false
が必要となる.
よって
total orderingの4つ目の条件より
a = c
が条件となる

区間X=[a,b],Y=[c,d],Z=[e,f]において
X EQ Y -> a = d (1)
Y EQ Z -> c = e (2)
となり
(1),(2)より
a = e
である
これは
X EQ Z
の条件
a = e
を満たす.
よって,
X EQ Y ∧ Y EQ Z ならば X EQ Z
である
よって,weak orderingの4つめの条件を満たす
Q.E.D.

```
つぎに,total orderingが4つめの条件
```
反対称律:less(x,y)=false かつ less(y,x)=falseならば x=y
```
を満たすことを証明する.  
```
X < Y iff if(a=c) -> b < d else -> a < c  
X > Y iff if(a=c) -> b > d else -> a > c  
とすると
a≠cと仮定すると
X < Y = false かつ
X > Y = false
であるためには
a < c ∧ a > d
でなければならない
a < c ∧ a > d　ならば　a=c
であるため
これは仮定と矛盾する
よって
a=cでなければならず
その場合
b < d ∧ b > d
でなけれならい
よって
a=c ∧ b=d
となる

これはtotal orderingの4つ目の条件を満たす
Q.E.D.

```

つまらない話はこれくらいにして実装の話に移ろう.  
まとめると以下のようになる.  

```
X = [ a , b ] , Y = [ c , d ]
とする。

interval_less(X,Y)
b < c のとき true

interval_greater(X,Y)
a < d のとき true

interval_unordered(X,Y)
interval_less(X,Y)==false ∧ interval_greater(X,Y)==false のとき true

partial_less(X,Y)
a < c ∧ b < d のとき true

partial_greater(X,Y)
a > c ∧ b > d のとき true

partial_unordered(X,Y)
partial_less(X,Y)==false ∧ partial_greater(X,Y)==false のとき true

weak_less(X,Y)
a < c のとき true

weak_greater(X,Y)
a > c のとき true

weal_equal(X,Y)
a == c のとき true

total_less(X,Y) <- operator<のデフォルト
a < c (a=cならばb < d)  のとき true

total_greater(X,Y) <- operator>のデフォルト
a > c (a=cならばb > d) のとき true

total_equal(X,Y) <- operator=(!=)のデフォルト
a == c ∧ b == d のとき true
```

また,順序の三値を返す関数を提供する.  

```cpp
interval_ordering interval_order(x,y);
partial_ordering partial_order(x,y);
weak_ordering weak_order(x,y);
total_ordering total_order(x,y);
```

この三値はstrong typed enum class で以下のように定義される.  
```cpp
enum class partial_ordering { less, unordered, greater };
enum class weak_ordering { less, equivalent, greater };
enum class total_ordering { less, equal, greater };
enum class interval_ordering { less, unordered, greater };
```

これにより,オーダーの種類を間違えるミスを型安全に防ぐことができる.  

因みに、これらは出力ストリームに出力すると文字列として出力できる.

####Advanced Interval Relational Function

```cpp
x.relational(y)
relational(x,y)
```
と呼び出す.  

この関数を使うと最も特殊化されたxとyの関係を返す.  

最も特殊化されたといってもわかりにくいであろうから具体的に説明すると

```
包含関係を調べる
包含関係でないなら
interval ordering を試す
interval unordered なら
partial ordering を試す
partial unordered なら
weak ordering を試す
equivalence なら
total ordering を試す

```

という具合である.  
要するに,xとyの順序における最も小さな集合を判定している.  

返却値はstrong typed enum class で以下のように定義される.  

```cpp
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
```

なお、これらも出力ストリームで文字列として出力できる.  


包含関係を調べる関数も提供される.
```cpp
x.is_contain(y)
x.is_part_of(y)
```
である.  

```
x = [ a , b ] , y = [ c , d ]

x.is_contain(y)は
y ∈ x
つまり
a <= c <= d <= b
のときにtrueとなる.  

x.is_part_of(y)は
x ∈ y
つまり
c <= a <= b <= d
のときにtrueとなる.  
```

これにて、区間の関数についての解説は終了である.  
次に,例外クラスと例外を送出する可能性のある関数について解説する.  


###例外
