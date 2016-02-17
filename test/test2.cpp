#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

int main()
{
	std::vector<int> c = {1, 2, 3, 4, 5, 6, 7};
	int *xx = new int;
	*xx = 7;

	struct {
		int *x;
		char b[0x20];
	} vv;

	vv.x = xx;

	int x = 5;
	c.erase(std::remove_if(c.begin(), c.end(), [x](int n) { return n < x; }), c.end());

	std::cout << "c: ";
	std::for_each(c.begin(), c.end(), [](int i){ std::cout << i << ' '; });
	std::cout << '\n';

	// the type of a closure cannot be named, but can be inferred with auto
	auto func1 = [](int i) { return i + 4; };
	std::cout << "func1: " << func1(6) << '\n';

	// like all callable objects, closures can be captured in std::function
	// (this may incur unnecessary overhead)
	std::function<int(int)> func2 = [](int i) { return i + 4; };
	std::cout << "func2: " << func2(6) << '\n';
	
	auto func3 = [&c](int *i) { return *i; };
	std::cout << "func3: " << func3(&x) << '\n';

	auto func4 = [&](int i) { return *vv.x + *xx; };
	std::cout << "func4: " << func4(x) << '\n';

	auto func5 = [xx](int i){ return i + *xx; };
	std::cout << "func5: " << func5(x) << '\n';

	auto func6 = [=](int i){ return *vv.x + *xx; };

	delete xx;

}
