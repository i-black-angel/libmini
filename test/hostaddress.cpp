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
#ifdef _MSC_VER
	WSADATA wsaData;
	int err = WSAStartup(WINSOCK_VERSION, &wsaData);
	if (0 != err) {
		fprintf(stderr, "WSAStartup failure!\n");
		return 1;
	}
#endif
	
    std::cout << "MHostAddress test" << std::endl;
    mpl::MHostAddress addr1;
    mpl::MHostAddress addr2(mpl::MHostAddress::Null);
    mpl::MHostAddress addr3(mpl::MHostAddress::Broadcast);
    mpl::MHostAddress addr4(mpl::MHostAddress::LocalHost);
    mpl::MHostAddress addr5 = mpl::MHostAddress::Any;
    mpl::MHostAddress addr6(0x0c224433);
    mpl::MHostAddress addr7("192.168.10.70");

    std::cout << addr1.toString() << std::endl;
    std::cout << addr2.toString() << std::endl;
    std::cout << addr3.toString() << std::endl;
    std::cout << addr4.toString() << std::endl;
    std::cout << addr5.toString() << std::endl;
    std::cout << addr6.toString() << std::endl;    
    std::cout << addr7.toString() << std::endl;    

    if (addr1 == mpl::MHostAddress::Broadcast) {
        std::cout << "addr1 is Any" << std::endl;
    }

    mpl::MHostAddress addr8("www.sina.com", 8843);

    std::cout << addr8.toString() << std::endl;
    std::cout << addr8 << std::endl;
    std::cout << addr6 << std::endl;
	
#ifdef _MSC_VER
	WSACleanup();
#endif
	getchar();
    return 0;
}
