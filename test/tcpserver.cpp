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

		fd_set rfds;
		int maxfds;
		int retval = 0;
		uint8_t buf[1024] = {0x00};
		size_t bufsize = sizeof(buf);
		
		for (; ;) {
			FD_ZERO(&rfds);
			FD_SET(clientfd, &rfds);

			maxfds = clientfd;

			retval = select(maxfds + 1, &rfds, NULL, NULL, NULL);
			std::cout << "retval: " << retval << std::endl;
			
			if (retval < 0) {
				log_error("select: %s", minion::error().c_str());
				break;
			} else if (retval) {
				if (FD_ISSET(clientfd, &rfds)) {
					ssize_t len = recv(clientfd, buf, bufsize, 0);
					if (0 == len) {
						// client is close
						std::cout << "close: " << len << std::endl;
						ssize_t slen = send(clientfd, buf, bufsize, 0);
						std::cout << "send: " << slen << std::endl;
						close(clientfd);
						break;
					} else if (len > 0) {
						std::cout << "recv: " << len << std::endl;
					}
				}
			}
		} // for
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
