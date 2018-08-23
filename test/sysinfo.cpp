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
	setlocale(LC_ALL, "");

	mpl::MSysinfo info;
	std::cout << info.uptime() << std::endl;
	std::cout << info.struptime(info.uptime()) << std::endl;
	std::cout << info.since() << std::endl;
	fprintf(stdout, "free:\t%s\n", mpl::scaleSize(info.freemem()).c_str());
	fprintf(stdout, "total:\t%s\n", mpl::scaleSize(info.totalmem()).c_str());
	fprintf(stdout, "used:\t%s\n", mpl::scaleSize(info.usedmem()).c_str());
	fprintf(stdout, "current procs:\t%u\n", info.procs());

	std::cout << info.kernelName() << std::endl;
	std::cout << info.nodename() << std::endl;
	std::cout << info.kernelRelease() << std::endl;
	std::cout << info.kernelVersion() << std::endl;
	std::cout << info.machine() << std::endl;
	
    return 0;
}
