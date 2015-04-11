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
第1にinterval classは内部クラスに上限（upper bound）と下限（lower bound）を保持します.  
interval classは内部クラスimplのインスタンスをスマートポインタで保持する.  

第2にinterval class はクラステンプレートになっており、宣言や定義にはテンプレート仮引数が必要である.  
区間の上下限をどの型で保持するのかを指定しなければならない.    

		auto x = interval<double>( ) ; // OK
		auto X = interval( ) ; // error! needs to template argument.

とはいえ、いちいちテンプレート引数を明示的に指定するのは甚だ面倒である.  
そこで、hullをつかう. すると初期の上下限から型が推測されinterval classが返される.


		auto x = hull( 1.0 , 2.0 ) ; // x は interval<double>


デフォルトコンストラクタ、2引数（左辺値）コンストラクタ、2引数（右辺値）コンストラクタ  
コピーコンストラクタ、ムーブコンストラクタ、コピー代入演算子が使えます.  

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

区間の定義の方法とコンストラクタ等のポリシーの解説は以上である.  
次に区間演算について解説する.  


##区間の四則演算

区間演算の定義について確認する.  

区間 X は連続する数の集合であると定義され. 上限と下限から

		X = [ a , b ]

と表す.  

ある実数の演算子●を考える. 区間 X , Y についてこの演算子は

		X ● Y = { x ● y |　x∈X, y∈Y }

と定義される. これを区間拡張という.  
このライブラリには区間拡張された演算子や関数が含まれる.  

###区間のオーバーロードされた演算子

interval class で再定義された演算子はこちら.

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

四則演算については区間同士の演算でしかも複合代入演算子のみ再定義される.  
その他はすべてグローバル関数で定義されている.  
operator+　を例にとる以下のようなコードだ  

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

つまり、`interval<U> + T` や `T + interval<U>` が可能である.  
返り値は`interval<U>`なので`T`は`U`に型変換されることに注意が必要だ.  
また、パフォーマンスを優先して複合代入演算子を呼び出すか、直感的にわかりやすいコードか、いずれかを選択することができる.  
つまり、こういうことだ.  

		interval<double> a , b , c ;
		
		// pattern 1
		auto x = a + b + c ;
		
		// pattern 2
		auto y(a) ;
		y+=b ;
		y+=c ;
		
パターン1はわかりやすいし数学ではこう書く.  
このとき、コードはどう解釈されるか？
答えはこうだ

		x = interval<double>( ) ;
		interval<double> tmp1 = a + b ;
		interval<double> tmp2 = tmp1 + c ;
		x = tmp2 ;

デフォルトコンストラクタがxを初期化する.  
次に左結合性に従い１つづつオブジェクトがたされ一時オブジェクトがスタックに入る.  
つxに代入したあとにスコープを抜けると一時オブジェクトは２つ破棄される.  
実にたくさんの無駄がある.  

パターン2はどうか

		y = interval<doubel>( a ) ;
		y += b ;
		y += c ;

まずコピーコンストラクタが呼び出されている.  
次に複合代入演算子を使うので新しいオブジェクトはできない.  
yだけが新しいオブジェクトなのでこれが最小のコストだといえる.  

ただし、コードは3行にわたっているし、わかりにくい.  

四則演算についてまとめると以下のようになる.  

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

注意が必要なのは複合代入演算子である.  

		T += interval<T>

はコンパイルエラーだ. 理由は少し考えればわかる.  
複合代入演算子は呼び出し元のオブジェクトを返すので.  
`T += interva<T>`の返り値は`T`である.  
しかし区間演算の結果は区間でなければならない.  
矛盾している. したがってこのようなよびだしはしてはならない.

   