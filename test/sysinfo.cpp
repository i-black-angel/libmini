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
	fprintf(stdout, "free:\t%s\n", mpl::scaleSize(info.freeMem()).c_str());
	fprintf(stdout, "total:\t%s\n", mpl::scaleSize(info.totalMem()).c_str());
	fprintf(stdout, "used:\t%s\n", mpl::scaleSize(info.usedMem()).c_str());
	fprintf(stdout, "cached:\t%s\n", mpl::scaleSize(info.cached()).c_str());
	fprintf(stdout, "active:\t%s\n", mpl::scaleSize(info.active()).c_str());
	fprintf(stdout, "inactive:\t%s\n", mpl::scaleSize(info.inactive()).c_str());
	fprintf(stdout, "current procs:\t%u\n", info.procs());

	fprintf(stdout, "kernel name:\t%s\n", info.kernelName().c_str());
	fprintf(stdout, "node name:\t%s\n", info.nodeName().c_str());
	fprintf(stdout, "kernel release:\t%s\n", info.kernelRelease().c_str());
	fprintf(stdout, "kernel version:\t%s\n", info.kernelVersion().c_str());
	fprintf(stdout, "machine:\t%s\n", info.machine().c_str());
	
	fprintf(stdout, "disk free:\t%s\n", mpl::scaleSize(info.freeDisk()).c_str());
	fprintf(stdout, "disk total:\t%s\n", mpl::scaleSize(info.totalDisk()).c_str());
	fprintf(stdout, "disk used:\t%s\n", mpl::scaleSize(info.usedDisk()).c_str());
	fprintf(stdout, "disk avail:\t%s\n", mpl::scaleSize(info.availDisk()).c_str());

    return 0;
}
