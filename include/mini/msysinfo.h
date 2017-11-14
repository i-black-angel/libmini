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

#include <mini/mcoredef.h>

MINI_BEGIN_NAMESPACE

class MSysinfo
{
public:
    explicit MSysinfo();
    virtual ~MSysinfo();

	static std::string hostName();
	
    static std::string buildCpuArchitecture();
    static std::string currentCpuArchitecture();
    static std::string buildAbi();

    static std::string kernelType();
    static std::string kernelVersion();
    static std::string productType();
    static std::string productVersion();
    static std::string prettyProductName();

	static uint64_t memoryAvailSize();
	static uint64_t memoryTotalSize();
	static uint64_t memoryUsedSize();
	static double memoryPercent();

	static double cpuPercent();

	static uint32_t uptimelong();
    static std::string uptime(); // how long the system has been running
	static std::string since();	// System up since, yyyy-mm-dd HH:MM:SS
};

MINI_END_NAMESPACE

#endif /* _MSYSINFO_H_ */
