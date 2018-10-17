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
#include <mpl/msocket.h>
#include <mpl/merror.h>
#include <mpl/mlog.h>

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

MPL_BEGIN_NAMESPACE

int mpl_sendto(int fd, const void *buf, size_t n,
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

int mpl_recvfrom(int fd, void *buf, size_t n,
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

MSocket::MSocket()
{
}

MSocket::MSocket(const MSocket &copy)
{
	_sockfd = copy._sockfd;
	_socketType = copy._socketType;
}

MSocket::~MSocket()
{
}

MSocket &MSocket::operator=(const MSocket &other)
{
	_socketType = other._socketType;
	_sockfd = other._sockfd;
	return *this;
}

int MSocket::socket(SocketType socketType)
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
	_sockfd = ::socket(AF_INET, type, 0);
	return _sockfd;
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

ssize_t MSocket::send(const void *buf, size_t n)
{
	if (_sockfd < 0) return -1;
	return ::send(_sockfd, buf, n, 0);
}

ssize_t MSocket::recv(void *buf, size_t n)
{
	if (_sockfd < 0) return -1;
	return ::recv(_sockfd, buf, n, 0);
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

bool MSocket::sendbuf(int bufsize)
{
	return setSocketOption(SO_SNDBUF, (const char *)&bufsize, sizeof(int));
}

bool MSocket::recvbuf(int bufsize)
{
	return setSocketOption(SO_RCVBUF, (const char *)&bufsize, sizeof(int));
}

int MSocket::sendbuf() const
{
	int bufsize = 0;
	socklen_t optlen = sizeof(socklen_t);
	::getsockopt(_sockfd, SOL_SOCKET, SO_SNDBUF, &bufsize, &optlen);
	return bufsize;
}

int MSocket::recvbuf() const
{
	int bufsize = 0;
	socklen_t optlen = sizeof(socklen_t);
	::getsockopt(_sockfd, SOL_SOCKET, SO_RCVBUF, &bufsize, &optlen);
	return bufsize;
}

bool MSocket::sendtimeout(int sec, long usec)
{
	struct timeval timeout = {sec, usec};
	return setSocketOption(SO_SNDTIMEO, &timeout, sizeof(struct timeval));
}

bool MSocket::recvtimeout(int sec, long usec)
{
	struct timeval timeout = {sec, usec};
	return setSocketOption(SO_RCVTIMEO, &timeout, sizeof(struct timeval));
}

bool MSocket::nonblock(int block)
{
	if (_sockfd < 0) return false;
#ifdef M_OS_WIN
	return ::ioctlsocket(_sockfd, FIONBIO, (unsigned long *)&val) != -1;
#else
	return ::fcntl(_sockfd, F_SETFL, fcntl(_sockfd, F_GETFL) | (block ? O_NONBLOCK : ~O_NONBLOCK)) != -1;
#endif /* M_OS_WIN */
}
	
bool MSocket::setSocketOption(int optname, const void *optval, socklen_t optlen)
{
	if (_sockfd < 0) return false;
	
	int res = setsockopt(_sockfd, SOL_SOCKET, optname, optval, optlen);

	return (res < 0) ? false : true;
}

bool MSocket::isEqual(const MSocket &sock) const
{
	return (_sockfd == sock._sockfd);
}

bool MSocket::operator==(const MSocket &sock) const
{
	return isEqual(sock);
}

MTcpClient::MTcpClient()
	: _connectFunctor(NULL)
	, _disconnectFunctor(NULL)
	, _arg1(NULL)
	, _arg2(NULL)
	, _init(false)
{
}

MTcpClient::~MTcpClient()
{
}

bool MTcpClient::init(const std::string &addr, int16_t port,
					  ConnectFunctor confunc, void *arg1,
					  DisconnectFunctor disfunc, void *arg2)
{
	if (!_addr.setAddress(addr)) { _init = false; return _init; }
	_addr.setPort(port);
	if (NULL != confunc) {
		_connectFunctor = confunc;
		_arg1 = arg1;
	}
	if (NULL != disfunc) {
		_disconnectFunctor = disfunc;
		_arg2 = arg2;
	}
	_init = true;
	return _init;
}

void MTcpClient::run()
{
}

MTcpServer::MTcpServer(size_t bufsize)
{
	_init = false;
	_buf = NULL;
	_bufsize = bufsize;
}

MTcpServer::~MTcpServer()
{
	if (_buf != NULL) {
		delete[] _buf;
		_buf = NULL;
	}
	_socket.close();
}

bool MTcpServer::bind(uint16_t port)
{
	try {
		if (_socket.socket() == -1) {
			return false;
		}
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
	static const uint8_t xdata[4] = {'S', 'T', 'O', 'P'};
	write(_pipefd[1], xdata, sizeof(xdata));
	MThread::stop();
}

int MTcpServer::registerEvent(int fd, uint32_t events)
{
	_ev.data.fd = fd;
	_ev.events = events;
	return epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &_ev);
}

int MTcpServer::unregisterEvent(int fd)
{
	return epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, NULL);
}

void MTcpServer::run()
{
	// if it's not init, just return
	if (!_init) return;

	try {
		if (_buf != NULL) {
			delete[] _buf;
			_buf = NULL;
		}

		_buf = new uint8_t[_bufsize];
	
		int nfds = 0;				// epoll events' count
		int fd = _socket.sockfd();	// tcpserver socket fd, accept clients

		// set SO_SNDBUF, SO_RCVBUF
		_socket.sendbuf(_bufsize);
		_socket.recvbuf(_bufsize);
		
		// epoll_create
		if ((_epollfd = epoll_create(eventsMaxSize)) == -1) {
			log_error("epoll_create error: %s", error().c_str());
			return;
		}

		// add fd
		if (registerEvent(fd) == -1) {
			log_error("register event failed: %s", error().c_str());
			return;
		}

		// add read pipe
		if (registerEvent(_pipefd[0]) == -1) {
			log_error("register event failed: %s", error().c_str());
			return;
		}

		while (!isInterrupted()) {
			// epoll_wait
			nfds = epoll_wait(_epollfd, _events, eventsMaxSize, -1);
			if (nfds == -1) {
				log_error("epoll_wait: %s", error().c_str());
				break;
			}

			for (int i = nfds - 1; i > -1; --i) {
				if (_events[i].data.fd == _socket.sockfd()) {
					// accept client
					MHostAddress address;
					int clientfd = _socket.accept(address);
					if (clientfd < 0) break;
					if (registerEvent(clientfd) == -1) {
						log_error("register event failed: %s", error().c_str());
						break;
					}
					_clients.push_back(clientfd);
					log_debug("client %s connected, clientfd: %d", address.toString().c_str(), clientfd);
					connection(clientfd, address);
				} else if (_events[i].data.fd == _pipefd[0]) {
					ssize_t len = read(_pipefd[0], _buf, _bufsize);
					interrupt();
					break;
				} else if (!handleEvent(&_events[i])) {
					interrupt();
					break;
				}
			} // for nfds
		} // while (!isInterrupted())

		closeClients();
		close(_pipefd[0]);
		close(_pipefd[1]);
		close(_epollfd);
	} catch (std::exception e) {
		log_error("%s", e.what());
	}
}

void MTcpServer::connection(int clientfd, const mpl::MHostAddress &addr)
{
}

bool MTcpServer::handleEvent(struct epoll_event *event)
{
	for (int i = _clients.size() - 1; i > -1; --i) {
		if (event->data.fd == _clients[i]) {
			ssize_t len = recv(_clients[i], _buf, _bufsize, 0);
			log_debug("client len: %d", len);
			if (0 == len) {
				// client is close
				log_debug("client close, clientfd: %d", _clients[i]);
				ssize_t slen = send(_clients[i], _buf, _bufsize, 0);
				close(_clients[i]);
				unregisterEvent(_clients[i]);
				_clients.erase(_clients.begin() + i);
				hasClosed(_clients[i]);
				continue;
			} else if (len > 0) {
				process(_clients[i], _buf, len);
			}
		}
	}

	return true;
}

void MTcpServer::closeClient(int fd)
{
	for (int i = _clients.size() - 1; i > -1; --i) {
		if (fd == _clients[i]) {
			log_debug("close clientfd: %d", fd);
			close(fd);
			unregisterEvent(fd);
			_clients.erase(_clients.begin() + i);
			break;
		}
	}
}

void MTcpServer::closeClients()
{
	for (int i = _clients.size() - 1; i > -1; --i) {
		log_debug("close clients' fd: %d", _clients[i]);
		close(_clients[i]);
	}
	_clients.clear();
}

void MTcpServer::process(int clientfd, const uint8_t *data, size_t len)
{
}

void MTcpServer::hasClosed(int clientfd)
{
}

MUdpSocket::MUdpSocket()
{
	using namespace std;
	
	if (socket(UdpSocket) == -1)
		throw runtime_error(string("socket error: ") + error());
}

MUdpSocket::~MUdpSocket()
{
	close();
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
		std::cerr << mpl::error() << std::endl;
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
		delete[] _buf;
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

MPL_END_NAMESPACE

#ifdef _MSC_VER
# pragma warning (pop)
#endif
