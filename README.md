Interval-Analysis
=================
数値解析に用いる目的で区間演算のライブラリを開発する。  
実数の閉区間についてのコードを開発している。  
整数の区間の完全なサポートや開区間のサポートは後回しになる。  
区間のライブラリとしては、boost::intervalがある  
これはsin等の超越関数が使えない。また、コードが古くさいと言わざるをえない。
boostよりモダンコードでよりで便利な関数をサポートすることを目標として開発している。
開発環境はVisual Studio 2013 ,コンパイラは MSVC(VC12) と clang3.5で動作確認している。
基本的にコンパイラには高い規格準拠度を求める。
