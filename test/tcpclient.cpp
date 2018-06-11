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
#include <minion.h>

using namespace std;

int main(int argc, char *argv[])
{
	minion::MSocket sock(minion::MSocket::TcpSocket);
	minion::MHostAddress address("localhost", 12700);

	sock.connect(address);

	sock.close();
	// sock.bind(12700);
	// sock.listen();

	// while (true) {
	// 	sleep(1);
	// }
    return 0;
}
