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

int main(int argc, char *argv[])
{
	std::string entry = "/tmp/hello";
	if (argc >= 2)
		entry = argv[1];

	return mpl::MDir::rmpath(entry) ? 0 : 1;
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
