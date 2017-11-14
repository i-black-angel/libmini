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
#include <mini/msocket.h>
#include <mini/merror.h>

MINI_BEGIN_NAMESPACE

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
		perror("socket error");
	}
}

MSocket::~MSocket()
{
	if (_sockfd != -1)
		::close(_sockfd);
}

bool MSocket::bind(uint16_t port)
{
	PHostAddress address(PHostAddress::Any, port);
	return bind(address);
}

bool MSocket::bind(const PHostAddress &address)
{
	if (_sockfd < 0) return false;
	
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(address.ipv4());
	addr.sin_port = htons(address.port());
	int res = ::bind(_sockfd, (const sockaddr *)&addr, sizeof(addr));
	if (res < 0) {
		perror("bind error");
		return false;
	}
	printf ("bind %s success\n", address.toString().c_str());
	return true;
}

MTcpSocket::MTcpSocket()
	: MSocket(TcpSocket)
{
}

MTcpSocket::~MTcpSocket()
{
}

MUdpSocket::MUdpSocket()
	: MSocket(UdpSocket)
{
}

MUdpSocket::~MUdpSocket()
{
}

// int64_t MUdpSocket::sendto(const uint8_t *data, size_t len)
// {
// 	sockaddr_in addr;
// 	memset((uint8_t *)&addr, 0x00, sizeof(addr));
// 	return ::sendto(_sockfd, data, len, 0, (const sockaddr *)&addr, sizeof(addr));
// }

int64_t MUdpSocket::sendto(const uint8_t *data, size_t len, const PHostAddress &host)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(host.ipv4());
	addr.sin_port = htons(host.port());

	return ::sendto(_sockfd, data, len, 0, (const sockaddr *)&addr, sizeof(addr));
}

int64_t MUdpSocket::sendto(const uint8_t *data, size_t len, const std::string &address, uint16_t port)
{
	return sendto(data, len, PHostAddress(address, port));
}

int64_t MUdpSocket::recvfrom(uint8_t *data, size_t len, PHostAddress &host)
{
	sockaddr_in addr;
	socklen_t slen = sizeof(addr);
	
	ssize_t rlen = ::recvfrom(_sockfd, data, len, 0, (sockaddr *)&addr, &slen);

	if (rlen <= 0) {
		std::cerr << mini::error() << std::endl;
		return rlen;
	}
	
	host.setAddress(ntohl(addr.sin_addr.s_addr));
	host.setPort(ntohs(addr.sin_port));

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
			perror("bind()");
			return false;
		}
		if (pipe(_pipefd) < 0) {
			perror("pipe()");
			return false;
		}
		return _init;

	} catch (std::exception e) {

		std::cerr << e.what() << std::endl;

	}

	return false;
}

void MUdpServer::run()
{
	if (!_init) return;
	
	size_t len = 0;
	PHostAddress address;
	int fd = _socket.sockfd();
	int retval = 0;
	
	for (; ;) {
		FD_ZERO(&_rfds);
		FD_SET(fd, &_rfds);
		FD_SET(_pipefd[0], &_rfds);

		_maxfds = std::max(fd, _pipefd[0]);

		retval = select(_maxfds + 1, &_rfds, NULL, NULL, NULL);
		if (retval < 0) {
			perror("select()");
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

void MUdpServer::process(const uint8_t *data, size_t len, const PHostAddress &host)
{
}

MINI_END_NAMESPACE
