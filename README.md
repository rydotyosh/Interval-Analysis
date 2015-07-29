Cranberries Interval-Library for C++
=================
精度保証つき大域最適化に用いる目的でC++区間演算のライブラリを開発する.  
実数の閉区間についてのコードを開発している.  
整数の区間の完全なサポートや開区間演算, 複素数区間のサポートは後回しになる.  


C++の区間ライブラリとしては,Boost.Numeric.Intervalがある.  
Boost.Numeric.Intervalでは,比較演算子が例外を送出するために非常につかいにくい.  
Cranberries Interval Libraryではtotal ordering,weak orderingを満たす比較法を提案している.  
本ライブラリは,よりモダンで,より便利なライブラリを目標として開発している.  

開発言語はC++14  
開発環境は現在Visual Studio 2015 RC  
コンパイルは  
VC14 or later  
gcc5.1.0 or later  
clang 3.4.0 or later  
で確認している.  
基本的にコンパイラには高い規格準拠度を求める.  
