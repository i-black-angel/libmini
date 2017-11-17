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
#include <minion.h>

int main(int argc, char *argv[])
{
	const char *msg = "M^0001^1^108^彩票打印机脱机";

	minion::MUdpSocket udpSocket;
	minion::MHostAddress addr("192.168.7.47", 8309);
	std::cout << addr << std::endl;

	std::cout << addr.ipv4() << std::endl;
	printf("%08x\n", addr.ipv4());
	
	ssize_t len = udpSocket.sendto((const uint8_t *)msg, strlen(msg) + 1, addr);
	sleep(1);

	const char *dealing = "M^0001^2^ssq,2017105,101,1,2,1,XSLPT-20170828103056101729,00200001,2017-08-28 10:30,1";
	len = udpSocket.sendto((const uint8_t *)dealing, strlen(dealing) + 1, addr);
	sleep(1);
	
    return 0;
}


