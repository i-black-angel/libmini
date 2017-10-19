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
#ifndef _PAPPLICATION_H_
#define _PAPPLICATION_H_

#include <punica/pcoredef.h>

PUNICA_BEGIN_NAMESPACE

class PApplication
{
public:
    explicit PApplication();
    virtual ~PApplication();

	static std::string applicationDirPath();
	static std::string applicationFilePath();
	static std::string applicationName();
	static std::string applicationVersion();
	static int64_t applicationPid();
	static uint32_t uptime();
};

PUNICA_END_NAMESPACE

#endif /* _PAPPLICATION_H_ */
