#include <iostream>
#include <iomanip>
#include <cmath>

int main ()
{
	std::cout << std::setprecision(25);
	double a = 2.0;
	double b = 3.0;
	auto up = nextafter(a/b,9999999999999);
	auto low = nextafter(a/b,-9999999999999);
	std::cout << "up : " << up << std::endl;
	std::cout << "low : " << low << std::endl;
}
