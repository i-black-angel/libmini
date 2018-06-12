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

class MouseServer : public minion::MTcpServer
{
public:
    explicit MouseServer() { }
    virtual ~MouseServer() { }
protected:
	virtual void process(int clientfd, const minion::MHostAddress &addr) {
		std::cout << clientfd << " - " << addr.toString() << std::endl;
	}
};

int main(int argc, char *argv[])
{
	MouseServer server;
	server.bind(12700);
	server.start();

	while (true) {
		sleep(1);
	}
	// minion::MSocket sock(minion::MSocket::TcpSocket);
	// sock.bind(12700);
	// sock.listen();

	// while (true) {
	// 	minion::MHostAddress addr;
	// 	int fd = sock.accept(addr);
	// 	std::cout << "accept " << addr.toString() << " fd:" << fd << std::endl;
	// 	std::cout << "getsockname " << sock.sockname().toString() << std::endl;
	// 	std::cout << "getpeername " << sock.peername(fd).toString() << std::endl;
	// }
    return 0;
}
