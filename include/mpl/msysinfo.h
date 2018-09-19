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
#ifndef _MSYSINFO_H_
#define _MSYSINFO_H_

#include <mpl/mcoredef.h>

MPL_BEGIN_NAMESPACE

std::string hostname();

class MCpuInfo
{
public:
	MCpuInfo();
	virtual ~MCpuInfo();
	
	std::string vendor() const;
	std::string model() const;
	std::string flags() const;
	int count() const;
	std::string architecture() const;
};

class MSysinfo
{
public:
    MSysinfo();
    virtual ~MSysinfo();

	std::string kernelName() const;
	std::string nodeName() const;
	std::string kernelRelease() const;
	std::string kernelVersion() const;
	std::string machine() const;

	uint64_t totalMem() const;	// Total usable main memory size
	uint64_t freeMem() const;	// Available memory size
	uint64_t usedMem() const;	// Used memory size
	uint64_t sharedMem() const;
	uint64_t bufferMem() const;
	uint64_t totalSwap() const;
	uint64_t freeSwap() const;
	uint64_t usedSwap() const;
	uint64_t cached() const;
	uint64_t active() const;
	uint64_t inactive() const;
	
	uint64_t totalDisk() const;	// Total disk space 
	uint64_t freeDisk() const;
	uint64_t usedDisk() const;
	uint64_t availDisk() const;	

	uint16_t procs() const;		// Number of current processes
	// how long the system has been running,
	// // and System up since, yyyy-mm-dd HH:MM:SS
	long uptime() const;		// Seconds since boot
	std::string struptime(long uptime_secs) const; 
	std::string since() const;
private:
#ifdef M_OS_LINUX
	struct utsname _utsname;
	struct sysinfo _sysinfo;
	struct statfs _statfs;
#endif
};

MPL_END_NAMESPACE

#endif /* _MSYSINFO_H_ */
