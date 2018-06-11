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
#ifndef _MURL_H_
#define _MURL_H_

#include <minion/mcoredef.h>

MINION_BEGIN_NAMESPACE

class MUrl
{
public:
    explicit MUrl();
	MUrl(const MUrl &other);
	MUrl(const std::string &url);
    virtual ~MUrl();

	std::string scheme() const;
	std::string host() const;
	int port() const;
	std::string path() const;
	std::string pathname() const;
	std::string query() const;
	std::string fragment() const;
	
	MUrl &operator=(const MUrl &url);
	bool operator!=(const MUrl &url) const;
	bool operator==(const MUrl &url) const;

	void setUrl(const std::string &url);
	std::string toString() const;

protected:
	void copy(const MUrl &other);

	std::string getPart(const std::string &url, const std::string &format, int l);
	void parse(const std::string &url);
	std::string parseProtocol(const std::string &url);
	bool isProtocol(const std::string &proto);
	bool isSSh(const std::string &ssh);
private:
	std::string _url;

	std::string _scheme;
	std::string _host;
	int _port;
	std::string _path;
	std::string _pathname;
	std::string _query;
	std::string _fragment;
};

std::ostream &operator<<(std::ostream &out, const MUrl &url);
std::istream &operator>>(std::istream &in, MUrl &url);

MINION_END_NAMESPACE

#endif /* _MURL_H_ */
