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

class logitem : public minion::MTaskInterface
{
public:
    explicit logitem(int a) : _arg1(a) { }
    virtual ~logitem() { }

	// void insert(int a) {
	// 	arg1 = a;
	// 	run();
	// }

	virtual void run() {
		std::cout << "recv: " << _arg1 << std::endl;
	}
private:
	int _arg1;
};

int main(int argc, char *argv[])
{
	minion::MThreadPool p(4);
	p.start();
	sleep(1);

	for (int i = 0; i < 100; ++i) {
		logitem *item = new logitem(i);
		p.push_back(item);
		usleep(1000);
	}

	getchar();
    return 0;
}
