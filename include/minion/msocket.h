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

#include <minion/mcoredef.h>
#include <minion/mhostaddress.h>
#include <minion/mthread.h>

MINION_BEGIN_NAMESPACE

class MSocket
{
public:
	enum SocketType {
        TcpSocket,
        UdpSocket,
        SctpSocket,
        UnknownSocketType = -1
    };
	
    explicit MSocket(SocketType socketType = minion::MSocket::TcpSocket);
	explicit MSocket(int sockfd);
    virtual ~MSocket();

	int accept(MHostAddress &);
	bool bind(const MHostAddress &address);
	bool bind(uint16_t port);
	bool connect(const std::string &address, uint16_t port);
	bool connect(const MHostAddress &address);
	void close();
	bool listen(int backlog = 1024);
	
	MHostAddress sockname() const;
	MHostAddress peername(int fd) const;
	SocketType socketType() const;

	bool setSocketOption(int optname, const void *optval, socklen_t optlen);
	
	int sockfd() const { return _sockfd; }
	
protected:
	SocketType _socketType;
	int _sockfd;
};

class MTcpSocket : public MSocket
{
public:
    explicit MTcpSocket();
    virtual ~MTcpSocket();
};

class MTcpServer : public MThread
{
public:
    explicit MTcpServer();
    virtual ~MTcpServer();
	bool bind(uint16_t port);
	void stop();
protected:
	void run();
	virtual void process(int clientfd, const MHostAddress &addr);

	MTcpSocket _socket;
	bool _init;
	
	// select module
	int _pipefd[2];
	fd_set _rfds;
	int _maxfds;
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
	
	virtual void process(const uint8_t *data, size_t len, const minion::MHostAddress &host);

	MUdpSocket _socket;
	uint8_t *_buf;
	int _bufsize;
	bool _init;

	// select module
	int _pipefd[2];
	fd_set _rfds;
	int _maxfds;
};

MINION_END_NAMESPACE

#endif /* _MSOCKET_H_ */
