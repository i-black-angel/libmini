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
#include <sys/epoll.h>

using namespace std;

class MouseServer : public mpl::MTcpServer
{
public:
    explicit MouseServer() { }
    virtual ~MouseServer() { }
protected:
	virtual void connection(int clientfd, const mpl::MHostAddress &addr) {
	}
	virtual void process(int clientfd, const uint8_t *data, size_t len) {
		char buf[1024] = {0x00};
		memcpy(buf, data, len);
		printf("clientfd:%d -- %s", clientfd, buf);
	}
};

int main(int argc, char *argv[])
{
	MouseServer server;
	server.bind(12700);
	server.start();

	MouseServer ftpserver;
	ftpserver.bind(12780);
	ftpserver.start();

	while (true) {
		sleep(1);
	}
	// mpl::MSocket sock(mpl::MSocket::TcpSocket);
	// sock.bind(12700);
	// sock.listen();

	// while (true) {
	// 	mpl::MHostAddress addr;
	// 	int fd = sock.accept(addr);
	// 	std::cout << "accept " << addr.toString() << " fd:" << fd << std::endl;
	// 	std::cout << "getsockname " << sock.sockname().toString() << std::endl;
	// 	std::cout << "getpeername " << sock.peername(fd).toString() << std::endl;
	// }
    return 0;
}
