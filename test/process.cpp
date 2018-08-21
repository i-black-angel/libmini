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
	fprintf(stdout, "pid:\t%d\n", mpl::process::pid());
	fprintf(stdout, "ppid:\t%d\n", mpl::process::ppid());
	fprintf(stdout, "pgrp:\t%d\n", mpl::process::pgrp());
	fprintf(stdout, "uid:\t%d\n", mpl::process::uid());
	fprintf(stdout, "euid:\t%d\n", mpl::process::euid());
	fprintf(stdout, "gid:\t%d\n", mpl::process::gid());
	fprintf(stdout, "egid:\t%d\n", mpl::process::egid());
	fprintf(stdout, "pwd:\t%s\n", mpl::process::pwd().c_str());
	fprintf(stdout, "user:\t%s\n", mpl::process::user().c_str());
	fprintf(stdout, "group:\t%s\n", mpl::process::group().c_str());
	fprintf(stdout, "login:\t%s\n", mpl::process::login().c_str());
	fprintf(stdout, "program:\t%s\n", mpl::process::program().c_str());

	// std::vector<std::string> envs = mpl::process::systemEnvironment();
	// std::vector<std::string>::iterator it = envs.begin();
	// for (; it != envs.end(); ++it) {
	// 	std::cout << *it << std::endl;		
	// }

    return 0;
}
