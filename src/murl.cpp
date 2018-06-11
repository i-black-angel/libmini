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
#include <minion/murl.h>

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4996)
#endif

MINION_BEGIN_NAMESPACE


/** 
 * URI Schemes 
 * http://en.wikipedia.org/wiki/URI_scheme 
 */  

static const char *URL_SCHEMES[] = {  
  // official IANA registered schemes  
  "aaa", "aaas", "about", "acap", "acct", "adiumxtra", "afp", "afs", "aim", "apt", "attachment", "aw",  
  "beshare", "bitcoin", "bolo", "callto", "cap", "chrome", "crome-extension", "com-evenbrite-attendee",  
  "cid", "coap", "coaps","content", "crid", "cvs", "data", "dav", "dict", "lna-playsingle", "dln-playcontainer",  
  "dns", "dtn", "dvb", "ed2k", "facetime", "fax", "feed", "file", "finger", "fish","ftp", "geo", "gg","git",  
  "gizmoproject", "go", "gopher", "gtalk", "h323", "hcp", "http", "https", "iax", "icap", "icon","im",  
  "imap", "info", "ipn", "ipp", "irc", "irc6", "ircs", "iris", "iris.beep", "iris.xpc", "iris.xpcs","iris.lws",  
  "itms", "jabber", "jar", "jms", "keyparc", "lastfm", "ldap", "ldaps", "magnet", "mailserver","mailto",  
  "maps", "market", "message", "mid", "mms", "modem", "ms-help", "mssettings-power", "msnim", "msrp",  
  "msrps", "mtqp", "mumble", "mupdate", "mvn", "news", "nfs", "ni", "nih", "nntp", "notes","oid",  
  "paquelocktoken", "pack", "palm", "paparazzi", "pkcs11", "platform", "pop", "pres", "prospero", "proxy",  
  "psyc","query", "reload", "res", "resource", "rmi", "rsync", "rtmp","rtsp",  "secondlife", "service","session",  
  "sftp", "sgn", "shttp", "sieve", "sip", "sips", "skype", "smb", "sms", "snews", "snmp", "soap.beep","soap.beeps",  
  "soldat", "spotify", "ssh", "steam", "svn", "tag", "teamspeak", "tel", "telnet", "tftp", "things","thismessage",  
  "tn3270", "tip", "tv", "udp", "unreal", "urn", "ut2004", "vemmi","ventrilo", "videotex", "view-source", "wais","webcal",  
  "ws", "wss", "wtai", "wyciwyg", "xcon", "xcon-userid", "xfire","xmlrpc.beep",  "xmlrpc.beeps", "xmpp", "xri","ymsgr",  
  
  // unofficial schemes  
  "javascript", "jdbc", "doi"  
};  

MUrl::MUrl()
{
}

MUrl::MUrl(const MUrl &other)
{
	copy(other);
}

MUrl::MUrl(const std::string &url)
{
	setUrl(url);
}

MUrl::~MUrl()
{
}

void MUrl::copy(const MUrl &other)
{
	_url = other._url;

	_scheme = other._scheme;
	_host = other._host;
	_port = other._port;
	_path = other._path;
	_query = other._query;
	_fragment = other._fragment;
}	

std::string MUrl::scheme() const
{
	return _scheme;
}

std::string MUrl::host() const
{
	return _host;
}

int MUrl::port() const
{
	return _port;
}

std::string MUrl::path() const
{
	return _path;
}

std::string MUrl::pathname() const
{
	return _pathname;
}

std::string MUrl::query() const
{
	return _query;
}

std::string MUrl::fragment() const
{
	return _fragment;
}
	
MUrl &MUrl::operator=(const MUrl &url)
{
	copy(url);
	return *this;
}

bool MUrl::operator!=(const MUrl &url) const
{
	return (_url != url._url);
}

bool MUrl::operator==(const MUrl &url) const
{
	return (_url == url._url);
}

void MUrl::setUrl(const std::string &url)
{
	_url = url;

	parse(_url);
}

std::string MUrl::toString() const
{
}

void MUrl::parse(const std::string &url)
{
	bool is_ssh = false;

	std::string protocol = parseProtocol(url);
	if (protocol.empty()) return;
	// length of protocol plus ://
	int protocol_len = protocol.size() + 3;
	is_ssh = isSSh(protocol);

	std::string auth;
	int auth_len = 0;
	if (url.find("@") != std::string::npos) {
		auth = getPart(url, "%[^@]", protocol_len);
		auth_len = auth.size();
		if (!auth.empty()) auth_len++;
	}

	std::string hostname;
	hostname = (is_ssh)
		? getPart(url, "%[^:]", protocol_len + auth_len)
		: getPart(url, "%[^/]", protocol_len + auth_len);
	if (hostname.empty()) return;

	char *host = (char *)malloc(hostname.size() * sizeof(char));
	if (!host) return;
	sscanf(hostname.c_str(), "%[^:]", host);
	_host = host;
	free(host);

	std::string tmp_path;
	tmp_path = (is_ssh)
		? getPart(url, ":%s", protocol_len + auth_len + hostname.size())
		: getPart(url, "/%s", protocol_len + auth_len + hostname.size());
	char *path = (char *)malloc(tmp_path.size() * sizeof(char));
	if (!path) return;
	std::string fmt = (is_ssh) ? "%s" : "/%s";
	sprintf(path, fmt.c_str(), tmp_path.c_str());
	_path = path;
	free(path);

	tmp_path = _path;
	char *pathname = (char *)malloc(tmp_path.size() * sizeof(char));
	sscanf(tmp_path.c_str(), "%[^? | ^#]", pathname);
	_pathname = pathname;
	free(pathname);

	char *search = (char *)malloc(sizeof(char));
	if (!search) return;
	tmp_path = tmp_path.substr(_pathname.size());
	strcat(search, "");
	sscanf(tmp_path.c_str(), "%[^#]", search);
	std::string _search = search;
	free(search);

	char *query = (char *)malloc(sizeof(char));
	if (!query) return;
	sscanf(_search.c_str(), "?%s", query);
	_query = query;

	char *hash = (char *)malloc(sizeof(char));
	if (!hash) return;
	tmp_path = _path.substr(_pathname.size() + _search.size());
	strcat(hash, "");
	sscanf(tmp_path.c_str(), "%s", hash);
	std::string _hash = hash;
	free(hash);

	char *port = (char *)malloc(sizeof(char));
	if (!port) return;
	std::string tmp_hostname = hostname.substr(_host.size() + 1); // +1 for ':' char
	sscanf(tmp_hostname.c_str(), "%s", port);
	_port = atoi(port);
	free(port);
}

std::string MUrl::parseProtocol(const std::string &url)
{
	char protocol[16] = {0x00};
	sscanf(url.c_str(), "%[^://]", protocol);
	if (isProtocol(protocol)) return protocol;
	return "";
}

bool MUrl::isProtocol(const std::string &proto)
{
	int count = sizeof(URL_SCHEMES) / sizeof(URL_SCHEMES[0]);

	for (int i = 0; i < count; ++i) {
		if (proto.compare(URL_SCHEMES[i]) == 0) {
			return true;
		}
	}

	return false;
}

bool MUrl::isSSh(const std::string &ssh)
{
	return ((ssh.compare("ssh") == 0) || (ssh.compare("git") == 0));
}

std::string MUrl::getPart(const std::string &url, const std::string &format, int l)
{
	if (url.empty()) return "";

	char tmp[1024] = {0x00};

	std::string fmt_url = url.substr(l);
	sscanf(fmt_url.c_str(), format.c_str(), tmp);

	if (0 != strcmp(tmp, url.c_str())) {
		return std::string(tmp);
	}
	return "";
}

std::ostream &operator<<(std::ostream &out, const MUrl &url)
{
	out << url.toString();
	return out;
}

std::istream &operator>>(std::istream &in, MUrl &url)
{
	std::string strurl;
	in >> strurl;
	url.setUrl(strurl);
	return in;
}

MINION_END_NAMESPACE

#ifdef _MSC_VER
# pragma warning (pop)
#endif
