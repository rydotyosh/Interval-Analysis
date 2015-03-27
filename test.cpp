#include <iostream>
#include <future>
#include <mutex>
#include <thread>
#include <vector>
#include <cmath>
#include <memory>
#include <atomic>
#include <numeric>
#include "Interval.h"

using std::cout;
using std::endl;

int main()
{
	std::shared_ptr<interval> obj1(new interval(1, 3));
	std::shared_ptr<interval> obj2(new interval(2, 4));
	std::shared_ptr<int> obj3(new int(2));
	std::shared_ptr<int> obj4(new int(3));
	std::vector<interval> v(10);
	
	auto obj5 = obj1 + obj2;
	auto obj6 = obj3 * obj4;
	cout << func.f(v) << endl;
	cout << (static_cast<int>(obj1->relation(*obj2))) << endl;
	cout << obj5 << endl;
	cout << obj6 << endl;

	return 0;
}