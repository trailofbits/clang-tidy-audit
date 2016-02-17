#include <iostream>
#include <list>

std::list<unsigned int> integer_list;

void populate_list()
{
	for (unsigned int i = 0; i < 0x100; i++)
	{
		unsigned short rr = arc4random() % 0x100;
		integer_list.push_back(rr);
	}
}

int main(int argc, char **argv)
{
	
	if (argc != 2)
	{
		return 1;
	}

	populate_list();

	unsigned int check = strtoul(argv[1], nullptr, 0);
	unsigned int count = 0;

	for (auto I = integer_list.begin(); I != integer_list.end(); ++I)
	{
		if (*I == check)
		{
			count++;
		}
	}

	std::cout << "Found " << count << " instances of " << check << "." << std::endl;

	count = 0;
	for (auto I : integer_list)
	{
		if (I == check)
		{
			count++;
		}
	}

	std::cout << "Found " << count << " instances of " << check << "." << std::endl;

	return count == 0;

}
