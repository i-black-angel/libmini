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
	char buf[1024] = {0x00};
	const char *fname = "/etc/resolv.conf";
	mpl::MFileInfo fileinfo = fname;
	if (fileinfo.isSymLink()) {
		std::cout << "symbolic link size: " << fileinfo.size() << std::endl;
	} else {
		std::cout << "file size: " << fileinfo.size() << std::endl;
	}
	int n = mpl::MFile(fname).readbuf(buf, sizeof(buf));
	if (n > 0) {
		std::cout << n << std::endl;
		std::cout << buf << std::endl;
	}
    return 0;
}
