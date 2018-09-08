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
#include <mpl.h>

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "");

	mpl::MUrl gh_url = "git://git@github.com:jwerle/url.h.git";
	mpl::MUrl http_url = "http://www.sina.com/p/a/t/h?query%20of%20info";
	mpl::MUrl url = "http://user:pass@subdomain.host.com:8080/p/a/t/h?query=string#hash";
	std::cout << url << std::endl;
	printf("==============API DEMO===============\n");
	printf("proto:\t%s\n", url.protocol().c_str());
	printf("auth:\t%s\n", url.auth().c_str());
	printf("host:\t%s\n", url.host().c_str());
	printf("hostname:\t%s\n", url.hostname().c_str());
	printf("port:\t%d\n", url.port());
	printf("path:\t%s\n", url.path().c_str());
	printf("pathname:\t%s\n", url.pathname().c_str());
	printf("hash:\t%s\n", url.hash().c_str());
	printf("search:\t%s\n", url.search().c_str());
	printf("query:\t%s\n", url.query().c_str());

	std::cout << gh_url << std::endl;
	printf("==============API DEMO===============\n");
	printf("proto:\t%s\n", gh_url.protocol().c_str());
	printf("auth:\t%s\n", gh_url.auth().c_str());
	printf("host:\t%s\n", gh_url.host().c_str());
	printf("hostname:\t%s\n", gh_url.hostname().c_str());
	printf("port:\t%d\n", gh_url.port());
	printf("path:\t%s\n", gh_url.path().c_str());
	printf("pathname:\t%s\n", gh_url.pathname().c_str());
	printf("hash:\t%s\n", gh_url.hash().c_str());
	printf("search:\t%s\n", gh_url.search().c_str());
	printf("query:\t%s\n", gh_url.query().c_str());
	
	std::cout << http_url << std::endl;


	mpl::MUrl invalid_url = "http://www.sian.com:65535";
	if (invalid_url.isValid()) {
		printf("%d\n", invalid_url.port());
		std::cout << sizeof(char) << std::endl;
		// std::cout << invalid_url.port() << std::endl;
	}
    return 0;
}
