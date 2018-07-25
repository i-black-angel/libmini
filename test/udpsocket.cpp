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

class UdpServer : public mpl::MUdpServer
{
public:
    explicit UdpServer() { }
    virtual ~UdpServer() { }
protected:
	virtual void process(const uint8_t *data, size_t len, const mpl::MHostAddress &host)
		{
			for (int m = 0; m < len; ++m) {
				printf ("%02X ", data[m]);
			}
			printf ("\n");
			std::cout << host << std::endl;
		}
};


int main(int argc, char *argv[])
{
	const uint8_t data[] = {0x00, 0x01, 0x02, 0x03};

	// mpl::MUdpSocket udpSocket;
	// mpl::MHostAddress addr("localhost", 7753);

	// ssize_t len = udpSocket.sendto(data, sizeof(data), addr);
	// std::cout << "send len: " << len << std::endl;
	// udpSocket.bind(7753);

	UdpServer srv;
	srv.bind(7753);
	srv.start();

	while (true) {
		sleep(10);
	}
	
    return 0;
}
