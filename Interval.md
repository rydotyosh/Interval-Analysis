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
第1に区間は内部クラスに上限（upper bound）と下限（lower bound）を保持します.  

第2にinterval class はクラステンプレートになっており、宣言や定義にはテンプレート仮引数が必要です.  

		auto x = interval<double>() ; // OK
		auto X = interval() ; // error! needs to template argument.

デフォルトコンストラクタ、2引数（左辺値）コンストラクタ、2引数（右辺値）コンストラクタ  
コピーコンストラクタ、ムーブコンストラクタ、コピー代入演算子が使えます.  

