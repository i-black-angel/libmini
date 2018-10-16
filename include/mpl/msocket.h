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
#ifndef _MSOCKET_H_
#define _MSOCKET_H_

#include <mpl/mcoredef.h>
#include <mpl/mhostaddress.h>
#include <mpl/mthread.h>

typedef void (*ConnectFunctor)(void *arg);
typedef void (*DisconnectFunctor)(void *arg);

MPL_BEGIN_NAMESPACE

// MSocket
class MSocket
{
public:
	enum SocketType {
        TcpSocket,
        UdpSocket,
        SctpSocket,
        UnknownSocketType = -1
    };

	MSocket();
	MSocket(const MSocket &copy);
    virtual ~MSocket();

	MSocket &operator=(const MSocket &other);
	
	int socket(SocketType socketType = MSocket::TcpSocket);
	int accept(MHostAddress &);
	bool bind(const MHostAddress &address);
	bool bind(uint16_t port);
	bool connect(const std::string &address, uint16_t port);
	bool connect(const MHostAddress &address);
	ssize_t send(const void *buf, size_t nbytes);
	ssize_t recv(void *buf, size_t nbytes);
	void close();
	bool listen(int backlog = 1024);
	
	MHostAddress sockname() const;
	MHostAddress peername(int fd) const;
	SocketType socketType() const;

	bool nonblock(int block = 1);
	bool sendbuf(int bufsize);
	bool recvbuf(int bufsize);
	int sendbuf() const;
	int recvbuf() const;
	bool sendtimeout(int sec, long usec);
	bool recvtimeout(int sec, long usec);
	bool setSocketOption(int optname, const void *optval, socklen_t optlen);
	
	int sockfd() const { return _sockfd; }

	bool isEqual(const MSocket &sock) const;
	bool operator==(const MSocket &sock) const;
	inline bool operator!=(const MSocket &sock) const
		{ return !operator==(sock); }
protected:
	SocketType _socketType;
	int _sockfd;
};

class MTcpClient : public MThread
{
public:
	MTcpClient();
    virtual ~MTcpClient();

	bool init(const std::string &addr, int16_t port,
			  ConnectFunctor confunc = NULL, void *arg1 = NULL,
			  DisconnectFunctor disfunc = NULL, void *arg2 = NULL);
protected:
	virtual void run();			// override
	
private:
	MHostAddress _addr;
	bool _init;
	
	ConnectFunctor _connectFunctor;
	DisconnectFunctor _disconnectFunctor;
	void *_arg1;
	void *_arg2;

	MSocket _socket;
};

// MTcpServer
class MTcpServer : public MThread
{
public:
	static const int eventsMaxSize = 512;

    explicit MTcpServer(size_t bufsize = 1024);
    virtual ~MTcpServer();

	void setbufsize(size_t bufsize) { _bufsize = bufsize; }
	size_t bufsize() const { return _bufsize; }
	
	bool bind(uint16_t port);
	void stop();
protected:
	void run();
	int registerEvent(int fd, uint32_t events = EPOLLIN);
	int unregisterEvent(int fd);
	void closeClient(int fd);
	void closeClients();
	virtual bool handleEvent(struct epoll_event *event);
	virtual void connection(int clientfd, const mpl::MHostAddress &addr);
	virtual void process(int clientfd, const uint8_t *data, size_t len);
	virtual void hasClosed(int clientfd);

	MSocket _socket;
	bool _init;
	
	int _pipefd[2];

	// epoll module
	int _epollfd;
	struct epoll_event _ev;
	struct epoll_event _events[eventsMaxSize];

	std::vector<int> _clients;
	uint8_t *_buf;
	size_t _bufsize;
};

class MUdpSocket : public MSocket
{
public:
    explicit MUdpSocket();
    virtual ~MUdpSocket();

	// int64_t sendto(const uint8_t *data, size_t len);
	int64_t sendto(const uint8_t *data, size_t len, const MHostAddress &host);
	int64_t sendto(const uint8_t *data, size_t len, const std::string &address, uint16_t port);

	int64_t recvfrom(uint8_t *data, size_t len, MHostAddress &address);
};

class MUdpServer : public MThread
{
public:
    explicit MUdpServer(int bufsize = 2048);
    virtual ~MUdpServer();

	bool bind(uint16_t port);
	void stop();
	
protected:
	
	void run();
	
	virtual void process(const uint8_t *data, size_t len, const mpl::MHostAddress &host);

	MUdpSocket _socket;
	uint8_t *_buf;
	int _bufsize;
	bool _init;

	// select module
	int _pipefd[2];
	fd_set _rfds;
	int _maxfds;
};

MPL_END_NAMESPACE

#endif /* _MSOCKET_H_ */
