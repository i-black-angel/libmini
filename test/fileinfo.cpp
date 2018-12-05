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
	mpl::MFileInfo file = "/tmp/./tmpflie";
	printf ("origin: %s\n", file.data().c_str());
	std::cout << file.filePath() << std::endl;
	std::cout << file.fileName() << std::endl;
	std::cout << file.dirName() << std::endl;
	std::cout << "exists: " << (file.isExists() ? "true" : "false") << std::endl;
	if (file.fileType() == mpl::MFileInfo::symbolic_link) {
		std::cout << file.readLink() << std::endl;
	}
	
	file = "/etc/./sys/.././ts.conf";
	if (file.isExists()) {
		printf ("%s %s an absolute filename\n", file.fileName().c_str(), file.isAbsolute() ? "is" : "is not");
		if (file.isSymLink()) {
			std::cout << file.readLink() << std::endl;
		}
		std::cout << file.canonicalFilePath() << std::endl;

		printf("permissions: 0x%x\n", file.permissions());

		if (file.permissions() & mpl::MFileInfo::ReadUser) {
			std::cout << "user read permission" << std::endl;
		}
		if (file.permissions() & mpl::MFileInfo::WriteUser) {
			std::cout << "user write permission" << std::endl;
		}
		if (file.permissions() & mpl::MFileInfo::ExecUser) {
			std::cout << "user execute permission" << std::endl;
		}
		if (file.permissions() & mpl::MFileInfo::ReadGroup) {
			std::cout << "group read permission" << std::endl;
		}
		if (file.permissions() & mpl::MFileInfo::WriteGroup) {
			std::cout << "group write permission" << std::endl;
		}

		printf("last modified: %s\n", file.lastModified().toString().c_str());
	}
	
	mpl::MFileInfo dirpath = "../hello";
	std::cout << dirpath.fileName() << std::endl;
	std::cout << dirpath.absoluteFilePath() << std::endl;
	std::cout << dirpath.canonicalFilePath() << std::endl;

	std::cout << dirpath.path() << std::endl;
	std::cout << dirpath.absolutePath() << std::endl;
	std::cout << dirpath.canonicalPath() << std::endl;
	
	mpl::MFileInfo homepath = "~/docker";
	std::cout << homepath.fileName() << std::endl;
	std::cout << "isdir: " << (homepath.isDir() ? "true" : "false") << std::endl;
    return 0;
}
