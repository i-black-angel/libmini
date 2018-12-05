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
	char data[1024] = {0x00};
	const char *fname = "/etc/resolv.conf";
	mpl::MFileInfo fileinfo = fname;
	if (fileinfo.isSymLink()) {
		std::cout << "symbolic link size: " << fileinfo.size() << std::endl;
	} else {
		std::cout << "file size: " << fileinfo.size() << std::endl;
	}
	int nbytes = mpl::MFile(fname).readbuf(data, sizeof(data));
	if (nbytes > 0) {
		std::cout << nbytes << std::endl;
		std::cout << data << std::endl;
	}

	memset(data, 0x00, sizeof(data));
	strcpy(data, "hello world!\n");
	std::cout << mpl::MFile("/tmp/abc").writebuf(data, strlen(data)) << std::endl;
	mpl::file::appendLine("/tmp/hello", "hello world!");


	// Read huge file
	const char *hugefile = "/tmp/hello";
	int fd = open(hugefile, O_RDONLY);
	if (fd == -1) return -1;
	int n = -1, pos = 0;
	uint8_t buf[8192] = { 0x00 };
	size_t size = sizeof(data);
	lseek(fd, 0, SEEK_SET);
	while ((n = read(fd, buf, size)) == size) {
		// do_something(buf, size)...
		printf("do_something(buf, size)\n");
		pos += n;
		lseek(fd, pos, SEEK_SET);
	}
	if (n > 0) {
        // do_something(buf, size)...
		printf("(n > 0) do_something(buf, size)\n");
	}
	close(fd);
    return 0;
}
