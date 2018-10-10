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

typedef std::vector<std::string> string_array;

int main(int argc, char *argv[])
{
	std::string shortopts = "hVc:v";
	std::cout << shortopts.size() << std::endl;
	printf("%s\n", mpl::format("%c:", 'n').c_str());
	printf("%s\n", mpl::format("%c:", 'x').c_str());
	shortopts += mpl::format("%c:", 'n');
	// shortopts += "n:";
	std::cout << shortopts.size() << std::endl;
	printf("%s\n", shortopts.c_str());
	shortopts += mpl::format("%c:", 'x');
	// shortopts += "x:";
	std::cout << shortopts.size() << std::endl;
	printf("%s\n", shortopts.c_str());

	string_array arr = mpl::split("hello/name//", "/");
	std::cout << arr.size() << std::endl;
	for (size_t i = 0; i < arr.size(); ++i) {
		std::cout << "'" << arr[i] << "'" << std::endl;
	}

	std::string hello = "hello world!";
	mpl::toupper(hello);
	std::cout << hello << std::endl;
	mpl::tolower(hello);
	std::cout << hello << std::endl;

	std::string filename = "  trim testing!  ";
	printf("trim_front: '%s'\n", mpl::trim_front(filename).c_str());
	printf("trim_back: '%s'\n", mpl::trim_back(filename).c_str());
	printf("trim: '%s'\n", mpl::trim(filename).c_str());

    return 0;
}
