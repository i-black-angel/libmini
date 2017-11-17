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
#ifndef _MHOSTADDRESS_H_
#define _MHOSTADDRESS_H_

#include <minion/mcoredef.h>

MINION_BEGIN_NAMESPACE

class MHostAddress
{
public:
	enum SpecialAddress {
		Null,
		Broadcast,
		LocalHost,
		Any,
	};
	MHostAddress();
	MHostAddress(SpecialAddress address, uint16_t port = 0);
	explicit MHostAddress(uint32_t ip4Addr, uint16_t port = 0);
	explicit MHostAddress(const std::string &address, uint16_t port = 0);
	MHostAddress(const MHostAddress &copy);
	virtual ~MHostAddress();

	MHostAddress &operator=(const MHostAddress &other);

	void setAddress(uint32_t ip4Addr);
	bool setAddress(const std::string &address);
	void setAddress(SpecialAddress address);
	void setPort(uint16_t port);
	
	std::string address() const;
	uint32_t ipv4() const;
	uint16_t port() const;
	std::string toString() const;

	bool isEqual(const MHostAddress &address) const;
	bool operator ==(const MHostAddress &address) const;
	inline bool operator !=(const MHostAddress &address) const
		{ return !operator==(address); }
	bool isNull() const;
	bool isLoopback() const;
	bool isMulticast() const;
private:
	uint32_t _ip4addr;
	uint16_t _port;
};

std::ostream &operator<<(std::ostream &, const MHostAddress &);
std::istream &operator>>(std::istream &, MHostAddress &);

MINION_END_NAMESPACE

#endif /* _MHOSTADDRESS_H_ */
