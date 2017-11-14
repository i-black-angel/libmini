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

#include <mini/mcoredef.h>
#include <mini/mhostaddress.h>
#include <mini/mthread.h>

MINI_BEGIN_NAMESPACE

class MSocket
{
public:
	enum SocketType {
        TcpSocket,
        UdpSocket,
        SctpSocket,
        UnknownSocketType = -1
    };
	
    explicit MSocket(SocketType socketType);
    virtual ~MSocket();

	bool bind(const PHostAddress &address);
	bool bind(uint16_t port);
	bool connect(const std::string &address, uint16_t port, int msecs = 30000);
	bool connect(const PHostAddress &address, int msec = 30000);
	void close();
	
	uint16_t localPort() const;
	PHostAddress localAddress() const;
	uint16_t peerPort() const;
	PHostAddress peerAddress() const;
	std::string peerName() const;
	SocketType socketType() const;

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

class MTcpServer
{
public:
    explicit MTcpServer();
    virtual ~MTcpServer();
};

class MUdpSocket : public MSocket
{
public:
    explicit MUdpSocket();
    virtual ~MUdpSocket();

	// int64_t sendto(const uint8_t *data, size_t len);
	int64_t sendto(const uint8_t *data, size_t len, const PHostAddress &host);
	int64_t sendto(const uint8_t *data, size_t len, const std::string &address, uint16_t port);

	int64_t recvfrom(uint8_t *data, size_t len, PHostAddress &address);
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
	
	virtual void process(const uint8_t *data, size_t len, const mini::PHostAddress &host);

	MUdpSocket _socket;
	uint8_t *_buf;
	int _bufsize;
	bool _init;

	// select module
	int _pipefd[2];
	fd_set _rfds;
	int _maxfds;
};

MINI_END_NAMESPACE

#endif /* _MSOCKET_H_ */
