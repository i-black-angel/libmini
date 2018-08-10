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
#include <stack>
#include <dirent.h>

bool rm_file(const std::string &path, bool is_dir = false)
{
	int flag = is_dir ? AT_REMOVEDIR : 0;
	if (unlinkat(AT_FDCWD, path.c_str(), flag) == 0)
		return true;

	log_error("cannot remove '%s': %s", path.c_str(), mpl::error().c_str());
	return false;
}

int main(int argc, char *argv[])
{
	std::string entry = "/tmp/hello";
	if (argc >= 2)
		entry = argv[1];

	std::stack<std::string> entries;
	struct dirent *dirp;
	std::string path = mpl::MFileInfo(entry).absoluteFilePath();
	std::cout << path << std::endl;
	DIR *dp = opendir(path.c_str());
	if (dp == NULL) {
		std::cout << "can't open " << entry << " directory" << std::endl;
		return 1;
	}
	closedir(dp);

	entries.push(path);
	while (!entries.empty()) {
		std::string path = entries.top();
		if (path.empty()) continue;
		DIR *dp = opendir(path.c_str());
		if (NULL == dp) continue;

		int subdir = 0;
		while((dirp = readdir(dp)) != NULL) {
			if (strcmp(dirp->d_name, ".") == 0
				|| strcmp(dirp->d_name, "..") == 0)
				continue;
			std::string fullpath = path + mpl::MFileInfo::separator() + std::string(dirp->d_name);
			if (dirp->d_type == DT_DIR) {
				DIR *subdp = opendir(fullpath.c_str());
				if (NULL != subdp) {
					entries.push(fullpath);
					++subdir;
					closedir(subdp);
				} else {
					rm_file(fullpath, true);
				}
			} else {
				rm_file(fullpath);
			}
 		}
		closedir(dp);
		if (subdir == 0) {
			rm_file(path, true);
			entries.pop();
		}
	}
    return 0;
}

/*

#!/bin/bash
mkdir -p /tmp/hello/abc
mkdir -p /tmp/hello/bcd
mkdir -p /tmp/hello/def
mkdir -p /tmp/hello/jon
mkdir -p /tmp/hello/good

echo "hello world" >> /tmp/hello/abc/kit.txt
echo "hello world" >> /tmp/hello/bcd/kit.txt
echo "hello world" >> /tmp/hello/kit.txt
echo "hello world" >> /tmp/hello/kid.txt
echo "hello world" >> /tmp/hello/def/kit.txt
echo "hello world" >> /tmp/hello/jon/kit.txt

 */
