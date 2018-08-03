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
	mpl::MString name = "name";
	mpl::MString mstr = "\\etc\\resolv.conf";
	std::cout << name << std::endl;
	mstr.replace('\\', '/');
	std::cout << mstr << std::endl;

	name = "another";
	std::cout << name << std::endl;
	name = mstr;
	std::cout << mstr << std::endl;

	mpl::MFileInfo info = name;
	std::cout << info.size() << std::endl;
	std::cout << info.path() << std::endl;
    return 0;
}
