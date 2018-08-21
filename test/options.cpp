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
	mpl::MOptions opt("1.0.0", "");
	opt.insert('n', "name", "This application's name'", true);
	opt.insert('x', "xman", "Wonderful count", true, "XMAN");
	opt.parse(argc, argv);

	if (opt.find('n')) {
		std::string name = opt.getstr('n');
		std::cout << name << std::endl;
	}
	if (opt.find('x')) {
		int count = opt.getint('x');
		std::cout << count << std::endl;
	}

	if (opt.find('c')) {
		std::string conf = opt.value('c');
		std::cout << conf << std::endl;
	}

	bool verbose = opt.find('v');
	std::cout << (verbose ? "true" : "false") << std::endl;

	std::cout << mpl::process::user() << std::endl;
	std::cout << mpl::process::group() << std::endl;
	std::cout << mpl::process::login() << std::endl;
	std::cout << mpl::process::pwd() << std::endl;
	std::cout << mpl::threadId() << std::endl;
    return 0;
}
