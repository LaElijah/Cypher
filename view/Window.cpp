#include <iostream>
#include <string>

using namespace std;

namespace View
{
	class Test
	{
		public:
    string name;
		Test(string input)
		{
			name = input;
		}
    static void announce()
    {
    cout << "I EXIST, KNOW ME" << endl;
    }
	};

	int main() 
	{

		Test test("hey");

		cout << test.name << endl;
		return 0;
	}
}

