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
#ifndef _PHOSTADDRESS_H_
#define _PHOSTADDRESS_H_

#include <punica/pcoredef.h>

PUNICA_BEGIN_NAMESPACE

class PHostAddress
{
public:
	enum SpecialAddress {
		Null,
		Broadcast,
		LocalHost,
		Any,
	};
	PHostAddress();
	PHostAddress(SpecialAddress address, uint16_t port = 0);
	explicit PHostAddress(uint32_t ip4Addr, uint16_t port = 0);
	explicit PHostAddress(const std::string &address, uint16_t port = 0);
	PHostAddress(const PHostAddress &copy);
	virtual ~PHostAddress();

	PHostAddress &operator=(const PHostAddress &other);
	// PHostAddress &operator=(SpecialAddress address);

	void setAddress(uint32_t ip4Addr);
	bool setAddress(const std::string &address);
	void setAddress(SpecialAddress address);
	void setPort(uint16_t port);
	
	std::string address() const;
	uint32_t ipv4() const;
	uint16_t port() const;
	std::string toString() const;

	bool isEqual(const PHostAddress &address) const;
	bool operator ==(const PHostAddress &address) const;
	// bool operator ==(SpecialAddress address) const;
	inline bool operator !=(const PHostAddress &address) const
		{ return !operator==(address); }
	// inline bool operator !=(SpecialAddress address) const
	// 	{ return !operator==(address); }
	bool isNull() const;
	bool isLoopback() const;
	bool isMulticast() const;
private:
	uint32_t _ip4addr;
	uint16_t _port;
};

// inline bool operator ==(PHostAddress::SpecialAddress address1, const PHostAddress &address2)
// { return address2 == address1; }
// inline bool operator !=(PHostAddress::SpecialAddress address1, const PHostAddress &address2)
// { return address2 != address1; }

std::ostream &operator<<(std::ostream &, const PHostAddress &);
std::istream &operator>>(std::istream &, PHostAddress &);

PUNICA_END_NAMESPACE

#endif /* _PHOSTADDRESS_H_ */
