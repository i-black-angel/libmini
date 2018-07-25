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
	const char *msg = "M^0001^1^108^彩票打印机脱机";

	mpl::MUdpSocket udpSocket;
	mpl::MHostAddress addr("localhost", 8309);
	
	ssize_t len = udpSocket.sendto((const uint8_t *)msg, strlen(msg) + 1, addr);
	log_debug("len = %d", len);
	log_debug("sendto");
	log_debug("%s", addr.toString().c_str());
	log_debug("%s", msg);
	// std::cout << "len = " << len << std::endl;
	// std::cout << "sendto " << std::endl;
	// std::cout << addr << std::endl;
	// std::cout << msg << std::endl;
    return 0;
}
