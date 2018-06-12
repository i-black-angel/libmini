/**
 * Copyright 2017 Shusheng Shao <iblackangel@163.com>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <minion/msocket.h>
#include <minion/merror.h>
#include <minion/mlog.h>

MINION_BEGIN_NAMESPACE

int minion_sendto(int fd, const void *buf, size_t n,
				  int flags,
				  const struct sockaddr *addr,
				  socklen_t addr_len)
{
#ifdef M_OS_WIN
	return ::sendto(fd, (const char *)buf, n, flags, addr, addr_len);
#else
	return ::sendto(fd, buf, n, flags, addr, addr_len);
#endif
}

int minion_recvfrom(int fd, void *buf, size_t n,
					int flags,
					struct sockaddr *addr,
					socklen_t *addr_len)
{
#ifdef M_OS_WIN
	return ::recvfrom(fd, (char *)buf, n, flags, addr, addr_len);
#else
	return ::recvfrom(fd, buf, n, flags, addr, addr_len);
#endif
}

MSocket::MSocket(SocketType socketType)
{
	int type = SOCK_STREAM;
	
	_socketType = socketType;
	switch (_socketType) {
	case TcpSocket:
		type = SOCK_STREAM;
		break;
	case UdpSocket:
		type = SOCK_DGRAM;
		break;
	case SctpSocket:
		type = SOCK_SEQPACKET;
	default:
		type = SOCK_RAW;
		break;
	}
	_sockfd = socket(AF_INET, type, 0);
	if (_sockfd < 0) {
		log_error("socket error: %s", error().c_str());
	}
}

MSocket::MSocket(int sockfd)
{
	_sockfd = sockfd;
}

MSocket::~MSocket()
{
	close();
}

int MSocket::accept(MHostAddress &hostAddr)
{
	if (_sockfd < 0) return -1;

	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	int clientfd = ::accept(_sockfd, (struct sockaddr *)&addr, &len);
	if (clientfd < 0) {
		log_error("accept error: %s", error().c_str());
		return clientfd;
	}

	MHostAddress address(addr);
	hostAddr = address;
	return clientfd;
}

bool MSocket::bind(uint16_t port)
{
	MHostAddress address(MHostAddress::Any, port);
	return bind(address);
}

bool MSocket::bind(const MHostAddress &address)
{
	if (_sockfd < 0) return false;

	int opt = 1;
	setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
	
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(address.ipv4());
	addr.sin_port = htons(address.port());
	int res = ::bind(_sockfd, (const sockaddr *)&addr, sizeof(addr));
	if (res < 0) {
		log_error("bind error: %s", error().c_str());
		return false;
	}
	log_debug ("bind %s success", address.toString().c_str());
	return true;
}

bool MSocket::connect(const std::string &address, uint16_t port)
{
	MHostAddress haddr(address, port);
	return connect(haddr);
}

bool MSocket::connect(const MHostAddress &address)
{
	if (_sockfd < 0) return false;

	// set _sockfd to be non-block
	fcntl(_sockfd, F_SETFL, fcntl(_sockfd, F_GETFL)| O_NONBLOCK);
	
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(address.ipv4());
	addr.sin_port = htons(address.port());	
	int res = ::connect(_sockfd, (const struct sockaddr *)&addr, sizeof(addr));
	if (res < 0) {
		log_error("connect error: %s", error().c_str());
		return false;
	}
	log_debug ("connect %s success", address.toString().c_str());
	return true;
}

bool MSocket::listen(int backlog)
{
	if (_sockfd < 0) return false;

	int res = ::listen(_sockfd, backlog);
	if (res < 0) {
		log_error("listen error: %s", error().c_str());
		return false;
	}
	return true;
}

void MSocket::close()
{
	if (_sockfd != -1) {
		::close(_sockfd);
		_sockfd = -1;
	}
}

MHostAddress MSocket::sockname() const
{
	if (_sockfd < 0) return MHostAddress();

	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	int res = getsockname(_sockfd, (struct sockaddr *)&addr, &len);
	if (res < 0) {
		log_error("getsockname failed: %s", error().c_str());
		return MHostAddress();
	}
	return MHostAddress(addr);
}

MHostAddress MSocket::peername(int fd) const
{
	if (fd < 0) return MHostAddress();

	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	int res = getpeername(fd, (struct sockaddr *)&addr, &len);
	if (res < 0) {
		log_error("getpeername failed: %s", error().c_str());
		return MHostAddress();
	}
	return MHostAddress(addr);
}

bool MSocket::setSocketOption(int optname, const void *optval, socklen_t optlen)
{
	if (_sockfd < 0) return false;
	
	int res = setsockopt(_sockfd, SOL_SOCKET, optname, optval, optlen);

	return (res < 0) ? false : true;
}

MTcpSocket::MTcpSocket()
	: MSocket(TcpSocket)
{
}

MTcpSocket::~MTcpSocket()
{
}

MTcpServer::MTcpServer()
{
	_init = false;
}

MTcpServer::~MTcpServer()
{
}

bool MTcpServer::bind(uint16_t port)
{
	try {
		_init = _socket.bind(port);
		if (!_init) {
			log_error("bind: %s", error().c_str());
			return false;
		}
		_init = _socket.listen();
		if (!_init) {
			log_error("listen: %s", error().c_str());
			return false;
		}
		if (pipe(_pipefd) < 0) {
			log_error("pipe: %s", error().c_str());
			_init = false;
			return false;
		}
		return _init;
	} catch (std::exception e) {
		log_error("%s", e.what());
	}
	return false;
}

void MTcpServer::stop()
{
	static const uint8_t xdata[4] = {0x01, 0x02, 0x03, 0x04};
	write(_pipefd[1], xdata, sizeof(xdata));
	MThread::stop();
}

void MTcpServer::run()
{
	if (!_init) return;

	int fd = _socket.sockfd();
	int retval = 0;

	for (; ;) {
		FD_ZERO(&_rfds);
		FD_SET(fd, &_rfds);
		FD_SET(_pipefd[0], &_rfds);

		_maxfds = std::max(fd, _pipefd[0]);

		retval = select(_maxfds + 1, &_rfds, NULL, NULL, NULL);
		if (retval < 0) {
			log_error("select: %s", error().c_str());
			break;
		}

		if (FD_ISSET(fd, &_rfds)) {
			MHostAddress address;
			int clientfd = _socket.accept(address);
			process(clientfd, address);
		}
		if (FD_ISSET(_pipefd[0], &_rfds)) {
			break;
		}
	}

	close(_pipefd[0]);
	close(_pipefd[1]);
}

void MTcpServer::process(int clientfd, const MHostAddress &addr)
{
}


MUdpSocket::MUdpSocket()
	: MSocket(UdpSocket)
{
}

MUdpSocket::~MUdpSocket()
{
}

int64_t MUdpSocket::sendto(const uint8_t *data, size_t len, const MHostAddress &host)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(host.ipv4());
	addr.sin_port = htons(host.port());

	return ::sendto(_sockfd, data, len, 0, (const sockaddr *)&addr, sizeof(addr));
}

int64_t MUdpSocket::sendto(const uint8_t *data, size_t len, const std::string &address, uint16_t port)
{
	return sendto(data, len, MHostAddress(address, port));
}

int64_t MUdpSocket::recvfrom(uint8_t *data, size_t len, MHostAddress &host)
{
	sockaddr_in addr;
	socklen_t slen = sizeof(addr);
	
	ssize_t rlen = ::recvfrom(_sockfd, data, len, 0, (sockaddr *)&addr, &slen);

	if (rlen <= 0) {
		std::cerr << minion::error() << std::endl;
		return rlen;
	}

	MHostAddress address(addr);
	host = address;

	return rlen;
}

MUdpServer::MUdpServer(int bufsize)
{
	_buf = NULL;
	_bufsize = bufsize;
	_init = false;
}

MUdpServer::~MUdpServer()
{
}

bool MUdpServer::bind(uint16_t port)
{
	if (_bufsize <= 0) return false;
	
	if (_buf != NULL) {
		delete _buf;
		_buf = NULL;
	}

	try {

		_buf = new uint8_t[_bufsize];
		_init = _socket.bind(port);
		if (!_init) {
			log_error("bind: %s", error().c_str());
			return false;
		}
		if (pipe(_pipefd) < 0) {
			log_error("pipe: %s", error().c_str());
			return false;
		}
		return _init;

	} catch (std::exception e) {

		log_error("%s", e.what());

	}

	return false;
}

void MUdpServer::run()
{
	if (!_init) return;
	
	size_t len = 0;
	MHostAddress address;
	int fd = _socket.sockfd();
	int retval = 0;
	
	for (; ;) {
		FD_ZERO(&_rfds);
		FD_SET(fd, &_rfds);
		FD_SET(_pipefd[0], &_rfds);

		_maxfds = std::max(fd, _pipefd[0]);

		retval = select(_maxfds + 1, &_rfds, NULL, NULL, NULL);
		if (retval < 0) {
			log_error("select: %s", error().c_str());
			break;
		} 

		if (FD_ISSET(fd, &_rfds)) {
			len = _socket.recvfrom(_buf, _bufsize, address);
			process(_buf, len, address);
		}
		if (FD_ISSET(_pipefd[0], &_rfds)) {
			break;
		}
	}

	close(_pipefd[0]);
	close(_pipefd[1]);
}

void MUdpServer::stop()
{
	static const uint8_t xdata[4] = {0x01, 0x02, 0x03, 0x04};
	write(_pipefd[1], xdata, sizeof(xdata));
	MThread::stop();
}

void MUdpServer::process(const uint8_t *data, size_t len, const MHostAddress &host)
{
}

MINION_END_NAMESPACE
