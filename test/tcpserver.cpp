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
	sock.bind(12700);
	sock.listen();

	while (true) {
		minion::MHostAddress addr;
		int fd = sock.accept(addr);
		std::cout << "accept " << addr.toString() << " fd:" << fd << std::endl;

		struct sockaddr_in addrin;
		socklen_t len = sizeof(addrin);
		getpeername(fd, (struct sockaddr *)&addrin, &len);
		minion::MHostAddress peeraddr(&addrin);
		std::cout << "getpeername " << peeraddr.toString() << std::endl;
	}
    return 0;
}
