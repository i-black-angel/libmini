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
#include <punica/phostaddress.h>

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

PUNICA_BEGIN_NAMESPACE

PHostAddress::PHostAddress()
{
	_ip4addr = 0x00000000;
	_port = 0x0000;
}

PHostAddress::PHostAddress(SpecialAddress address, uint16_t port)
{
	setAddress(address);
	setPort(port);
}

PHostAddress::PHostAddress(const PHostAddress &copy)
{
	_ip4addr = copy._ip4addr;
	_port = copy._port;
}

PHostAddress::PHostAddress(uint32_t ip4Addr, uint16_t port)
{
	setAddress(ip4Addr);
	setPort(port);
}

PHostAddress::PHostAddress(const std::string &address, uint16_t port)
{
	setAddress(address);
	setPort(port);
}

PHostAddress::~PHostAddress()
{
}

PHostAddress &PHostAddress::operator=(const PHostAddress &other)
{
	_ip4addr = other._ip4addr;
	_port = other._port;
	return *this;
}

void PHostAddress::setAddress(uint32_t ip4Addr)
{
	_ip4addr = ip4Addr;
}

bool PHostAddress::setAddress(const std::string &address)
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
		freeaddrinfo(ailist);
		return true;
	}
	
	if (ailist != NULL)
		freeaddrinfo(ailist);

	return false;
}

void PHostAddress::setAddress(SpecialAddress address)
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

void PHostAddress::setPort(uint16_t port)
{
	_port = port;
}

uint32_t PHostAddress::ipv4() const
{
	return _ip4addr;
}

std::string PHostAddress::toString() const
{
	std::string content;

	int len = snprintf(NULL, 0, "%s:%d", address().c_str(), port());

	content.resize(len + 2);

	snprintf(&content[0], len + 1, "%s:%d", address().c_str(), port());

	return content;
}

std::string PHostAddress::address() const
{
	char str[INET_ADDRSTRLEN] = { 0x00 };
	uint32_t ip4 = ntohl(_ip4addr);
	if (NULL == inet_ntop(AF_INET, &ip4, str, sizeof(str))) {
		return std::string();
	}
	return std::string(str);
}

uint16_t PHostAddress::port() const
{
	return _port;
}

bool PHostAddress::isEqual(const PHostAddress &address) const
{
	return ( (_ip4addr == address._ip4addr) &&
			 (_port == address._port) );
}

bool PHostAddress::operator ==(const PHostAddress &address) const
{
	return isEqual(address);
}

bool PHostAddress::isNull() const
{
	if ((_ip4addr & 0xFFFFFFFF) == 0xFFFFFFFF)
		return true;
	return false;
}

bool PHostAddress::isLoopback() const
{
	if ((_ip4addr & 0xFF000000) == 0x7F000000)
		return true;
	return false;
}

bool PHostAddress::isMulticast() const
{
	if ((_ip4addr & 0xF0000000) == 0xE0000000)
        return true; // 224.0.0.0-239.255.255.255 (including v4-mapped IPv6 addresses)
	return false;
}

std::ostream &operator<<(std::ostream &out, const PHostAddress &address)
{
	out << address.toString();
	return out;
}

std::istream &operator>>(std::istream &in, PHostAddress &address)
{
	uint32_t ip4;
	in >> ip4;
	address.setAddress(ip4);
	return in;
}

PUNICA_END_NAMESPACE

#ifdef _MSC_VER
# pragma warning (pop)
#endif
