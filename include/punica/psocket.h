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
#ifndef _PSOCKET_H_
#define _PSOCKET_H_

#include <punica/pcoredef.h>
#include <punica/phostaddress.h>
#include <punica/pthread.h>

PUNICA_BEGIN_NAMESPACE

class PSocket
{
public:
	enum SocketType {
        TcpSocket,
        UdpSocket,
        SctpSocket,
        UnknownSocketType = -1
    };
	
    explicit PSocket(SocketType socketType);
    virtual ~PSocket();

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
protected:
	SocketType _socketType;
	int _sockfd;
};

class PTcpSocket : public PSocket
{
public:
    explicit PTcpSocket();
    virtual ~PTcpSocket();
};

class PTcpServer
{
public:
    explicit PTcpServer();
    virtual ~PTcpServer();
};

class PUdpSocket : public PSocket
{
public:
    explicit PUdpSocket();
    virtual ~PUdpSocket();

	// int64_t sendto(const uint8_t *data, size_t len);
	int64_t sendto(const uint8_t *data, size_t len, const PHostAddress &host);
	int64_t sendto(const uint8_t *data, size_t len, const std::string &address, uint16_t port);

	int64_t recvfrom(uint8_t *data, size_t len, PHostAddress &address);
};

class PUdpServer : public PThread
{
public:
    explicit PUdpServer(int bufsize = 2048);
    virtual ~PUdpServer();

	bool bind(uint16_t port);
protected:
	virtual void run();
	virtual void process(const uint8_t *data, size_t len, const punica::PHostAddress &host);

	PUdpSocket _socket;
	uint8_t *_buf;
	int _bufsize;
	bool _init;
};

PUNICA_END_NAMESPACE

#endif /* _PSOCKET_H_ */
