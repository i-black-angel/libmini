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
#include <minion/mhostaddress.h>

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

MINION_BEGIN_NAMESPACE

MHostAddress::MHostAddress()
{
	_ip4addr = 0x00000000;
	_port = 0x0000;
}

MHostAddress::MHostAddress(SpecialAddress address, uint16_t port)
{
	setAddress(address);
	setPort(port);
}

MHostAddress::MHostAddress(const MHostAddress &copy)
{
	_ip4addr = copy._ip4addr;
	_port = copy._port;
}

MHostAddress::MHostAddress(uint32_t ip4Addr, uint16_t port)
{
	setAddress(ip4Addr);
	setPort(port);
}

MHostAddress::MHostAddress(const std::string &address, uint16_t port)
{
	setAddress(address);
	setPort(port);
}

MHostAddress::MHostAddress(const struct sockaddr_in &addr)
{
	setAddress(ntohl(addr.sin_addr.s_addr));
	setPort(ntohs(addr.sin_port));
}

MHostAddress::~MHostAddress()
{
}

MHostAddress &MHostAddress::operator=(const MHostAddress &other)
{
	_ip4addr = other._ip4addr;
	_port = other._port;
	return *this;
}

void MHostAddress::setAddress(uint32_t ip4Addr)
{
	_ip4addr = ip4Addr;
}

bool MHostAddress::setAddress(const std::string &address)
{
	struct addrinfo *ailist = NULL, *aip = NULL;
	struct addrinfo hint;
	hint.ai_flags = 0;
	hint.ai_family = 0;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = 0;
	hint.ai_addrlen = 0;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;

	getaddrinfo(address.c_str(), NULL, &hint, &ailist);

	for (aip = ailist; aip != NULL; aip = aip->ai_next) {
		struct sockaddr_in *addr = (sockaddr_in *)aip->ai_addr;
		_ip4addr = htonl(addr->sin_addr.s_addr);
		if (_ip4addr == 0x00000000) continue;
		freeaddrinfo(ailist);
		return true;
	}
	
	if (ailist != NULL)
		freeaddrinfo(ailist);

	return false;
}

void MHostAddress::setAddress(SpecialAddress address)
{
	uint32_t ip4 = INADDR_ANY;
	switch (address) {
	case Null:
		ip4 = INADDR_NONE;
		break;;
	case Broadcast:
		ip4 = INADDR_BROADCAST;
		break;
	case LocalHost:
		ip4 = INADDR_LOOPBACK;
		break;
	case Any:
		break;
	}

	_ip4addr = ip4;
}

void MHostAddress::setPort(uint16_t port)
{
	_port = port;
}

uint32_t MHostAddress::ipv4() const
{
	return _ip4addr;
}

std::string MHostAddress::toString() const
{
	std::string content;

	int len = snprintf(NULL, 0, "%s:%d", address().c_str(), port());

	content.resize(len + 2);

	snprintf(&content[0], len + 1, "%s:%d", address().c_str(), port());

	return content;
}

std::string MHostAddress::address() const
{
	char str[INET_ADDRSTRLEN] = { 0x00 };
	uint32_t ip4 = ntohl(_ip4addr);
	if (NULL == inet_ntop(AF_INET, &ip4, str, sizeof(str))) {
		return std::string();
	}
	return std::string(str);
}

uint16_t MHostAddress::port() const
{
	return _port;
}

bool MHostAddress::isEqual(const MHostAddress &address) const
{
	return ( (_ip4addr == address._ip4addr) &&
			 (_port == address._port) );
}

bool MHostAddress::operator ==(const MHostAddress &address) const
{
	return isEqual(address);
}

bool MHostAddress::isNull() const
{
	if ((_ip4addr & 0xFFFFFFFF) == 0xFFFFFFFF)
		return true;
	return false;
}

bool MHostAddress::isLoopback() const
{
	if ((_ip4addr & 0xFF000000) == 0x7F000000)
		return true;
	return false;
}

bool MHostAddress::isMulticast() const
{
	if ((_ip4addr & 0xF0000000) == 0xE0000000)
        return true; // 224.0.0.0-239.255.255.255 (including v4-mapped IPv6 addresses)
	return false;
}

std::ostream &operator<<(std::ostream &out, const MHostAddress &address)
{
	out << address.toString();
	return out;
}

std::istream &operator>>(std::istream &in, MHostAddress &address)
{
	uint32_t ip4;
	in >> ip4;
	address.setAddress(ip4);
	return in;
}

MINION_END_NAMESPACE

#ifdef _MSC_VER
# pragma warning (pop)
#endif
