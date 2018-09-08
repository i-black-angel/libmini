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

#include <mpl/mcoredef.h>

MPL_BEGIN_NAMESPACE

class MUrl
{
public:
    explicit MUrl();
	MUrl(const char *s);
	MUrl(const std::string &str);
	MUrl(const MUrl &other);
    virtual ~MUrl();

	inline std::string url() const { return _url; }
	inline std::string protocol() const { return _protocol; }
	inline std::string auth() const { return _auth; }
	inline std::string host() const { return _host; }
	inline std::string hostname() const { return _hostname; }
	inline std::string pathname() const { return _pathname; }
	inline std::string search() const { return _search; }
	inline std::string path() const { return _path; }
	inline std::string hash() const { return _hash; }
	inline std::string query() const { return _query; }
	inline int port() const { return _port; }
	MUrl &operator=(const char *s);
	MUrl &operator=(const std::string &str);
	MUrl &operator=(const MUrl &url);

	bool operator==(const MUrl &url) const;
	inline bool operator!=(const MUrl &url) const
		{ return !operator==(url); }

	std::string toString() const;
	inline bool isValid() const { return _valid; }
protected:
	void setUrl(const std::string &url);
	std::string getPart(const std::string &url, const std::string &format, int l);
	bool parse(const std::string &url);
	std::string getProtocol(const std::string &url);
	bool isProtocol(const std::string &proto);
	bool isSSh(const std::string &ssh);
private:
	void inner_copy(const MUrl &other) {
		_url = other._url;

		_protocol = other._protocol;
		_auth = other._auth;
		_host = other._host;
		_hostname = other._hostname;
		_pathname = other._pathname;
		_search = other._search;
		_path = other._path;
		_hash = other._hash;
		_query = other._query;
		_port = other._port;
		_valid = other._valid;
	}

	std::string _url;

	std::string _protocol;
	std::string _auth;
	std::string _host;
	std::string _hostname;
	std::string _pathname;
	std::string _search;
	std::string _path;
	std::string _hash;
	std::string _query;
	int _port;
	bool _valid;
};

std::ostream &operator<<(std::ostream &out, const MUrl &url);
std::istream &operator>>(std::istream &in, MUrl &url);

MPL_END_NAMESPACE

#endif /* _MURL_H_ */
