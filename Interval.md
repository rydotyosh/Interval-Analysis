#Systax and Features of Interval Library
---
##はじめに[]
本ライブラリの開発環境はVisual Studio 2013 Ultimate(compiler=VC12)です.  
clangでコンパイルすることはありますが、基本的にしていないと心得てください.  
記載されたコードはファイルインクルードや名前空間の修飾が省略されており、そのままではほぼ動作しません.  
あらかじめ了承ください.  

##まえがき
これ必要かな...
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
区間の四則演算、数学関数、区間の関係性判定など.  

Interval.h に宣言、Interval.cpp に定義が記述されている.  
すべてのコードはInterval名前空間にはいっている.

##クラス宣言[Class Declaration]

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
第1にinterval classは内部クラスに上限（upper bound）と下限（lower bound）を保持する.  
interval classは内部クラスimplのインスタンスをスマートポインタで保持する.  

第2にinterval class はクラステンプレートになっており、宣言や定義にはテンプレート仮引数が必要である.  
区間の上下限をどの型で保持するのかを指定しなければならない.
```cpp
auto x = interval<double>( ) ; // OK
auto x = interval( ) ; // error! needs to template argument.

```
とはいえ、テンプレート引数を明示的に指定するのは甚だ面倒である.  
そこで、hullをつかう. 引数から型が推測されinterval classが返される.
```cpp

auto x = hull( 1.0 , 2.0 ) ; // x は interval<double>

```
デフォルトコンストラクタ、2引数（左辺値）コンストラクタ、2引数（右辺値）コンストラクタ  
コピーコンストラクタ、ムーブコンストラクタ、コピー代入演算子が使える.  
```cpp
// デフォルトコンストラクタ 上下限はdouble()で初期化される
auto v = interval<double>( ) ;

// 2引数コンストラクタ 上下限は double(1.0), double(2.0)で初期化される
// つまり暗黙の型変換が起こるため注意されたし
auto w = interval<double>( 1.0 , 2.0 ) ;

// コピーコンストラクタ 上下限が v.low_bound v.upper_bound で初期化される
// つまり深いコピーが行われ、内部クラスは共有しない
auto x(v) ;

// コピー代入演算子 まずデフォルトコンストラクタが呼ばれ
// y.low_bound = x.low_bound , y.upper_bound = x.upper_bound となる
auto y = x ;
```
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

###区間のオーバーロードされた演算子[Interval Overloaded Functions]

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
Relational operator については、関数のrelational functionsの項目で桑あしく解説するのでそちらを参照してもらいたい.  
四則演算については区間同士の演算でしかも複合代入演算子のみ再定義される.  
その他はすべてグローバル関数で定義されている.  
`operator+`を例に挙げる.  
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

##区間関数[Interval Functions]
区間の関数は大きく分けて3種類ある.
数学関数、setter / getter、関係性だ.  
そのすべてを以下に列挙する.  

```cpp
template<typename T>
class interval
{
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
	Interval_Relation relational(interval const&) const;
}

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
整数乗のみの実装である.  
実数乗は実装されていない.  
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

###セッターとゲッター[Setter and Getter]
####setter
区間の上下限は`set_up()`と`set_low()`を使って再設定できる.  
このとき上限が下限を下回る、または下限が上限を上回るような呼び出しをした場合には  
Interval::invalid_errorがthrowされる.  

```cpp
auto x = hull( 2.0 , 3.0 ) ;
x.set_low( 1.0 ) ; // OK
x.set_up( 0.0 ) ; // error [ 1.0 , 0.0] is invalid!
```
####getter
区間の上下限は`get_up()`と`get_low()`を使って取得できる.  
```cpp
auto x = hull( 0.0 , 1.0 ) ;
auto up = x.get_up() ; // up = x.upper_bound
auto low = x.get_low() ; // low = x.low_bound
```

###区間の関係性関数[Interval Relational Functions]
区間の関係は大小関係だけでなく包含関係もあり複雑である.  
大小比較にはtotal、weak、partialの3種類があり、
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
Interval::partial_ordering partial_order(Interval::interval<T>& x, Interval::interval<T>& y)
{
	if (partial_less(x, y)){ return partial_ordering::less; }
	else if (partial_greater(x, y)){ return partial_ordering::greater; }
	else{ return partial_ordering::unordered; }
}
template<typename T>
Interval::weak_ordering weak_order(Interval:: interval<T>& x, Interval::interval<T>& y)
{
	if (weak_less(x, y)){ return weak_ordering::less; }
	else if (weak_greater(x, y)){ return weak_ordering::greater; }
	else{ return weak_ordering::equivalent; }
}
template<typename T>
Interval::total_ordering total_order(Interval::interval<T>& x, Interval::interval<T>& y)
{
	if (total_less(x, y)){ return total_ordering::less; }
	else if (total_greater(x, y)){ return total_ordering::greater; }
	else{ return total_ordering::equal; }
}
template<typename T>
Interval::interval_ordering interval_order(Interval::interval<T>& x, Interval::interval<T>& y)
{
	if (interval_less(x, y)){ return Interval::interval_ordering::less; }
	else if (interval_greater(x, y)){ return Interval::interval_ordering::greater; }
	else{ return Interval::interval_ordering::unordered; }
}

```

これらの挙動を解説する前に、なぜこのような種類の関数が用意されているのかを説明せねばならない.  
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
1. （非）反射性：f(x, x)は常にfalseである
2. （非）対称性：f(x, y)がtrueならば!f(y, x)もtrueである
3. 推移性：f(x, y)がtrueでありf(y, z)がtrueであるならf(x, z)はtrueである
4.  f(x, y)がfalseでありf(y, x)もfalseであるならば，xとyは等しい(equal)

よく分からないので具体的な全順序の例としてless<int>に登場してもらって，条件の意味を順に追っていく．

（非）反射性
```cpp
int i = 3;
assert( !less<int>()(i, i) );
```
要するに同じ値に対して常にfalseを返す，というのが一つ目の条件「（非）反射性」である．

この条件に関して非常に重要な指摘として，less_equal<int>やgreater_equal<int>はこの条件を満たさないということだ．これらの「順序」は同じ値に対してtrueを返してしまう．このような「順序」はSTLにおける比較の条件として絶対に使えない.  


（非）対称性
```cpp
int i;
int j;
...
if( less<int>()(i, j) ){
  assert( !less<int>()(j, i) );
}
```
これは「順序」という言葉から直感的に分かる条件だ．具体的には例えば「aはbより小さい」なら「bはaより小さくない」と言えるのは当然でしょう，ということだ．

推移性
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

最後の条件
```cpp
int i, j;
...
if( !less<int>()(i, j) && !less<int>()(j, i) ){
  assert( i == j );
}
```
これも分かりやすい条件だ．「iがjよりも小さいことはない」かつ「jがiよりも小さいことはない」なら「iとjは同じ値である」ということは，通常の順序なら当然でしょう．そもそも，「全」順序という言葉は異なる2つの要素x, yの間全てにf(x, y)またはf(y, x)が成り立つことから来ている名前で，そのどちらも成り立たないということは，そもそもxとyは同じ要素だったのだ，ということが言える．

さて，これを踏まえた上で，次に厳密で弱い順序の定義を見ていく．

厳密で弱い順序では全順序の最後の条件だけが緩められている．

1. （非）反射性：f(x, x)は常にfalseである
2. （非）対称性：xとyが（equalityの意味で）等しくないとき，f(x, y)がtrueならば!f(y, x)もtrueである
3.  推移性：f(x, y)がtrueでありf(y, z)がtrueであるならf(x, z)はtrueである
4. equivalenceに対する推移性：xとyがequivalentでありyとzもequivalentであるとき，xとzもequivalentである

ここで初めてequivalenceという言葉が出てきる．xとyがequivalentであるというのは，f(x, y)がfalseでありかつf(y, x)がfalseである，ということだ．  

具体的な例で見てみる．厳密で弱い順序として一番有名なのが「大文字小文字を無視した文字列の大小比較」だ．この文字列比較の実装は簡単なので省略して，この順序（文字列として小さいほうが前にきたらtrue）を定義した述語をci_lessとする．
```cpp
assert( ci_less("ABC", "acb") );
```
さて問題は"ABC"と"aBc"にこのci_lessで順序をつけるとどうなるか，だ．当然，ci_less("ABC", "aBc")もci_less("aBc", "ABC")も両方falseを返す．

ここで，全順序の定義の最後の条件を思い出してほしい．全順序の定義では上のように双方の比較がfalseだった場合，その値は等しい（equal）ことが必要でした．ところがci_lessによる順序付けができないこの"ABC"と"aBc"は値が等しいとはいえない．要するにci_lessは，異なる値である"ABC"と"aBc"の大小を決定できないのだ．

この，ci_lessにおける"ABC"と"aBc"の比較のように，値として等しくないにもかかわらず，双方の順序関係が定義できないような比較が存在する．つまり，値は違うけれどもある順序付けによっては「等しいとみなされる」値の関係が存在する．これが"Equivalence"だ．例えば上の例で言えば，ci_lessにおいては"ABC"と"aBc"はequivalentである，というふうに言うことができる．

さて，もう一度厳密で弱い順序の定義に戻る．

1. （非）反射性：f(x, x)は常にfalseである
2. （非）対称性：xとyが等しくないとき，f(x, y)がtrueならば!f(y, x)もtrueである
3.  推移性：f(x, y)がtrueでありf(y, z)がtrueであるならf(x, z)はtrueである
4. equivalenceに対する推移性：xとyがequivalentでありyとzもequivalentであるとき，xとzもequivalentである

実はさっき例に出したci_lessは4番目の条件を満たす．具体的に言うと，ci_lessでは"abc"と"abC"の順序を決められない．同時に，"abC"と"aBc"の順序も決められない．このとき，"abc"と"aBc"の順序も決められない，というのが最後の条件だ．

このweak ordering の最後の条件である.  
4. equivalenceに対する推移性：xとyがequivalentでありyとzもequivalentであるとき，xとzもequivalentである  

という条件はなぜ必要なのか？
1~3の条件だけでは何が困るのか？

結論をいうと、この条件はソートの事後条件として非常に重要である.  
ソートは隣接要素を比較するため  
```
a < b , b と c が equivalent -> a < c
```
とならなければならない.  
このことが実はweak orderingの最後の条件と同値なのである.  

すべての隣接要素を比較するようなソートでは問題ないが、クイックソートのようにすべての隣接要素を比較しないソートではソートが正しく行われなくなってしまうためこの条件が必要となる.  
言い換えればソートの複雑性が`O(NlogN)`であるための最低限の条件がequivalenceに対する推移性ということになる.  

####区間の順序問題[Interval Ordering Problem]

区間の順序について考えることにしよう.  
区間の順序の付け方については3つの候補が考えられる.  
```
X = [ a , b ] , Y = [ c , d ]　について

1. X < Y iff b < c
2. X < Y iff c < a < b < d
3. X < Y iff a < c ∧ b < d
```
1.はInterval Orderingと呼ばれており、自然に区間拡張された順序といえる.  
もう少し厳密に定義すると、
```
X < Y iff ∀x,y (x < y)
```
という定義である.  

2.は包含関係である.  
つまり
```
X∈Y
or
∃x∀y , (x=y)
```
という定義だ.  

3.は1.を弱めた定義で
```
∃x∀y, (x<y) ∨　∃x∀y, (x>y)
```
という定義である.  

証明は割愛する(簡単な反例を挙げるだけだが)がこの3つの定義すべてがpartial orderingになっている.  
ソートにはweak orderingが必要なのでこれらを直接用いることはできない.  

そこで、本ライブラリでは3.をpartial orderingとして採用しweak orderingとtotal orderingを定義する.  
また、1.は重要な概念のためinterval orderingとして採用する.  


weak orderingでは
条件を
```
a < c iff X < Y
```
とする.  
厳密にいうと
```
∃x∀y,x<y iff X <Y
```
と定義する.  

total orderingでは
```
a < c ∨ b < d iff X < Y
```
とする.  
厳密にいうと
```
∃x∀y,x<y ∨ ∃x∀y,x>y iff X <Y
```
と定義します.  

簡単に証明をしておきます.  
厳密な証明をすると集合論の王国へ入国してしまうのでここではしません.
以下の証明では実数の順序
```
x < y
```

がtotal orderingであることを自明として証明を行います.  
このことを議論するのは楽しいですが、非常に厄介なので避けます.  
また
XとYがequivalentであることを
```
X EQ Y
```
とあらわし、
XとYがequalityであることは
```
a = b ∧ b = d iff X = Y
```  
と定義します.


weak ordering がequivalenceに対する推移性を満たすことの証明を行います.  
```
a < c iff X < Y
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
Q.E.D.

```
つぎに、total orderingが4つめの条件
```
less(x,y)=false かつ less(y,x)=falseならば x=y
```
を満たすことを証明する.  
```
a < c ∨ b < d iff X < Y
とすると
X < Y = false かつ
X > Y = false
であることは
￢(X < Y) = true かつ
￢(X > Y) = true
と同値である.

ド・モルガンの法則により
X < Y と X > Yの補集合を考える.
￢(X < Y)
={ ￢(a < c ∨ b < d) }
={ ￢(a < c) ∧ ￢(b < d) }
={ a>=c ∧ b >=c } (1)
である.  
同様に
￢(X > Y)
={ a<=c ∧ b <=c } (2)
(1),(2)を同時に満たすためには
a=c ∧ b=d
でなければならない
つまり
X = Y
である.
よって、
X < Y = false ∧ X > Y = false ならば X=Y
である.
これはtotal orderingの4つ目の条件を満たす
Q.E.D.

```
