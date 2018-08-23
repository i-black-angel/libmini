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


class MCpuInfo
{
public:
	MCpuInfo();
	virtual ~MCpuInfo();
	
	std::string vendor() const;
	std::string model() const;
	std::string flags() const;
};

std::string hostname();
std::string cpuArchitecture();
int numberOfCores();

std::string productType();
std::string productVersion();
std::string prettyProductName();

double cpuPercent();

class MSysinfo
{
public:
    MSysinfo();
    virtual ~MSysinfo();

	std::string kernelName() const;
	std::string nodename() const;
	std::string kernelRelease() const;
	std::string kernelVersion() const;
	std::string machine() const;

	uint64_t totalmem() const;	// Total usable main memory size
	uint64_t freemem() const;	// Available memory size
	uint64_t usedmem() const;	// Used memory size
	uint16_t procs() const;		// Number of current processes
	// how long the system has been running,
	// // and System up since, yyyy-mm-dd HH:MM:SS
	long uptime() const;		// Seconds since boot
	std::string struptime(long uptime_secs) const; 
	std::string since() const;
private:
	struct utsname _utsname;
	struct sysinfo _sysinfo;
};

MPL_END_NAMESPACE

#endif /* _MSYSINFO_H_ */
