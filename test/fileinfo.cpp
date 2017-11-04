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
#include <punica.h>

int main(int argc, char *argv[])
{
	punica::PFileInfo fileinfo(argv[0]);
	punica::PFileInfo cf(fileinfo);
	std::cout << fileinfo.filePath() << std::endl;

	std::cout << fileinfo.readLink() << std::endl;

	std::cout << "exists " << fileinfo.exists() << std::endl;
	std::cout << "isReadable " << cf.isReadable() << std::endl;
	std::cout << "isWritable " << cf.isWritable() << std::endl;
	std::cout << "isExecutable " << cf.isExecutable() << std::endl;

	std::cout << cf.basename() << std::endl;
	std::cout << cf.filename() << std::endl;
	std::cout << cf.suffix() << std::endl;
	std::cout << cf.dirname() << std::endl;

	punica::PFileInfo test(cf.dirname());
	std::cout << test.basename() << std::endl;
	std::cout << test.filename() << std::endl;
	std::cout << test.suffix() << std::endl;

	punica::PFileInfo root("/");
	std::cout << root.basename() << std::endl;
	std::cout << root.filename() << std::endl;
	std::cout << root.suffix() << std::endl;
	std::cout << root.dirname() << std::endl;

	punica::PFileInfo file("music.mp3");
	std::string ext = file.suffix(); // ext = "mp3"
	std::cout << file.basename() << std::endl;
	std::cout << file.filename() << std::endl;
	std::cout << file.suffix() << std::endl;
	std::cout << file.dirname() << std::endl;

	punica::PFileInfo rootfile("/etc/ts.conf");
	std::cout << rootfile.exists() << std::endl;
	std::cout << rootfile.isWritable() << std::endl;

	std::cout << punica::PFileInfo::exists("/etc/resolv.conf") << std::endl;
    return 0;
}
