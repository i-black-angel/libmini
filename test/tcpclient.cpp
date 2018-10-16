#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <locale>
#include <cerrno>
#include <mpl.h>

using namespace std;

int main(int argc, char *argv[])
{
	mpl::MSocket sock;
	mpl::MHostAddress address("localhost", 12700);

	sock.socket();
	sock.connect(address);
	std::cout << sock.sockname().toString() << std::endl;

    return 0;
}
